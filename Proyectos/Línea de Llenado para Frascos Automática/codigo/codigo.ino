//*******************************************************************************************
//Línea de Llenado para Frascos Automática
//-------------------------------------------------------------------------------------------
//Se incluyen las librerías de las funciones que vamos a utilizar
#include "HX711.h"
#include "Servo.h"

// Pin de datos y de reloj para las celdas de poder
byte pinData = 4;
byte pinClk = 5;
byte pinData2 = 7;
byte pinClk2 = 6;

//Declaración de pines a utilizar en el Arduino Mega
int bomba = 53;
int in1 = 13;
int in2 = 12;
int ena = 11;
int in3 = 9;
int in4 = 8;
int enb = 10;

//Declaración de variables a utilizar
double peso1;
double peso2;
int aux1 = 0;

//Creación de objetos para los controladores
HX711 bascula;
HX711 bascula2;
Servo myservo;
Servo myservo2;
Servo myservo3;

// Parámetro para calibrar el peso y el sensor
float factor_calibracion = 10760.0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //Inicialización del serial
//Declaración de pines como entrada o salida
 pinMode (bomba, OUTPUT);
 pinMode (in1, OUTPUT);
 pinMode (in2, OUTPUT);
 pinMode (ena, OUTPUT);
 pinMode (in3, OUTPUT);
 pinMode (in4, OUTPUT);
 pinMode (enb, OUTPUT);

 //Instrucciones para la calibración manual del sensor
 Serial.println("HX711 programa de calibración");
 Serial.println("Quita cualquier peso de la bascula");
 Serial.println("Una vez empiece a mostrar informacion de medidas, coloca un peso conocido encima
de la bascula");
 Serial.println("Presiona + para incrementar el factor de calibracion");
 Serial.println("Presiona - para disminuir el factor de calibracion");

 // Iniciar sensores
 bascula.begin(pinData, pinClk);
 bascula2.begin(pinData2, pinClk2);

 // Aplicación de la calibración
 bascula.set_scale();
 bascula2.set_scale();

 // Calibración inicial
 bascula.tare();
bascula2.tare();

// Obtención de lectura de referencia
 long zero_factor = bascula.read_average();
 long zero_factor2 = bascula2.read_average();

 // Mostrar la primera desviación
 Serial.print("Zero factor: ");
 Serial.println(zero_factor);
 Serial.print("Zero factor: ");
 Serial.println(zero_factor2);

//Pines de datos de los servomotores
 myservo.attach(52);
 myservo2.attach(50);
 myservo3.attach(48);

 //Posición inicial de los servomotores
 myservo.write(25);
 delay(1000);
 myservo2.write(120);
 delay(1000);
 myservo3.write(126);
 delay(1000);

}

//-------------------------------------------------------------------------
//--------[Ajustes generales]------------
void loop() {
  //Obtención de peso de las celdas de carga
 peso1 = bascula.get_units();
 peso2 = bascula2.get_units();

// Aplicación de calibración
 bascula.set_scale(factor_calibracion);
 bascula2.set_scale(factor_calibracion);

 // Se muestra la información de peso y calibración
 Serial.print("Leyendo: ");
 Serial.print(peso1);
 Serial.print(" kgs");
 Serial.print(" factor_calibracion f: ");
Serial.print(factor_calibracion);
 Serial.println();
 Serial.print("Leyendo: ");
 Serial.print(peso2);
 Serial.print(" kgs");
 Serial.print(" factor_calibracion i: ");
 Serial.print(factor_calibracion);
 Serial.println();
  if (Serial.available())
 {
    char temp = Serial.read();
 if (temp == '+')
    factor_calibracion += 10;
 else if (temp == '-')
    factor_calibracion -= 10;
 }

 //Condición para inicio del proceso automático
 if ((peso2 > 0.06) && (peso2 < 0.25)) {
    aux1 = 1;
 }
 if (aux1 == 1) {
//Estados de los motorreductores para el avance o paro de la banda transportadora
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(ena, 90);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enb, 90);
    delay(550);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    delay(1000);
    digitalWrite(bomba, HIGH);
    delay(1950);
    digitalWrite(bomba, LOW);
    delay(500);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(ena, 90);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enb, 90);
    delay(800);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    delay(1000);
    peso1 = bascula.get_units();
    Serial.print("Leyendo: ");
    Serial.print(peso1);
    Serial.print(" kgs");
    Serial.print(" factor_calibracion f: ");
    Serial.print(factor_calibracion);
    Serial.println();

//Condición que detecte el 60% del envase
 if (peso1 > 2.2 && peso1 < 2.8){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(ena, 90);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enb, 90);
    delay(880);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    delay(3500);

//Accionamiento de los servomotores para el sellado y entrega de las vacunas
    myservo.write(130);
    delay(1000);
    myservo2.write(70);
    delay(1000);
    myservo2.write(120);
    delay(1000);
    myservo3.write(10);
    delay(1000);
    myservo3.write(126);
    delay(1000);
    myservo.write(25);
    delay(1000);
 }
 aux1 = 0;
 Serial.print(aux1);
 }

}
