#include <QMainWindow>
#include <QTimer>
#include "networkscanner.h"
#include "client.h"
#include <QHostAddress>
#include <QMessageBox>
#include <passworddialog.h>

namespace Ui {
class Socket;
}

class Socket : public QMainWindow
{
    Q_OBJECT

public:
    explicit Socket(QWidget* parent = nullptr);
    ~Socket();

    QHostAddress getSelectedAddress() const;
    bool sendFileToSelectedAddress(const QString& filePath);


private slots:
    void checkConnections();
//    void handleSelectionChanged();
    void on_pushButtonStop_clicked();
    void on_pushButtonReconnect_clicked();
    void on_pushButton_clicked();
    void readServerResponse();
    void sendPassword(const QString& password); // Ajoutez cette ligne
private:
    Ui::Socket* ui;
    NetworkScanner* scanner;
    QTimer* timer;
    QList<QHostAddress> previousAddresses;
    QTcpSocket* socket;
    bool IsConnected = false;

};

