#ifndef SENDING_H
#define SENDING_H

#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QMutex>
#include <QKeyEvent>

#include "server.h"
#include "server.h"

class Sending : public QThread
{
    Q_OBJECT
    Q_WINSTRICT


public:
    explicit Sending(server* srv, QObject* parent = nullptr);

    void Sending_Identifier();

private:
    server * m_server;
    std::vector<qintptr> Descriptor;
    std::vector<QTcpSocket*> Sockets;

    bool first_run = false;

protected:
};

#endif // SENDING_H
