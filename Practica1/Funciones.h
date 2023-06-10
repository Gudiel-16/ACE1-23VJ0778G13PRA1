#ifndef FUNCIONES
#define FUNCIONES
#include "LedControl.h"

LedControl ledControl = LedControl(51, 53, 49, 1);
int filas[] = {24, 22, 2, 3, 4, 5, 6, 7};        // filas encienden con 0 (--> +y)
int columnas[] = {8, 9, 10, 11, 12, 13, 23, 25}; // columnas encienden con 1 (--> +x)
int DIR_LEFT = 52, DIR_RIGHT = 50, K = 48, DISP = 46;

byte buffer[8][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

byte Mensajexd[8][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

// Estados del menú
enum EstadoMenu
{
    MENSAJE,
    MENU_PRINCIPAL,
    JUGAR,
    ESSTADISTICAS,
    CONFIG,
    MENU_PAUSA
};
EstadoMenu estadoActual = MENSAJE;

void inizializarMatrizSinDriver()
{

    // Inicializando pines
    for (int i = 0; i < 8; i++)
    {
        pinMode(columnas[i], OUTPUT);
    }
    for (int i = 0; i < 8; i++)
    {
        pinMode(filas[i], OUTPUT);
    }

    // Limpiar Matriz
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(columnas[i], LOW);
    }
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(filas[i], HIGH);
    }
}

void inicializarMatrizDriver()
{
    ledControl.shutdown(0, false);  // shutdown(# de dispositivo (indice), estado)
    ledControl.setIntensity(0, 15); // setIntensity(# de dispositivo (indice), intensidad de brillo del led de 0 a 15)
    ledControl.clearDisplay(0);     // clearDisplay(# de dispositivo), apaga todos los leds de la pantalla
}

void pintarLED(int x, int y)
{
    digitalWrite(filas[y], LOW);
    digitalWrite(columnas[x], HIGH);
    delayMicroseconds(1100);
    digitalWrite(filas[y], HIGH);
    digitalWrite(columnas[x], LOW);
}

void mostrarMatriz(byte matrzXd[8][16])
{
    // Con driver
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            ledControl.setLed(0, i, j, matrzXd[i][j] == 1 ? true : false);
    // Sin driver
    for (int k = 0; k < 3; k++)
        for (int i = 0; i < 8; i++)
            for (int j = 8; j < 16; j++)
                if (matrzXd[i][j] == 1)
                    pintarLED(j - 8, i);
                else
                    delayMicroseconds(50);
}

#endif