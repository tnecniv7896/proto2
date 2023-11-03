#include <BluetoothSerial.h>
#include <Keyboard.h>
#include <SegaController.h>

BluetoothSerial SerialBT;

typedef struct
{
    word bouton;
    char touche;
} sortie;

// Mappage des boutons de la manette à des touches du clavier
sortie sortieMap[] = {
    { SC_BTN_UP,    KEY_UP_ARROW },
    { SC_BTN_DOWN,  KEY_DOWN_ARROW },
    { SC_BTN_LEFT,  KEY_LEFT_ARROW },
    { SC_BTN_RIGHT, KEY_RIGHT_ARROW },
    { SC_BTN_START, KEY_RETURN },
    { SC_BTN_A,     'z' },
    { SC_BTN_B,     'x' },
    { SC_BTN_C,     'c' },
    { SC_BTN_X,     'a' },
    { SC_BTN_Y,     's' },
    { SC_BTN_Z,     'd' },
    { SC_BTN_MODE,  'q' }
};

const byte BOUTONS = 12;

// Broches de la manette DB9 (vu de face à l'extrémité de la fiche) :
//
// 5 4 3 2 1
//  9 8 7 6
//
// Connectez la broche 5 à +5V et la broche 8 à la masse (GND)
// Connectez les autres broches aux broches d'E/S numériques (voir ci-dessous)

// Spécifiez les broches Arduino connectées à
// la broche DB9 7, la broche DB9 1, la broche DB9 2, la broche DB9 3, la broche DB9 4, la broche DB9 6, la broche DB9 9
SegaController controller(8, 2, 3, 4, 5, 6, 7);

// États de la manette
word etatCourant = 0;
word dernierEtat = 0;

// Définir des variables pour suivre l'état des boutons U1 et U2
bool u1Presse = false;
bool u2Presse = false;

void setup()
{
    Serial.begin(115200);
    SerialBT.begin("ESP32-Emetteur-Recepteur");  // Nom du module Bluetooth
    Keyboard.begin();
}

void loop()
{
    etatCourant = controller.getState();
    envoyerEtats();
    
    if (SerialBT.available()) {
        String commande = SerialBT.readString();
        
        if (commande == "INVERSION_ACTIVEE") {
            Serial.println("Inversion activée");

            // Réaliser l'inversion de la croix directionnelle
            inverserCroix();
        } else if (commande == "U2_ACTIVE") {
            Serial.println("U2 activé");

            // Inverser les boutons A, B et C
            inverserBoutons();
        } else {
            Serial.println("Commande inconnue");
        }
    }

    if (digitalRead(BOUTON_U1) == LOW) {
        if (!u1Presse) {
            SerialBT.println("INVERSION_ACTIVEE");  // Envoie la commande d'inversion via Bluetooth
            u1Presse = true;
        }
    } else {
        u1Presse = false;
    }

    if (digitalRead(BOUTON_U2) == LOW) {
        if (!u2Presse) {
            SerialBT.println("U2_ACTIVE");  // Envoie la commande U2 via Bluetooth
            u2Presse = true;
        }
    } else {
        u2Presse = false;
    }
    
    delay(1000);
}

// Fonction pour inverser la croix directionnelle
void inverserCroix() {
    // Ajoutez ici le code pour inverser la croix directionnelle
    // Par exemple, vous pouvez inverser les valeurs des boutons HAUT et BAS
    if (etatCourant & SC_BTN_UP) {
        etatCourant &= ~SC_BTN_UP;
        etatCourant |= SC_BTN_DOWN;
    } else if (etatCourant & SC_BTN_DOWN) {
        etatCourant &= ~SC_BTN_DOWN;
        etatCourant |= SC_BTN_UP;
    }
}

// Fonction pour inverser les boutons A, B et C (ABC devient CAB)
void inverserBoutons() {
    word etatTemporaire = 0;

    // Inverser les boutons A, B et C
    if (etatCourant & SC_BTN_A) {
        etatTemporaire |= SC_BTN_B;
    }
    if (etatCourant & SC_BTN_B) {
        etatTemporaire |= SC_BTN_C;
    }
    if (etatCourant & SC_BTN_C) {
        etatTemporaire |= SC_BTN_A;
    }

    etatCourant = etatTemporaire;
}
