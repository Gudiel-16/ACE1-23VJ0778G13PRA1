#include <MD_MAX72xx.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 1
#define CLK_PIN 11
#define DATA_PIN 13
#define CS_PIN 12
#include "complementos.h"

//inicia el objeto de tipo MD_MAX para el control de la matriz con driver
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

int columna[8] = { 29, 28, 27, 26, 25, 24, 23, 22 };
int fila[8] = { 46, 47, 48, 49, 50, 51, 52, 53 };
bool desplazamientoArriba = true, direccionMensaje = true;
int posicionNumero;
int izquierda, derecha, start, estado, posicionVisor, puntaje, vidas;
long contador;

//matriz que contiene la logica del proyecto
int matrizLogica[][32] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

//matriz que se encarga de guardar la partida cuando se pone y quita la pausa
int matrizTemporal[][32] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

//matriz que permite voltear los valores para que se pueda pintar el segmento dentro de la matriz sin driver
int matrizLogicaDriver[][8] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
};

void setup() {
  // put your setup code here, to run once:
  izquierda = 0;
  derecha = 0;
  start = 0;
  estado = 0;
  posicionVisor = 15;
  vidas = 3;
  puntaje = 0;

  mx.begin();
  Inicializar();
  Serial.begin(9600);
  valorDesplazamiento();
}

//inicializa los componentes
void Inicializar() {
  //bucle for para los pines de 22 a 29 y de 43 a 53
  for (int x = 22; x <= 29; x++) {
    pinMode(x, OUTPUT);
    pinMode(x + 24, OUTPUT);
  }

  //bucle for para los pines con voltaje alto de la matriz sin draver de leds
  for (int x = 29; x >= 22; x--) {
    digitalWrite(x, HIGH);
  }
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
}

//metodo encargado de pintar la matriz con driver
void pintarMatrizConDriver() {
  mx.clear();
  for (int fila = 0; fila < 8; fila++) {
    for (int columna = 0; columna < 8; columna++) {
      if (matrizLogica[fila][columna + 8] > 0) {
        mx.setPoint(7 - fila, columna, true);
      }
    }
  }
}

//metodo encargado de guardar la partida cuando se pone la pausa
void guardarTemporal() {
  Serial.println("");
  for (int fila = 0; fila < 8; fila++) {
    for (int columna = 0; columna < 16; columna++) {
      matrizTemporal[fila][columna] = matrizLogica[fila][columna];
      Serial.print(matrizTemporal[fila][columna]);
    }
    Serial.println("");
  }
}

//metodo encargado de cargar la partida cuando se quita la pausa
void setTemporal() {
  for (int fila = 0; fila < 8; fila++) {
    for (int columna = 0; columna < 16; columna++) {
      matrizLogica[fila][columna] = matrizTemporal[fila][columna];
    }
  }
}

//pinta la matriz logica en la matriz sin driver
void pintarMatrizSinDriver() {
  voltearSegmento();
  for (int i = 0; i < 8; i++) {
    digitalWrite(fila[i], HIGH);
    for (int j = 7; j >= 0; j--) {
      if (matrizLogicaDriver[i][j] > 0) {
        digitalWrite(columna[j], LOW);
      }
    }
    delay(1);
    digitalWrite(fila[i], LOW);
    for (int j = 7; j >= 0; j--) {
      digitalWrite(columna[j], HIGH);
    }
  }

  digitalWrite[fila[0], LOW];
  digitalWrite[columna[7], HIGH];

  delay(1);
}

//metodo encargado de pintar los numeros en la matriz
void pintarNumeros(int decena, int unidad) {
  for (int fila = 0; fila < 8; fila++) {
    for (int columna = 0; columna < 16; columna++) {
      if (columna < 8) {
        matrizLogica[fila][columna] = numeros[decena][fila][columna];
      } else {
        matrizLogica[fila][columna] = numeros[unidad][fila][columna - 8];
      }
    }
  }
}

//metodo que se encarga de trasladar el mensaje de la matriz mensaje a la matrizLogica
void pintarMensaje(int posicion) {
  for (int fila = 0; fila < 8; fila++) {
    for (int columna = 0; columna < 16; columna++) {
      matrizLogica[fila][columna] = mensaje[posicion - columna][fila];
    }
  }
}

//voltea un segmento de la matriz logica para que se pueda pintar
void voltearSegmento() {
  for (int fila = 0; fila < 8; fila++) {
    for (int columna = 0; columna < 8; columna++) {
      if (matrizLogica[fila][columna] > 0) {
        matrizLogicaDriver[columna][7 - fila] = 1;
      } else {
        matrizLogicaDriver[columna][7 - fila] = 0;
      }
    }
  }
}

//limpia la matriz sin driver
void limpiarMatrizSinDriver() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(fila[i], LOW);
    digitalWrite(columna[i], HIGH);
  }
}

//metodo encargado de iniciar el bloque de puntos para el juego
void iniciarBloque() {
  for (int fila = 0; fila < 4; fila++) {
    for (int columna = 0; columna < 16; columna++) {
      matrizLogica[fila][columna] = 2;
    }
  }
}

//inicia la barra del juego
void iniciarBarra() {
  matrizLogica[7][6] = 3;
  matrizLogica[7][7] = 3;
  matrizLogica[7][8] = 3;
  matrizLogica[7][9] = 3;
  matrizLogica[7][10] = 3;
}

//inicia un nuevo punto en representacion de la pelota
void iniciarPelota() {
  int rm = random(6, 11);
  for (int fila = 0; fila < 8; fila++) {
    for (int columna = 0; columna < 16; columna++) {
      if (matrizLogica[fila][columna] == 1) {
        matrizLogica[fila][columna] = 0;
      }
    }
  }
  matrizLogica[6][rm] = 1;
}

//genera una nueva direccion del desplazamiento
void valorDesplazamiento() {
  int rm = random(15, 18);
  if (desplazamientoArriba) {
    posicionNumero = rm * -1;
  } else {
    posicionNumero = rm;
  }
}

//metodo encargado para limpiar la matriz logica
void limpiarMatrizLogica() {
  for (int fila = 0; fila < 8; fila++) {
    for (int columna = 0; columna < 16; columna++) {
      matrizLogica[fila][columna] = 0;
    }
  }
}

//metodo encargado del desplazamiento del punto dentro de la matriz de forma ascendente
void desplazamientoArribaPelota() {
  for (int fila = 0; fila < 8; fila++) {
    for (int columna = 0; columna < 16; columna++) {
      if (matrizLogica[fila][columna] == 1) {
        if (fila == 0) {
          desplazamientoArriba = false;  // cambia de sentido porque esta en la parte superior
          valorDesplazamiento();
          return;
        } else if (columna == 0) {
          if (posicionNumero == -17) {
            posicionNumero = -15;
          }
        } else if (columna == 15) {
          if (posicionNumero == -15) {
            posicionNumero = -17;
          }
        }
        int numeroSiguiente = rowMajor(fila, columna) + posicionNumero;
        if (colision(numeroSiguiente)) {
          desplazamientoArriba = false;
          valorDesplazamiento();
          return;
        } else {
          setSiguientePosicion(numeroSiguiente);
          matrizLogica[fila][columna] = 0;
        }

        break;
      }
    }
  }
}

//metodo encargado del desplazamiento del punto dentro de la matriz de forma descendente
void desplazamientoAbajoPelota() {
  for (int fila = 7; fila >= 0; fila--) {
    for (int columna = 0; columna < 16; columna++) {
      if (matrizLogica[fila][columna] == 1) {
        if (fila == 6) {
          if (matrizLogica[fila + 1][columna] == 3) {
            desplazamientoArriba = true;
            valorDesplazamiento();
            return;
          }
        } else if (fila == 7) {
          matrizLogica[fila][columna] = 0;
          perdedor();
          return;
        } else if (columna == 0) {
          if (posicionNumero == 15) {
            posicionNumero = 17;
          }

        } else if (columna == 15) {
          if (posicionNumero == 17) {
            posicionNumero = 15;
          }
        }
        int numeroSiguiente = rowMajor(fila, columna) + posicionNumero;
        if (colision(numeroSiguiente)) {
          desplazamientoArriba = true;
          valorDesplazamiento();
          return;
        } else {
          setSiguientePosicion(numeroSiguiente);
          matrizLogica[fila][columna] = 0;
        }
        break;
      }
    }
  }
}

//metodo encargado de devolver la ubicacion de la pelota en la matriz
int rowMajor(int fila, int columna) {
  return (fila * 16) + columna;
}

//metodo encargado de verificar la colision con los bloques
bool colision(int numeroSiguiente) {
  int fila = numeroSiguiente / 16;
  int columna = numeroSiguiente % 16;

  if (matrizLogica[fila][columna] == 2) {
    if (columna % 2 == 0) {
      matrizLogica[fila][columna] = 0;
      matrizLogica[fila][columna + 1] = 0;

    } else {
      matrizLogica[fila][columna - 1] = 0;
      matrizLogica[fila][columna] = 0;
    }


    puntaje++;
    Serial.print("Puntaje: ");
    Serial.println(puntaje);
    return true;
  }
  return false;
}

//Como yo :v jajajajajaja metodo encargado de restar vidas e iniciar nuevo juego
void perdedor() {
  for (int columna = 15; columna > 0; columna--) {
    matrizLogica[7][columna] = 0;
  }
  iniciarBarra();
  iniciarPelota();
  vidas--;
  Serial.println(vidas);
}

//incerta el punto en el siguiente espacio correspondiente
void setSiguientePosicion(int numero) {
  int fila = numero / 16;
  int columna = numero % 16;
  matrizLogica[fila][columna] = 1;
}

//metodo encargado del desplazamiento a la izquierda de la barra del juego
void desplazamientoIzquierdaBarra() {
  int posicion = 0;
  for (int columna = 0; columna < 16; columna++) {
    if (matrizLogica[7][columna + 1] == 3) {
      posicion = columna;
      break;
    }
  }
  for (int columna = 0; columna < 16; columna++) {
    matrizLogica[7][columna] = 0;
  }

  for (int columna = posicion; columna < posicion + 5; columna++) {
    matrizLogica[7][columna] = 3;
  }
}

//metodo encargado del desplazamiento a la derecha de la barra del juego
void desplazamientoDerechaBarra() {
  int posicion = 0;
  for (int columna = 15; columna >= 0; columna--) {
    if (matrizLogica[7][columna - 1] == 3) {
      posicion = columna;
      break;
    }
  }
  for (int columna = 15; columna >= 0; columna--) {
    matrizLogica[7][columna] = 0;
  }

  for (int columna = posicion; columna > posicion - 5; columna--) {
    matrizLogica[7][columna] = 3;
  }
}

//metodo encargado de pintar el porcentaje en la matrizLogica
void pintarPotenciometro(int valor){
  for(int fila = 7; fila >= 0; fila-- ){
    for(int columna = 0; columna < 16; columna++){
      if(columna < valor){
        matrizLogica[fila][columna] = 4;
      }
    }
  }
}

//permite cambiar a una escala de 20 el valor del potenciometro
int parcerPotenciometroMensaje(int potenciometro){
  double valor = 725-potenciometro;
  double valorTotal = 475-valor;
  int porcentaje = (valorTotal/475)*20;
  return porcentaje;
}

//permite cambiar a una escala de 16 el valor del potenciometro
int parcerPorcentajeMatriz(int potenciometro){
  double valor = 725-potenciometro;
  double valorTotal = 475-valor;
  int porcentaje = (valorTotal/475)*16;
  return porcentaje;
}

void loop() {

  int potenciometro = analogRead(A4);
  potenciometro = map(potenciometro, 0, 1023, 250, 725);
  
  //estado de inicio
  if (estado == 0) {
    if (digitalRead(A2) == LOW) {
      delay(1000);
      if (digitalRead(A2) == LOW) {
        delay(1000);
        if (digitalRead(A2) == LOW) {
          delay(1000);
          if (digitalRead(A2) == LOW) {
            iniciarPelota();
            iniciarBarra();
            iniciarBloque();
            estado = 1;
          }
        }
      }
    }

    //boton derecha controla direccion de lectura
    if (digitalRead(A1) == LOW) {
      derecha = 1;
    }

    if (digitalRead(A1) == HIGH & derecha == 1) {
      direccionMensaje = true;
      derecha = 0;
    }

    //button izquierda controla direccion de lectura
    if (digitalRead(A0) == LOW) {
      izquierda = 1;
    }

    if (digitalRead(A0) == HIGH & izquierda == 1) {
      direccionMensaje = false;
      izquierda = 0;
    }
    if(contador % 20 == 0){
      Serial.println(potenciometro);
      Serial.print(parcerPotenciometroMensaje(potenciometro));
    }
    
    //velocidad del mensaje dada por el potenciometro
    if (contador % parcerPotenciometroMensaje(potenciometro) == 0) {
      limpiarMatrizLogica();
      pintarMensaje(posicionVisor);
      if (direccionMensaje) {
        posicionVisor++;
        if (posicionVisor > 198) {
          posicionVisor = 15;
        }

      } else {
        posicionVisor--;
        if (posicionVisor < 15) {
          posicionVisor = 198;
        }
      }
      contador = 0;
    }
  }  //estado de juego
  else if (estado == 1) {

    if (vidas > 0) {

      //velocidad de la pelota
      if (contador % 12 == 0) {
        if (desplazamientoArriba) {
          desplazamientoArribaPelota();
        }
        if (!desplazamientoArriba) {
          desplazamientoAbajoPelota();
        }
        contador = 0;
      }

      //inicia pausa
      if (digitalRead(A2) == LOW) {
        delay(1000);
        if (digitalRead(A2) == LOW) {
          delay(1000);
          if (digitalRead(A2) == LOW) {
            delay(1000);
            if (digitalRead(A2) == LOW) {
              guardarTemporal();
              estado = 2;
            }
          }
        }
      }

      //iniciarBarra();
      if (digitalRead(A0) == LOW) {
        izquierda = 1;
      }
      if (digitalRead(A0) == HIGH & izquierda == 1) {
        desplazamientoIzquierdaBarra();
        izquierda = 0;
      }

      if (digitalRead(A1) == LOW) {
        derecha = 1;
      }
      if (digitalRead(A1) == HIGH & derecha == 1) {
        desplazamientoDerechaBarra();
        derecha = 0;
      }
    }else{
      estado = 4;
    }

  //estado pausa
  } else if (estado == 2) {
    limpiarMatrizLogica();
    pintarNumeros(0, vidas);
    if (digitalRead(A2) == LOW) {
      delay(1000);
      if (digitalRead(A2) == LOW) {
        delay(1000);
        estado = 3;
        if (digitalRead(A2) == LOW) {
          delay(1000);
          if (digitalRead(A2) == LOW) {
            estado = 4;
          }
        }
      }
      
    }
    //iniciarBarra();
    if (digitalRead(A0) == LOW) {
      izquierda = 1;
    }
    if (digitalRead(A0) == HIGH & izquierda == 1) {
      estado = 5;
      izquierda = 0;
    }

    if (digitalRead(A1) == LOW) {
      derecha = 1;
    }
    if (digitalRead(A1) == HIGH & derecha == 1) {
      estado = 6;
      derecha = 0;
    }

  //estado reanudar juego
  } else if (estado == 3) {
    limpiarMatrizLogica();
    setTemporal();
    estado = 1;

  //estado salir del juego
  } else if (estado == 4) {
    limpiarMatrizLogica();
    pintarNumeros(puntaje / 10, puntaje % 10);
    if (contador % 150 == 0) {
      limpiarMatrizLogica();
      estado = 0;
      puntaje = 0;
      vidas = 3;
      contador = 0;
    }
    //estado ver puntaje
  } else if(estado == 5){
      limpiarMatrizLogica();
      pintarNumeros(puntaje/10, puntaje%10);
      if (digitalRead(A0) == LOW) {
        izquierda = 1;
      }
      if (digitalRead(A0) == HIGH & izquierda == 1) {
        estado = 2;
        izquierda = 0;
      }

      
    //estado subir potencia
  } else if(estado == 6){
    if(contador%30 == 0){
      limpiarMatrizLogica();
      pintarPotenciometro(parcerPorcentajeMatriz(potenciometro));
      contador = 0;
    }
    if (digitalRead(A1) == LOW) {
        derecha = 1;
      }
      if (digitalRead(A1) == HIGH & derecha == 1) {
        estado = 2;
        derecha = 0;
      }
  }

  delay(20);
  contador++;
  pintarMatrizConDriver();
  pintarMatrizSinDriver();
}
