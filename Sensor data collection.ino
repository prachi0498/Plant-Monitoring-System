#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <ThingSpeak.h>
#include <Wire.h>
WiFiClient client;
#include "DHT.h"


#define DHTTYPE DHT11         // specify the type of DHT sensor
#define DHTPIN 13 
DHT dht(DHTPIN, DHTTYPE);
#define SOIL_MOISTURE_PIN 39

#define ONE_WIRE_BUS 23
int keyIndex = 0;          // your network key Index number (needed only for WEP)


#define LDR_PIN 34

const char* ssid = "Galaxy A30s9F8F";
const char* password = "12340987";
unsigned long myChannelNumber = 2029049;
const char * myWriteAPIKey = "RKGRPA9O8V4K6JWQ";

OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  pinMode(LDR_PIN, INPUT);
  sensors.begin();
  while (WiFi.status() != WL_CONNECTED) {
    delay(10000);
    Serial.println("Connecting to WiFi...");
  }
  ThingSpeak.begin(client);
  
}

void loop() {
  float temperature =dht.readTemperature();
  float humidity =dht.readHumidity();
  int soilMoistureValue =  ( (analogRead(SOIL_MOISTURE_PIN) / 1023.00) * 100 ) ;
  Serial.println("Soil Moisture: " + String(soilMoistureValue));
  int ldrValue = analogRead(LDR_PIN);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  sensors.requestTemperatures(); // Send the command to get temperatures

  delay(10000);

  ThingSpeak.setField(1, soilMoistureValue);
  ThingSpeak.setField(2, temperature);
  ThingSpeak.setField(3, humidity);
  ThingSpeak.setField(4, sensors.getTempCByIndex(0));
  ThingSpeak.setField(5, ldrValue);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Data sent to ThingSpeak");
  }
  else{
    Serial.println("Sending failed");
  }
  delay(10000);
}
