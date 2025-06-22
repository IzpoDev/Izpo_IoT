#include <LiquidCrystal.h>
#include "DHT.h"

// LCD (rs, en, d4, d5, d6, d7)
LiquidCrystal lcd(22, 23, 5, 18, 19, 21);

// DHT22
#define DHTPIN 4
DHT dht(DHTPIN, DHT11);

// Pines
int ledLlenado = 15;
int ledVaciado = 16;
int ledCalentador = 17;
int pinSensorNivel = 34;

// Variables
float temperatura = 0;
float nivel = 0;

//Estado : 1(llenado), 2(calentado), 3(vaciado)
int estado = 1;

long tiempoInicioCalentador = 0;

void setup() {
lcd.begin(16, 2);
pinMode(ledLlenado, OUTPUT);
pinMode(ledVaciado, OUTPUT);
pinMode(ledCalentador, OUTPUT);
dht.begin();
}

void loop() {
// Lectura de sensores
temperatura = dht.readTemperature();
int valorSensor = analogRead(pinSensorNivel);
nivel = map(valorSensor, 0, 4095, 0, 10);

// Mostrar en LCD: solo temperatura y nivel
lcd.setCursor(0, 0);
lcd.print("T=");
lcd.print(temperatura, 1);
lcd.print("C ");

lcd.setCursor(0, 1);
lcd.print("N=");
lcd.print(nivel,1);
lcd.print("m");

// Lógica del proceso
switch (estado) {
case 1:
digitalWrite(ledLlenado, HIGH);
digitalWrite(ledVaciado, LOW);
digitalWrite(ledCalentador, LOW);

if (nivel >= 10) {
digitalWrite(ledLlenado, LOW);
digitalWrite(ledCalentador, HIGH);
tiempoInicioCalentador = millis();
estado = 2;
}
break;

case 2:
if (millis() - tiempoInicioCalentador >= 10000) {
digitalWrite(ledCalentador, LOW);
digitalWrite(ledVaciado, HIGH);
estado = 3;
}
break;

case 3:
if (nivel <= 0) {
digitalWrite(ledVaciado, LOW);
estado = 1;
}
break;
}

delay(500);
}