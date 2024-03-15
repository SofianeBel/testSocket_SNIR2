#include "socket.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Socket w;

    QFile file("/I");
    file.open(QFile::ReadOnly);

    QString styleSheet{ QLatinlString(file.readAll()) };

    //setup stylesheet
    a.setStyleSheet(styleSheet);

    w.show();

    return a.exec();
}
