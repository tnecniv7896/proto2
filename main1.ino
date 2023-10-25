#include <BLEDevice.h>
#include <BLEHIDDevice.h>
#include <BLEKeyboard.h>
#include <SoftwareSerial.h>

const int selectPin = 2;
const int startPin = 3;
const int aPin = 4;
const int bPin = 5;
const int cPin = 6;
const int upPin = 7;
const int downPin = 8;
const int leftPin = 9;
const int rightPin = 10;
const int u1Pin = 11;  // Bouton U1
const int u2Pin = 12;  // Bouton U2

BLEServer *pServer;
BLEHIDDevice* hid;
BLECharacteristic* inputKeyboard;

SoftwareSerial btSerial(13, 14);  // Configuration des broches RX et TX pour la communication Bluetooth

bool inverserDirections = false;
bool abcEnCab = false;

void setup() {
  pinMode(selectPin, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(aPin, INPUT_PULLUP);
  pinMode(bPin, INPUT_PULLUP);
  pinMode(cPin, INPUT_PULLUP);
  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(leftPin, INPUT_PULLUP);
  pinMode(rightPin, INPUT_PULLUP);
  pinMode(u1Pin, INPUT_PULLUP);
  pinMode(u2Pin, INPUT_PULLUP);

  BLEDevice::init("ManetteBluetooth");
  pServer = BLEDevice::createServer();
  hid = new BLEHIDDevice(pServer);
  inputKeyboard = hid->inputReport(1);

  hid->reportMap((uint8_t*)_hidReportDescriptor, sizeof(_hidReportDescriptor));
  hid->startServices();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->setAppearance(HID_KEYBOARD);
  pAdvertising->addServiceUUID(hid->hidService()->getUUID());
  pAdvertising->setDiscoverable(true);
  pAdvertising->start();

  BLEKeyboard.begin();

  // Configuration de la communication Bluetooth
  btSerial.begin(9600);
}

void loop() {
  if (digitalRead(selectPin) == LOW) {
    BLEKeyboard.press(KEY_S);
    BLEKeyboard.release(KEY_S);
  }

  if (digitalRead(startPin) == LOW) {
    BLEKeyboard.press(KEY_T);
    BLEKeyboard.release(KEY_T);
  }

  if (digitalRead(aPin) == LOW) {
    if (abcEnCab) {
      BLEKeyboard.press(KEY_C);
      BLEKeyboard.release(KEY_C);
    } else {
      BLEKeyboard.press(KEY_A);
      BLEKeyboard.release(KEY_A);
    }
  }

  if (digitalRead(bPin) == LOW) {
    if (abcEnCab) {
      BLEKeyboard.press(KEY_A);
      BLEKeyboard.release(KEY_A);
    } else {
      BLEKeyboard.press(KEY_B);
      BLEKeyboard.release(KEY_B);
    }
  }

  if (digitalRead(cPin) == LOW) {
    if (abcEnCab) {
      BLEKeyboard.press(KEY_B);
      BLEKeyboard.release(KEY_B);
    } else {
      BLEKeyboard.press(KEY_C);
      BLEKeyboard.release(KEY_C);
    }
  }

  if (digitalRead(upPin) == LOW) {
    if (inverserDirections) {
      BLEKeyboard.press(KEY_DOWN);
      BLEKeyboard.release(KEY_DOWN);
    } else {
      BLEKeyboard.press(KEY_UP);
      BLEKeyboard.release(KEY_UP);
    }
  }

  if (digitalRead(downPin) == LOW) {
    if (inverserDirections) {
      BLEKeyboard.press(KEY_UP);
      BLEKeyboard.release(KEY_UP);
    } else {
      BLEKeyboard.press(KEY_DOWN);
      BLEKeyboard.release(KEY_DOWN);
    }
  }

  if (digitalRead(leftPin) == LOW) {
    if (inverserDirections) {
      BLEKeyboard.press(KEY_RIGHT);
      BLEKeyboard.release(KEY_RIGHT);
    } else {
      BLEKeyboard.press(KEY_LEFT);
      BLEKeyboard.release(KEY_LEFT);
    }
  }

  if (digitalRead(rightPin) == LOW) {
    if (inverserDirections) {
      BLEKeyboard.press(KEY_LEFT);
      BLEKeyboard.release(KEY_LEFT);
    } else {
      BLEKeyboard.press(KEY_RIGHT);
      BLEKeyboard.release(KEY_RIGHT);
    }
  }

  if (digitalRead(u1Pin) == LOW) {
    // Envoyer la commande d'inversion via Bluetooth
    btSerial.println("INVERSION_ON");
  }

  if (digitalRead(u2Pin) == LOW) {
    // Envoyer la commande de changement de boutons via Bluetooth
    btSerial.println("CHANGEMENT_BOUTONS");
  }

  // Recevoir des commandes via Bluetooth
  while (btSerial.available()) {
    String commande = btSerial.readString();
    if (commande == "INVERSION_ON") {
      inverserDirections = true;
    } else if (commande == "CHANGEMENT_BOUTONS") {
      abcEnCab = !abcEnCab;
    }
  }
}
