const int PinData = 10;
const int PinLatch = 12;
const int PinClock = 13;
int ContadorBits;
int matriz[8][8];
int arreglo[8];

void store(){
    digitalWrite(PinLatch, HIGH);
    digitalWrite(PinLatch, LOW);
}

void ledWrite(int led){
    shiftOut(PinData, PinClock, MSBFIRST, led);
    store();
}

void setup(){
    Serial.begin(9600);
    pinMode(PinData, OUTPUT);
    pinMode(PinLatch, OUTPUT);
    pinMode(PinClock, OUTPUT);

}


void Menu(){
    /* Esta funcion se encarga de mostrar un menu de opciones */
    clearScreen();
    Serial.println("Menu:");
    Serial.println("1. Verificar funcionamiento de los leds.");
    Serial.println("2. Visualizar los patrones.");
    Serial.println("3. Visualizar secuencia elegida por el usuario.");
    Serial.println("Escoja una opcion");
}


void clearScreen(){
    /* Esta funcion da la sensacion de limpiar el monitor serial. */
    for(int i=0; i<20; ++i){
        Serial.println();
    }
}

void verificacion(){
    for(int i=0; i<8; ++i){
        shiftOut(PinData, PinClock, LSBFIRST, 255);delay(500);
        digitalWrite(PinLatch, HIGH);
        digitalWrite(PinLatch, LOW);
    }
    for(int i=0; i<8; ++i){
        shiftOut(PinData, PinClock, LSBFIRST, 0);delay(500);
        digitalWrite(PinLatch, HIGH);
        digitalWrite(PinLatch, LOW);
    }
}

int BitoDecimal(int binario[]){
    /* Esta funcion convierte un numero binario a decimal. */
    int decimal = 0;
    for(int i=7, e=0; i>=0; --i, ++e){
        decimal = decimal + binario[i]*potencia(2,e);
    }
    return decimal;
}

int potencia(int b, int e){
    /* Esta funcion calcula la potencia de un numero b elevado a e*/
    int numero = 1;
    for(int i=0; i<e; ++i){
        numero *= b;
    }
    return numero;
}

void patronOn(){
    /* Esta funcion se encarga de encender los leds correspondientes al
  patron almacenado en el arreglo 'patron'. */
    for(int i=7; i>=0; --i){
        shiftOut(PinData, PinClock, LSBFIRST, BitoDecimal(matriz[i])); delay(100);
    }
    digitalWrite(PinLatch, HIGH);
    digitalWrite(PinLatch, LOW);

};

void setPatron(){
    /* Esta funcion le asigna a todas las posiciones de 'patron'  el valor
  de cero. */
    for(int i=0; i<8; ++i){
        for(int j=0; j<8; j++){
            matriz[i][j] = 0;
        }
    }
}
void loop(){
    verificacion();
}

