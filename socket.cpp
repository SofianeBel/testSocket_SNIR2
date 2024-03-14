#include "socket.h"
#include "ui_socket.h"
#include "networkscanner.h"

Socket::Socket(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Socket),
    scanner(new NetworkScanner(this)),
    timer(new QTimer(this)),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);
    ui->listWidgetNetwork->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(timer, &QTimer::timeout, this, &Socket::checkConnections);
    timer->start(5000);

    connect(socket, &QTcpSocket::connected, this, &Socket::handleConnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &Socket::handleError);
    connect(socket, &QTcpSocket::readyRead, this, &Socket::readServerResponse);

    socket->connectToHost("172.20.20.15", 5001);
    if (!socket->waitForConnected(5000)) {
        QMessageBox::critical(this, "Erreur de connexion", "Impossible de se connecter au serveur.");
        return;
    }
    QByteArray response = socket->readAll();
    qDebug() << "Reponse du serveur: " << response;
    IsConnected = true;
}

Socket::~Socket()
{
    delete ui;
    delete scanner;
    delete timer;
}

void Socket::readServerResponse() {
    QByteArray response = socket->readAll();
    qDebug() << "Reponse du serveur: " << response;

    if (response.contains("Veuillez entrer le mot de passe :")) {
        PasswordDialog dialog(this);
        connect(&dialog, &PasswordDialog::sendPassword, this, &Socket::sendPassword);
        if (dialog.exec() == QDialog::Accepted) {
            QString password = dialog.getPassword();
            socket->write(password.toUtf8());
        }
    }
}

void Socket::checkConnections()
{
    QList<QHostAddress> currentAddresses = scanner->getAllNetworkAddresses();

    if (currentAddresses != previousAddresses) {
        previousAddresses = currentAddresses;

        ui->listWidgetNetwork->clear();

        for (const QHostAddress& address : currentAddresses) {
            ui->listWidgetNetwork->addItem(address.toString());
        }
    }
}

void Socket::handleConnected() {
    qDebug() << "Connectée!";
    IsConnected = true;
}

void Socket::handleError(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError)
        QMessageBox::critical(this, "Erreur de connexion", "Impossible de se connecter au serveur.");
    qDebug() << "Erreur: " << socket->errorString();
    IsConnected = false;
}

void Socket::sendPassword(const QString& password) {
    socket->write(password.toUtf8());
    if (!socket->waitForBytesWritten(5000)) {
        QMessageBox::critical(this, "Erreur d'ecriture", "Impossible d'ecrire sur le socket.");
        return;
    }
    qDebug() << "Mot de passe envoye avec succes.";
}

void Socket::on_pushButtonReconnect_clicked()
{
    socket->connectToHost("172.20.20.15", 5001);
}

void Socket::on_pushButtonStop_clicked()
{
    if (IsConnected) {
        socket->write("stop\n");
        if (!socket->waitForBytesWritten(5000)) {
            QMessageBox::critical(this, "Erreur d'écriture", "Impossible d'écrire sur le socket.");
            return;
        }
        socket->disconnectFromHost();
        if (socket->state() == QAbstractSocket::ConnectedState && !socket->waitForDisconnected(5000)) {
            QMessageBox::critical(this, "Erreur de déconnexion", "Impossible de se déconnecter du serveur.");
            return;
        }
        qDebug() << "Déconnectée !";
        IsConnected = false;
    }
    else {
        qDebug() << "Erreur lors de la déconnexion !";
    }
}

void Socket::on_pushButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "All Files (*.*)");
    if (fileName.isEmpty()) {
        qDebug() << "No file selected";
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Erreur d'ouverture de fichier", "Impossible d'ouvrir le fichier.");
        qDebug() << "Error opening file: " << file.errorString();
        return;
    }

    if (socket->state() != QAbstractSocket::ConnectedState) {
        QMessageBox::critical(this, "Erreur de connexion", "Pas connecte au serveur.");
        file.close();
        return;
    }

    QByteArray response = socket->readAll();
    if (response.contains("Mot de passe incorrect. Deconnexion en cours...")) {
        QMessageBox::critical(this, "Erreur de connexion", "Mot de passe incorrect. Deconnexion en cours...");
        file.close();
        return;
    }

    QCoreApplication::processEvents();

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(file.size());

    qint64 bytesSent = 0;
    while (!file.atEnd()) {
        QByteArray data = file.read(1024);
        bytesSent += data.size();
        qDebug() << data;

        qint64 fileSize = file.size();
        QByteArray sizeArray = QByteArray::number(fileSize);

        // Envoyez la taille du fichier
        qint64 bytesWritten = socket->write("Fichier de " + sizeArray + " octets\n");
        if (!socket->waitForBytesWritten(5000)) {
            QMessageBox::critical(this, "Erreur d'ecriture", "Impossible d'ecrire sur le socket.");
            file.close();
            return;
        }

        // Envoyez le contenu du fichier
        bytesWritten = socket->write(data);
        if (!socket->waitForBytesWritten(5000)) {
            QMessageBox::critical(this, "Erreur d'ecriture", "Impossible d'ecrire sur le socket.");
            file.close();
            return;
        }

        ui->progressBar->setValue(bytesSent);

        if (bytesWritten != data.size()) {
            QMessageBox::critical(this, "Erreur d'ecriture", "Erreur lors de l'envoi des donnees.");
            qDebug() << "Error sending data: " << socket->errorString();
            file.close();
            return;
        }

        QApplication::processEvents();
        qDebug() << "Sent " + QString::number(bytesSent) + " bytes";
    }
    file.close();

    socket->write("\n");
    if (!socket->waitForBytesWritten(5000)) {
        QMessageBox::critical(this, "Erreur d'ecriture", "Impossible d'ecrire sur le socket.");
        return;
    }

    QDateTime currentTime = QDateTime::currentDateTime();
    QString formattedTime = currentTime.toString("dd/MM/yyyy hh:mm:ss");
    QString fileNameOnly = QFileInfo(fileName).fileName();

    qDebug() << "Fichier envoye avec succes.";
    qDebug() << "Envoye a: " << formattedTime;
    qDebug() << "Nom du fichier: " << fileNameOnly;
}





