//ARDUINO ESCLAVO

#include <Wire.h>

//ULTRASONICO
int TRIG = 31;
int ECO = 32;
int DURACION;
int DISTANCIA;

//Motor 1
int m1_1 = 12;
int m1_2 = 11;

//Motor 2
int m2_1 = 10;
int m2_2 = 9;

//Velocidad
int v1 = 8;
int v2 = 7;

long tiempoTotal = millis();
int girosDerecha = 0;
int girosIzquierda = 0;

int estado = 0;
int estadoManual = 0;
bool activo = true;
int noComando = 0;
bool siguienteComando = false;
int estadoAnterior = 0;
bool pausa = false;
bool finalizado = false;
bool salidaEncontrada = false; //Salida del Laberinto
int caminoSinSalida = 0; // Contador de veces que el auto retrocedio

#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define salidaTCS 6

unsigned long tiempoInicio = 0;
unsigned long tiempoEjecucion = 0;
unsigned long tiempoAhora = 0;
//Variable que guardara los comandos
byte comando = 0;
byte code = 0;

int lista_comandos [20];

void setup(){
  Wire.begin(0x01);
  Serial.begin(9600);
  pinMode(m1_1, OUTPUT);
  pinMode(m1_2, OUTPUT);

  pinMode(m2_1, OUTPUT);
  pinMode(m2_2, OUTPUT);

  pinMode(v1, OUTPUT);
  pinMode(v2, OUTPUT);

  pinMode(TRIG, OUTPUT);  // trigger como salida
  pinMode(ECO, INPUT);    // echo como entrada
  pinMode(30, OUTPUT);
  pinMode(33, OUTPUT);
  digitalWrite(30, HIGH);
  analogWrite(v1, 235);
  analogWrite(v2, 255);

  pinMode(S0, OUTPUT);    // pin 4 como salida
  pinMode(S1, OUTPUT);    // pin 5 como salida
  pinMode(S2, OUTPUT);    // pin 6 como salida
  pinMode(S3, OUTPUT);    // pin 7 como salida
  pinMode(salidaTCS, INPUT);  // pin 8 como salida

  digitalWrite(S0, HIGH); // establece frecuencia de salida
  digitalWrite(S1, LOW);  // del modulo al 20 por ciento

  Wire.onReceive(ComandoDeEntrada);
  Wire.onRequest(peticion);
}

void loop(){
  if (estado == 0){
    if (comando == 1){
      //    Modo Manual
      Serial.println("En modo manual");
      estado = 3;

    }else if (comando == 2){
      //    Modo Automatico
      Serial.println("En modo automatico");
      estado = 1;
      modoMecanico();
    }
  }

  if (estado == 3){
    Serial.println("Recibiendo comandos:");
    Serial.println("Comando No: ");
    Serial.println(noComando + 1);
    if (noComando < 20){
      if (comando == 10){
        Serial.println("COMANDO AVANZA");
        lista_comandos[noComando] = comando;
        noComando++;
        comando = 0;
      }
      if (comando == 11){
        Serial.println("COMANDO RETROCEDE");
        lista_comandos[noComando] = comando;
        noComando++;
        comando = 0;
      }
      if (comando == 12){
        Serial.println("COMANDO IZQUIERDA");
        lista_comandos[noComando] = comando;
        noComando++;
        comando = 0;
      }
      if (comando == 13){
        Serial.println("COMANDO DERECHA");
        lista_comandos[noComando] = comando;
        noComando++;
        comando = 0;
      }
    }else{
      Serial.println("Lista de comandos llena");
      estado = 2;
      noComando = 0;
      siguienteComando = true;
      tiempoEjecucion = millis();
    } 
    delay(500);
  }
  
  if (estado == 2){
    if (siguienteComando){
      comando = lista_comandos[noComando];
      siguienteComando = false;
    }
    if (!pausa){
      if (comando == 10){
        Serial.println("AVANZA");
        estadoManual = 1;
        tiempoInicio = millis();
        comando = 0;
      }
      if (comando == 11){
        Serial.println("RETROCEDE");
        estadoManual = 2;
        tiempoInicio = millis();
        comando = 0;
      }
      if (comando == 12){
        Serial.println("IZQUIERDA");
        estadoManual = 3;
        tiempoInicio = millis();
        comando = 0;
      }
      if (comando == 13){
        Serial.println("DERECHA");
        estadoManual = 4;
        tiempoInicio = millis();
        comando = 0;
      }
    }
    
    if (estadoManual == 1){
      Serial.println("Avanzando");
      Serial.println(millis() - tiempoInicio);
      code = 1;
      if (millis() - tiempoInicio > 10000){
        estadoManual = 5;
        siguienteComando = true;
      }
      digitalWrite(m1_1, HIGH);
      digitalWrite(m1_2, LOW);

      digitalWrite(m2_1, LOW);
      digitalWrite(m2_2, HIGH);
    }else if (estadoManual == 2){
      code = 2;
      Serial.println("Retrocediendo");
      Serial.println(millis() - tiempoInicio);
      if (millis() - tiempoInicio > 10000)
      {
        estadoManual = 5;
        siguienteComando = true;
      }
      digitalWrite(m1_1, LOW);
      digitalWrite(m1_2, HIGH);

      digitalWrite(m2_1, HIGH);
      digitalWrite(m2_2, LOW);
    }else if (estadoManual == 3){
      code = 3;
      Serial.println("Doblando izquierda");
      Serial.println(millis() - tiempoInicio);
      if (millis() - tiempoInicio > 2000){
        estadoManual = 5;
        siguienteComando = true;
      } 
      digitalWrite(m1_1, LOW);
      digitalWrite(m1_2, LOW);

      digitalWrite(m2_1, LOW);
      digitalWrite(m2_2, HIGH);
    }else if (estadoManual == 4){
      code = 4;
      Serial.println("Doblando derecha");
      Serial.println(millis() - tiempoInicio);
      if (millis() - tiempoInicio > 2000){
        estadoManual = 5;
        siguienteComando = true;
      }
      digitalWrite(m1_1, HIGH);
      digitalWrite(m1_2, LOW);

      digitalWrite(m2_1, LOW);
      digitalWrite(m2_2, LOW);
    }else if (estadoManual == 5){
      if (finalizado){
        code = 6;
      }else{
        code = 5;
      }
      Serial.println("Detenido");
      digitalWrite(m1_1, LOW);
      digitalWrite(m1_2, LOW);

      digitalWrite(m2_1, LOW);
      digitalWrite(m2_2, LOW);

      if (pausa){
        if (comando == 10){
          pausa = false;
          comando = estadoAnterior;
          Serial.print("SE READNUO: ");
          Serial.println(estadoAnterior);
          delay(500);
        }
      }
    }
    if (siguienteComando){
      noComando++;
      if (noComando >= 20){
        comando = 0;
        estadoManual = 5;
        Serial.println("finalizo la secuencia");
        finalizado = true;
        delay(500);
        code = 6;
      }
    }
    Serial.println((millis() - tiempoEjecucion) / 1000);
  }


  //  ULTRASONICO
  digitalWrite(TRIG, HIGH);     // generacion del pulso a enviar
  delay(1);       // al pin conectado al trigger
  digitalWrite(TRIG, LOW);    // del sensor

  DURACION = pulseIn(ECO, HIGH);  // con funcion pulseIn se espera un pulso
  // alto en Echo
  DISTANCIA = DURACION / 58.2;    // distancia medida en centimetros
  Serial.println(DISTANCIA);    // envio de valor de distancia por monitor serial
  if (DISTANCIA <= 10 && DISTANCIA >= 0){
    if (estado == 2){
      Serial.println("cercaaaa");
      estadoAnterior = lista_comandos[noComando];
      estadoManual = 5;
      pausa = true;
    }
  }

  digitalWrite(S2, LOW);    // establece fotodiodos
  digitalWrite(S3, LOW);    // con filtro rojo
  int rojo = pulseIn(salidaTCS, LOW); // obtiene duracion de pulso de salida del sensor
  delay(5);       // demora de 200 mseg

  digitalWrite(S2, HIGH);   // establece fotodiodos
  digitalWrite(S3, HIGH);   // con filtro verde
  int verde = pulseIn(salidaTCS, LOW);  // obtiene duracion de pulso de salida del sensor
  delay(5);       // demora de 200 mseg

  digitalWrite(S2, LOW);    // establece fotodiodos
  digitalWrite(S3, HIGH);   // con filtro azul
  int azul = pulseIn(salidaTCS, LOW); // obtiene duracion de pulso de salida del sensor
  delay(5);       // demora de 200 mseg

  Serial.print("R:");     // muestra texto
  Serial.print(rojo);     // muestra valor de variable rojo

  Serial.print("\t");     // espacio de tabulacion

  Serial.print("V:");     // muestra texto
  Serial.print(verde);      // muestra valor de variable verde

  Serial.print("\t");     // espacio de tabulacion

  Serial.print("A:");     // muestra texto
  Serial.println(azul);     // muestra valor de variable azul
}
//************************************************************************

void modoMecanico() {
  noInterrupts();
  tiempoTotal = millis();
  //salidaEncontrada va a ser true cuando el sensor de color identifique el color de salida
  while (salidaEncontrada == false) {
    DISTANCIA = getDistancia();
    int cmRight = 0;
    int cmLeft = 0;
    int cmBack = 0;
    delay(50);

    if (DISTANCIA < 35) {
      Serial.println(DISTANCIA);
      Stop();

      ///PROCEDE A ESCANEAR DANDO UNA VUELTA DE 360 GRADOS
      turnAround(1);// Vuelta hacia la derecha de 90 grados
      cmRight = getDistancia();
      turnAround(1);
      cmBack = getDistancia();
      turnAround(1);
      cmLeft = getDistancia();
      turnAround(1);
      
      ///SI NO HAY SALIDA VA HACIA ATRAS DURANTE 13 SEGUNDOS
      if (cmLeft < 25 && cmRight < 25){
        caminoSinSalida++;
        tiempoAhora = millis();
        while (millis() < tiempoAhora + 1300) {
          goBackward();
        }
        Stop();
        delay(300);

        // Al llegar hasta atras, escanea para saber a donder ir
        turnAround(1);
        cmRight = getDistancia();
        turnAround(1);
        cmBack = getDistancia();
        turnAround(1);
        cmLeft = getDistancia();
        turnAround(1);
      }
      /// SI HAY SALIDA HACIA LA IZQUIERDA, HACE UN GIRO HACIA ESE LADO
      if (cmLeft > cmRight) {
        tiempoAhora = millis();
        while (millis() < tiempoAhora + 520) {
          goLeft();
        }
        delay(300);
        girosIzquierda++;
        Stop();
      }else if (cmRight > cmLeft){
        turnAround(1);
        Stop();
      }
    }else{
      goForward();
    }
  }

  tiempoTotal = tiempoTotal/1000;
  Serial.print("Tiempo total = ");
  Serial.print(tiempoTotal);
  Serial.println(" segundos");

  Serial.print("Pasillos Sin Salida = ");
  Serial.println(caminoSinSalida);

  Serial.print("Giros a la derecha = ");
  Serial.println(girosDerecha);

  Serial.print("Giros a la izquierda = ");
  Serial.println(girosIzquierda);

  // La media y moda de los angulos que el automovil gira siempre seran 90 grados
  Serial.print("Media = 90°, Moda = 90°");

  // Reinicio de variables
  tiempoTotal = 0;
  caminoSinSalida = 0;
  girosDerecha = 0;
  girosIzquierda = 0;
  estado = 0;
  salidaEncontrada = false;
  interrupts();
}




//************************************************************************
int getDistancia() { //  ULTRASONICO
  int distance1;
  int distance;
  digitalWrite(TRIG, HIGH);     // generacion del pulso a enviar
  delay(1);       // al pin conectado al trigger
  digitalWrite(TRIG, LOW);    // del sensor

  distance1 = pulseIn(ECO, HIGH);  // con funcion pulseIn se espera un pulso
  // alto en Echo
  distance = distance1 / 58.2;    // distancia medida en centimetros
  return distance;
}


void Stop() {
  digitalWrite(m1_1, LOW);
  digitalWrite(m1_2, LOW);

  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, LOW);
}

void goLeft() {
  Serial.println("ESCANEANDO....");
  digitalWrite(m1_1, LOW);
  digitalWrite(m1_2, LOW);

  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, HIGH);
}

void goRight() {
  Serial.println("ESCANEANDO....");

  digitalWrite(m1_1, HIGH);
  digitalWrite(m1_2, LOW);

  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, LOW);
}

void goForward() {
  Serial.println("AVANZANDO....");
  digitalWrite(m1_1, HIGH);
  digitalWrite(m1_2, LOW);

  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, HIGH);
}

void goBackward() {
  Serial.println("RETROCEDIENDO....");
  digitalWrite(m1_1, LOW);
  digitalWrite(m1_2, HIGH);

  digitalWrite(m2_1, HIGH);
  digitalWrite(m2_2, LOW);
}

void turnAround(int noVuelta) {
   tiempoAhora = millis();
   int periodo = 520*noVuelta;
   
   while (millis() < tiempoAhora+ periodo) {
       goRight();
   }
   Stop();
   delay(300);
   girosDerecha++;
}


//*****************************************************************
void ComandoDeEntrada(int aux){
  while (Wire.available() > 0){
    comando = Wire.read();
    Serial.println(comando);
  }
}

void peticion(){
  Wire.write(code);
}
