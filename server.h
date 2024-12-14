#ifndef SERVER_H
#define SERVER_H

#include "qmutex.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QByteArray>
#include <QDebug>

class server : public QTcpServer {
    Q_OBJECT

public:
    server();
    QList<QTcpSocket*>& getSocketFromServer() const;  // Объявление метода

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void slotsReadyRead();
    // void handleDisconnection();
   // void SendIdentificator();
    void Comunication(quintptr RESEVER, quintptr SENDER, QString text );

private:
    void ConectionHub();
    void SendToClient(QTcpSocket *clientSocket, const QString& str);
    void SendTOClientData(QVector<QTcpSocket *> identificators);
    static QList<QTcpSocket*> Sockets;
    static QMutex mutex;
    QByteArray Data;

signals:
    void ComunicationPare(QVector<QTcpSocket*> identificators);
    void ComunicationMesage(quintptr RESEVER, quintptr SENDER, QString text );
//  void newConnection(QTcpSocket* socket);

public slots:
     // void Resive_Identifier(QList<QTcpSocket*> resiving_identifier);

};

#endif // SERVER_H
