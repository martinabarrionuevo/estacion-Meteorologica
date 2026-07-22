//C++
// --- CÓDIGO OPCIÓN 3 UNIFICADO CON BASE DE DATOS Y BUZZER ---
#include "DHT.h"
#define DHTPIN 2        	// Pin digital de señal del DHT11
#define DHTTYPE DHT11   	// Tipo de sensor

const int pinLedCalor = 13; // LED Rojo
const int pinLedFrio = 12;  // LED Azul
const int pinBuzzer = 11;   // Buzzer para Alerta Sonora

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(pinLedCalor, OUTPUT);
  pinMode(pinLedFrio, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  Serial.begin(9600);  
  dht.begin();
}

void loop() {
  // Esperamos los 2 segundos obligatorios del sensor
  delay(2000);
  
  float humedad = dht.readHumidity();       
  float temperatura = dht.readTemperature(); 

  // Control de fallos físicos
  if (isnan(humedad) || isnan(temperatura)) {
    Serial.println("Error de lectura: Revisar conexiones del DHT11");
    return;
  }

  // --- IMPRESIÓN ADAPTADA PARA PYTHON (Humedad,Temperatura) --- 
  Serial.print(humedad);
  Serial.print(",");
  Serial.println(temperatura);  

  // --- LÓGICA DE DECISIÓN AVANZADA (CON BUZZER Y RANGOS) ---  
  if (temperatura >= 35) {
    // CASO 1: PELIGRO POR CALOR EXTREMO (Rojo + Sonido)
    digitalWrite(pinLedCalor, HIGH);
    digitalWrite(pinLedFrio, LOW);
    tone(pinBuzzer, 1000); // Sonido de alerta de 1KHz
  } 
  else if (temperatura >= 30 && temperatura < 35) {
    // CASO 2: PRECAUCIÓN (Ambos LEDs prendidos = "Amarillo", sin sonido)
    digitalWrite(pinLedCalor, HIGH);
    digitalWrite(pinLedFrio, HIGH);
    noTone(pinBuzzer);
  }
  else if (temperatura <= 15) {
    // CASO 3: FRÍO EXTREMO (Azul prendido)
    digitalWrite(pinLedCalor, LOW);
    digitalWrite(pinLedFrio, HIGH);
    noTone(pinBuzzer);
  }
  else {
    // CASO 4: ESTADO ÓPTIMO (Todo apagado)
    digitalWrite(pinLedCalor, LOW);
    digitalWrite(pinLedFrio, LOW);
    noTone(pinBuzzer);
  }
}

