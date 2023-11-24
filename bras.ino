
#include <Keyboard.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

const int boutonA = 2;
const int boutonB = 3;
const int boutonC = 4;
const int boutonStart = 5;
const int boutonDirectionHaut = 6;
const int boutonDirectionBas = 7;
const int boutonDirectionGauche = 8;
const int boutonDirectionDroite = 9;

int konamiCode[] = {boutonDirectionHaut,boutonDirectionHaut, boutonDirectionBas, boutonDirectionBas,boutonDirectionGauche, boutonDirectionDroite, boutonDirectionGauche, boutonDirectionDroite, 'B', 'A'};
int konamiIndex = 0;

int secretCode[] = {KEY_DOWN_ARROW, boutonDirectionBas, 'A', 'B', 'C', 'C', 'B', 'A', 'A', 'B'};
int secretIndex = 0;

int anotherSecretCode[] = {boutonDirectionHaut, boutonDirectionHaut,boutonDirectionBas,boutonDirectionBas, boutonDirectionGauche, boutonDirectionDroite, boutonDirectionGauche, boutonDirectionDroite, 'B', 'A', 'A', 'B', 'C', 'C'};
int anotherSecretIndex = 0;

bool invertedDirections = false;
unsigned long invertedStartTime = 0;
const unsigned long invertedDuration = 30000; // 30 secondes

bool disableABC = false;
void setup() {
  pinMode(boutonA, INPUT_PULLUP);
  pinMode(boutonB, INPUT_PULLUP);
  pinMode(boutonC, INPUT_PULLUP);
  pinMode(boutonStart, INPUT_PULLUP);
  pinMode(boutonDirectionHaut, INPUT_PULLUP);
  pinMode(boutonDirectionBas, INPUT_PULLUP);
  pinMode(boutonDirectionGauche, INPUT_PULLUP);
  pinMode(boutonDirectionDroite, INPUT_PULLUP);

  Keyboard.begin();

  SerialBT.begin("ESP32_BT");  // Nom du module Bluetooth
}
void loopBluetooth() {
  if (SerialBT.available()) {
    // Lire les données Bluetooth
    String command = SerialBT.readStringUntil('\n');

    // Traiter la commande reçue
    processBluetoothCommand(command);
  }
}
void loop() {
  int etatA = digitalRead(boutonA);
  int etatB = digitalRead(boutonB);
  int etatC = digitalRead(boutonC);
  int etatStart = digitalRead(boutonStart);
  int etatDirectionHaut = digitalRead(boutonDirectionHaut);
  int etatDirectionBas = digitalRead(boutonDirectionBas);
  int etatDirectionGauche = digitalRead(boutonDirectionGauche);
  int etatDirectionDroite = digitalRead(boutonDirectionDroite);

  if (checkKonamiCode(etatA, etatB, etatC, etatStart, etatDirectionHaut, etatDirectionBas, etatDirectionGauche, etatDirectionDroite)) {
    sendBluetoothCommand("U1=1");
  }

  if (checkSecretCode(etatA, etatB, etatC, etatStart, etatDirectionHaut, etatDirectionBas, etatDirectionGauche, etatDirectionDroite)) {
    sendBluetoothCommand("B=1");
    invertedDirections = true;
    invertedStartTime = millis();
  }

  if (checkAnotherSecretCode(etatA, etatB, etatC, etatStart, etatDirectionHaut, etatDirectionBas, etatDirectionGauche, etatDirectionDroite)) {
    sendBluetoothCommand("C=1");
  }

  if (etatA == LOW && !disableABC) {
    Keyboard.write('A');
    delay(100);
  }

  if (etatB == LOW && !disableABC) {
    Keyboard.write('B');
    delay(100);
  }

  if (etatC == LOW && !disableABC) {
    Keyboard.write('C');
    delay(100);
  }

  if (etatStart == LOW) {
    // Ajoutez le code correspondant au bouton Start ici
    delay(100);
  }

  // Répétez le processus pour les autres boutons...

  if (disableABC) {
    etatA = HIGH;
    etatB = HIGH;
    etatC = HIGH;
  }

  if (invertedDirections) {
    // Inverser les directions pendant 30 secondes
    if (millis() - invertedStartTime < invertedDuration) {
      etatDirectionHaut = !etatDirectionHaut;
      etatDirectionBas = !etatDirectionBas;
      etatDirectionGauche = !etatDirectionGauche;
      etatDirectionDroite = !etatDirectionDroite;
    } else {
      invertedDirections = false;
    }
  }

  delay(50);
}



void processBluetoothCommand(String command) {
  // Logique pour traiter la commande Bluetooth ici
  // Par exemple, imprimer la commande reçue
  Serial.println("Commande Bluetooth reçue : " + command);

  // Ajouter des instructions pour traiter différentes commandes
  if (command == "U1=1") {
    // Logique pour traiter la commande U1=1
  } else if (command == "B=1") {
    // Logique pour traiter la commande B=1
  } else if (command == "C=1") {
    // Logique pour traiter la commande C=1
  }
}

