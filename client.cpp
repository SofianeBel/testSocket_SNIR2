#include "client.h"

MyTcpSocket::MyTcpSocket(QObject* parent) : QObject(parent), socket(new QTcpSocket(this)), noopTimer(new QTimer(this)) {
//    connect(noopTimer, &QTimer::timeout, this, &MyTcpSocket::sendNoop);
//    noopTimer->start(5000); // Envoie une commande NOOP toutes les 5 secondes

    connect(socket, &QTcpSocket::readyRead, this, &MyTcpSocket::readResponse); // Ajoutez cette ligne
}

MyTcpSocket::~MyTcpSocket() {
    delete socket;
}

void MyTcpSocket::connectToServer()
{
    socket->connectToHost("172.20.20.13", 80);
    if (socket->waitForConnected(5000) && !IsConnected) {
        qDebug() << "Connectée!";
            IsConnected = true;
    }
    else {
        qDebug() << "Erreur: " << socket->errorString();
            IsConnected = false;
    }
}

void MyTcpSocket::sendFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Erreur d'ouverture du fichier: " << file.errorString();
        return;
    }

    while (!file.atEnd()) {
        QByteArray data = file.read(1024); // Lire 1024 octets à la fois
        socket->write(data);
    }

    file.close();
}

void MyTcpSocket::readResponse() 
{
    QByteArray response = socket->readAll();
    qDebug() << "Réponse du serveur: " << response;
}

void MyTcpSocket::sendStop()
{
    if(IsConnected==true){
    socket->write("stop");
    socket->disconnect();
    qDebug() << "Déconnectée !";
    IsConnected = false;
    }
        else{
        qDebug() << "Erreur lors de la déconnexion !";
    }
}

void MyTcpSocket::write()
{

}


//void MyTcpSocket::sendNoop() // Ajoutez cette méthode
//{
//    if (socket->state() == QAbstractSocket::ConnectedState) {
//        socket->write("NOOP\r\n");
//    }
//}
