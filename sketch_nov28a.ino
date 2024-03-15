#include <SPIFFS.h>
#include <Arduino.h>
#include <WiFi.h>


WiFiServer server(5001);
WiFiClient client;

const char* ssid = "Projet_Machine";
const char* password = "Adminproj24";
const char* serverPassword = "123456";  // Password du serveur
bool isConnected = false;
int expectedFileSize = 0;
bool isReceivingFile = false;

File file;

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connexion au Wi-Fi en cours...");
    }
    Serial.println("Connecte au Wi-Fi");

    server.begin();
    Serial.println("Serveur demarre");

    Serial.println(WiFi.localIP());

    if (!SPIFFS.begin(true)) {
        Serial.println("Erreur d'initialisation de SPIFFS");
        return;
    }

    // Supprimer le fichier s'il existe déjà
    if (SPIFFS.exists("/receivedData.txt")) {
        if (!SPIFFS.remove("/receivedData.txt")) {
            Serial.println("Erreur lors de la suppression du fichier");
            return;
        }
    }

    file = SPIFFS.open("/receivedData.txt", FILE_WRITE);
    if (!file) {
        Serial.println("Erreur d'ouverture du fichier");
        return;
    }
}

void loop() {
    if (server.hasClient()) {
        if (client) {
            client.stop();
        }
        client = server.available();
        Serial.println("Client connecte !");
        client.println("Veuillez entrer le mot de passe :");
    }

    if (client && client.available()) {
        String receivedData = client.readStringUntil('\n');
        receivedData.trim();

        if (!isConnected) {
            if (receivedData == serverPassword) {
                isConnected = true;
                client.println("Mot de passe correct. Vous etes autorise a effectuer des operations sur le serveur.");
                Serial.println("C'est bon !!!");
            }
            else {
                isConnected = false;
                Serial.println("CHEH");
                client.println("Mot de passe incorrect. Deconnexion en cours...");
                client.stop();
                Serial.println("Connexion arretee avec le client");
            }
        }

        if (isConnected && expectedFileSize == 0 && receivedData.startsWith("Fichier de ")) {
            int sizeStartIndex = strlen("Fichier de ");
            int sizeEndIndex = receivedData.indexOf(" octets");
            String sizeStr = receivedData.substring(sizeStartIndex, sizeEndIndex);
            expectedFileSize = sizeStr.toInt();
            client.println("Taille du fichier recue : " + String(expectedFileSize));
            isReceivingFile = true;
        }
        else if (isConnected && expectedFileSize > 0 && isReceivingFile) {
            unsigned long receptionTime = millis();
            int receivedDataLength = 0;

            while (client.available()) {
                char c = client.read();
                receivedDataLength++;
                Serial.print(c);
                file.print(c);
            }

            // Si les données reçues sont inférieures à la taille attendue, continuez à lire les données
            if (receivedDataLength < expectedFileSize) {
                return;
            }

            // Si les données reçues sont égales ou supérieures à la taille attendue, terminez la réception du fichier
            if (receivedDataLength >= expectedFileSize) {
                client.println("**Donnees recues avec succes**");
                client.println("**Temps de reception : " + String(receptionTime) + " ms");

                expectedFileSize = 0;
                isReceivingFile = false;
                closeFile();
                printFileContents();
            }
            else {
                client.println("**ERREUR : Les donnees n'ont pas ete recues correctement**");
            }
        }
        if (receivedData == "stop") {
            client.println("La connexion va s'arreter !");
            client.stop();
            Serial.println("Connexion arretee avec le client");
        }
    }
}





void closeFile() {
    if (file) {
        file.close();
    }
}

void printFileContents() {
    file = SPIFFS.open("/receivedData.txt", FILE_READ);
    if (!file) {
        Serial.println("Erreur d'ouverture du fichier");
        return;
    }

    while (file.available()) {
        String line = file.readStringUntil('\n');
        Serial.println(line);
    }

    file.close();
}
