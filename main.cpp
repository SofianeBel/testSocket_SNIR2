#include "socket.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Socket w;

    QFile file("path/StyleExample.qss");
    file.open(QFile::ReadOnly);

    QString styleSheet{ QLatinlString(file.readAll()) };

    //setup stylesheet
    app.setStyleSheet(styleSheet);

    w.show();

    return a.exec();
}
