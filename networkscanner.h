#ifndef NETWORKSCANNER_H
#define NETWORKSCANNER_H

#include <QtNetwork>
#include <QObject>

/**
 * @brief La classe NetworkScanner représente un scanner de réseau.
 * 
 * Cette classe permet de scanner un réseau et d'obtenir la liste de toutes les adresses IP disponibles.
 */
class NetworkScanner : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe NetworkScanner.
     * 
     * @param parent L'objet parent de NetworkScanner.
     */
    NetworkScanner(QObject* parent = nullptr);

    /**
     * @brief Obtient la liste de toutes les adresses IP disponibles dans le réseau.
     * 
     * @return La liste des adresses IP disponibles.
     */
    QList<QHostAddress> getAllNetworkAddresses();

private:
    /**
     * @brief Effectue un scan du réseau sur le port spécifié.
     * 
     * @param port Le port à scanner.
     * @return La liste des adresses IP disponibles sur le port spécifié.
     */
    QList<QHostAddress> scan(int port);

    bool isScanning; /**< Indique si le scanner est en cours d'exécution. */
    bool isLocalhost; /**< Indique si le scanner est en cours d'exécution sur la machine locale. */
};

#endif // NETWORKSCANNER_H
