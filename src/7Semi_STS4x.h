/**
 * 7Semi STS4x Temperature Sensor Library
 *
 * - Fixed bugs and improved reliability
 * - All comments follow 7Semi standard
 * - Clean and production-ready implementation
 */

#ifndef _7SEMI_STS4X_H_
#define _7SEMI_STS4X_H_

#include <Arduino.h>
#include <Wire.h>

#define STS4X_I2C_ADDR 0x44

/**
 * STS4x Commands
 */
#define STS4X_READ_SERIAL 0x89
#define STS4X_I2C_SOFT_RESET 0x94

/**
 * Measurement Precision 
 *
 * - LOW    : fastest, lowest accuracy
 * - MEDIUM : balanced mode
 * - HIGH   : best accuracy
 */
enum class Precision {
    PRECISION_LOW = 0xE0,
    PRECISION_MEDIUM = 0xF6,
    PRECISION_HIGH = 0xFD
};

class STS4x_7Semi {
public:
    STS4x_7Semi();

    /**
     * Initialize sensor
     *
     * - Sets I2C port and speed
     * - Optional ESP32 SDA/SCL override
     * - Checks sensor presence
     *
     * - returns : true if sensor detected
     */
    bool begin(uint8_t i2cAddress = STS4X_I2C_ADDR,
               TwoWire &i2cPort = Wire,
               uint32_t i2cClock = 100000,
               uint8_t sda = 255,
               uint8_t scl = 255);

    /**
     * Read temperature (single shot)
     *
     * - Uses selected precision
     * - Applies smoothing if enabled
     *
     * - temperature : output Celsius value
     *
     * - returns : true if success
     */
    bool readTemperature(float &temperature, Precision precision = Precision::PRECISION_HIGH);

    /**
     * Quick temperature read
     *
     * - Uses HIGH precision
     * - Returns NAN on failure
     */
    float readTemperature();

    /**
     * Perform soft reset
     *
     * - Clears internal state
     * - Reboots sensor internally
     */
    bool softReset();

    /**
     * Read serial number (64-bit)
     *
     * - Unique sensor ID with CRC
     */
    bool readSerialNumber(uint64_t &serial_number);

    /**
     * Read serial number (32-bit)
     *
     * - Lower 32 bits only without CRC
     */
    bool readSerialNumber(uint32_t &serial_number);

    /**
     * Enable / disable smoothing filter
     *
     * - factor : smoothing strength (0.01 to 1.0)
     */
    void enableSmooth(bool enable, float factor = 0.1f);

private:
    TwoWire *i2c;
    uint8_t address;

    /**
     * Smoothing configuration
     *
     * - smoothing : enable flag
     * - smooth_temperature : filtered value
     * - smoothing_factor : weight of new value
     */
    bool smoothing = true;
    float smooth_temperature = 0;
    float smoothing_factor = 0.1f;
    bool filterInitialized = false;

    /**
     * Send command to sensor
     *
     * - cmd : 8-bit command
     */
    bool sendCommand(uint8_t cmd);

    /**
     * Convert raw sensor value
     *
     * - Formula from Sensirion datasheet
     * - Range approx : -45°C to +130°C
     */
    float getTemperatureC(uint16_t raw);

    /**
     * CRC calculation
     *
     * - Polynomial : 0x31
     * - Initial value : 0xFF
     * - Used to validate sensor data
     */
    uint8_t getCRC(const uint8_t *data, uint8_t len);

    /**
     * Apply smoothing filter
     *
     * - Exponential moving average
     */
    float smooth(float new_temperature);
};

#endif

