//Code that the ESP32 device is running to collect mpu and bme data
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <SPI.h>
#include "BluetoothSerial.h"
String device_name = "ESP32-BT";


#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
Adafruit_MPU6050 mpu;
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);

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
    getMPUData();
    getBMEData();

    digitalWrite(2,HIGH);
    delay(100);
    digitalWrite(2,LOW);
    Serial.println("");
  }
  
  delay(500);

}

void getMPUData(){
    // put your main code here, to run repeatedly:
    sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  SerialBT.print("Acceleration X: ");
  SerialBT.print(a.acceleration.x);
  SerialBT.print(", Y: ");
  SerialBT.print(a.acceleration.y);
  SerialBT.print(", Z: ");
  SerialBT.print(a.acceleration.z);
  SerialBT.println(" m/s^2");

  SerialBT.print("Rotation X: ");
  SerialBT.print(g.gyro.x);
  SerialBT.print(", Y: ");
  SerialBT.print(g.gyro.y);
  SerialBT.print(", Z: ");
  SerialBT.print(g.gyro.z);
  SerialBT.println(" rad/s");

  SerialBT.print("Temperature: ");
  SerialBT.print(temp.temperature);
  SerialBT.println(" degC");
}

void getBMEData(){
  
  SerialBT.print("Temperature = ");
  SerialBT.print(bme.readTemperature());
  SerialBT.println(" °C");

  SerialBT.print("Pressure = ");

  SerialBT.print(bme.readPressure() / 100.0F);
  SerialBT.println(" hPa");

  SerialBT.print("Approx. Altitude = ");
  SerialBT.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  SerialBT.println(" m");

  SerialBT.print("Humidity = ");
  SerialBT.print(bme.readHumidity());
  SerialBT.println(" %");

  SerialBT.println();

}
