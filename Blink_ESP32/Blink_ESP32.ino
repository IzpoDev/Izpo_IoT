#define BLYNK_AUTH_TOKEN "SFLzVP-YPJVtxrwJa_7kXBWoCEJtwvu2"
#define BLYNK_TEMPLATE_ID "TMPL20LwUoRWV"
#define BLYNK_TEMPLATE_NAME "Practica Calificada"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

BlynkTimer timer;

char network[] = "IzpoWifi";
char password[] = "Izpo12345678901";

#include "DHT.h"
#define DHTPIN 13 
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

// Definiciones para potenciómetro y LEDs
#define POT_PIN 34
#define LED1 2
#define LED2 0
#define LED3 4

void readSensor(){
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Blynk.virtualWrite(V0,t);
  Blynk.virtualWrite(V1,h);
  
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));

  // Leer potenciómetro (voltaje)
  int analogValue = analogRead(POT_PIN);
  float voltage = (analogValue / 4095.0) * 3.3; // Convertir a voltaje (0-3.3V)
  Blynk.virtualWrite(V2, voltage);
  
  Serial.print(F(" Voltage: "));
  Serial.print(voltage);
  Serial.println(F("V"));
}

// Control de LEDs desde Blynk
BLYNK_WRITE(V3) { digitalWrite(LED1, param.asInt()); }
BLYNK_WRITE(V4) { digitalWrite(LED2, param.asInt()); }
BLYNK_WRITE(V5) { digitalWrite(LED3, param.asInt()); }

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN,network,password);
  dht.begin();
  
  // Configurar pines de LEDs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  
  timer.setInterval(2000L,readSensor);
}

void loop() {
 Blynk.run();
 timer.run();
}