/**
 * \file create_json.h
 *
 * \brief Create JSON string out of given values
 *
 * This file is used to create JSON messages for different sensors, according to their destination and send to differnt servers.
 * 
 * \date 19.08.2019
 */

/// Externally define variable to check if using bme280
extern bool use_bme280;
/// Externally define variable to check if using dht22
extern bool use_dht22;
///Software version of the crowdsensor for json messages
const String SOFTWARE_VERSION = "esp32-crowdsensor-v"+String(FIRMWARE_VERSION);
/// The JSON Buffer length
const int bufferLength = 1024;

/**************************************************************************/
/*!
    @brief  create a json string
    @param status_sds SDS sensor status
    @param pm10 sensor's pm10 float value
    @param pm25 sensor's pm25 float value
    @param temp temperature float value
    @param hum humidity float value
    @param atm pressure float value
    @returns output String response
*/
/**************************************************************************/
String json_madavi(bool status_sds, float pm10, float pm25, float temp, float hum, float atm) {
  // create Json buffer for send2madavi-data. inside brackets: size of the pool in bytes.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<bufferLength> doc;
  //StaticJsonBuffer<1000> jsonBuffer; //TODO set a good buffer size
  // create json object
  //JsonObject& data_json = jsonBuffer.createObject();
  // add all key-value pairs to json object
  doc["esp32id"] = esp32_chipid;
  doc["software_version"] = SOFTWARE_VERSION;

  JsonArray sensordatavalues = doc.createNestedArray("sensordatavalues");
  //JsonArray& sensordatavalues = data_json.createNestedArray("sensordatavalues");

  if (!status_sds) {
    JsonObject sensordatavalues_0 = sensordatavalues.createNestedObject();
    sensordatavalues_0["value_type"] = "SDS_P1";
    sensordatavalues_0["value"] = pm10;
    JsonObject sensordatavalues_1 = sensordatavalues.createNestedObject();
    sensordatavalues_1["value_type"] = "SDS_P2";
    sensordatavalues_1["value"] = pm25;
  }

  if (use_dht22 && !isnan(temp)) {
    JsonObject sensordatavalues_2 = sensordatavalues.createNestedObject();
    sensordatavalues_2["value_type"] = "temperature";
    sensordatavalues_2["value"] = temp;
    JsonObject sensordatavalues_3 = sensordatavalues.createNestedObject();
    sensordatavalues_3["value_type"] = "humidity";
    sensordatavalues_3["value"] = hum;
  }

  if (use_bme280 && !isnan(temp)) {
    JsonObject sensordatavalues_4 = sensordatavalues.createNestedObject();
    sensordatavalues_4["value_type"] = "BME280_temperature";
    sensordatavalues_4["value"] = temp;
    JsonObject sensordatavalues_5 = sensordatavalues.createNestedObject();
    sensordatavalues_5["value_type"] = "BME280_humidity";
    sensordatavalues_5["value"] = hum;
    JsonObject sensordatavalues_6 = sensordatavalues.createNestedObject();
    sensordatavalues_6["value_type"] = "BME280_pressure";
    sensordatavalues_6["value"] = atm;
  }

  String output;
  serializeJson(doc, output);
  //doc.printTo(output);
  return output;
}


/**************************************************************************/
/*!
    @brief  create a json string for SDS values
    @param status_sds SDS sensor status
    @param pm10 pm10 float value
    @param pm25 pm25 float value
    @returns output String response
*/
/**************************************************************************/
String json_sds(bool status_sds, float pm10, float pm25) {
  // create Json buffer for send2madavi-data. inside brackets: size of the pool in bytes.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<bufferLength> doc;
  //StaticJsonBuffer<1000> jsonBuffer; //TODO set a good buffer size
  // create json object
  //JsonObject& data_json = jsonBuffer.createObject();
  // add all key-value pairs to json object
  doc["esp32id"] = esp32_chipid;
  doc["software_version"] = SOFTWARE_VERSION;

  JsonArray sensordatavalues = doc.createNestedArray("sensordatavalues");
  //JsonArray& sensordatavalues = data_json.createNestedArray("sensordatavalues");

  if (!status_sds) {
    JsonObject sensordatavalues_0 = sensordatavalues.createNestedObject();
    sensordatavalues_0["value_type"] = "P1";
    sensordatavalues_0["value"] = pm10;
    JsonObject sensordatavalues_1 = sensordatavalues.createNestedObject();
    sensordatavalues_1["value_type"] = "P2";
    sensordatavalues_1["value"] = pm25;
  }

  String output;
  serializeJson(doc, output);
  //data_json.printTo(output);
  return output;
}


/**************************************************************************/
/*!
    @brief  create a json string for temperature & Humidity
    @param temp temperature value
    @param hum humidity value
    @param atm pressure value
    @returns JSON String
*/
/**************************************************************************/
String json_RHT(float temp, float hum, float atm) {
  // create Json buffer for send2madavi-data. inside brackets: size of the pool in bytes.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<bufferLength> doc;
  //StaticJsonBuffer<1000> jsonBuffer; //TODO set a good buffer size
  // create json object
  //JsonObject& data_json = jsonBuffer.createObject();
  // add all key-value pairs to json object
  doc["esp32id"] = esp32_chipid;
  doc["software_version"] = SOFTWARE_VERSION;

  JsonArray sensordatavalues = doc.createNestedArray("sensordatavalues");


  if (use_bme280 && !isnan(temp)) {
    JsonObject sensordatavalues_0 = sensordatavalues.createNestedObject();
    sensordatavalues_0["value_type"] = "BME280_temperature";
    sensordatavalues_0["value"] = temp;
    JsonObject sensordatavalues_1 = sensordatavalues.createNestedObject();
    sensordatavalues_1["value_type"] = "BME280_humidity";
    sensordatavalues_1["value"] = hum;
    JsonObject sensordatavalues_2 = sensordatavalues.createNestedObject();
    sensordatavalues_2["value_type"] = "BME280_pressure";
    sensordatavalues_2["value"] = atm;
  }

  else if (use_dht22 && !isnan(temp)) {
    JsonObject sensordatavalues_0 = sensordatavalues.createNestedObject();
    sensordatavalues_0["value_type"] = "temperature";
    sensordatavalues_0["value"] = temp;
    JsonObject sensordatavalues_1 = sensordatavalues.createNestedObject();
    sensordatavalues_1["value_type"] = "humidity";
    sensordatavalues_1["value"] = hum;
  }

  String output;
  serializeJson(doc, output);
  return output;
}
