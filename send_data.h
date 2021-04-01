/**
 * \file send_data.h
 *
 * \brief function to send json data to an API
 *
 * This is the files that contains the functions to send json data to an API.
 * 
 * \date 19.08.2019
 */

/// Externally define variable for Chip ID
extern String esp32_chipid;
/// Externally define variable to check if using bme280
extern bool use_bme280;
/// Externally define variable to check if using dht22
extern bool use_dht22;


/**************************************************************************/
/*!
    @brief  Send JSON data to API
    @param data_json the JSON Message to be sent
    @param destination the destination API  madavi or luftdaten
    @param pin the pin-code values
    @returns void
*/
/**************************************************************************/

void sendData(const char *data_json, char destination[], String pin) {
  
  
  HTTPClient http;   
  
  http.begin(destination);  //Specify destination for HTTP request
  http.addHeader("Content-Type", "application/json");             //Specify content-type header
  http.addHeader("X-PIN", pin);
  http.addHeader("X-Sensor", "esp32-"+esp32_chipid);
  
  int httpResponseCode = http.POST(data_json);   //Send the actual POST request
  Serial.print("data: ");
  Serial.println(data_json);
  if(httpResponseCode>0){
  
  String response = http.getString();                       //Get the response to the request
  Serial.print("httpResponseCode: ");
  Serial.println(httpResponseCode);   //Print return code
  Serial.print("response: ");           //Print request answer
  Serial.println(response);         
//  return "hallo";
  }
  else{
  Serial.print("Error on sending POST: ");
  Serial.println(httpResponseCode);
  }
   http.end();  //Free resources
}
