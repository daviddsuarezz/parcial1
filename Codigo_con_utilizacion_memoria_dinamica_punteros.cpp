const int PinData = 10;
const int PinLatch = 12;
const int PinClock = 13;
int option;
int (*matriz)[8];

// Declaración de las funciones
void store();
void ledWrite(int led);
void mostrarMatriz(int (*matriz)[8]);
int (*crearMatriz(int n))[8];
void liberarMatriz(int (*matriz)[8]);
int (*redimensionarMatriz(int (*matriz)[8], int n))[8];
void setup();
void loop();
void imagen(int (*matriz)[8]);
void imagen_2();
void clearScreen();
void verificacion();
int BitoDecimal(int *binario);
int potencia(int b, int e);
void patronOn();
void setPatron();
void patrones(int (*matriz)[8]);
void patron1(int (*matriz)[8]);
void patron2(int (*matriz)[8]);
void patron3(int (*matriz)[8]);
void patron4(int (*matriz)[8]);
void publik();

// Definición de las funciones

void store(){
    digitalWrite(PinLatch, HIGH);
    digitalWrite(PinLatch, LOW);
}

void ledWrite(int led){
    shiftOut(PinData, PinClock, MSBFIRST, led);
    store();
}

// Esta función recibe un puntero a una matriz de 8x8 y la muestra en la matriz de LED
void mostrarMatriz(int (*matriz)[8]){
    for(int i=7; i>=0; --i){
        shiftOut(PinData, PinClock, LSBFIRST, BitoDecimal(*matriz)); // Usa *matriz para acceder al valor apuntado
        matriz++; // Incrementa el puntero para apuntar a la siguiente fila
    }
    digitalWrite(PinLatch, HIGH);
    digitalWrite(PinLatch, LOW);
}

// Esta función recibe un número entero n y devuelve un puntero a una matriz dinámica de n x 8
int (*crearMatriz(int n))[8]{
    // Reserva n bloques de memoria del tamaño de un arreglo de 8 enteros
    int (*matriz)[8] = (int (*)[8]) malloc(n * sizeof(int[8]));
    return matriz; // Devuelve el puntero a la matriz dinámica
}

// Esta función recibe un puntero a una matriz dinámica y libera la memoria ocupada por ella
void liberarMatriz(int (*matriz)[8]){
    free(matriz); // Libera el bloque de memoria apuntado por el puntero
}

// Esta función recibe un puntero a una matriz dinámica, un número entero n y devuelve un puntero a una nueva matriz dinámica de n x 8
int (*redimensionarMatriz(int (*matriz)[8], int n))[8]{
    // Cambia el tamaño del bloque de memoria apuntado por el puntero a n bloques del tamaño de un arreglo de 8 enteros
    int (*nuevaMatriz)[8] = (int (*)[8]) realloc(matriz, n * sizeof(int[8]));
    return nuevaMatriz; // Devuelve el puntero a la nueva matriz dinámica
}

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

    // Crea una matriz dinámica de 8 x 8
    int (*matriz)[8] = crearMatriz(8);

    if(option == 1){
        verificacion();
    }
    else if(option == 2){
        imagen_2();
    }
    else if(option == 3){
        patrones(matriz); // Usa matriz como argumento para la función patrones
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

// Esta función recibe un puntero a una matriz dinámica de 8 x 8 y le asigna el patrón ingresado por el usuario
void imagen(int (*matriz)[8]){
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

        if(estado == '+'){(*matriz)[columna-1] = 1;} // Si estado es igual a +, se enciende el led ubicado en fila-columna.
        else{(*matriz)[columna-1] = 0;} // Si estado es igual a -, se apaga el led ubicado en fila-columna.

        patronOn(); // Se muestra el led ubicado en fila-columna dependiendo del valor de 'estado'.

        matriz++; // Incrementa el puntero para apuntar a la siguiente fila
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

    // Crea una matriz dinámica de numeroPatrones x 8
    int (*secuencia)[8] = crearMatriz(numeroPatrones);

    for(int i=0; i<numeroPatrones; ++i){
        Serial.print("Ingrese una a una la posicion de los leds a encender para el patron "); Serial.println(i+1);
        imagen(secuencia); // Usa secuencia como argumento para la función imagen
        secuencia += 8; // Incrementa el puntero para apuntar al inicio de la siguiente matriz
        setPatron();
        patronOn();
        Serial.print("Patron "); Serial.print(i+1); Serial.println(" guardado."); Serial.println();
    }

    // Resta numeroPatrones x 8 al puntero para volver al inicio de la matriz
    secuencia -= numeroPatrones * 8;

    Serial.println("Ingrese el intervalo de tiempo(milisegundos) para cambiar de patron."); Serial.read();
    while(Serial.available() == 0); // Mientras no se envie ningun dato por serial el programa no avanzara.
    tiempo = Serial.parseInt();
    clearScreen();

    Serial.println("Para interrumpir ingrese cualquier caracter."); Serial.read();
    while(Serial.available() == 0){
        for(int i=0; i<numeroPatrones; ++i){
            mostrarMatriz(secuencia); // Usa secuencia como argumento para la función mostrarMatriz
            secuencia += 8; // Incrementa el puntero para apuntar al inicio de la siguiente matriz
            delay(tiempo);
        }
        // Resta numeroPatrones x 8 al puntero para volver al inicio de la matriz
        secuencia -= numeroPatrones * 8;
    }

    // Libera la memoria ocupada por la matriz secuencia
    liberarMatriz(secuencia);

}
void clearScreen(){
    /* Esta funcion da la sensacion de limpiar el monitor serial. */
    for(int i=0; i<20; ++i){
        Serial.println();
    }

}

// Esta función recibe un puntero a una matriz dinámica de 8 x 8 y le asigna 255 a todos sus elementos
void verificacion(){
    int delay_leds;
    Serial.println("con cuanto tiempo quieres que se enciendan los leds?"); Serial.read();
    while(Serial.available() == 0); // Mientras no se envie ningun dato por serial el programa no avanzara.
    delay_leds = Serial.parseInt();
    clearScreen();
    for(int i=0; i<8; ++i){
        for(int j=0; j<8; ++j){
            (*matriz)[j] = 255; // Asigna 255 al elemento apuntado
        }
        matriz++; // Incrementa el puntero para apuntar a la siguiente fila
    }
    mostrarMatriz(matriz); // Usa matriz como argumento para la función mostrarMatriz
    delay(delay_leds);
    for(int i=0; i<8; ++i){
        for(int j=0; j<8; ++j){
            (*matriz)[j] = 0; // Asigna 0 al elemento apuntado
        }
        matriz++; // Incrementa el puntero para apuntar a la siguiente fila
    }
    mostrarMatriz(matriz); // Usa matriz como argumento para la función mostrarMatriz
    delay(delay_leds);
}

int potencia(int b, int e){
    /* Esta funcion calcula la potencia de un numero b elevado a e*/
    int numero = 1;
    for(int i=0; i<e; ++i){
        numero *= b;
    }
    return numero;
}


// Esta función recibe un puntero a un arreglo de 8 enteros y devuelve el valor decimal correspondiente
int BitoDecimal(int *binario){
    /* Esta funcion convierte un numero binario a decimal. */
    int decimal = 0;
    for(int i=7, e=0; i>=0; --i, ++e){
        decimal = decimal + (*binario)*potencia(2,e); // Usa *binario para acceder al valor apuntado
        binario++; // Incrementa el puntero para apuntar al siguiente elemento
    }
    return decimal;
}

// Esta función recibe un puntero a una matriz dinámica de 8 x 8 y la muestra en la matriz de LED
void patronOn(){
    /* Esta funcion se encarga de encender los leds correspondientes al
  patron almacenado en el arreglo 'patron'. */

    mostrarMatriz(matriz); // Usa matriz como argumento para la función mostrarMatriz
}

// Esta función recibe un puntero a una matriz dinámica de 8 x 8 y le asigna ceros a todos sus elementos
void setPatron(){
    /* Esta funcion le asigna a todas las posiciones de 'patron'  el valor
  de cero. */
    for(int i=0; i<8; ++i){
        for(int j=0; j<8; j++){
            (*matriz)[j] = 0; // Asigna 0 al elemento apuntado
        }
        matriz++; // Incrementa el puntero para apuntar a la siguiente fila
    }
}
void patrones(int (*matriz)[8]){
    int delay_patrones;
    Serial.println("Dime el tiempo de retardo a mostrar los patrones, en milisegundos?"); Serial.read();
    while(Serial.available() == 0); // Mientras no se envie ningun dato por serial el programa no avanzara.
    delay_patrones = Serial.parseInt();
    clearScreen();
    delay(delay_patrones);
    patron1(matriz); // Usa matriz como argumento para la función patron1
    mostrarMatriz(matriz); // Usa matriz como argumento para la función mostrarMatriz
    setPatron();
    delay(delay_patrones);
    patron2(matriz); // Usa matriz como argumento para la función patron2
    mostrarMatriz(matriz); // Usa matriz como argumento para la función mostrarMatriz
    setPatron();
    delay(delay_patrones);
    patron3(matriz); // Usa matriz como argumento para la función patron3
    mostrarMatriz(matriz); // Usa matriz como argumento para la función mostrarMatriz
    setPatron();
    delay(delay_patrones);
    patron4(matriz); // Usa matriz como argumento para la función patron4
    mostrarMatriz(matriz); // Usa matriz como argumento para la función mostrarMatriz
}
// Esta función recibe un puntero a una matriz de 4 x 8 y le asigna el patrón 1
void patron1(int (*matriz)[8]){
    for (int i = 0; i < 4; i++) { // Recorre las primeras 4 filas de la matriz
        for (int j = 0; j < 8; j++) { // Recorre todas las columnas de la matriz
            // Si el índice de la columna está dentro del rango (3-i) a (4+i), asigna 1, de lo contrario asigna 0
            (*matriz)[j] = (j >= 3-i && j <= 4+i) ? 1 : 0;
        }
        matriz++; // Incrementa el puntero para apuntar a la siguiente fila
    }
}

// Esta función recibe un puntero a una matriz de 8 x 8 y le asigna el patrón 2
void patron2(int (*matriz)[8]){
    for (int i = 0; i < 8; i++) { // Recorre todas las filas de la matriz
        (*matriz)[i] = 1; // Asigna 1 a los elementos en la diagonal principal
        (*matriz)[7-i] = 1; // Asigna 1 a los elementos en la diagonal secundaria
        matriz++; // Incrementa el puntero para apuntar a la siguiente fila
    }
}

// Esta función recibe un puntero a una matriz de 8 x 8 y le asigna el patrón 3
void patron3(int (*matriz)[8]){
    for (int i = 0; i < 8; i++) { // Recorre todas las filas de la matriz
        for (int j = 0; j < 8; j++) { // Recorre todas las columnas de la matriz
            // Si la fila es par, asigna 1 a los elementos en las columnas pares y 0 en las impares
            // Si la fila es impar, asigna 0 a los elementos en las columnas pares y 1 en las impares
            (*matriz)[j] = ((i/2)%2 == 0) ? ((j/2)%2 == 0) : !((j/2)%2 == 0);
        }
        matriz++; // Incrementa el puntero para apuntar a la siguiente fila
    }
}

// Esta función recibe un puntero a una matriz de 8 x 8 y le asigna el patrón 4
void patron4(int (*matriz)[8]){
    // Este primer bucle recorre las primeras 4 filas de la matriz.
    for(int i = 0; i < 4; i++) {
        // Este bucle interno recorre las 8 columnas de la matriz.
        for(int j = 0; j < 8; j++) {
            // Si el índice de la columna es mayor o igual al índice de la fila y menor que 4 más el índice de la fila,
            // entonces se coloca un 1 en esa posición de la matriz.
            if(j >= i && j < 4 + i) {
                (*matriz)[j] = 1;
            } else {
                // De lo contrario, se coloca un 0 en esa posición de la matriz.
                (*matriz)[j] = 0;
            }
        }
        matriz++; // Incrementa el puntero para apuntar a la siguiente fila
    }

    // Este segundo bucle recorre las siguientes 4 filas de la matriz.
    for(int i = 0; i < 4; i++) {
        // Este bucle interno recorre las 8 columnas de la matriz.
        for(int j = 0; j < 8; j++) {
            // Si el índice de la columna es mayor o igual a 3 menos el índice de la fila y menor que 7 menos el índice de la fila,
            // entonces se coloca un 1 en esa posición de la matriz.
            if(j >= 3 - i && j < 7 - i) {
                (*matriz)[j] = 1;
            } else {
                // De lo contrario, se coloca un 0 en esa posición de la matriz.
                (*matriz)[j] = 0;
            }
        }
        matriz++; // Incrementa el puntero para apuntar a la siguiente fila
    }

}
