#include "7Semi_STS4x.h"

STS4x_7Semi::STS4x_7Semi() {}

bool STS4x_7Semi::begin(uint8_t i2cAddress, TwoWire &i2cPort, uint32_t i2cClock, uint8_t sda, uint8_t scl)
{
    i2c = &i2cPort;
    address = i2cAddress;

#ifdef ARDUINO_ARCH_ESP32
    if (sda != 255 && scl != 255)
        i2c->begin(sda, scl);
    else
        i2c->begin();
#else
    i2c->begin();
#endif

    i2c->setClock(i2cClock);


    // If ACK received sensor present
    i2c->beginTransmission(address);
    return (i2c->endTransmission() == 0);
}

bool STS4x_7Semi::sendCommand(uint8_t cmd)
{
    // Send 8-bit command
    i2c->beginTransmission(address);
    i2c->write(cmd & 0xFF);
    return (i2c->endTransmission() == 0);
}

float STS4x_7Semi::getTemperatureC(uint16_t raw)
{
    /**
     * Conversion formula
     *
     * - T = -45 + 175 * raw / 65535
     */
    return -45.0f + (175.0f * raw) / 65535.0f;
}

void STS4x_7Semi::enableSmooth(bool enable, float factor)
{
    smoothing = enable;

    // Clamp smoothing factor
    if (factor > 1.0f)
        smoothing_factor = 1.0f;
    else if (factor <= 0.0f)
        smoothing_factor = 0.01f;
    else
        smoothing_factor = factor;
}

float STS4x_7Semi::smooth(float new_temperature)
{
    // if not initialize 
    if (!filterInitialized)
    {
         // Initialize smoothning
        smooth_temperature = new_temperature;
        filterInitialized = true;
        return smooth_temperature;
    }
     
    // temperature smoothing 
    smooth_temperature = smoothing_factor * new_temperature + (1.0f - smoothing_factor) * smooth_temperature;
    return smooth_temperature;
}

bool STS4x_7Semi::readTemperature(float &temperature, Precision precision)
{
    //Send command to read temperature measurement 
    if (!sendCommand((uint8_t)precision))
        return false;

    // Wait for measurement to ready
    delay(10);

    uint8_t buf[3];

    if (i2c->requestFrom(address, (uint8_t)3) != 3)
        return false;

    for (int i = 0; i < 3; i++)
        buf[i] = i2c->read();

    // Validate CRC
    if (getCRC(buf, 2) != buf[2])
        return false;

    // Mearge 8 bits to 16 bits
    uint16_t raw = (buf[0] << 8) | buf[1];

    // Convert raw temperature to temperature in celcius
    temperature = getTemperatureC(raw);

    //Smooth temperature values
    if (smoothing)
        temperature = smooth(temperature);

    return true;
}

float STS4x_7Semi::readTemperature()
{
    float temp;
    //Read temeprature in high precision
    return readTemperature(temp, Precision::PRECISION_HIGH) ? temp : NAN;
}

bool STS4x_7Semi::readSerialNumber(uint64_t &serial_number)
{
    if (!sendCommand(STS4X_READ_SERIAL))
        return false;

    uint8_t buf[6];

    if (i2c->requestFrom(address, (uint8_t)6) != 6)
        return false;

    for (int i = 0; i < 6; i++)
        buf[i] = i2c->read();

    // Validate both CRC blocks 
    if (getCRC(buf, 2) != buf[2]) 
        return false;
    if (getCRC(&buf[3], 2) != buf[5]) 
        return false;

    // Assemble serial number into 64 bits
    serial_number =
        ((uint64_t)buf[0] << 40) |
        ((uint64_t)buf[1] << 32) |
        ((uint64_t)buf[2] << 24) |
        ((uint64_t)buf[3] << 16) |
        ((uint64_t)buf[4] <<  8) |
        ((uint64_t)buf[5] <<  0);

    return true;
}

bool STS4x_7Semi::readSerialNumber(uint32_t &serial_number)
{
 if (!sendCommand(STS4X_READ_SERIAL))
        return false;

    uint8_t buf[6];

    if (i2c->requestFrom(address, (uint8_t)6) != 6)
        return false;

    for (int i = 0; i < 6; i++)
        buf[i] = i2c->read();

    // Validate both CRC blocks 
    if (getCRC(buf, 2) != buf[2]) 
        return false;
    if (getCRC(&buf[3], 2) != buf[5]) 
        return false;

    // Assemble serial number into 32 bits
    serial_number =
        ((uint32_t)buf[0] << 24) |
        ((uint32_t)buf[1] << 16) |
        ((uint32_t)buf[3] <<  8) |
        ((uint32_t)buf[4] <<  0);

    return true;
}

bool STS4x_7Semi::softReset()
{

    // Send reset command
    return sendCommand(STS4X_I2C_SOFT_RESET);
}

uint8_t STS4x_7Semi::getCRC(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xFF;

    for (uint8_t j = 0; j < len; j++)
    {
        crc ^= data[j];

        for (uint8_t i = 0; i < 8; i++)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc <<= 1;
        }
    }

    return crc;
}
