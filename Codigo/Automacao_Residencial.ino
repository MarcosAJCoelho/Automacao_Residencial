#include <IRremote.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include "DHT.h"
#include <SimpleTimer.h>

#define UM 3125149440             // Tecla 1
#define DOIS 3108437760             // Tecla 2
#define TRES 3091726080             // Tecla 3
#define QUATRO 3141861120             // Tecla 4
#define DHTPIN 2  
#define DHTTYPE DHT11

Servo myservo;
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer Temporizador;

int VENT = 6;
int IR_RECEIVE_PIN = 3; 
float armazenavalor;
bool flag = false;
int soundSensor=4;
int LED=5;
boolean LEDStatus=false;


void setup() {
  IrReceiver.begin(IR_RECEIVE_PIN);
  myservo.attach(9);
  pinMode(VENT, OUTPUT);
  pinMode(soundSensor,INPUT);
  pinMode(LED,OUTPUT);
  dht.begin();
  Temporizador.setInterval(30000);
  lcd.begin(16, 2);
  lcd.clear();
  delay(15);
  lcd.setCursor(0,0);
  lcd.print("   Automacao    ");
  lcd.setCursor(0,1);
  lcd.print("  Residencial   ");
  delay(2000);
}

void loop() {
  if (IrReceiver.decode()) {  
    armazenavalor = (IrReceiver.decodedIRData.decodedRawData);  
    if (armazenavalor == UM) {
      myservo.write(90);        
      delay(15);
      lcd.setCursor(0,1);
      lcd.print("  Porta Aberta  ");  
    }
    if (armazenavalor == DOIS) {
      myservo.write(0);         
      delay(15);
      lcd.setCursor(0,1);
      lcd.print(" Porta Fechada  "); 
    }
    if (armazenavalor == TRES) {
      digitalWrite(VENT, HIGH);
    }
    if (armazenavalor == QUATRO) {
      digitalWrite(VENT, LOW);
    }
    IrReceiver.resume();
  }

  int SensorData = digitalRead(soundSensor); 
  if (SensorData==1) {
    if (LEDStatus == false) {
      LEDStatus=true;
      digitalWrite(LED,HIGH);
    }
    else {
      LEDStatus = false;
      digitalWrite(LED,LOW);
    }
  }
  
  if (Temporizador.isReady()) { 
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (flag == false) {
      lcd.setCursor(0,0);
      lcd.print("   Automacao    ");
      lcd.setCursor(0,1);
      lcd.print("  Residencial   ");
      flag = true;
    }
    else if (flag == true) {
      lcd.setCursor(0,0);
      lcd.print("Temperatura:");
      lcd.print(t);
      lcd.print("Humidade: ");
      lcd.print(h);
      flag = false;
    }        
    Temporizador.reset();                     
  }
  

}
