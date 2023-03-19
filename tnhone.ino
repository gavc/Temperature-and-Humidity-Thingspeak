/*

Write to ThingSpeak Temp and Humidity
Thanks to: https://github.com/mathworks/thingspeak-arduino/tree/master/examples/ESP8266/program%20board%20directly/WriteMultipleFields

*/

#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <WiFiManager.h>
#include <ThingSpeak.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();
WiFiManager wifiManager;

float temperature = 0;
float humidity = 0;
String myStatus = "";

unsigned long myChannelNumber = SECRET_CHANNEL;
const char * myWriteAPIKey = "SECRET_API";

unsigned long previousMillis = 0;
const long interval = 60000;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for serial port to connect
  }

  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // WiFiManager
  wifiManager.autoConnect("SHT30Sensor");

  if (!sht31.begin(0x45)) { // Address of SHT31 on the I2C bus
    Serial.println("Couldn't find SHT31");
    while (1) {}
  }

  Serial.println("SHT31 found!");

  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  float temperature = sht31.readTemperature();
  float humidity = sht31.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(1000); // Wait for 1 second before taking another reading

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    float temperature = sht31.readTemperature();
    float humidity = sht31.readHumidity();

    // set the fields with the values
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }
}
