//******************************************************************************
// Controlador de Temperatura. Emplea el CI LM35.
//------------------------------------------------------------------------------
//---[ BIBLIOTECAS ]---
#include <LiquidCrystal.h>
// Inicialización de la LCD
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

const int LED_RED = 10;     // LED Rojo
const int LED_GREEN = 11;   // LED Verde
const int RELAY = 12;       // Motor o Relevador

// Conexiones de Teclas
const int up_key = 3;
const int down_key = 2;

int SetPoint = 30;
//-----[ AJUSTES GENERALES ]-----
void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(up_key, INPUT);
  pinMode(down_key, INPUT);

  // "Pull up" para las teclas
  digitalWrite(up_key, HIGH);
  digitalWrite(down_key, HIGH);

  // Ajuste de características de la LCD
  lcd.begin(20, 4);                 // ¡OJO! Debe usarse una LCD de 20x4

  lcd.print("Control de Temperatura");
  lcd.setCursor(0, 1);              // Segunda línea
  lcd.print("Temp. Controller");
  lcd.setCursor(0, 2);              //Tercera línea
  lcd.print("Modificado por:");
  lcd.setCursor(2, 3);              //Cuarta línea
  lcd.print("Equipo 2");
  digitalWrite(LED_GREEN, HIGH);    // Apagado del LED Verde
  digitalWrite(LED_RED, LOW);       // Encendido del LED Rojo
  digitalWrite(RELAY, LOW);         // Apagado del Relevador o Motor
  delay(2000);                      // Espera para ajustes
}
//-----[ LAZO PRINCIPAL ]-----
void loop() {
  // Escalamiento de 10mV por degree 0.01V/C.
  double Temperature = ((5.0 / 1024.0) * analogRead(A0)) * 100;

  lcd.setCursor(0, 0);
  lcd.print("Temperature:");        // Sin introducir las teclas
  lcd.print(Temperature);

  // Obtención de los ajustes del usuario
  if (digitalRead(down_key) == LOW) {
    if (SetPoint > 0)
    { SetPoint--;
    }
  }
  if (digitalRead(up_key) == LOW)
  {
    if (SetPoint < 150)
    {
      SetPoint++;
    }
  }

  // Muestra del "Set point"
  lcd.setCursor(0, 1);
  lcd.print("Set Point:");
  lcd.print(SetPoint);
  lcd.print("C   ");

  // Verificación si la Temperatura está en el límite
  if (Temperature > SetPoint)
  {
    digitalWrite(RELAY, LOW);       // Apagado del calefactor
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);   // Encendido del LED Verde
  }
  else
  {
    digitalWrite(RELAY, HIGH);      // Encendido del calefactor
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);     // Encendido del LED Rojo
  }
  delay(100);                     // ACtualización cada 100 milisegundos
}
//******************************************************************************
