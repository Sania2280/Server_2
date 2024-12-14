#include "sending.h"
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QMutex>


QMutex mutex1;

Sending::Sending(server* srv, QObject* parent)
    : QThread(parent), m_server(srv){
    Sending::Sending_Identifier();
}

void Sending::Sending_Identifier() {
    QTimer* timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [this]() {
        QMutexLocker locker(&mutex1);

        // Получаем текущие сокеты из сервера
        QList<QTcpSocket*> qlistSockets = m_server->getSocketFromServer();
        std::vector<QTcpSocket*> sockets(qlistSockets.begin(), qlistSockets.end());

        locker.unlock();

        // Проверяем, есть ли сокеты
        if (sockets.empty()) return;

        // Инициализация в первый раз
        if (!first_run) {
            Sockets = sockets;  // Копируем текущий список
            Descriptor.resize(sockets.size());

            // Заполняем дескрипторы
            for (size_t i = 0; i < sockets.size(); i++) {
                Descriptor[i] = sockets[i]->socketDescriptor();
            }
            first_run = true;
        }

        // Удаление сокетов, которые больше не актуальны
        std::vector<QTcpSocket*> Sockets_To_Delete;

        for (QTcpSocket* s : Sockets) {
            if (std::find(sockets.begin(), sockets.end(), s) == sockets.end()) {
                Sockets_To_Delete.push_back(s);
            }
        }

        // Удаляем устаревшие сокеты
        for (QTcpSocket* s : Sockets_To_Delete) {
            auto it = std::remove(Sockets.begin(), Sockets.end(), s);
            Sockets.erase(it, Sockets.end());
            qDebug() << "Удаляем сокет:" << s->socketDescriptor();
        }

        // Обновляем список сокетов и дескрипторов
        Sockets = sockets;
        Descriptor.resize(sockets.size());

        for (size_t i = 0; i < sockets.size(); i++) {
            Descriptor[i] = sockets[i]->socketDescriptor();
        }

        qDebug() << "Текущий список сокетов:";
        for (QTcpSocket* s : Sockets) {
            qDebug() << s->socketDescriptor();
        }

        // Обработка отключённых сокетов (аналогично вашему коду)
        QList<QTcpSocket*> disconnectedSockets;

        for (QTcpSocket* socket : sockets) {
            if (socket->state() != QAbstractSocket::ConnectedState) {
                disconnectedSockets.append(socket);
            }
        }

        // Удаление отключённых сокетов
        for (QTcpSocket* socket : disconnectedSockets) {
            locker.relock();
            m_server->getSocketFromServer().removeOne(socket); // Удаляем из оригинального списка
            locker.unlock();

            socket->deleteLater();
            qDebug() << "Client disconnected, removing socket:" << socket->peerAddress().toString();
        }

        // Отправка идентификаторов (аналогично вашему коду)
        for (QTcpSocket* socketTarget : sockets) {
            if (socketTarget->state() == QAbstractSocket::ConnectedState) {
                for (QTcpSocket* socketToSend : sockets) {
                    QByteArray data;
                    QDataStream out(&data, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_6_0);

                    quintptr descriptorToSend = socketToSend->socketDescriptor();
                    QString identifier = (socketToSend == socketTarget)
                                             ? QString("mYthEinDeNtIfIcAtOr %1, %2")
                                                   .arg(socketToSend->peerAddress().toString())
                                                   .arg(QString::number(descriptorToSend))
                                             : QString("thEinDeNtIfIcAtOr %1, %2")
                                                   .arg(socketToSend->peerAddress().toString())
                                                   .arg(QString::number(descriptorToSend));

                    out << identifier;
                    socketTarget->write(data);
                }
            }
        }
    });

    timer->start(10000);  // Период 1 секунда
}


