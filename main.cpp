#include <QCoreApplication>
#include "server.h"
#include "sending.h"




int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    server s;  // Создаем объект сервера

    Sending sendObj(&s);

    sendObj.start();

    return a.exec();
}
