# 1 "G:\\Archivos\\Escritorio\\Ingenieria\\Junio 2023\\Arqui 1\\P1-ARQUI1-BACKUP\\Practica1\\Practica1.ino"
# 2 "G:\\Archivos\\Escritorio\\Ingenieria\\Junio 2023\\Arqui 1\\P1-ARQUI1-BACKUP\\Practica1\\Practica1.ino" 2
# 3 "G:\\Archivos\\Escritorio\\Ingenieria\\Junio 2023\\Arqui 1\\P1-ARQUI1-BACKUP\\Practica1\\Practica1.ino" 2

void setup()
{
    Serial.begin(9600);
    inizializarMatrizSinDriver();
    inicializarMatrizDriver();
    pinMode(DIR_LEFT, 0x2); // DIRECCION IZQUIERDA
    pinMode(DIR_RIGHT, 0x2); // DIRECCION DERECHA
    pinMode(K, 0x2); // K
    pinMode(DISP, 0x0); // DISPARO
}

void loop()
{
    MenuPrincipal();
}
