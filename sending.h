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
    void Timer();

private:
    server * m_server;
signals:
    // void Return_Identifier(QList<QTcpSocket*> return_identifier);

public slots:
    void Get_New_Client(QTcpSocket* socet);

};

#endif // SENDING_H
