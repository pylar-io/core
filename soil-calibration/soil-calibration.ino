// calibrate soil moisture sensor

#define SensorPin A0 

#define muxA D4
#define muxB D8
#define muxC D7


float sensorValue = 0; 



void setup() { 
  pinMode(muxA, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(muxB, OUTPUT);
  pinMode(muxC, OUTPUT);
  digitalWrite(muxA, LOW);
  digitalWrite(muxB, LOW);
  digitalWrite(muxC, LOW);
  Serial.begin(9600); 
} 
void loop() { 

  
 for (int i = 0; i <= 100; i++) 
 { 
   sensorValue = sensorValue + analogRead(SensorPin); 
   delay(1); 
 } 
 sensorValue = sensorValue/100.0; 
 Serial.println(sensorValue); 
 delay(30); 
} 


//  
//const int dry = 842;   
//const int wet = 433;
//
//int soilMoistureValue = 0;
//int soilmoisturepercent=0;
//
//void setup() {
//  Serial.begin(9600);
//}
//void loop() {
//soilMoistureValue = analogRead(A0);
//Serial.println(soilMoistureValue);
//soilmoisturepercent = map(soilMoistureValue, dry, wet, 0, 100);
//if(soilmoisturepercent >= 100)
//{
//  Serial.println("100 %");
//}
//else if(soilmoisturepercent <=0)
//{
//  Serial.println("0 %");
//}
//else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
//{
//  Serial.print(soilmoisturepercent);
//  Serial.println("%");
//  
//}
//  delay(250);
//}
