//Incluir Librerias
#include <Wire.h>                           //Libreria para la realización de la comunicación i2C
#include <LiquidCrystal_I2C.h>              //Libreria para LCD

LiquidCrystal_I2C lcd ( 0x27 , 20 , 4 );    //Dirección donde se encuentra el modulo en la comunicación IU2C,cantidad de filas y columnas.

//Declaración de variables
int TS = 2;                                 //Variable para leer transport stream (TS)
bool boton = LOW;
int perdida = 0;
int conteo = 0;

int Sonido = A0;                            //Variable para leer audio modulado
int silencio = 0;                           //Variable para detectar silencios
int contar = 0;                             //Variable para contar silencios

int Pot = A2;                               //Variable para cambiar valor de silencios nos contados
int valorPot = 0;                           //Variable para almacenar variable pot

int buzzer = 3;
int LED5 = 13;                              //Variable para LED
int LED4 = 12;                              //Variable para LED 
int LED3 = 11;                              //Variable para LED
int LED2 = 10;                              //Variable para LED

void setup() {
  Serial.begin(9600);                       //Comunicación serial

  lcd.init();                               //Iniciar LCD
  lcd.backlight();                          //Encender LCD

  lcd.setCursor(2,1);                       //Posición de texto
  lcd.print("SISTEMA INTEGRAL");            //Imprimir texto de LCD
  delay(5000);                              //Tiempo en que se mantendrá en texto mostrado 
  lcd.clear();                              //Limpiar LCD
  
  pinMode(Pot,INPUT);                       //Declarar Pot como entrada
  
  pinMode(LED5,OUTPUT);                     //Declarar LED como salida
  pinMode(LED4,OUTPUT);                     //Declarar LED como salida
  pinMode(LED3,OUTPUT);                     //Declarar LED como salida
  pinMode(LED2,OUTPUT);                     //Declarar LED como salida

  pinMode(buzzer,OUTPUT);
  
  pinMode(TS,INPUT);
}

void loop() {
  TS = digitalRead(2);
  boton = digitalRead(TS);
  //Serial.println(boton);
  if(boton == HIGH)
  {
    Serial.println("SIIIIIIIIIIIIIIII");    //Imprimir texto "siii.." si no exiten silencios
  }
  else{
  Serial.println("ERROOOOOOOOOOOOOORR");    //Imprimir texto "nooo.." si existen silencios
  }
  perdida++;                             //Aumentar de 1 en 1 en número de silencio
    if(boton==HIGH){                           //Si existe algun sonido
      conteo = 0;                           //Se reicia la variable 0
      perdida = conteo;
     }
//---------------------------------------------------------------------------------------------
  
  Sonido = analogRead (A0);                 //Leer señal analogica de audio modulado
  //Serial.println(Sonido);
   if(Sonido>1)
  {
    Serial.println("SIIIIIIIIIIIIIIII");    //Imprimir texto "siii.." si no exiten silencios
  }
  else{
  Serial.println("ERROOOOOOOOOOOOOORR");    //Imprimir texto "nooo.." si existen silencios
    } 
    silencio++;                             //Aumentar de 1 en 1 en número de silencio
    if(Sonido>1){                           //Si existe algun sonido
      contar = 0;                           //Se reicia la variable 0
      silencio = contar;
     }
  Serial.println(silencio);                 //Imprimir número de silencios

  valorPot = analogRead(Pot);               //Almacenamos el valor del Pot en valor
//Imprimir valor del potenciometro equivalente al segundos (texto)
  lcd.setCursor(1,0);
  lcd.print("Elija el valor de");
  lcd.setCursor(1,1);
  lcd.print("s de silencio");
  
    if(valorPot >= 0 && valorPot <= 204)
  {
    lcd.setCursor(8,2);
    lcd.print("5 seg       ");
  }
  if(valorPot >= 205 && valorPot <= 409)
  {
    lcd.setCursor(8,2);
    lcd.print("4 seg       ");
  }
  if(valorPot >= 410 && valorPot <= 614)
  {
    lcd.setCursor(8,2);
    lcd.print("3 seg       ");
  }
  if(valorPot >= 615 && valorPot <= 818)
  {
    lcd.setCursor(8,2);
    lcd.print("2 seg       ");
  }
  if(valorPot >= 819 && valorPot <= 1023)
  {
    lcd.setCursor(8,2);
    lcd.print("0 seg       ");
  }
  
  //-------------- Variación de valores para ignorar silencios en segundos (2 segundos, 3 segundos, 4 segundos y 5 segundos)
  
  if(valorPot >= 0 && valorPot <= 204 && silencio > 201){
      digitalWrite(LED5,HIGH);       
      digitalWrite(LED4,HIGH);       
      digitalWrite(LED3,HIGH);       
      digitalWrite(LED2,HIGH);       
      digitalWrite(buzzer,HIGH);
      delay(1500);
      
      digitalWrite(LED5,LOW);
      digitalWrite(LED4,LOW);
      digitalWrite(LED3,LOW);
      digitalWrite(LED2,LOW);
      digitalWrite(buzzer,LOW);
      delay(500);
  } 
  
  if(valorPot >= 205 && valorPot <= 409 && silencio>167)
  {
      digitalWrite(LED5,HIGH);       
      digitalWrite(LED4,HIGH);       
      digitalWrite(LED3,HIGH);       
      digitalWrite(LED2,HIGH);       
      digitalWrite(buzzer,HIGH);
      delay(1500);
      
      digitalWrite(LED5,LOW);
      digitalWrite(LED4,LOW);
      digitalWrite(LED3,LOW);
      digitalWrite(LED2,LOW);
      digitalWrite(buzzer,LOW);
      delay(500);
  }
  
  if(valorPot >= 410 && valorPot <= 614 && silencio>128)
  {
      digitalWrite(LED5,HIGH);       
      digitalWrite(LED4,HIGH);       
      digitalWrite(LED3,HIGH);       
      digitalWrite(LED2,HIGH);       
      digitalWrite(buzzer,HIGH);
      delay(1500);
      
      digitalWrite(LED5,LOW);
      digitalWrite(LED4,LOW);
      digitalWrite(LED3,LOW);
      digitalWrite(LED2,LOW);
      digitalWrite(buzzer,LOW);
      delay(500);
  }
  
  if(valorPot >= 615 && valorPot <= 818 && silencio>90)
  {
      digitalWrite(LED5,HIGH);       
      digitalWrite(LED4,HIGH);       
      digitalWrite(LED3,HIGH);       
      digitalWrite(LED2,HIGH);
      digitalWrite(buzzer,HIGH);       
      delay(1500);
      
      digitalWrite(LED5,LOW);
      digitalWrite(LED4,LOW);
      digitalWrite(LED3,LOW);
      digitalWrite(LED2,LOW);
      digitalWrite(buzzer,LOW);
      delay(500);
  }

  if(perdida > 201){                         //Si se presenta una falla en un lapso de 5s los leds y buzzer se activan
      digitalWrite(LED5,HIGH);       
      digitalWrite(LED4,HIGH);       
      digitalWrite(LED3,HIGH);       
      digitalWrite(LED2,HIGH);
      digitalWrite(buzzer,HIGH);       
      delay(1500);
      
      digitalWrite(LED5,LOW);
      digitalWrite(LED4,LOW);
      digitalWrite(LED3,LOW);
      digitalWrite(LED2,LOW);
      digitalWrite(buzzer,LOW);
      delay(500);
    }
}
