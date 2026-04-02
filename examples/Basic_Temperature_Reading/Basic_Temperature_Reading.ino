/**
* 7Semi STS4x Temperature Sensor - Basic Example
*
* Library Information
* ---
* * Library  : 7Semi STS4x
* * Version  : 1.0.0
* * Author   : 7Semi
* ---
* Hardware Connection (I2C)
* ---
* STS4x        →    MCU (ESP32 / Arduino)
* ---
* VCC          →    3.3V / 5V
* GND          →    GND
* SDA          →    SDA (ESP32: GPIO21)
* SCL          →    SCL (ESP32: GPIO22)
*/

#include <Wire.h>
#include "7Semi_STS4x.h"

STS4x_7Semi sensor;

/**
* Setup function
*
* * Initializes Serial and I2C
* * Checks sensor connection
*/
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("7Semi STS4x Basic Example");

  /**
  * Initialize sensor
  *
  * * Uses default I2C (Wire)
  * * Default address 0x44
  */
  if (!sensor.begin(0x44)) {
    Serial.println("Sensor not detected!");
    while (1)
      ;
  }

  Serial.println("Sensor initialized successfully");
}

void loop() {
  float temperature;

  if (sensor.readTemperature(temperature, Precision::PRECISION_HIGH)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
  } else {
    Serial.println("Read failed!");
  }

  delay(100);
}
