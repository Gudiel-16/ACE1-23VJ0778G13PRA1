#include "LedControl.h"

LedControl ledControl = LedControl(51, 53, 49, 1); // LedControl(DIN, CLK, CS / LOAD, # de dispositivos (cantidad de modulos) de 1 a 8)

//////////////////////////////////////////////////////////////  VARIABLES LETRERO  ////////////////////////////////////////////////////////////// 
// PINES MATRIZ SIN DRIVER
int filas[] = {24, 22, 2, 3, 4, 5, 6, 7}; // filas encienden con 0 (--> +y)
int columnas[] = {8, 9, 10, 11, 12, 13, 23, 25}; // columnas encienden con 1 (--> +x)

int DIR_LEFT = 52, DIR_RIGHT = 50, K = 48, DISP = 46;
boolean direccionLetrero = true; // true -> derecha
int velocidadLetrero = 200;
int posicionControlador = 123; // 116 + 8 tablero = 124 => 123 porque empieza en 0

long int t0 = millis();
long int t1 = millis();

String letrero[8] {
  "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
  "00000011110011000010000000000111110000011110111101111011110001000000011110100101111011110000001000101110111011101000",
  "01111010010100100110000010010000100000000010100100001000010010011111010010100101001010010000000101000100010001000100",
  "01001011110100101010111010010000100111011110100101111000100100010001011110100101111010010000000010000100010001000010",
  "01111011000111100010000010010100100000010000100101000000010100011111011000100101000010010000000010000100010001000010",
  "01000010100100100010000010010100100000010000100101000010010010000001010100100101000010010000000101000100010001000100",
  "01000010010100100010000001100111100000011110111101111001100001000001010010111101000011110000001000101110111011101000",
  "01000000000000000000000000000000000000000000000000000000000000011110000000000000000000000000000000000000000000000000"
};

//////////////////////////////////////////////////////////////  VARIABLES MOSTRAR NUMEROS ////////////////////////////////////////////////////////////// 

// PUNTUACION MATRIZ SIN DRIVER (cada matriz forma un numero, solo estan del 0 al 4)
int numero0[8][8] = { {0, 0, 0, 0, 0, 0, 0, 0}, 
                      {0, 0, 0, 1, 1, 0, 0, 0}, 
                      {0, 0, 1, 0, 0, 1, 0, 0}, 
                      {0, 0, 1, 0, 0, 1, 0, 0},
                      {0, 0, 1, 0, 0, 1, 0, 0}, 
                      {0, 0, 1, 0, 0, 1, 0, 0}, 
                      {0, 0, 0, 1, 1, 0, 0, 0}, 
                      {0, 0, 0, 0, 0, 0, 0, 0} };

int numero1[8][8] = { {0, 0, 0, 0, 0, 0, 0, 0}, 
                      {0, 0, 0, 0, 1, 1, 0, 0}, 
                      {0, 0, 0, 1, 0, 1, 0, 0}, 
                      {0, 0, 1, 0, 0, 1, 0, 0},
                      {0, 0, 0, 0, 0, 1, 0, 0}, 
                      {0, 0, 0, 0, 0, 1, 0, 0}, 
                      {0, 0, 0, 0, 0, 1, 0, 0}, 
                      {0, 0, 0, 0, 0, 0, 0, 0} };

int numero2[8][8] = { {0, 0, 0, 0, 0, 0, 0, 0}, 
                      {0, 0, 0, 1, 1, 0, 0, 0}, 
                      {0, 0, 1, 0, 0, 1, 0, 0}, 
                      {0, 0, 0, 0, 0, 1, 0, 0},
                      {0, 0, 0, 0, 1, 0, 0, 0}, 
                      {0, 0, 0, 1, 0, 0, 0, 0}, 
                      {0, 0, 1, 1, 1, 1, 0, 0}, 
                      {0, 0, 0, 0, 0, 0, 0, 0} };

int numero3[8][8] = { {0, 0, 0, 0, 0, 0, 0, 0}, 
                      {0, 0, 1, 1, 1, 1, 0, 0}, 
                      {0, 0, 0, 0, 0, 1, 0, 0}, 
                      {0, 0, 0, 0, 1, 0, 0, 0},
                      {0, 0, 0, 0, 0, 1, 0, 0}, 
                      {0, 0, 1, 0, 0, 1, 0, 0}, 
                      {0, 0, 0, 1, 1, 0, 0, 0}, 
                      {0, 0, 0, 0, 0, 0, 0, 0} };

int numero4[8][8] = { {0, 0, 0, 0, 0, 0, 0, 0}, 
                      {0, 0, 0, 0, 1, 1, 0, 0}, 
                      {0, 0, 0, 1, 0, 1, 0, 0}, 
                      {0, 0, 1, 0, 0, 1, 0, 0},
                      {0, 0, 1, 1, 1, 1, 1, 0}, 
                      {0, 0, 0, 0, 0, 1, 0, 0}, 
                      {0, 0, 0, 0, 0, 1, 0, 0}, 
                      {0, 0, 0, 0, 0, 0, 0, 0} };

// PUNTUACION MATRIZ CON DRIVER (Cada fila forma un numero, solo estan del 0 al 4)
byte puntuacion[5][8] {
  {B00000000, B00011000, B00100100, B00100100, B00100100, B00100100, B00011000, B00000000},//0
  {B00000000, B00001100, B00010100, B00100100, B00000100, B00000100, B00000100, B00000000},//1
  {B00000000, B00011000, B00100100, B00000100, B00001000, B00010000, B00111100, B00000000},//2
  {B00000000, B00111100, B00000100, B00001000, B00000100, B00100100, B00011000, B00000000},//3
  {B00000000, B00001100, B00010100, B00100100, B00111110, B00000100, B00000100, B00000000} //4
};

//////////////////////////////////////////////////////////////  VARIABLES LOGICA AVION  ////////////////////////////////////////////////////////////// 
byte tableroJuego[8][16] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0 } };

byte tableroBalas[8][16] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

int xAvion = 0;
int yAvion = 0;
long int t00 = 0;
long int t11 = 0;

void setup() {
  inizializarMatrizSinDriver();
  inicializarMatrizDriver();
  pinMode(DIR_LEFT, INPUT); // DIRECCION IZQUIERDA
  pinMode(DIR_RIGHT, INPUT); // DIRECCION DERECHA
  pinMode(K, INPUT); // K
  pinMode(DISP, INPUT); // DISPARO
}

void loop() {

//  if (digitalRead(DISP) == HIGH) {
//    direccionLetrero = !direccionLetrero;
//  }
      
//  delay(velocidadLetrero);
  
  if (direccionLetrero == false) {
    moverCartelIZQ();
  } else  {
    moverCartelDCH();
  }

//  mostrarPunteo(1,4); // los muestra en -> (matriz sin driver, matriz con driver)

//  jugar();

}

void inizializarMatrizSinDriver() {

  // Inicializando pines
  for (int i = 0; i < 8; i++) {
    pinMode(columnas[i], OUTPUT);
  }
  for (int i = 0; i < 8; i++) {
    pinMode(filas[i], OUTPUT);
  }

  // Limpiar Matriz
  for (int i = 0; i < 8; i++) {
    digitalWrite(columnas[i], LOW);
  }
  for (int i = 0; i < 8; i++) {
    digitalWrite(filas[i], HIGH);
  }
}

void inicializarMatrizDriver() {
  ledControl.shutdown(0, false); // shutdown(# de dispositivo (indice), estado)
  ledControl.setIntensity(0, 15); // setIntensity(# de dispositivo (indice), intensidad de brillo del led de 0 a 15)
  ledControl.clearDisplay(0); // clearDisplay(# de dispositivo), apaga todos los leds de la pantalla
}

void moverCartelDCH() {

    encenderMatrizDriver(0, posicionControlador - 8);
    encenderMatrizDriver(1, posicionControlador - 7);
    encenderMatrizDriver(2, posicionControlador - 6);
    encenderMatrizDriver(3, posicionControlador - 5);
    encenderMatrizDriver(4, posicionControlador - 4);
    encenderMatrizDriver(5, posicionControlador - 3);
    encenderMatrizDriver(6, posicionControlador - 2);
    encenderMatrizDriver(7, posicionControlador - 1);

//    delay(1);

    encenderMatriz(posicionControlador, posicionControlador + 7);

    if (posicionControlador == -7) {
      posicionControlador = 123;
    }

    t1 = millis();
    if(t1-t0 >= velocidadLetrero){
      posicionControlador--;
      t0 = millis();
    }
}

void moverCartelIZQ() {
  
    encenderMatrizDriver(0, posicionControlador - 8);
    encenderMatrizDriver(1, posicionControlador - 7);
    encenderMatrizDriver(2, posicionControlador - 6);
    encenderMatrizDriver(3, posicionControlador - 5);
    encenderMatrizDriver(4, posicionControlador - 4);
    encenderMatrizDriver(5, posicionControlador - 3);
    encenderMatrizDriver(6, posicionControlador - 2);
    encenderMatrizDriver(7, posicionControlador - 1);

    encenderMatriz(posicionControlador, posicionControlador + 7);

    if (posicionControlador == 126) {
      posicionControlador = -7;
    }

    t1 = millis();
    if(t1-t0 >= velocidadLetrero){
      posicionControlador++;
      t0 = millis();
    }
}

void encenderMatrizDriver(int valorColumna, int valor) {
  for (int valorFila = 0; valorFila < 8; valorFila++) { // recorremos fila de la matriz
    if (letrero[valorFila].charAt(valor) == '1') { // al intentar ingresar a la posicion: letrero[0].charAt(39), no muestra error, ya que maneja internamente la excepcion
      ledControl.setLed(0, valorFila, valorColumna, true); // (direccion, fila, columna, true = encendido)
    } else {
      ledControl.setLed(0, valorFila, valorColumna, false);
    }
  }
}

void encenderMatriz(int inicio, int fin) {
  int aux = 0;
  for (int posFila = 0; posFila < 8; posFila++) { // recorremos filas de matriz
    
    digitalWrite(filas[posFila], LOW); // fila a utilizar

    for (int j = inicio; j <= fin; j++) { // de cada fila del letro se van leyendo 8 caraceres a la vez, ej: (31->38); (30->37); (29->36)
      if (letrero[posFila].charAt(j) == '1') {
        digitalWrite(columnas[aux], HIGH);
      } else {
        digitalWrite(columnas[aux], LOW);
      }
      aux++;
    }

    delay(1);
//    delayMicroseconds(1000);
    digitalWrite(filas[posFila], HIGH);
    aux = 0;
  }
}

//////////////////////////////////////////////////////////////  PUNTUACION ////////////////////////////////////////////////////////////// 

void mostrarPunteo(int punteo1, int punteo2) {

  // MOSTRAR PUNTEO MATRIZ CON DRIVER
  for (int i = 0; i < 8; i++) {
    ledControl.setRow(0, i, puntuacion[punteo2][i]);
  }

  // MOSTRAR PUNTEO MATRIZ SIN DRIVER
  for (int i = 0; i < 8; i++) {
    if (punteo1 == 0) {
      mostrarPuntuacion(numero0);
    } else if (punteo1 == 1) {
      mostrarPuntuacion(numero1);
    } else if (punteo1 == 2) {
      mostrarPuntuacion(numero2);
    } else if (punteo1 == 3) {
      mostrarPuntuacion(numero3);
    } else if (punteo1 == 4) {
      mostrarPuntuacion(numero4);
    }
  }

  ledControl.clearDisplay(0);
}

void mostrarPuntuacion(int numero[8][8]) { // numero en matriz sin driver
  for (int i = 0; i < 8; i++) {
    digitalWrite(columnas[i], HIGH);
    for (int j = 0; j < 8; j++) {
      if (numero[j][i] == 1) {
        digitalWrite(filas[j], LOW);
      }
    }
    delay(1);
    digitalWrite(columnas[i], LOW);
    for (int j = 0; j < 8; j++) {
      digitalWrite(filas[j], HIGH);
    }
  }
}

////////////////////////////////////////////////////////////// AVION ////////////////////////////////////////////////////////////// 
void pintarAvion() {
//    tableroJuego[yAvion][xAvion+2]   = 1;
//    tableroJuego[yAvion+1][xAvion+2] = 1;
//    tableroJuego[yAvion+1][xAvion+1] = 1;
//    tableroJuego[yAvion+1][xAvion]   = 1;
//
    tableroJuego[yAvion][xAvion]     = 1;
    tableroJuego[yAvion+1][xAvion]   = 1;
    tableroJuego[yAvion+1][xAvion+1] = 1;
//    tableroJuego[yAvion+2][xAvion+1] = 1; // bala
    tableroJuego[yAvion+1][xAvion+2] = 1;
}

void borrarAvion() {
//    tableroJuego[yAvion][xAvion+2]   = 0;
//    tableroJuego[yAvion+1][xAvion+2] = 0;
//    tableroJuego[yAvion+1][xAvion+1] = 0;
//    tableroJuego[yAvion+1][xAvion]   = 0;
    tableroJuego[yAvion][xAvion]     = 0;
    tableroJuego[yAvion+1][xAvion]   = 0;
    tableroJuego[yAvion+1][xAvion+1] = 0;
    tableroJuego[yAvion+1][xAvion+2] = 0;
}

void pintarBala(){
  tableroJuego[yAvion+2][xAvion+1] = 1;
  
}

void borrarBala(){
  tableroJuego[yAvion+2][xAvion+1] = 0;
}

void mostrarMatriz() {
    // Con driver
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            ledControl.setLed(0, i, j, tableroJuego[i][j] == 1 ? true : false);
    // Sin driver
    for (int k = 0; k < 3; k++)
        for (int i = 0; i < 8; i++)
            for (int j = 8; j < 16; j++)
                if (tableroJuego[i][j] == 1)
                    pintarLED(j - 8, i);
                else
                    delayMicroseconds(50);
}

void pintarLED(int x, int y) {
    digitalWrite(filas[y], LOW);
    digitalWrite(columnas[x], HIGH);
    delayMicroseconds(1100);
    digitalWrite(filas[y], HIGH);
    digitalWrite(columnas[x], LOW);
}

void jugar() {
  
//  borrarBala();
  borrarAvion();
  t11 = millis();
  if ((t11 - t00) >= 400) {
      t00 = millis();
      xAvion++;
  }
  
  if (digitalRead(DISP) == HIGH) {
    pintarBala();
  }
 
  pintarAvion();    
  mostrarMatriz();
}
