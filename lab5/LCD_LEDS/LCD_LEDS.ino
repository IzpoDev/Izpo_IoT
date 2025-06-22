#include <LiquidCrystal.h>
//Pantalla Lcd
LiquidCrystal lcd(22,23,5,18,19,21);
//Pines de salida y entrada
int led1 = 13;
int led2 = 12;
int led3 = 14;
int led4 = 27;
int port = 34;


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int valor = analogRead(port);
  float volt = 3.3*valor/4095.0;
  lcd.setCursor(0,0);
  String mensaje = "Voltaje = " + String(volt) + " v";
  lcd.print(mensaje);
  if(volt<1.0){
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    lcd.setCursor(0,1);
    lcd.print("0,0,0,0");
  }
    if(volt<2.0 && volt>=1){
    digitalWrite(led1,HIGH);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    lcd.setCursor(0,1);
    lcd.print("1,0,0,0");
  }
    if(volt<3.0 && volt>=2){
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    lcd.setCursor(0,1);
    lcd.print("1,1,0,0");
  }
    if(volt<3.2 && volt>=3){
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
    digitalWrite(led3,HIGH);
    digitalWrite(led4,LOW);
    lcd.setCursor(0,1);
    lcd.print("1,1,1,0");
  }
    if(volt>=3.2){
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
    digitalWrite(led3,HIGH);
    digitalWrite(led4,HIGH);
    lcd.setCursor(0,1);
    lcd.print("1,1,1,1");
  }
  delay(200);
}