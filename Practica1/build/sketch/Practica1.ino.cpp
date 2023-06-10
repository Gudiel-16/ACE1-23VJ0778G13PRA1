#include <Arduino.h>
#line 1 "G:\\Archivos\\Escritorio\\Ingenieria\\Junio 2023\\Arqui 1\\P1-ARQUI1-BACKUP\\Practica1\\Practica1.ino"
#include "Funciones.h"
#include "MenuPrincipal.h"

#line 4 "G:\\Archivos\\Escritorio\\Ingenieria\\Junio 2023\\Arqui 1\\P1-ARQUI1-BACKUP\\Practica1\\Practica1.ino"
void setup();
#line 15 "G:\\Archivos\\Escritorio\\Ingenieria\\Junio 2023\\Arqui 1\\P1-ARQUI1-BACKUP\\Practica1\\Practica1.ino"
void loop();
#line 4 "G:\\Archivos\\Escritorio\\Ingenieria\\Junio 2023\\Arqui 1\\P1-ARQUI1-BACKUP\\Practica1\\Practica1.ino"
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

