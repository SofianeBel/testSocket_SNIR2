#include "socket.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Socket w;
    w.show();

    return a.exec();
}
