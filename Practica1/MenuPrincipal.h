#ifndef MENUPRINCIPAL
#define MENUPRINCIPAL
#include "Funciones.h"
#include "Pausa.h"
byte MenuP[8][16] = {{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
                     {0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0},
                     {0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0},
                     {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0},
                     {0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0},
                     {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}};



void MenuPrincipal()
{
    int estadoBotonLeft = digitalRead(DIR_LEFT);
    int estadoBotonDisp = digitalRead(DISP);
    int estadoBotonRight = digitalRead(DIR_RIGHT);
    int estadoBotonK = digitalRead(K);
    switch (estadoActual)
    {
    case MENSAJE:
        mostrarMatriz(Mensajexd);
        if (digitalRead(K) == LOW)
        {
            Serial.println("1s");
            delay(1000);
            if (digitalRead(K) == LOW)
            {
                Serial.println("2s");
                delay(1000);
                if (digitalRead(K) == LOW)
                {
                    estadoActual = MENU_PRINCIPAL;
                }
            }
        }
        break;
    case MENU_PRINCIPAL:
        mostrarMatriz(MenuP);
        Serial.println(".");
        if (estadoBotonLeft == LOW)
        {
            // Botón 1 presionado: ir a OPCION_1
            estadoActual = JUGAR;
            delay(100); // Pequeña pausa para evitar cambios de estado rápidos
        }
        else if (estadoBotonDisp == LOW)
        {
            // Botón 2 presionado: ir a OPCION_2
            estadoActual = ESSTADISTICAS;
            delay(100);
        }
        else if (estadoBotonRight == LOW)
        {
            // Botón 3 presionado: ir a OPCION_3
            estadoActual = CONFIG;
            delay(100);
        }
        break;
    case JUGAR:
        Serial.println("JUGAR");

        if (estadoBotonK == LOW)
        {
            // Botón 3 presionado: volver al MENÚ PRINCIPAL
            estadoActual = MENU_PAUSA;
            delay(100);
            while (estadoActual == MENU_PAUSA)
            {
                MenuPausa();
            }
            
            delay(100);
        }

        break;
    case ESSTADISTICAS:
        Serial.println("ESTADISTICAS");

        if (estadoBotonK == LOW)
        {
            // Botón 3 presionado: volver al MENÚ PRINCIPAL
            estadoActual = MENU_PRINCIPAL;
            delay(100);
        }
        break;
    case CONFIG:
        Serial.println("CONFIG");

        if (estadoBotonK == LOW)
        {
            // Botón 3 presionado: volver al MENÚ PRINCIPAL
            estadoActual = MENU_PRINCIPAL;
            delay(100);
        }
        break;
    default:
        break;
    }
}

#endif