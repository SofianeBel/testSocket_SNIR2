#include "networkscanner.h"
#include <QDebug>

/**
 * @brief Constructeur de la classe NetworkScanner.
 * @param parent Objet parent de NetworkScanner.
 */
NetworkScanner::NetworkScanner(QObject* parent) : QObject(parent) {}

/**
 * @brief Obtient toutes les adresses réseau disponibles.
 * @return Une liste d'adresses QHostAddress.
 */
QList<QHostAddress> NetworkScanner::getAllNetworkAddresses()
{
    QList<QHostAddress> machines;

    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const auto& interface : interfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            const auto entries = interface.addressEntries();
            for (const auto& entry : entries) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    // Convertir l'adresse IP en chaîne de caractères
                    QString ipString = entry.ip().toString();
                    // Vérifier si l'adresse IP commence par "172.20.20"
                    if (ipString.startsWith("172.20.20")) {
                        machines.append(entry.ip());
                    }
                }
            }
        }
    }

    return machines;
}

