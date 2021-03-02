#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

const char* ssid = "Pythagoras"; 
const char* password = "kktsbp1234"; 

const char* serverName = "http://192.168.1.37/test.php";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

#define floatSw   D2
#define waterPump D3
#define servo     D6
#define moisture  A0 
#define temp      D5

#define mDry 825
#define mWet 438

OneWire oneWire(temp); 
DallasTemperature sensors(&oneWire);

// 4051 multiplexer
#define muxA D4
#define muxB D8
#define muxC D7

float soilMoistureValue = 0;

#define opened true
#define closed false

Servo airFlap;  
int floatSwState = 0;

void setup() {
  Serial.begin(115200);
  sensors.begin();
  pinMode(servo, OUTPUT);
  airFlap.attach(servo);
  pinMode(muxA, OUTPUT);
  pinMode(muxB, OUTPUT);
  pinMode(muxC, OUTPUT);
  pinMode(waterPump, OUTPUT);
  pinMode(floatSw, INPUT);
//  startWiFi();
//  Serial.println(getMoisture());
//  updateDashboard();
}

void startWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  lastTime = millis();
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis()-lastTime>60000) {  // reset every minute
      ESP.reset();
    }
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.println(getMoisture());

}

float getMoisture() {
  digitalWrite(muxA, LOW);
  digitalWrite(muxB, LOW);
  digitalWrite(muxC, LOW);
  delay(500);
  soilMoistureValue = analogRead(moisture); // raw analog
//  return soilMoistureValue;
  return map(soilMoistureValue,mWet,mDry,0,100); // add ,ping to percentage
}

float getBat() {
  digitalWrite(muxA, HIGH);
  digitalWrite(muxB, HIGH);
  digitalWrite(muxC, HIGH);
  delay(500);
  soilMoistureValue = analogRead(moisture); // moisture pin but for battery cause idek
return map((int)((soilMoistureValue/1023)*4.2*100),300,420,0,100); // return as percentage with 3 as low and 4.2 as high 
}

void setAirFlap(boolean position) { // set airFlap servo position
  if (position == closed) {
    airFlap.write(0); 
  }
  else if (position == opened) {
    airFlap.write(90); 
  }
}

boolean getFloatSw() {
   floatSwState = digitalRead(floatSw);
  if (floatSwState == HIGH) {
    return true;
  } 
  else {
    return false;
  }
}

int getTemp() {

   sensors.begin();sensors.requestTemperatures();sensors.begin();
  return sensors.getTempFByIndex(0); // return farenheit 
}

void pump() {
  while ((getFloatSw())) {
    digitalWrite(waterPump, HIGH);
  }
  digitalWrite(waterPump, LOW);
}

boolean getTilt() {
  return true;
}

boolean getFill() {
  return false;
}

void updateDashboard() {
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"temp\":"+ String(getTemp()) + ",\"humidity\":" +String(getMoisture()) +  ",\"battery\":" + String(getBat()) + ",\"tilt\":" +String(getTilt()) + ",\"fill\":" +String(getFill())+"}" );
      String payload = http.getString(); 
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println(payload);
      JSONVar myObject = JSON.parse(payload);
      if (myObject.hasOwnProperty("vent")) {
        setAirFlap((bool) myObject["vent"]);
      }
      
        
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    
  delay(500); // let servo finish rotating
  ESP.deepSleep(10e6); 
  
}
