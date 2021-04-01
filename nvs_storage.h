/**
 * \file nvs_storage.h
 *
 * \brief function to store and read nvs flash variables
 *
 * This is the files that contains the functions to save and read data from the nvs flash.
 * 
 * \date 19.08.2019
 * 
 */
 
#include <Preferences.h>

/// Initialize a Preferences Object to store the data into ESP32 flash
Preferences preferences;


/**************************************************************************/
/*!
    @brief  store given value of a key in a namespace
    @param used_namespace the intended namespace to store the values
    @param key_nvs the key to be stored at
    @param value the String key to be stored
    @returns void
*/
/**************************************************************************/
void set_value(const char used_namespace[14], const char key_nvs[14], String value) {
  // Open Preferences with my-app namespace.
  // RW-mode if second parameter is false
  Serial.print("[NVS] Saving ");
  Serial.print(key_nvs);
  Serial.print(" to ");
  Serial.print(used_namespace);
  Serial.print(": ");
  Serial.print(value);
  preferences.begin(used_namespace, false);
  preferences.putString(key_nvs, value);
  // Close the Preferences
  preferences.end();
  Serial.println(" ...done.");
}

/**************************************************************************/
/*!
    @brief  retrieve given value of a key in a namespace
    @param used_namespace the intended namespace to get values
    @param key_nvs the String key to be retrieved
    @returns String  retrieved string value
*/
/**************************************************************************/
String get_value(const char used_namespace[14], const char key_nvs[14]) {
  Serial.print("[NVS] Loading ");
  Serial.print(key_nvs);
  Serial.print(" from ");
  Serial.print(used_namespace);
  Serial.print(": ");
  // Open Preferences with my-app namespace.
  // RW-mode if second parameter is false
  preferences.begin(used_namespace, true);
  // get value of key, assume value is String
  // if key does not exist, return a default value
//  const char *default_value = "none";
  String tmp = preferences.getString(key_nvs);
  // Close the Preferences
  preferences.end();
  Serial.print(tmp);
  Serial.println(" ...done.");
  return tmp;
}

/**************************************************************************/
/*!
    @brief  store given int value of a key in a namespace
    @param used_namespace the intended namespace to be used
    @param key_nvs the key to be stored
    @param value the integer key to be stored
    @returns void
*/
/**************************************************************************/
void set_integer(const char used_namespace[14], const char key_nvs[14], unsigned int value) 
{
  Serial.print("[NVS] Saving ");
  Serial.print(key_nvs);
  Serial.print(" to ");
  Serial.print(used_namespace);
  Serial.print(": ");
  Serial.print(value);
  preferences.begin(used_namespace, false);
  preferences.putUInt(key_nvs, value);
  // Close the Preferences
  preferences.end();
  Serial.println(" ...done.");
}
/**************************************************************************/
/*!
    @brief  retrieve given int value of a key in a namespace
    @param used_namespace the intended namespace
    @param key_nvs the key to be retrieved
    @returns int retrieved integer value
*/
/**************************************************************************/
unsigned int get_integer(const char used_namespace[14], const char key_nvs[14]) 
{
  Serial.print("[NVS] Loading ");
  Serial.print(key_nvs);
  Serial.print(" from ");
  Serial.print(used_namespace);
  Serial.print(": ");
  preferences.begin(used_namespace, true);
  unsigned int tmp = preferences.getUInt(key_nvs,0);
  preferences.end();
  Serial.print(tmp);
  Serial.println(" ...done.");
  return tmp;
}
