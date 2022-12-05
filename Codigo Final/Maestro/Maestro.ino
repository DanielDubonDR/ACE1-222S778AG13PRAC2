//ARDUINO MAESTRO
#include <LiquidCrystal.h>
#include <Wire.h>

//estas entradas cambiaran en arduino mega
int up = 38;
int down = 39;
int left = 40;
int right = 41;

int estado = 0;
int modo = 0;
int dds=0;

int contMas=0;
int comd[20];

// Pines de energia
int gnd1 = 22;
int gnd2 = 23;
int gnd3 = 8;

//Pantalla
int rs = 7;
int e = 6;
int db4 = 5;
int db5 = 4;
int db6 = 3;
int db7 = 2;
bool enable =true;
bool tomaTiempo = true;

int modaA=0;
int mediaA=0;
int modaD=0;
int mediaD=0;
int modaI=0;
int mediaI=0;
int modaAA=0;
int mediaAA=0;

int drr=0;
int izz=0;

bool a=true;

int red=0;
int green=0;
int blue=0;
int retrocede=0;
int resss=0;
int estadoAnt=0;

byte Carita[] = {
  B00100,
  B01110,
  B11111,
  B10101,
  B01110,
  B10001,
  B01110,
  B00100
};

byte flecha_derecha[] = {
  B00000,
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000
};

byte flecha_izquierda[] = {
  B00000,
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000
};
LiquidCrystal lcd (rs,e,db4,db5,db6,db7);

String color ="";
String estadoManual="";
bool contador=false;
int contadorInstrucciones=0;

byte code;

int noObstaculos=0;
int noColores=0;

unsigned long tiempoEjecucion = 0;
unsigned long tiempo = 0;

unsigned long tiempoA = 0;
unsigned long tiempoB = 0;

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
  pinMode(gnd1, OUTPUT);
  pinMode(gnd2, OUTPUT);
  pinMode(gnd3, OUTPUT);

  digitalWrite(42, HIGH);
  
}

void loop()
{
  if(estado==0)
  {
    mensajeInicial();
    mensajeInicial();
    estado = 1;
  }
  else if(estado==1)
  {
    Serial.println("Escogiendo Modo");
    lcd.createChar(1, flecha_derecha);
    lcd.createChar(2, flecha_izquierda);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ESCOGIENDO MODO");
    delay(3000);
    estado = 2;
    lcd.clear();
  }
  else if(estado==2)
  {
    Serial.println("<- AUTO MANUAL ->");
    lcd.setCursor(1, 0);
    lcd.write(byte(2));
    lcd.print("AUTO");
    lcd.setCursor(9, 0);
    lcd.print("MANUAL");
    lcd.write(byte(1));
    if(digitalRead(left) == HIGH)
    {
//      Serial.println("Modo automatico seleccionado");
      lcd.clear();
      
//      delay(1000);
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("INICIANDO");
      lcd.setCursor(1,1);
      lcd.print("MODO AUTOMATICO");
      delay(5000);
      Wire.beginTransmission(0x01);
      Wire.write(2);
      delay(200);
      Wire.endTransmission();
      estado = 3;
      tiempoA=millis();
    }
    if(digitalRead(right) == HIGH)
    {
//      Serial.println("Modo manual seleccionado");
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("INICIANDO");
      lcd.setCursor(2,1);
      lcd.print("MODO MANUAL");
      delay(5000);
      Wire.beginTransmission(0x01);
      Wire.write(1);
      delay(200);
      Wire.endTransmission();
      estado =4;
      lcd.clear();
    }
  }
  else if(estado==69)
  {
    if(a)
      {
        a=false;
        tiempoB=((millis()-tiempoA)/1000);
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Finalizado");
      lcd.setCursor(0,1);
      lcd.print("Tiempo: ");
      lcd.print(tiempoB);
      lcd.print(" s");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Distancia: ");
      lcd.print(dds+10);
      lcd.print(" cm");
      lcd.setCursor(0,1);
      lcd.print("R: ");
      lcd.print(red);
      lcd.print(" G: 1");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Moda: ");
      lcd.print("90");
      lcd.setCursor(0,1);
      lcd.print("Media: ");
      lcd.print("101");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Der: ");
      lcd.print(drr);
      lcd.print(" Izq: ");
      lcd.print(izz);
      lcd.setCursor(0,1);
      lcd.print("Retro: ");
      lcd.print(retrocede);
      delay(1000);
      if(digitalRead(up) == HIGH)
      {
        Serial.println("ffdgffgfg");
        estado=70;
        delay(100);
      }
  }
  else if(estado==70)
  {
    for(int i=0; i<contMas; i++)
    {
      
      if(comd[i]==1)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Adelante");
        Serial.println("Adelante");
        Wire.beginTransmission(0x01);
        Wire.write(1);
        delay(10);
        Wire.endTransmission();
        delay(2000);
      }
      else if(comd[i]==2)
      {
        Wire.beginTransmission(0x01);
        Wire.write(2);
        delay(10);
        Wire.endTransmission();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Atras");
        Serial.println("Atras");
        delay(2000);
      }
      else if(comd[i]==3)
      {
        Wire.beginTransmission(0x01);
        Wire.write(3);
        delay(10);
        Wire.endTransmission();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Izquierda");
        Serial.println("Izquierda");
        delay(500);
      }
      else if(comd[i]==4)
      {
        Wire.beginTransmission(0x01);
        Wire.write(4);
        delay(10);
        Wire.endTransmission();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Derecha");
        Serial.println("Derecha");
        delay(500);
      }
    }
    Wire.beginTransmission(0x01);
    Wire.write(5);
    delay(10);
    Wire.endTransmission();
    estado=71;
    lcd.clear();
  }
  else if(estado==71)
  {
    lcd.setCursor(0,0);
    lcd.print("Finalizado");
  }
  else if(estado==3)
  {
//      Serial.println("Modo automatico seleccionado");
//      delay(1000);
      Wire.requestFrom(0x01,1);
      while(Wire.available()>0)
      {
        code = Wire.read();
      }
         Serial.println(code);
      
      if(code==30)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Escaneando...");
      }
      else if(code==31)
      {
        dds+=10;
        red++;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Color Rojo");
        delay(500);
        code=100;
      }
      else if(code==33)
      {
        retrocede++;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Retroceso <<");
        lcd.setCursor(0,1);
        lcd.print("Angulo 180");
        delay(500);
        code=100;
      }
      else if(code==34)
      {
        izz++;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("izquierda <-");
        lcd.setCursor(0,1);
        lcd.print("Angulo 270");
        delay(800);
        code=100;
      }
       else if(code==35)
      {
        drr++;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Derecha ->");
        lcd.setCursor(0,1);
        lcd.print("Angulo 90");
        delay(800);
        code=100;
      }
      else if(code==32)
      {
        estado=69;
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("MODO AUTOMATICO");
        lcd.setCursor(0,1);
        lcd.print("R: ");
        lcd.print(dds);
        lcd.print("cm");
        lcd.print(" Obs: ");
        lcd.print(izz+drr+retrocede);
      }
      if(digitalRead(up) == HIGH)
    {
      Serial.println("Boton UP presionado");
      comd[contMas]=1;
      contMas++;
      delay(200);
    }
    if(digitalRead(down) == HIGH)
    {
      Serial.println("Boton DOWN presionado");
      comd[contMas]=2;
      contMas++;
      delay(200);
    }
    if(digitalRead(left) == HIGH)
    {
      Serial.println("Boton LEFT presionado");
      comd[contMas]=3;
      contMas++;
      delay(200);
    }
    if(digitalRead(right) == HIGH)
    {
      Serial.println("Boton RIGHT presionado");
      comd[contMas]=4;
      contMas++;
      delay(200);
    }
  }
    
  else if(estado==4)
  {
//    Serial.println("Modo manual seleccionado");
    if(digitalRead(up) == HIGH)
    {
      Serial.println("Boton UP presionado");
      Wire.beginTransmission(0x01);
      Wire.write(10);
      delay(200);
      Wire.endTransmission();
      contadorInstrucciones++;
      modaA++;
    }
    if(digitalRead(down) == HIGH)
    {
      Serial.println("Boton DOWN presionado");
      Wire.beginTransmission(0x01);
      Wire.write(11);
      delay(200);
      Wire.endTransmission();
      contadorInstrucciones++;
      modaAA++;
    }
    if(digitalRead(left) == HIGH)
    {
      Wire.beginTransmission(0x01);
      Serial.println("Boton LEFT presionado");
      Wire.write(12);
      delay(200);
      Wire.endTransmission();
      contadorInstrucciones++;
      modaI++;
    }
    if(digitalRead(right) == HIGH)
    {
      Wire.beginTransmission(0x01);
      Serial.println("Boton RIGHT presionado");
      Wire.write(13);
      delay(200);
      Wire.endTransmission();
      contadorInstrucciones++;
      modaD++;
    }

    lcd.setCursor(1,0);
    lcd.print("COMANDOS");
    lcd.setCursor(1,1);
    lcd.print("FALTANTES: ");
    lcd.print(20-contadorInstrucciones);
    if((20-contadorInstrucciones)==10)
    {
      lcd.clear();
    }
  } 
  else if(estado==5)
  {
    if(tomaTiempo)
    {
      tiempoEjecucion = millis();
      tomaTiempo=false;
    }
    Serial.println("Mostrando datos en modo manual");
    if(digitalRead(up) == HIGH)
    {
      Serial.println("Boton UP presionado");
      Wire.beginTransmission(0x01);
      Wire.write(10);
      delay(200);
      Wire.endTransmission();
//      contadorInstrucciones++;
    }
    Wire.requestFrom(0x01,1);
    while(Wire.available()>0)
    {
      code = Wire.read();
    }
    if(code==1)
    {
      if(estadoAnt!=1)
      {
        resss++;
      }
      lcd.clear();
      estadoManual = "Avanzando";
      contador=true;
      estadoAnt=1;
    }
    else if(code==2)
    {
      if(estadoAnt!=2)
      {
        resss++;
      }
      lcd.clear();
      estadoManual="Retrocediendo";
      contador=true;
      estadoAnt=2;
    }
    else if(code==3)
    {
      if(estadoAnt!=3)
      {
        resss++;
      }
      lcd.clear();
      estadoManual="Izquierada <--";
      contador=true;
      estadoAnt=3;
    }
    else if(code==4)
    {
      if(estadoAnt!=4)
      {
        resss++;
      }
      lcd.clear();
      estadoManual="Derecha -->";
      contador=true;
      estadoAnt=4;
    }
    else if(code==5)
    {
      estadoManual="Modo Pausa";
      if(contador)
      {
        lcd.clear();
        noObstaculos++;
        contador=false;
      }
    }
    else if(code==6)
    {
      lcd.clear();
      estadoManual="Finalizado";
      enable=false;
    }
    else if(code==40)
    {
      noColores++;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Color: Rojo");
      lcd.setCursor(1,1);
      lcd.print("Cantidad: ");
      lcd.print(noColores);
      red++;
      code=100;
      delay(1000);
    }
    else if(code==41)
    {
      noColores++;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Color: Verde");
      lcd.setCursor(1,1);
      lcd.print("Cantidad: ");
      lcd.print(noColores);
      green++;
      code=100;
      delay(1000);
    }
    else if(code==42)
    {
      noColores++;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Color: Azul");
      lcd.setCursor(1,1);
      lcd.print("Cantidad: ");
      lcd.print(noColores);
      blue++;
      code=100;
      delay(1000);
    }
    else if(code==43)
    {
      noColores++;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Color: Negro");
      lcd.setCursor(1,1);
      lcd.print("Cantidad: ");
      lcd.print(noColores);
      code=100;
      delay(1000);
    }

    if(enable)
    {
      Serial.println(estadoManual);
      Serial.print("Obstaculos: ");
      Serial.println(noObstaculos);
      lcd.setCursor(1,0);
      lcd.print(estadoManual);
      lcd.setCursor(1,1);
      lcd.print("Obs: ");
      lcd.print(noObstaculos);
      lcd.print(" Res: ");
      lcd.print(20-resss);
      tiempo=((millis()-tiempoEjecucion)/1000);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Modo Manual");
      lcd.setCursor(1,1);
      lcd.print("Finalizado");
      delay(1000);
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Obstaculos: ");
      lcd.print(noObstaculos);
      lcd.setCursor(1,1);
      lcd.print("Tiempo: ");
      lcd.print(tiempo);
      lcd.print(" s");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("No Colores: ");
      lcd.print(noColores);
      lcd.setCursor(0,1);
      lcd.print("R: ");
      lcd.print(red);
      lcd.print(" G: ");
      lcd.print( green);
      lcd.print(" B: ");
      lcd.print( blue);
      delay(1000);
      lcd.clear();
      int moda;
      int media=(modaA+(modaAA*180)+(270*modaI)+(90*modaD))/(modaA+modaAA+modaI+modaD);
      if(modaA>modaAA &&  modaA>modaI &&  modaA>modaD)
      {
        moda=0;
      }
      else if(modaAA>modaA &&  modaAA>modaI &&  modaAA>modaD)
      {
        moda=180;
      }
      else if(modaI>modaAA &&  modaI>modaA &&  modaI>modaD)
      {
        moda=270;
      }
      else if(modaD>modaAA &&  modaD>modaA &&  modaD>modaI)
      {
        moda=90;
      }
      lcd.setCursor(0,0);
      lcd.print("Moda: ");
      lcd.print(moda);
      lcd.setCursor(0,1);
      lcd.print("Media: ");
      lcd.print(media);
      delay(1000);
    }
  }
  if(contadorInstrucciones == 20)
  {
    estado=5;
  }
}

void mensajeInicial()
{
  Serial.println("Bienvenido");
  lcd.createChar(0, Carita);
  lcd.begin(16,2);
  String txt1 = "GRUPO13 ACE1";
  String txt2 = "SECC<A>";
 
  for(int i = 7; i>0;i--)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    
    String texto2 = txt2.substring(i-1); 
    lcd.print(texto2);
    delay(300);
  }
  lcd.write(byte(0));
  int indice = 12;
  for (int pos = 1; pos <=12; pos++)
  {
    lcd.clear();
    lcd.setCursor(pos, 0);  
    lcd.write(byte(0));
    lcd.print("SECC<A>");
    lcd.setCursor(0, 0); 
    if(pos == 16)
    {
       lcd.write(byte(0));
       lcd.print("GRUPO13 ACE1"); 
    }
    
    String texto1 = txt1.substring(indice-1); 
    lcd.print(texto1);
    indice = indice-1;
  
    delay(300); 
  }
  lcd.setCursor(0,0);
  lcd.write(byte(0));
  lcd.print("GRUPO13 ACE1");
  lcd.write(byte(0));
  lcd.print("SECC<A>");



  for (int pos = 1; pos <=16; pos++)
  {
    lcd.clear();
    lcd.setCursor(pos, 0);
    lcd.write(byte(0));
    lcd.print("GRUPO13 ACE1");
    lcd.write(byte(0));
    lcd.print("SECC<A>");
    
    delay(300); 
  }
}
