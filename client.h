#include <QFile>
#include <QTcpSocket>
#include <QTimer>
#include <QFileDialog>

class MyTcpSocket : public QObject
{
    Q_OBJECT

public:
    explicit MyTcpSocket(QObject* parent = nullptr);
    ~MyTcpSocket();

public slots:
    void connectToServer();
    void sendFile(const QString& filePath);
    void sendStop();
    void readResponse();
    void write();
//    void sendNoop(); // Ajoutez cette ligne

private:
    QTcpSocket* socket;
    QTimer* noopTimer;
    bool IsConnected = false;
};

