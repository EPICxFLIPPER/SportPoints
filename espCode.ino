#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <SPI.h>
#include "BluetoothSerial.h"
#include <ArduinoJson.h> // Include ArduinoJson library

String device_name = "ESP32-BT";

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

const int buttonPinOne = 12;
const int buttonPinTwo = 25;
const int buttonPinThree = 34;
const int ledPin = 5;

int buttonStateOne = 0;
int buttonStateTwo = 0;
int buttonStateThree = 0;

Adafruit_BME280 bme;
Adafruit_MPU6050 mpu;
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);

  pinMode(buttonPinOne, INPUT);
  pinMode(buttonPinTwo, INPUT);
  pinMode(buttonPinThree, INPUT);
  pinMode(ledPin, OUTPUT);

  if (!SerialBT.begin(device_name)) { // Start Bluetooth with the device name
    Serial.println("An error occurred initializing Bluetooth");
  } else {
    Serial.println("Bluetooth initialized. Waiting for connection...");
  }

  Wire.begin();

  // Try to initialize!
  if (!mpu.begin(0x68) || !bme.begin(0x76)) {
    Serial.println("Failed to find one of the chips");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {
  if (SerialBT.hasClient()) {
    // Declare and initialize a new JSON document at the start of each loop iteration
    StaticJsonDocument<1024> doc;

    String received = SerialBT.readString();
    received.trim();

    if (received == "collect") {
      digitalWrite(ledPin, HIGH);
      dataCollect(doc);
      String output;
      serializeJson(doc, output);
      SerialBT.println(output);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }
  
  delay(10);
}

void dataCollect(StaticJsonDocument<1024>& doc){
    
  getMPUData(doc);
  getBMEData(doc);
    
      
}

void getMPUData(StaticJsonDocument<1024>& doc) {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  doc["acceleration"]["x"] = a.acceleration.x;
  doc["acceleration"]["y"] = a.acceleration.y;
  doc["acceleration"]["z"] = a.acceleration.z;

  doc["gyro"]["x"] = g.gyro.x;
  doc["gyro"]["y"] = g.gyro.y;
  doc["gyro"]["z"] = g.gyro.z;

  doc["temperature"] = temp.temperature;
}

void getBMEData(StaticJsonDocument<1024>& doc) {
  doc["bme"]["temperature"] = bme.readTemperature();
  doc["bme"]["pressure"] = bme.readPressure() / 100.0F;
  doc["bme"]["altitude"] = bme.readAltitude(SEALEVELPRESSURE_HPA);
  doc["bme"]["humidity"] = bme.readHumidity();
}

