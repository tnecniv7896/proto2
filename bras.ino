#include <BleKeyboard.h>

BleKeyboard bleKeyboard;
unsigned long Cu1, Cu2, Cu3; // Déclaration des chronomètres

void setup() {
  Serial.begin(115200);

  // Configurer les broches en mode d'entrée
  pinMode(25, INPUT);
  pinMode(22, INPUT);
  pinMode(32, INPUT);
  pinMode(18, INPUT);
  pinMode(35, INPUT);
  pinMode(19, INPUT);
  pinMode(33, INPUT);
  pinMode(23, INPUT);
  pinMode(34, INPUT);

  SerialBT.begin("ESP32-Emetteur-Recepteur");  // Nom du module Bluetooth
  Keyboard.begin();

  // Initialiser les chronomètres
  Cu1 = millis();
  Cu2 = millis();
  Cu3 = millis();
}

void loop() {
  // Lire les valeurs des broches et les imprimer
  int A = digitalRead(25);
  int B = digitalRead(22);
  int C = digitalRead(32);
  int haut = digitalRead(18);
  int bas = digitalRead(35);
  int droite = digitalRead(19);
  int gauche = digitalRead(33);
  int select = digitalRead(23);
  int what = digitalRead(34);

  if (A) {
    bleKeyboard.press(KEY_NUM_1);
    bleKeyboard.releaseAll();
    Serial.println("waza1");
    Cu1 = millis(); // Réinitialiser le chronomètre C1
  }

  if (B) {
    bleKeyboard.press(KEY_NUM_2);
    bleKeyboard.releaseAll();
    Serial.println("waza2");
    Cu2 = millis(); // Réinitialiser le chronomètre C2
  }

  if (C) {
    bleKeyboard.press(KEY_NUM_3);
    bleKeyboard.releaseAll();
    Serial.println("waza3");
    Cu3 = millis(); // Réinitialiser le chronomètre C3
  }

  if (haut) {
    bleKeyboard.press(KEY_NUM_4);
    bleKeyboard.releaseAll();
    Serial.println("waza4");
  }

  if (droite) {
    bleKeyboard.press(KEY_NUM_5);
    bleKeyboard.releaseAll();
    Serial.println("waza5");
  }

  if (bas) {
    bleKeyboard.press(KEY_NUM_6);
    bleKeyboard.releaseAll();
    Serial.println("waza6");
  }

  if (gauche) {
    bleKeyboard.press(KEY_NUM_7);
    bleKeyboard.releaseAll();
    Serial.println("waza7");
  }

  if (select) {
    bleKeyboard.press(KEY_NUM_8);
    bleKeyboard.releaseAll();
    Serial.println("waza8");
  }

  // Appeler les fonctions si nécessaire
  inverserCroix();
  inverserBoutons();

  // Exemple d'utilisation des chronomètres (temps en millisecondes)
  unsigned long tempsEcouléC1 = millis() - Cu1;
  unsigned long tempsEcouléC2 = millis() - Cu2;
  unsigned long tempsEcouléC3 = millis() - Cu3;

  // Faire quelque chose avec les temps écoulés si nécessaire
}

void inverserCroix() {
  // Implémenter la logique pour inverser les boutons de la croix si nécessaire
}

void inverserBoutons() {
  // Implémenter la logique pour inverser les boutons A, B et C si nécessaire
}
