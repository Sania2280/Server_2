#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QByteArray>

class server : public QTcpServer {
    Q_OBJECT

public:
    server();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void slotsReadyRead();
    void handleDisconnection();
    void SendIdentificator();

private:
    void SendToClient(QTcpSocket *clientSocket, const QString& str);
    QList<QTcpSocket*> Sockets;
    QByteArray Data;
};

#endif // SERVER_H
