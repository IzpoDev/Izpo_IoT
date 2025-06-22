#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

const char* ssid = "IzpoWifi";
const char* password = "Izpo1234567890";

// Cambia esta IP/URL por la de tu FlowFuse si es diferente
const char* server = "http://team-pablo-enrique-asmad-morgado-pabloasmad15-f8a84fe9.flowfuse.cloud";

#define DHTPIN 13
#define DHTTYPE DHT11
#define POT_PIN 34

const int LED_PIN_1 = 2;
const int LED_PIN_2 = 0;
const int LED_PIN_3 = 4;
const int LED_PIN_4 = 16;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
Serial.begin(115200);
WiFi.begin(ssid, password);

Serial.print("Conectando a WiFi");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println("\nWiFi conectado!");
Serial.println(WiFi.localIP());

dht.begin();

pinMode(LED_PIN_1, OUTPUT);
pinMode(LED_PIN_2, OUTPUT);
pinMode(LED_PIN_3, OUTPUT);
pinMode(LED_PIN_4, OUTPUT);

digitalWrite(LED_PIN_1, LOW);
digitalWrite(LED_PIN_2, LOW);
digitalWrite(LED_PIN_3, LOW);
digitalWrite(LED_PIN_4, LOW);
}

void loop() {
if (WiFi.status() == WL_CONNECTED) {
float h = dht.readHumidity();
float t = dht.readTemperature();
int potValue = analogRead(POT_PIN);
float voltage = potValue * (3.3 / 4095.0);

if (!isnan(h) && !isnan(t)) {
String json = "{\"temperatura\":" + String(t, 2) +
",\"humedad\":" + String(h, 2) +
",\"voltaje\":" + String(voltage, 2) + "}";

HTTPClient http;
http.begin(String(server) + "/datos");
http.addHeader("Content-Type", "application/json");
int httpResponseCode = http.POST(json);
Serial.print("POST /datos code: ");
Serial.println(httpResponseCode);
http.end();
}

// Leer comando desde FlowFuse
HTTPClient http2;
http2.begin(String(server) + "/comando");
int httpCode = http2.GET();

if (httpCode == 200) {
String comando = http2.getString();
Serial.print("Comando recibido: ");
Serial.println(comando);

if (comando == "LED1_ON") digitalWrite(LED_PIN_1, HIGH);
else if (comando == "LED1_OFF") digitalWrite(LED_PIN_1, LOW);
else if (comando == "LED2_ON") digitalWrite(LED_PIN_2, HIGH);
else if (comando == "LED2_OFF") digitalWrite(LED_PIN_2, LOW);
else if (comando == "LED3_ON") digitalWrite(LED_PIN_3, HIGH);
else if (comando == "LED3_OFF") digitalWrite(LED_PIN_3, LOW);
else if (comando == "LED4_ON") digitalWrite(LED_PIN_4, HIGH);
else if (comando == "LED4_OFF") digitalWrite(LED_PIN_4, LOW);
}

http2.end();
} else {
Serial.println("WiFi desconectado. Reconectando...");
WiFi.begin(ssid, password);
}

delay(5000); // Espera 5 segundos
}