#ifndef NETWORKSCANNER_H
#define NETWORKSCANNER_H

#include <QtNetwork>
#include <QObject>

/**
 * @brief La classe NetworkScanner repr�sente un scanner de r�seau.
 * 
 * Cette classe permet de scanner un r�seau et d'obtenir la liste de toutes les adresses IP disponibles.
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
     * @brief Obtient la liste de toutes les adresses IP disponibles dans le r�seau.
     * 
     * @return La liste des adresses IP disponibles.
     */
    QList<QHostAddress> getAllNetworkAddresses();

private:
    /**
     * @brief Effectue un scan du r�seau sur le port sp�cifi�.
     * 
     * @param port Le port � scanner.
     * @return La liste des adresses IP disponibles sur le port sp�cifi�.
     */
    QList<QHostAddress> scan(int port);

    bool isScanning; /**< Indique si le scanner est en cours d'ex�cution. */
    bool isLocalhost; /**< Indique si le scanner est en cours d'ex�cution sur la machine locale. */
};

#endif // NETWORKSCANNER_H
