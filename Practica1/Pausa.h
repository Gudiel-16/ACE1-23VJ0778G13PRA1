#ifndef PAUSA
#define PAUSA
#include "Funciones.h"

byte PausaXd[8][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

void MenuPausa()
{
    mostrarMatriz(PausaXd);
    if (digitalRead(K) == LOW)
        {
            delay(1000);
            Serial.println("1s");
            if (digitalRead(K) == LOW)
            {
                delay(1000);
                Serial.println("2s");
                estadoActual = JUGAR;
                if (digitalRead(K) == LOW)
                {
                    delay(1000);
                    Serial.println("3s");
                    if (digitalRead(K) == LOW)
                    {
                        estadoActual = MENU_PRINCIPAL;
                    }
                }
            }
        }
}
#endif