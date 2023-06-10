#include "LedControl.h"

LedControl ledControl = LedControl(51, 53, 49, 1); // LedControl(DIN, CLK, CS / LOAD, # de dispositivos (cantidad de modulos) de 1 a 8)

int velocidad = 200;

//////////////////////////////////////////////////////////////  VARIABLES LETRERO  ////////////////////////////////////////////////////////////// 
// PINES MATRIZ SIN DRIVER
int filas[] = {24, 22, 2, 3, 4, 5, 6, 7}; // filas encienden con 0 (--> +y)
int columnas[] = {8, 9, 10, 11, 12, 13, 23, 25}; // columnas encienden con 1 (--> +x)

int DIR_LEFT = 52, DIR_RIGHT = 50, K = 48, DISP = 46;
boolean direccionLetrero = true; // true -> derecha
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
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

byte tableroBalas[8][16] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

int tableroTorres[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;
int puntajes[5] = { -1, -1, -1, -1, -1 } ;

int xAvion = 0;
int yAvion = 0;
long int tAvion0 = 0; // velocidad avion, bala
long int tAvion1 = 0; // velocidad avion, bala
long int tAvionDesplazY0 = 0; // desplazamiento de avion (eje y)
long int tAvionDesplazY1 = 0; // desplazamiento de avion (eje y)
bool direccionAvion = true; // true -> derecha
bool poderDisparar = true; // true -> puede disparar
int nivelJuego = 1;
int vidas = 3;
int puntos = 0;
bool construirNuevasTorres = true; // true -> construye torres
int cantidadTorresPorDestruir = 3;

//////////////////////////////////////////////////////////////  VARIABLES ESTADISTICAS  ////////////////////////////////////////////////////////////// 

byte tableroEstadisticas[8][16] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
                                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

void setup() {
  Serial.begin(9600);
  inizializarMatrizSinDriver();
  inicializarMatrizDriver();
  pinMode(DIR_LEFT, INPUT); // DIRECCION IZQUIERDA
  pinMode(DIR_RIGHT, INPUT); // DIRECCION DERECHA
  pinMode(K, INPUT); // K
  pinMode(DISP, INPUT); // DISPARO
}

void loop() {

//  float var = 2.49;
//  int var2 = round(var);
//  float var3 = 2.51;
//  int var4 = round(var3;
//  Serial.println(var < 2 ? "true": "false");
//  Serial.println(var4);
//  Serial.println("************");

//  if (digitalRead(DISP) == HIGH) {
//    direccionLetrero = !direccionLetrero;
//  }
      
//  delay(velocidad);
  
//  if (direccionLetrero == false) {
//    moverCartelIZQ();
//  } else  {
//    moverCartelDCH();
//  }

//  mostrarPunteo(1,4); // los muestra en -> (matriz sin driver, matriz con driver)

  jugar();

//  visualizarEstadisticas();


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
    if(t1-t0 >= velocidad){
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
    if(t1-t0 >= velocidad){
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

void pintarAvionDirDerecha(){
  
  // DIBUJO AVION DERECHA
  //[yAvion]                         [xAvion-2]
  //[yAvion+1][yAvion+1][yAvion+1]   [xAvion-2][xAvion-1][xAvion]

  if(xAvion < 18){ // parte final del avion
    if((xAvion -2) >= 0) // asignar valores siempre que esten en el rango de la matriz
    {
      tableroJuego[yAvion][xAvion-2]     = 1;
      tableroJuego[yAvion+1][xAvion-2]   = 1;
    }
  }

  if(xAvion < 17){ // parte medio del avion
    if((xAvion -1) >= 0) // asignar valores siempre que esten en el rango de la matriz
    {
      tableroJuego[yAvion+1][xAvion-1] = 1; 
    }
  }

  if(xAvion < 16){ // trompa del avion
    tableroJuego[yAvion+1][xAvion] = 1;    
  }

}

void pintarAvionDirIzquierda() {

  // DIBUJO AVION IZQUIERDA
  //                      [yAvion]                     [xAvion+2]
  //[yAvion+1][yAvion+1][yAvion+1]   [xAvion][xAvion+1][xAvion+2]

  if(xAvion > -3){ // parte final del avion
    if((xAvion + 2) <= 15){ // asignar valores siempre que esten en el rango de la matriz
      tableroJuego[yAvion][xAvion+2]   = 1;
      tableroJuego[yAvion+1][xAvion+2] = 1;
    }
  }

  if(xAvion > -2){ // parte media del avion
    if((xAvion + 1) <= 15){ // asignar valores siempre que esten en el rango de la matriz
      tableroJuego[yAvion+1][xAvion+1] = 1;
    }
  }

  if(xAvion > -1){ // trompa del avion
    tableroJuego[yAvion+1][xAvion]   = 1;
  }

}

void borrarAvionDirDerecha(){
  
  if(xAvion < 18){ // parte final del avion
    if((xAvion -2) >= 0) // asignar valores siempre que esten en el rango de la matriz
    {
      tableroJuego[yAvion][xAvion-2]     = 0;
      tableroJuego[yAvion+1][xAvion-2]   = 0;
    }
  }

  if(xAvion < 17){ // parte media del avion
    if((xAvion -1) >= 0) // asignar valores siempre que esten en el rango de la matriz
    {
      tableroJuego[yAvion+1][xAvion-1] = 0; 
    }
  }

  if(xAvion < 16){ // trompa del avion
    tableroJuego[yAvion+1][xAvion] = 0;    
  }
}

void borrarAvionDirIzquierda() {

  if(xAvion > -3){ // parte final del avion
    if((xAvion + 2) <= 15){ // asignar valores siempre que esten en el rango de la matriz
      tableroJuego[yAvion][xAvion+2]   = 0;
      tableroJuego[yAvion+1][xAvion+2] = 0;
    }
  }

  if(xAvion > -2){ // parte media del avion
    if((xAvion + 1) <= 15){ // asignar valores siempre que esten en el rango de la matriz
      tableroJuego[yAvion+1][xAvion+1] = 0;
    }
  }

  if(xAvion > -1){ // trompa del avion
    tableroJuego[yAvion+1][xAvion]   = 0;
  }

}

void pintarBala(){
  if(direccionAvion){ // avion va hacia derecha
    if((xAvion - 1) >= 0 && xAvion <= 16){ // Dispara cuando la parte media esta dentro de la matriz 
      tableroJuego[yAvion+2][xAvion-1] = 1;
      tableroBalas[yAvion+2][xAvion-1] = 1; // guardamos posicion de bala en su tablero
    }
  }else{
    if((xAvion + 1) <= 15 && xAvion >= -1){ // Dispara cuando la parte media esta dentro de la matriz
      tableroJuego[yAvion+2][xAvion+1] = 1;
      tableroBalas[yAvion+2][xAvion+1] = 1; // guardamos posicion de bala en su tablero
    }
  }  
}

void pintarNuevaPosicionBalas(){

  // Recorremos matriz de balas, cuando encontramos una, solo la desplazamos hacia abajo
  for (int i = 7; i > (yAvion+1); i--){
    for (int j = 0; j < 16; j++){
      if (tableroBalas[i][j] == 1){ // si hay bala

        if(i == 7){ // cuando se este en la ultima fila de la matriz
          if(tableroTorres[j] == 1) { // si hay torre

            // torre destruida
            tableroTorres[j] = 0; // quitamos torre
            cantidadTorresPorDestruir--;

            // punto
            puntos++;
            if(puntos % 5 == 0){
              vidas++;
              Serial.println("5 puntos, vida extra");
            }

            if(cantidadTorresPorDestruir == 0 ) { // siguiente nivel
              Serial.println("Siguiente nivel");
              nivelJuego++;
              mostrarNivel();
              construirNuevasTorres = true;
              yAvion = 0; // reseteamos altura de avion
              xAvion = 0;
              tAvionDesplazY0 = millis(); // reseteamos tiempo para que se desplaze a los 2s nuevamente
            }
          }
        } 

        // borramos bala en posicion actual
        tableroBalas[i][j] = 0;
        tableroJuego[i][j] = 0;

        // para que no haga el desplazamiento cuando este en la ultima fila de la matriz
        if(i < 7){
          tableroBalas[i+1][j] = 1;
          tableroJuego[i+1][j] = 1;
        }

        // la bala ya esta en la ultima fila de la matriz
        if(i == 7){ 
          poderDisparar = true;
        }        
      }
    }
  }        
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
//  Serial.print(xAvion); 

  // Cambia a direccion izquierda el avion
  if (digitalRead(DIR_LEFT) == HIGH) {
    borrarAvionDirDerecha();
    direccionAvion = false;
  }

  // Cambia a direccion derecha el avion
  if (digitalRead(DIR_RIGHT) == HIGH) {
    borrarAvionDirIzquierda();
    direccionAvion = true;
  }

  if(direccionAvion) {    
    jugarDirDerecha();  
  } 
  else {
    jugarDirIzquierda();
  }

  if(construirNuevasTorres){
    construirTorres();
    construirNuevasTorres = false;
  }
  
}

void jugarDirDerecha(){
  
  borrarAvionDirDerecha(); 
//  pintarNuevaPosicionBalas();
  
  // velocidad de avion y bala
  tAvion1 = millis();
  if ((tAvion1 - tAvion0) >= velocidad) {
      pintarNuevaPosicionBalas();
      tAvion0 = millis();
      xAvion = (xAvion + 1) % 18; // 18: saldra dos pocisiones del lado derecho de la matriz, para hacer el efecto que va saliendo el avion
  }
  
  desplazamientoAvionY();
  verificarChoque();
    
  if (digitalRead(DISP) == HIGH) {
    if(poderDisparar){
      pintarBala();
      poderDisparar = false;
    }
  }
  
  pintarAvionDirDerecha();   
  mostrarMatriz();

}

void jugarDirIzquierda(){
  
  borrarAvionDirIzquierda(); 
//  pintarNuevaPosicionBalas();

  // velocidad de avion y bala
  tAvion1 = millis();
  if ((tAvion1 - tAvion0) >= velocidad) {
      pintarNuevaPosicionBalas();
      tAvion0 = millis();
      xAvion--;
      if(xAvion < -2) { // -2: saldra dos pocisiones del lado izquierdo de la matriz, para hacer el efecto que va saliendo el avion
        xAvion = 15;
      }
  }

  desplazamientoAvionY();
  verificarChoque();
  
  if (digitalRead(DISP) == HIGH) {
    if(poderDisparar){
      pintarBala();
      poderDisparar = false;
    }
  }
  
  pintarAvionDirIzquierda();   
  mostrarMatriz();
}

void desplazamientoAvionY(){
  tAvionDesplazY1 = millis();
  if ((tAvionDesplazY1 - tAvionDesplazY0) >= 2000) { // se desplaza cada 2 segundos
    tAvionDesplazY0 = millis();
    yAvion++;
  }
  
  if(yAvion == 7){ // llego a la parte baja del tablero
    avionLlegoAlFinal();
  }
}

void construirTorres(){

  for(int i = 0; i < 16; i++){
    tableroTorres[i] = 0; // reseteamos
  }

  cantidadTorresPorDestruir = nivelJuego + 2;

  if (cantidadTorresPorDestruir > 16){ // la cantidadmaxima de torres a generar es de 16 (0 al 15)
    cantidadTorresPorDestruir = 16;
  }
  
  for(int i = 0; i < cantidadTorresPorDestruir; i++){
    int posicionTorreEnTablero = random(0, 16); // (min, max-1)
    int alturaTorre = random(1, 5); // (min, max-1)

    if(tableroTorres[posicionTorreEnTablero] != 1){ // aun no hay torre en esa posicion (ya que random las genera en la misma posicion aveces)
      
      tableroTorres[posicionTorreEnTablero] = 1; // con 1 indicamos que hay torre en esa posicion
//      Serial.println(posicionTorreEnTablero);
  
      for(int j = 0; j < 16; j++){ // recorreo matriz de forma horizontal
        if(j == posicionTorreEnTablero){
          for(int k = 0; k < alturaTorre; k++){ // recorro para formar altura de torre
            tableroJuego[7-k][posicionTorreEnTablero] = 1; // [pintando para arriba][columna]
          }
        }
      } 
    }else {
      i--;  // para que calcule otro random 
    }
  }
}

void verificarChoque(){

  if(direccionAvion){
    
    if(xAvion < 18){ // parte final del avion
      if((xAvion -2) >= 0) // asignar valores siempre que esten en el rango de la matriz
      {
        if(tableroJuego[yAvion][xAvion-2] == 1 || tableroJuego[yAvion+1][xAvion-2] == 1){
          Serial.println("choque parte final del avion");
          
        }
      }
    }
  
    if(xAvion < 17){ // parte medio del avion
      if((xAvion -1) >= 0) // asignar valores siempre que esten en el rango de la matriz
      {
        if(tableroJuego[yAvion+1][xAvion-1] == 1){
          Serial.println("choque parte medio del avion");
          avionChoco();
        }
      }
    }
  
    if(xAvion < 16){ // trompa del avion
      if(tableroJuego[yAvion+1][xAvion] == 1){
          Serial.println("choque trompa del avion");
          avionChoco();
      }
    }
  }else {
    if(xAvion > -3){ // parte final del avion
      if((xAvion + 2) <= 15){ // asignar valores siempre que esten en el rango de la matriz
        if(tableroJuego[yAvion][xAvion+2] == 1 || tableroJuego[yAvion+1][xAvion+2] == 1){
          Serial.println("choque parte final del avion");
          avionChoco();
        }
      }
    }
  
    if(xAvion > -2){ // parte media del avion
      if((xAvion + 1) <= 15){ // asignar valores siempre que esten en el rango de la matriz
        if(tableroJuego[yAvion+1][xAvion+1] == 1){
          Serial.println("choque parte medio del avion");
          avionChoco();
        }        
      }
    }
  
    if(xAvion > -1){ // trompa del avion
      if(tableroJuego[yAvion+1][xAvion] == 1){
        Serial.println("choque trompa del avion");
        avionChoco();
      }
    }
  }
}

void avionChoco(){
  vidas--;
  if(vidas == 0){
    guardarPuntaje();
    resetCuandoNoHayVidas();
//    Serial.println("Sin vidas"); // MOSTRAR MENSAJE INICIAL
  }else{
    yAvion = yAvion - 2;
  }
}

void avionLlegoAlFinal(){ // es ejecutado desde el metodo desplazamientoAvionY, cuando llega hasta abajo del tablero
  vidas--;
  if(vidas == 0){
    guardarPuntaje();
    resetCuandoNoHayVidas();
//    Serial.println("Sin vidas"); // MOSTRAR MENSAJE INICIAL
  }else{
    yAvion = yAvion - 3;
  }
}

void resetCuandoNoHayVidas(){
  puntos = 0;
  vidas = 3;
  xAvion = 0;
  yAvion = 0;
  nivelJuego = 1;
}

void mostrarNivel() { // Mostrar nivel por 2 segundos

  unsigned long tiempo1 = millis();
  unsigned long tiempo2 = millis();

  while (true) {
//    Serial.println(nivelJuego); // aca mostrar numero
    tiempo1 = millis();
    if (tiempo1 >= (tiempo2 + 2000)) {
      tiempo1 = 0;
      break;
    }
  }
}

void guardarPuntaje(){
  // [posVieja,x,x,x,PosNueva]
  bool flatArrayLleno = true;
  for(int i = 0; i < 5; i++){ // recorremos array puntaje
    if(puntajes[i] == -1){ // no hay nada en esa posicion
      puntajes[i] = puntos;
      flatArrayLleno = false;
      break;
    }
  }

  if(flatArrayLleno){ // array lleno, hay que desplazar posiciones y guardar el punteo nuevo
    for(int i = 0; i < 4; i++){
      puntajes[i] = puntajes[i+1];
    }
    puntajes[4] = puntos;
  }

  Serial.println("PUNTAJES");
  for(int i = 0; i < 5; i++){
    Serial.println(puntajes[i]);
  }  
}

void visualizarEstadisticas(){
  // limpiando matriz
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 16; j++){
        tableroEstadisticas[i][j] = 0;
    }
  }

  float promedio = 0;
  float valores[5] = { 0, 0, 0, 0, 0 };
  int alturaBarra[5] = { 0, 0, 0, 0, 0 };

  // calculado promedio
  for(int i = 0; i < 5; i++){
    if(puntajes[i] > -1){
      promedio = promedio + (float)puntajes[i];
    }
  }

  // valores para altura de cada barra de grafica
  for(int i = 0; i < 5; i++){
    if(puntajes[i] > -1){
      valores[i] = ((float)puntajes[i] / promedio) * 8;
    }    
  }

  // redondeando valores
  for(int i = 0; i < 5; i++){
    if(valores[i] > 0 && valores[i] <= 1){
      alturaBarra[i] = 1; 
    }
    else if(valores[i] > 1 && valores[i] <= 2){
      alturaBarra[i] = 2; 
    }
    else if(valores[i] > 2 && valores[i] <= 3){
      alturaBarra[i] = 3; 
    }
    else if(valores[i] > 3 && valores[i] <= 4){
      alturaBarra[i] = 4; 
    }
    else if(valores[i] > 4 && valores[i] <= 5){
      alturaBarra[i] = 5; 
    }
    else if(valores[i] > 5 && valores[i] <= 6){
      alturaBarra[i] = 6; 
    }
    else if(valores[i] > 6 && valores[i] <= 7){
      alturaBarra[i] = 7; 
    }
    else if(valores[i] > 7 && valores[i] <= 8){
      alturaBarra[i] = 8; 
    }
    else{
      alturaBarra[i] = 0; 
    }
  }

  // pintando en matriz
  for(int i = 0; i < 5; i++){
      if(alturaBarra[i] > 0){
        for(int j = 0; j < alturaBarra[i]; j++){
          int desplazamientoEjex = i * 3;
          tableroEstadisticas[7-j][desplazamientoEjex+1] = 1;
          tableroEstadisticas[7-j][desplazamientoEjex+2] = 1;
        }
      }
  }

  mostrarMatrizEstadistica();
}

void mostrarMatrizEstadistica() {
    // Con driver
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            ledControl.setLed(0, i, j, tableroEstadisticas[i][j] == 1 ? true : false);
    // Sin driver
    for (int k = 0; k < 3; k++)
        for (int i = 0; i < 8; i++)
            for (int j = 8; j < 16; j++)
                if (tableroEstadisticas[i][j] == 1)
                    pintarLED(j - 8, i);
                else
                    delayMicroseconds(50);
}
