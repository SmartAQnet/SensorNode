/**
 * \file timer_definitions.h
 *
 * \brief variable and function definitions for timings
 *
 * This is the files that contains the variables, macros, pins definitions and functions for timing related routines.
 * 
 * \date 19.08.2019
 */
/// will store the last time the event (in loop) was triggered for sending sensor values to FrostManager
unsigned long previousMillis_sensor = 0; 
/// will store the last time the event (in loop) was triggered sending sensor values to  madavi
unsigned long previousMillis_send2madavi = 0; 
/// will store the last time the event (in loop) was triggered sending sensor values to luftdaten
unsigned long previousMillis_send2luftdaten = 0;
/// time interval to send values to Frostmanager
bool  sensorInterval;
/// time interval to send values to MADAVI
bool  send2madaviInterval;
/// time interval to send values to LUFTDATEN
bool  send2luftdatenInterval;


/**************************************************************************/
/*!
    @brief  ISR functions called for Reset ESP32
    @returns void
*/
/**************************************************************************/
void IRAM_ATTR resetModule() {
  ets_printf("reboot in 5sec...\n");
  delay(5000);
  esp_restart();
}
