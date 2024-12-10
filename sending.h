#ifndef SENDING_H
#define SENDING_H

#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QMutex>

#include "server.h"
#include "server.h"

class Sending : public QThread
{
    Q_OBJECT


public:
    explicit Sending(server* srv, QObject* parent = nullptr);

    void Sending_Identifier();

private:
    server * m_server;
signals:
    // void Return_Identifier(QList<QTcpSocket*> return_identifier);

};

#endif // SENDING_H
