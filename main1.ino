#include <BLEDevice.h>
#include <BLEHIDDevice.h>
#include <BLEKeyboard.h>
#include <SoftwareSerial.h>

// Déclaration des boutons de la manette
const int BOUTON_SELECT = 2;
const int BOUTON_START = 3;
const int BOUTON_A = 4;
const int BOUTON_B = 5;
const int BOUTON_C = 6;
const int BOUTON_HAUT = 7;
const int BOUTON_BAS = 8;
const int BOUTON_GAUCHE = 9;
const int BOUTON_DROITE = 10;
const int BOUTON_U1 = 11;  // Bouton U1
const int BOUTON_U2 = 12;  // Bouton U2

BLEServer *serveurBLE;
BLEHIDDevice* manetteHID;
BLECharacteristic* rapportClavier;

SoftwareSerial communicationBluetooth(13, 14);  // Configuration des broches RX et TX pour la communication Bluetooth

bool inversionActivee = false;
bool inversionDeclenchée = false;
unsigned long dernierTempsInversion = 0;
unsigned long dureeInversion = 30 * 1000;  // 30 secondes
unsigned long intervalleInversion = 60 * 1000;  // Une inversion par minute

bool ordreBoutonsABC = false;
bool u2Active = false;
unsigned long dernierTempsU2 = 0;
unsigned long dureeU2 = 30 * 1000;  // 30 secondes

void setup() {
  // Configuration des broches en entrée avec résistance de tirage
  pinMode(BOUTON_SELECT, INPUT_PULLUP);
  pinMode(BOUTON_START, INPUT_PULLUP);
  pinMode(BOUTON_A, INPUT_PULLUP);
  pinMode(BOUTON_B, INPUT_PULLUP);
  pinMode(BOUTON_C, INPUT_PULLUP);
  pinMode(BOUTON_HAUT, INPUT_PULLUP);
  pinMode(BOUTON_BAS, INPUT_PULLUP);
  pinMode(BOUTON_GAUCHE, INPUT_PULLUP);
  pinMode(BOUTON_DROITE, INPUT_PULLUP);
  pinMode(BOUTON_U1, INPUT_PULLUP);
  pinMode(BOUTON_U2, INPUT_PULLUP);

  BLEDevice::init("ManetteBluetooth");
  serveurBLE = BLEDevice::createServer();
  manetteHID = new BLEHIDDevice(serveurBLE);
  rapportClavier = manetteHID->inputReport(1);

  manetteHID->reportMap((uint8_t*)_hidReportDescriptor, sizeof(_hidReportDescriptor));
  manetteHID->startServices();

  BLEAdvertising *publiciteBLE = serveurBLE->getAdvertising();
  publiciteBLE->setAppearance(HID_KEYBOARD);
  publiciteBLE->addServiceUUID(manetteHID->hidService()->getUUID());
  publiciteBLE->setDiscoverable(true);
  publiciteBLE->start();

  BLEKeyboard.begin();

  // Configuration de la communication Bluetooth
  communicationBluetooth.begin(9600);
}

void loop() {
  if (digitalRead(BOUTON_SELECT) == LOW) {
    BLEKeyboard.press(KEY_S);
    BLEKeyboard.release(KEY_S);
  }

  if (digitalRead(BOUTON_START) == LOW) {
    BLEKeyboard.press(KEY_T);
    BLEKeyboard.release(KEY_T);
  }

  if (digitalRead(BOUTON_A) == LOW) {
    if (ordreBoutonsABC) {
      BLEKeyboard.press(KEY_C);
      BLEKeyboard.release(KEY_C);
    } else {
      BLEKeyboard.press(KEY_A);
      BLEKeyboard.release(KEY_A);
    }
  }

  if (digitalRead(BOUTON_B) == LOW) {
    if (ordreBoutonsABC) {
      BLEKeyboard.press(KEY_A);
      BLEKeyboard.release(KEY_A);
    } else {
      BLEKeyboard.press(KEY_B);
      BLEKeyboard.release(KEY_B);
    }
  }

  if (digitalRead(BOUTON_C) == LOW) {
    if (ordreBoutonsABC) {
      BLEKeyboard.press(KEY_B);
      BLEKeyboard.release(KEY_B);
    } else {
      BLEKeyboard.press(KEY_C);
      BLEKeyboard.release(KEY_C);
    }
  }

  if (digitalRead(BOUTON_HAUT) == LOW) {
    if (inversionActivee) {
      BLEKeyboard.press(KEY_DOWN);
      BLEKeyboard.release(KEY_DOWN);
    } else {
      BLEKeyboard.press(KEY_UP);
      BLEKeyboard.release(KEY_UP);
    }
  }

  if (digitalRead(BOUTON_BAS) == LOW) {
    if (inversionActivee) {
      BLEKeyboard.press(KEY_UP);
      BLEKeyboard.release(KEY_UP);
    } else {
      BLEKeyboard.press(KEY_DOWN);
      BLEKeyboard.release(KEY_DOWN);
    }
  }

  if (digitalRead(BOUTON_GAUCHE) == LOW) {
    if (inversionActivee) {
      BLEKeyboard.press(KEY_RIGHT);
      BLEKeyboard.release(KEY_RIGHT);
    } else {
      BLEKeyboard.press(KEY_LEFT);
      BLEKeyboard.release(KEY_LEFT);
    }
  }

  if (digitalRead(BOUTON_DROITE) == LOW) {
    if (inversionActivee) {
      BLEKeyboard.press(KEY_LEFT);
      BLEKeyboard.release(KEY_LEFT);
    } else {
      BLEKeyboard.press(KEY_RIGHT);
      BLEKeyboard.release(KEY_RIGHT);
    }
  }

  unsigned long tempsActuel = millis();

  if (digitalRead(BOUTON_U1) == LOW) {
    if (!inversionDeclenchée) {
      if (tempsActuel - dernierTempsInversion >= intervalleInversion) {
        inversionActivee = true;
        dernierTempsInversion = tempsActuel;
        inversionDeclenchée = true;
      }
    }
  } else {
    inversionDeclenchée = false;
  }

  // Désactiver l'inversion après 30 secondes
  if (inversionActivee && (tempsActuel - dernierTempsInversion >= dureeInversion)) {
    inversionActivee = false;
  }

  if (digitalRead(BOUTON_U2) == LOW) {
    if (!u2Active) {
      u2Active = true;
      dernierTempsU2 = tempsActuel;
    }
  }

  // Désactiver la fonction U2 après 30 secondes
  if (u2Active && (tempsActuel - dernierTempsU2 >= dureeU2)) {
    u2Active = false;
  }

  // Envoyer l'état d'inversion et de la fonction U2 via Bluetooth
  if (inversionActivee) {
    communicationBluetooth.println("INVERSION_ACTIVEE");
  } else {
    communicationBluetooth.println("INVERSION_DESACTIVEE");
  }

  if (u2Active) {
    communicationBluetooth.println("U2_ACTIVE");
  } else {
    communicationBluetooth.println("U2_INACTIF");
  }

  // Recevoir des commandes via Bluetooth
  while (communicationBluetooth.available()) {
    String commande = communicationBluetooth.readString();
    if (commande == "INVERSION_ACTIVEE") {
      inversionActivee = true;
    } else if (commande == "INVERSION_DESACTIVEE") {
      inversionActivee = false;
    } else if (commande == "U2_ACTIVE") {
      u2Active = true;
    } else if (commande == "U2_INACTIF") {
      u2Active = false;
    }
  }
}
