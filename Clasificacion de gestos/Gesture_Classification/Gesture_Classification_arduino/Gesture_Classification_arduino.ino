#include <Wire.h>
#include <MPU6050.h>
#include "Pipeline.h"
#include "Classifier.h"

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Serial.println("Started");

  Wire.begin(21, 22); // SDA en D21 y SCL en D22
  mpu.initialize();

  Serial.println("Testing MPU6050 connections...");
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  Serial.print("Accelerometer sample rate = ");
  Serial.print(mpu.getRate());
  Serial.println("Hz");
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;  // Cambiados a int16_t

  // Espera por datos
  if (!mpu.getIntStatus() & 0x01) {
    return;
  }

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);  // Se pasan punteros de tipo int16_t

  // Realiza extracción de características
  float features[] = {float(ax), float(ay), float(az), float(gx), float(gy), float(gz)};
   
  if (!pipeline.transform(features)) {
    return;
  }

  // Realiza clasificación
  Serial.print("Predicted gesture: ");
  Serial.print(forest.predictLabel(pipeline.X));
  Serial.print(" (DSP: ");
  Serial.print(pipeline.latencyInMicros());
  Serial.print(" micros, Classifier: ");
  Serial.print(forest.latencyInMicros());
  Serial.println(" micros)");
}


