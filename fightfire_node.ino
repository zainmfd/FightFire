#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <SHT1x.h>
#include <Adafruit_BME280.h>
const int id = 001;
#define DHTPIN 5.

#define DHTTYPE DHT11

#define dataPin 7                       
#define clockPin 8
SHT1x sht1x(dataPin, clockPin); //FOR SHT1x

int radar =6; //FOR Doppler radar  
int flame =4;
Adafruit_BME280 bme; // use I2C interface
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

void setup() {
 Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");
//  LoRa.setSyncWord(0xF3); 

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
 pinMode(radar,INPUT);
 pinMode(flame,INPUT);
  Serial.println(F("BME280 Sensor event test"));

  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    while (1) delay(10);
  }
  
  bme_temp->printSensorDetails();
  bme_pressure->printSensorDetails();
  bme_humidity->printSensorDetails();
  Serial.println("FIGHT-FIRE test");

    Serial.println();

     
}

void loop() 
    {
      
  int radarState = digitalRead(radar);
  int flameState = digitalRead(flame);
  
  int temp_c;
  int humidity;
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  
  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  humidity = sht1x.readHumidity();

  char envDataBuf[1000];

sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);
}

sprintf(envDataBuf, "{\"ID\":%d,\"temp\":%d,\"hum\":%d,\"sTemp\":%d,\"Moist\":%d,\"Motion\":%d,\"Flame\":%d}", id, temp_event.temperature, humidity_event.relative_humidity, temp_c, humidity, radarState, flameState);
 
   Serial.println(envDataBuf);

 LoRa.beginPacket();

   LoRa.println(envDataBuf);
   LoRa.endPacket();
 
 delay(2000);
}
