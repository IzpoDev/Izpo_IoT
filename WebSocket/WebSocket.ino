#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

// Pines y sensores
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define POT_PIN 34
#define LED1 2
#define LED2 0
#define LED3 4

// *** CAMBIA ESTOS VALORES ***
const char* ssid = "IzpoWifi";                    // Cambia por tu red WiFi
const char* password = "Izpo12345678901";            // Cambia por tu contraseña WiFi
const char* websocket_server = "192.168.229.83";  // Cambia por la IP de tu PC
const uint16_t websocket_port = 1880;            // Puerto de Node-RED (normalmente 1880)
const char* websocket_path = "/ws";              // No cambies esto

WebSocketsClient webSocket;
unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 5000; // Enviar datos cada 5 segundos

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("WebSocket Desconectado");
      break;
      
    case WStype_CONNECTED:
      Serial.printf("WebSocket Conectado a: %s\n", payload);
      break;
      
    case WStype_TEXT:
      {
        String msg = String((char*)payload);
        Serial.printf("Mensaje recibido: %s\n", msg.c_str());
        
        // Parsear JSON recibido
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, msg);
        
        // Controlar LEDs
        if (doc.containsKey("led1")) {
          String estado = doc["led1"];
          digitalWrite(LED1, estado == "ON" ? HIGH : LOW);
          Serial.printf("LED1: %s\n", estado.c_str());
        }
        if (doc.containsKey("led2")) {
          String estado = doc["led2"];
          digitalWrite(LED2, estado == "ON" ? HIGH : LOW);
          Serial.printf("LED2: %s\n", estado.c_str());
        }
        if (doc.containsKey("led3")) {
          String estado = doc["led3"];
          digitalWrite(LED3, estado == "ON" ? HIGH : LOW);
          Serial.printf("LED3: %s\n", estado.c_str());
        }
      }
      break;
      
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando ESP32...");
  
  // Inicializar sensores y LEDs
  dht.begin();
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  // Configurar ADC
  analogReadResolution(12);        // 0-4095
  analogSetAttenuation(ADC_11db);  // 0-3.3V
  
  // Conectar a WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.printf("WiFi conectado! IP: %s\n", WiFi.localIP().toString().c_str());
  
  // Conectar WebSocket
  webSocket.begin(websocket_server, websocket_port, websocket_path);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
  
  Serial.println("ESP32 listo!");
}

void loop() {
  webSocket.loop();
  
  // Enviar datos de sensores cada 5 segundos
  if (millis() - lastSensorRead >= sensorInterval) {
    lastSensorRead = millis();
    
    // Leer sensores
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    uint16_t rawValue = analogRead(POT_PIN);
    float voltage = rawValue * 3.3 / 4095.0;
    
    // Verificar si las lecturas son válidas
    if (!isnan(h) && !isnan(t)) {
      // Crear JSON con los datos
      DynamicJsonDocument doc(1024);
      doc["temperatura"] = round(t * 10) / 10.0;  // Redondear a 1 decimal
      doc["humedad"] = round(h * 10) / 10.0;
      doc["voltaje"] = round(voltage * 100) / 100.0; // Redondear a 2 decimales
      
      String jsonString;
      serializeJson(doc, jsonString);
      
      // Enviar por WebSocket
      webSocket.sendTXT(jsonString);
      
      // Mostrar en monitor serie
      Serial.printf("Enviado: %s\n", jsonString.c_str());
      Serial.printf("LEDs - 1:%d 2:%d 3:%d\n", 
                    digitalRead(LED1), digitalRead(LED2), digitalRead(LED3));
    } else {
      Serial.println("Error leyendo sensores DHT");
    }
  }
}