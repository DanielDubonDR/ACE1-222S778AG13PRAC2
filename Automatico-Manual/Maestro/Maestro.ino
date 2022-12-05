//ARDUINO MAESTRO

#include <Wire.h>

//estas entradas cambiaran en arduino mega
int up = 38;
int down = 39;
int left = 40;
int right = 41;

int estado = 0;
int modo = 0;

byte CODE;


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  pinMode(up, INPUT);
  pinMode(down, INPUT);
  pinMode(left, INPUT);
  pinMode(right, INPUT);
  pinMode(42, OUTPUT);
  pinMode(43, OUTPUT);

  digitalWrite(42, HIGH);
  
}

void loop(){

  if(estado==0){
    mensajeInicial();
    estado = 1;
  }else if(estado==1){
    Serial.println("Escogiendo Modo");
    delay(3000);
    estado = 2;
  }else if(estado==2){
    Serial.println("<- AUTO MANUAL ->");
    if(digitalRead(left) == HIGH){
      //Serial.println("Modo automatico seleccionado");
      delay(200);
      estado = 3;
    }
    if(digitalRead(right) == HIGH){
      //Serial.println("Modo manual seleccionado");
      Wire.beginTransmission(0x01);
      Wire.write(1);
      delay(200);
      Wire.endTransmission();
      estado =4;
    }
    
  }else if(estado==3){
      Serial.println("Modo automatico seleccionado");
      delay(1000);
      Wire.beginTransmission(0x01);
      Wire.write(2);
      delay(200);
      Wire.endTransmission();
      estado = 5;
      
  }else if(estado==4){
    Serial.println("Modo manual seleccionado");
    if(digitalRead(up) == HIGH){
      Serial.println("Boton UP presionado");
      Wire.beginTransmission(0x01);
      Wire.write(10);
      delay(200);
      Wire.endTransmission();
    }
    if(digitalRead(down) == HIGH){
      Serial.println("Boton DOWN presionado");
      Wire.beginTransmission(0x01);
      Wire.write(11);
      delay(200);
      Wire.endTransmission();
    }
    if(digitalRead(left) == HIGH){
      Wire.beginTransmission(0x01);
      Serial.println("Boton LEFT presionado");
      Wire.write(12);
      delay(200);
      Wire.endTransmission();
    }
    if(digitalRead(right) == HIGH){
      Wire.beginTransmission(0x01);
      Serial.println("Boton RIGHT presionado");
      Wire.write(13);
      delay(200);
      Wire.endTransmission();
    }
  }  
}

void mensajeInicial(){
  Serial.println("Bienvenido");
  delay(10000);
}
