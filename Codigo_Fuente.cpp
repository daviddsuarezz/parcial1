const int PinData = 10;
const int PinLatch = 12;
const int PinClock = 13;
int ContadorBits;

void LedWrite(int led){
    shiftOut(PinData, PinClock, LSBFIRST, led);
}

void store(){
    digitalWrite(PinLatch, HIGH);
    digitalWrite(PinLatch, LOW);
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




void loop(){

    for(int i=0; i<8; ++i){
        for(int j= 0; j<8; j++){
            ContadorBits = 1 << j;
            LedWrite(ContadorBits); delay(100);
        }
        store();
    }
