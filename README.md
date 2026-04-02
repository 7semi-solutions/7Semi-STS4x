# 7Semi STS4x Arduino Library

Arduino driver for the Sensirion STS4x digital temperature sensors.

The STS4x series are high-accuracy digital temperature sensors with fast response time and low power consumption. These sensors support I²C communication and are ideal for applications such as:

- Environmental monitoring  
- HVAC systems  
- Industrial sensing  
- IoT devices  

This library provides a clean and efficient interface for:

- Temperature (°C) reading  
- CRC-validated data communication  
- Configurable measurement precision (repeatability)  
- Optional smoothing filter  

---

## Features

- Temperature measurement (°C)
- Supports STS40 / STS41 / STS45
- High, medium, and low precision modes
- CRC validation for reliable data
- Optional smoothing (EMA filter)
- Soft reset support
- Serial number reading
- Lightweight and fast implementation
- ESP32 compatible

---

## Supported Platforms

- Arduino UNO / Mega
- ESP32
- Any board supporting Wire (I²C)

---

## Hardware

### Supported Sensors

**7Semi Temperature Sensor Breakouts:**

- STS40  
- STS41  
- STS45  

---

## Connection

The sensor uses **I²C communication**.

### I²C Wiring

| Sensor Pin | MCU Pin | Description |
|------------|--------|------------|
| VCC        | 3.3V / 5V | Sensor power |
| GND        | GND    | Ground |
| SDA        | SDA    | I²C data |
| SCL        | SCL    | I²C clock |

### I²C Notes

- Default address: `0x44`  
- Recommended speed: `100kHz – 400kHz` 

---

## Installation

### Arduino Library Manager

1. Open Arduino IDE  
2. Go to **Library Manager**  
3. Search for `7Semi STS4x`  
4. Click **Install**

### Manual Installation

1. Download repository as ZIP  
2. Go to **Sketch → Include Library → Add .ZIP Library**

---

## Example

```cpp
#include <7Semi_STS4x.h>

STS4x_7Semi sensor;

void setup()
{
    Serial.begin(115200);

    if(!sensor.begin())
    {
        Serial.println("Sensor not detected");
        while(1);
    }
}

void loop()
{
    float temperature;

    if(sensor.readTemperature(temperature))
    {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" °C");
    }
    else
    {
        Serial.println("Read failed!");
    }

    delay(100);
}

```
---

## Library Overview


## Reading Temperature
```cpp
float temperature;
sensor.readTemperature(temperature);

Serial.print("Temperature: ");
Serial.println(temperature);
```
Returns the measured temperature in degrees Celsius.

## Quick Read (Single Line)
```cpp
Serial.print("Temperature: ");
Serial.println(sensor.readTemperature(),2);
```
## Sensor Configuration

```cpp
sensor.readTemperature(temp, PRECISION_HIGH);
```
Parameters:

- Precision – Measurement precision level

Precision options:

```cpp
PRECISION_LOW
PRECISION_MEDIUM
PRECISION_HIGH
```

## Enable Smoothing

```cpp
sensor.enableSmooth(true, 0.1f);

```
Applies exponential moving average filter.

## Read Serial Number

```cpp
uint64_t serial;
sensor.readSerialNumber(serial);

```
Returns unique sensor ID.

## Sensor Reset

```cpp

sensor.softReset();

```

Resets internal sensor state.

## Example Applications

Typical applications include:

- Environmental monitoring

- HVAC systems

- IoT temperature sensing

- Data logging systems

- Industrial monitoring

- Smart home monitoring

---
 ## License

### MIT License

## Author

### 7Semi

