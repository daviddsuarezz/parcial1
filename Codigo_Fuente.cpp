const int PinData = 10;
const int PinLatch = 12;
const int PinClock = 13;
int ContadorBits;
int matriz[8][8];
int arreglo[8];
int option = 0;


void store(){
    digitalWrite(PinLatch, HIGH);
    digitalWrite(PinLatch, LOW);
}


void ledWrite(int led){
    shiftOut(PinData, PinClock, MSBFIRST, led);
    store();
}

void verificacion();
void imagen();
void publik();
int BitoDecimal(int binario[]);
int potencia(int b, int e);
void patronOn();
void setPatron();
void clearScreen();
void patrones();
void  patron1();
void  patron2();
void  patron3();
void patron4();

void setup(){
    Serial.begin(9600);
    pinMode(PinData, OUTPUT);
    pinMode(PinLatch, OUTPUT);
    pinMode(PinClock, OUTPUT);

}
void loop(){

    while(Serial.available() == 0);
    option = Serial.parseInt();

    clearScreen();
    if(option == 1){
        verificacion();
    }
    else if(option == 2){
        imagen_2();
    }
    else if(option == 3){
        patrones();
    }

    publik();
}


void publik(){
    /* Esta funcion se encarga de mostrar un menu de opciones */
    clearScreen();
    Serial.println("Menu:");
    Serial.println("1. Verificar funcionamiento de los leds.");
    Serial.println("2. Mostrar imagen de prueba.");
    Serial.println("3. Mostrar de forma alternada los patrones 1 al 4.");
    Serial.println("Escoja una opcion");
}

void imagen(){
    /* Esta funcion se encarga de ir encendiendo los leds que quiera
  el usuario hasta que de esa manera se forme un patron. */

    int fila; // fila le dira al programa la fila donde se encuentra el led que el usuario quiere encender o apagar.
    int columna; // columna le dira al programa la columna donde se encuentra el led que el usuario quiere encender o apagar.
    char estado; // estado le dira al programa si debe encender o apagar el led ubicado en la posicion fila-columna.

    setPatron(); // Se convierte el arreglo 'patron' en un arreglo de solo ceros.
    while(true){
        Serial.println("Ingrese la ubicacion del led a encender o apagar.");
        Serial.print("Fila: ");Serial.read(); // Se le pide al usuario la fila donde esta ubicado el led que quiere encender.
        while(Serial.available() == 0); // Mientras no se envie ningun dato por serial el programa no avanzara.
        fila = Serial.parseInt(); // Se le asigna a 'fila' la fila ingresada por el usuario.
        clearScreen(); // Se limpia el monitor serial.

        Serial.println("Ingrese la ubicacion del led a encender o apagar.");
        Serial.print("Fila: "); Serial.println(fila);
        Serial.println("Columna: ");Serial.read(); // Se le pide al usuario la columna donde esta ubicado el led que quiere encender.
        while(Serial.available() == 0); // Mientras no se envie ningun dato por serial el programa no avanzara.
        columna = Serial.parseInt(); // Se le asigna a 'columna' la fila ingresada por el usuario.
        clearScreen(); // Se limpia el monitor serial.

        Serial.println("Ingrese la ubicacion del led a encender o apagar.");
        Serial.print("Fila: "); Serial.println(fila);
        Serial.print("Colunma: "); Serial.println(columna);

        /* Si 'fila' o 'columna' son iguales a cero significa que el usuario a terminado de armar
       su patron, por tanto se rompe el ciclo y se termina la funcion. */
        if(fila == 0 || columna == 0){
            clearScreen(); // Se limpia el monitor serial.
            break; // Se rompe el ciclo infinito.
        }

        Serial.print("Estado: ");Serial.read(); // Se le pide al usuario que decida si quiere encender o apagar el led ubicado en fila-columna.
        while(Serial.available() == 0); // Mientras no se envie ningun dato por serial el programa no avanzara.
        estado = Serial.read(); // Se le asigna a 'estado' el caracter ingresado por el usuario.
        clearScreen(); // Se limpia el monitor serial.

        if(estado == '+'){matriz[fila-1][columna-1] = 1;} // Si estado es igual a +, se enciende el led ubicado el fila-columna.
        else{matriz[fila-1][columna-1] = 0;} // Si estado es igual a -, se apaga el led ubicado el fila-columna.

        patronOn(); // Se muestra el led ubicado en fila-columna dependiendo del valor de 'estado'.
    }
}

void imagen_2(){
    /* Esta funcion es la encargada de mostrar una secuencia de patrones
  ingresados por el usuario. */
    int numeroPatrones, tiempo;

    Serial.println("Cuantos patrones quiere ingresar?"); Serial.read();
    while(Serial.available() == 0); // Mientras no se envie ningun dato por serial el programa no avanzara.
    numeroPatrones = Serial.parseInt();
    clearScreen();

    int secuencia[numeroPatrones][8];

    for(int i=0; i<numeroPatrones; ++i){
        Serial.print("Ingrese una a una la posicion de los leds a encender para el patron "); Serial.println(i+1);
        imagen();
        for(int j=0; j<8; j++){
            secuencia[i][j] = BitoDecimal(matriz[j]);
        }
        setPatron();
        patronOn();
        Serial.print("Patron "); Serial.print(i+1); Serial.println(" guardado."); Serial.println();
    }

    Serial.println("Ingrese el intervalo de tiempo(milisegundos) para cambiar de patron."); Serial.read();
    while(Serial.available() == 0); // Mientras no se envie ningun dato por serial el programa no avanzara.
    tiempo = Serial.parseInt();
    clearScreen();

    Serial.println("Para interrumpir ingrese cualquier caracter."); Serial.read();
    while(Serial.available() == 0){
        for(int i=0; i<numeroPatrones; ++i){
            for(int j=7; j>=0; --j){
                shiftOut(PinData, PinClock, LSBFIRST, secuencia[i][j]);
            }
            digitalWrite(PinLatch, HIGH);
            digitalWrite(PinLatch, LOW);
            delay(tiempo);
        }
    }
    for(int i=0; i<8; ++i){
        shiftOut(PinData, PinClock, LSBFIRST, 0);
    }
    digitalWrite(PinLatch, HIGH);
    digitalWrite(PinLatch, LOW);
}

int BintoDeci(int binario[]){
    /* Esta funcion convierte un numero binario a decimal. */
    int decimal = 0;
    for(int i=7, e=0; i>=0; --i, ++e){
        decimal = decimal + binario[i]*potencia(2,e);
    }
    return decimal;
}


void clearScreen(){
    /* Esta funcion da la sensacion de limpiar el monitor serial. */
    for(int i=0; i<20; ++i){
        Serial.println();
    }
}

void verificacion(){
    int delay_leds;
    Serial.println("con cuanto tiempo quieres que se enciendan los leds?"); Serial.read();
    while(Serial.available() == 0); // Mientras no se envie ningun dato por serial el programa no avanzara.
    delay_leds = Serial.parseInt();
    clearScreen();
    for(int i=0; i<8; ++i){
        shiftOut(PinData, PinClock, LSBFIRST, 255);delay(delay_leds);
        digitalWrite(PinLatch, HIGH);
        digitalWrite(PinLatch, LOW);
    }
    for(int i=0; i<8; ++i){
        shiftOut(PinData, PinClock, LSBFIRST, 0);delay(delay_leds);
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

void patrones(){
    int delay_patrones;
    Serial.println("Dime el tiempo de retardo a mostrar los patrones, en milisegundos?"); Serial.read();
    while(Serial.available() == 0); // Mientras no se envie ningun dato por serial el programa no avanzara.
    delay_patrones = Serial.parseInt();
    clearScreen();
    delay(delay_patrones);
    patron1();
    patronOn();
    setPatron();
    delay(delay_patrones);
    patron2();
    patronOn();
    setPatron();
    delay(delay_patrones);
    patron3();
    patronOn();
    setPatron();
    delay(delay_patrones);
    patron4();
    patronOn();
    setPatron();
    patronOn();

}

void patron1(){
    for (int i = 0; i < 4; i++) { // Recorre las primeras 4 filas de la matriz
        for (int j = 0; j < 8; j++) { // Recorre todas las columnas de la matriz
            // Si el índice de la columna está dentro del rango (3-i) a (4+i), asigna 1, de lo contrario asigna 0
            matriz[i][j] = (j >= 3-i && j <= 4+i) ? 1 : 0;
        }
    }
    for (int i = 0; i < 4; i++) { // Recorre las últimas 4 filas de la matriz
        for (int j = 0; j < 8; j++) { // Recorre todas las columnas de la matriz
            // Si el índice de la columna está dentro del rango i a (8-i), asigna 1, de lo contrario asigna 0
            matriz[i+4][j] = (j >= i && j < 8-i) ? 1 : 0;
        }
    }
}
void patron2(){
    for (int i = 0; i < 8; i++) { // Recorre todas las filas de la matriz
        matriz[i][i] = 1; // Asigna 1 a los elementos en la diagonal principal
        matriz[i][7-i] = 1; // Asigna 1 a los elementos en la diagonal secundaria
    }
}



void patron3(){
    for (int i = 0; i < 8; i++) { // Recorre todas las filas de la matriz
        for (int j = 0; j < 8; j++) { // Recorre todas las columnas de la matriz
            // Si la fila es par, asigna 1 a los elementos en las columnas pares y 0 en las impares
            // Si la fila es impar, asigna 0 a los elementos en las columnas pares y 1 en las impares
            matriz[i][j] = ((i/2)%2 == 0) ? ((j/2)%2 == 0) : !((j/2)%2 == 0);
        }
    }
}

void patron4(){
    // Este primer bucle recorre las primeras 4 filas de la matriz.
    for(int i = 0; i < 4; i++) {
        // Este bucle interno recorre las 8 columnas de la matriz.
        for(int j = 0; j < 8; j++) {
            // Si el índice de la columna es mayor o igual al índice de la fila y menor que 4 más el índice de la fila,
            // entonces se coloca un 1 en esa posición de la matriz.
            if(j >= i && j < 4 + i) {
                matriz[i][j] = 1;
            } else {
                // De lo contrario, se coloca un 0 en esa posición de la matriz.
                matriz[i][j] = 0;
            }
        }
    }

    // Este segundo bucle recorre las siguientes 4 filas de la matriz.
    for(int i = 0; i < 4; i++) {
        // Este bucle interno recorre las 8 columnas de la matriz.
        for(int j = 0; j < 8; j++) {
            // Si el índice de la columna es mayor o igual a 3 menos el índice de la fila y menor que 7 menos el índice de la fila,
            // entonces se coloca un 1 en esa posición de la matriz.
            if(j >= 3 - i && j < 7 - i) {
                matriz[i+4][j] = 1;
            } else {
                // De lo contrario, se coloca un 0 en esa posición de la matriz.
                matriz[i+4][j] = 0;
            }
        }
    }

}

