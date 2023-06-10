#include "Funciones.h"
#include "MenuPrincipal.h"

void setup()
{
    Serial.begin(9600);
    inizializarMatrizSinDriver();
    inicializarMatrizDriver();
    pinMode(DIR_LEFT, INPUT_PULLUP);  // DIRECCION IZQUIERDA
    pinMode(DIR_RIGHT, INPUT_PULLUP); // DIRECCION DERECHA
    pinMode(K, INPUT_PULLUP);         // K
    pinMode(DISP, INPUT);             // DISPARO
}

void loop()
{
    MenuPrincipal();
}
