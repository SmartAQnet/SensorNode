/**
 * \file sensor.h
 *
 * \brief variable and definitions for sensors
 *
 * This is the files that contains the variables, pins definitions and functions for sensors to be connected.
 * 
 * \date 19.08.2019
 */


/// DHT22 sensor communication pin
#define DHT_PIN 21
/// BME280 I2C Address 0x77 for Red, 0x76 for Purple (chinese)
#define BME_ADDR  0x77
/// BME280 I2C SDA pin
#define BME_SDA 21
/// BME280 I2C SCL pin
#define BME_SCL 22

/// use Serial 2 Object
HardwareSerial SDS_SERIAL(2); // SDS011: use Serial 2 
/// SDS011 TXD pin is connected at RXD of Serial2 Object
#define SDS011_TXD  13
/// SDS011 RXD pin is connected at TXD of Serial2 Object (no need to physically connect)
#define SDS011_RXD  16

/// Dust sensor object
//SDS011 sds; 
sds011 sds;

/// BME280 Pressure & Humidity Sensor Object 
Adafruit_BME280 bme; 

/// DHT22 Temperature & Humidity Sensor Object
DHT dht(DHT_PIN, DHT22);
/// conflicts of same defining by dht lib and esp32_thingsapi lib
#ifdef DHT22 
#undef DHT22
#endif 
