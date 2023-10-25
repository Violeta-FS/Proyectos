//******************************************************************************
// Sistema Contador de Revoluciones Por Minuto
// Adecuado para odómetros en bicicletas y similares
// Emplea un generador de pulsos para simular la entrada
//------------------------------------------------------------------------------
#include <TimerOne.h>

#define MainPeriod 100                // período de acumulación de pulsos y salida en serie, milisegundos
long previousMillis = 0;              // Almacena el tiempo de fin del ciclo
volatile unsigned long duration = 0;  // Acumulador del Ancho del Pulso
volatile unsigned int pulsecount = 0;
volatile unsigned long previousMicros = 0;

const int Clock = 10;                 // Conexiones para el CI 74HC595
const int Data = 9;
const int Latch = 8;

const int SEG0 = 7;
const int SEG1 = 6;
const int SEG2 = 5;
const int SEG3 = 4;

int cc = 0;
char Value[4];
const char SegData[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

//==============================================================================
//-----[ AJUSTES GENERALES ]-----
//==============================================================================
void setup() {
  // Iniciaización de elementos que manejan el desplegador
  attachInterrupt(0, myinthandler, RISING); // Manejador de Interrupción Timer 1
  pinMode(Data, OUTPUT);
  pinMode(Clock, OUTPUT);
  pinMode(Latch, OUTPUT);
  pinMode(SEG0, OUTPUT);
  pinMode(SEG1, OUTPUT);
  pinMode(SEG2, OUTPUT);
  pinMode(SEG3, OUTPUT);

  // Inicalizar el rastreo del Desplegador de Dígitos
  cc = 0;
  // Ajuster de Timer de un valor de 100000 microsegundos (0.1 sec - o 10Hz)
  // => Un LED parpadearía 5 veces por segundo, Onn-Off.
  Timer1.initialize(10000);
  Timer1.attachInterrupt( timerIsr );       // Subrutina servicio a la interrupción
}
//==============================================================================
//-----[ LAZO PRINCIPAL ]-----
//==============================================================================
void loop() {
  char rpm[4];
  int RPM;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= MainPeriod) {
    previousMillis = currentMillis;
    // necesita almacenar en búfer para evitar fallas
    unsigned long _duration = duration;
    unsigned long _pulsecount = pulsecount;
    duration = 0;                            // contadores claros
    pulsecount = 0;
    float Freq = 1e6 / float(_duration);     //La duración está en uSegundos por lo que es 1e6 / T
    Freq *= _pulsecount;                     //calcular 
    //Convertir frecuencia a RPM
    RPM = Freq * 60.0 * 1.0;                 //RPM = Freq * 60 * (Número de pulsos por revolución)
    // Mostrar frecuencia en segmentos
    sprintf(rpm, "%04d", RPM);               //Obtenemos una matriz ASCII en voltios

    Value[0] = rpm[0] & 0x0F;               //Anding con 0x0F para eliminar el nibble superior
    Value[1] = rpm[1] & 0x0F;               //Ex. el número 2 en ASCII es 0x32, queremos solo 2
    Value[2] = rpm[2] & 0x0F;
    Value[3] = rpm[3] & 0x0F;
  }
  delay(200);
}
//==============================================================================
//-----[ GENERACIÓN de DATOS ]-----
//==============================================================================
void DisplayDigit(char d) {
  int i;
  for (i = 0; i < 8; i++) {                //Cambio de datos bit a bit en el registro de desplazamiento
    if ((d & 0x80) == 0x80) {
      digitalWrite(Data, HIGH);
    }
    else
    {
      digitalWrite(Data, LOW);
    }
    d = d << 1;
    //Dar pulso de reloj
    digitalWrite(Clock, LOW);
    digitalWrite(Clock, HIGH);
  }
  //Bloquear los datos
  digitalWrite(Latch, LOW);
  digitalWrite(Latch, HIGH);
}
//==============================================================================
// INTERRUPCIÓN DE DESBORDAMIENTO DEL TEMPORIZADOR 1 PARA PANTALLA
//==============================================================================
void timerIsr() {
  cc++;
  if (cc == 5)                              // Sólo se tienen 4 dígitos
  {cc = 1;}
  Scanner();
  TCNT0 = 0xCC;
}
//==============================================================================
// FUNCIÓN DE PANTALLA DE ESCANEO
//==============================================================================
void Scanner() {
  switch (cc) {
    // Selección del dígito a actualizar 
    case 1:
      digitalWrite(SEG3, HIGH);
      DisplayDigit(SegData[Value[0]]);
      digitalWrite(SEG0, LOW);
      break;
    case 2:
      digitalWrite(SEG0, HIGH);
      DisplayDigit(SegData[Value[1]]);
      digitalWrite(SEG1, LOW);
      break;
    case 3:
      digitalWrite(SEG1, HIGH);
      DisplayDigit(SegData[Value[2]]);
      digitalWrite(SEG2, LOW);
      break;
    case 4:
      digitalWrite(SEG2, HIGH);
      DisplayDigit(SegData[Value[3]]);
      digitalWrite(SEG3, LOW);
      break;
  }
}
//==============================================================================
// MANEJADOR de INTERRUPCIONES del TIMER
//==============================================================================
void myinthandler(){
  unsigned long currentMicros = micros();
  duration += currentMicros - previousMicros;
  previousMicros = currentMicros;
  pulsecount++;
  }
//******************************************************************************
