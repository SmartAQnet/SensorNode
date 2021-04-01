/**
 * \file esp32-stationary.ino
 *
 * \brief This is the Arduino .ino file.
 *
 * This is the ESP32 implementation file, which implement the overall flow of the project by calling different files/functions.
 * It implements the state-machine like flow as described in the <a href="StatemachineFlow.pdf" target="_blank"><b>flowchart</b></a>.  
 * 
 * \date 19.08.2019
 */


#include "main.h"


/**************************************************************************/
/*!
    @brief  initialization of peripherals attached to ESP32 board
    @returns void
*/
/**************************************************************************/

void setup() 
{
  pinMode(RST, OUTPUT);
  digitalWrite(RST,HIGH);


  // make serial monitor printing available
  Serial.begin(115200);

  digitalWrite(RST,LOW);
  delay(100);
  digitalWrite(RST,HIGH);
  delay(100);
  
  // print FW version and devmode|release setting over serial
  Serial.print("\nVERSION: Crowdsensor FW ");
  Serial.println(FIRMWARE_VERSION);
  
  MY_DBGln("\nWARNING: YOU ARE RUNNING A DEVELOPMENT VERSION OF THIS FIRMWARE!!!\n");

  // setup display
  u8g2.begin();
  u8g2.setFont(u8g2_font_pxplusibmcga_8f);// choose a suitable font
  u8g2.setContrast(255);//Test Contrasts
  
    //Test Monograms of KIT and TECO
  delay(1000);
  u8g2.drawXBMP(0, 0, 128, 64, kit);
  u8g2.sendBuffer();
  delay(1000);
  
  u8g2.clearBuffer();
  u8g2.drawXBMP(0, 0, 128, 64, teco);
  u8g2.sendBuffer();
  delay(1000);
  
  u8g2.clearBuffer();
  u8g2.drawStr(0,7,("FW: "+ String(FIRMWARE_VERSION)).c_str());//2nd Parameter indicates lines i.e. 7=Line1, 15=Line2, 23=Line3, 31=Line4, 39=Line5, 47=Line6, 55=Line7, 63=Line8
  
  u8g2.sendBuffer(); 
 
  //update global variables for location, license and firmware
  abovenn = 0;
  abovennEstimated = abovenn+NAN;
  device_Serial = getChipId();
  latitude = get_value("user_data", "latitude").toFloat();
  longitude = get_value("user_data", "longitude").toFloat();
  SDS011_Serial = String(get_value("user_data", "sds011id"));
  humanReadableLocation = String(get_value("user_data", "address"));
  rhtSensor = String(get_value("user_data", "rht_sensor"));
  license_type = get_value("user_data", "data_license");
  if (!(license_type == "")) 
  {
    license_owner = get_value("user_data", "data_owner");
    license_url = get_value("user_data", "data_url");
    
    MY_DBG("[Setup] Building license object: ");
    MY_DBGln(license_type + " " + license_owner + " " + license_url);
    
  }
  
  //fetch statistics
  oldErrCode = get_integer("user_data", "warning");
  timeOfMeasure = get_value("user_data", "tom");
  tObservations = get_integer("user_data", "tObservation");
  nObservations = get_integer("user_data", "nObservation");
  resetCounter = get_integer("user_data", "rCounter");
  resetCounter++;
  set_integer("user_data", "rCounter",resetCounter);


  delay(1000);
  u8g2.clearBuffer();
  
  //  messages on display
  u8g2.drawStr(0,63, ("ID:"+esp32_chipid).c_str());
  u8g2.sendBuffer();
  
  //sds.begin(&SDS_SERIAL, SDS011_TXD, SDS011_RXD);  // initialize SDS011 sensor
  sds.begin(&SDS_SERIAL,SDS011_TXD,SDS011_RXD);
  
  MY_DBGln("[Setup] Initializing temperature sensor ");
  
  // start bme280 if chosen in webinterface
  if (rhtSensor == "BME280")
  {
    
    MY_DBG("[Setup] BME280... ");
    
    bool wireStatus = Wire1.begin(BME_SDA, BME_SCL);
    if (!wireStatus) Serial.print("Wire1 failed to init");

    bmeAddress = BME_ADDR;
    use_bme280 = bme.begin(bmeAddress, &Wire1);
    
    MY_DBG(" done: ");
    MY_DBGln(use_bme280);
    
  }
  // start dht22 if chosen in webinterface
  else if (rhtSensor == "DHT22")
  {
    
    MY_DBG("[Setup] DHT22... ");
    
    dht.begin(); // initialse DHT22
    
    MY_DBGln(" done. ");
    
  }

  Config.title = "TECO Crowdsensor";
  Config.homeUri = "/_ac";
  Config.bootUri = AC_ONBOOTURI_HOME;//AC_ONBOOTURI_ROOT;//;
  //Config.autoReconnect = true; //connect to the previously reconfigured wifi even after WiFi disconnection whether manually/automatically
  // end sending AP after timeout, go into loop
  Config.portalTimeout = APMODE_REBOOT_TIMEOUT; 
  Config.boundaryOffset = CREDENTIAL_OFFSET; 
  // use last 6 digits of mac address for ssid (or apid)
  Config.apid = esp32_chipid.c_str();
  Config.apip = IPAddress(192,168,244,1);
  Config.gateway = IPAddress(192,168,244,1);
  Config.psk = "12345678";
  Portal.config(Config);

  //add custom pages to autoconnect menu
  cfgSensorPageAux["cfgtitle"].value = "<h1>Crowdsensor Konfiguration Firmware: " + String(FIRMWARE_VERSION) + "</h1>";
  Portal.join({cfgSensorPageAux,cfgUpdatePageAux});

  // Load values from flash for AutoConnect pages /
  Portal.on("/", handleRoot);
  // handle the page /config
  Server.on("/config", handleConfig);
  // handle the page /dummy
  Server.on("/dummy", handleDummy);
  Server.on("/dummyResult", handleDummyResult);

  Server.on("/otaUpdate", HTTP_GET, otaUpdateHandler);
  //handling uploading firmware file 
  Server.on("/update", HTTP_POST, updateHandler , updateHandlerResponse );
  
  state = init_state;
}

/**************************************************************************/
/*!
    @brief  reading the Microphone with timer function
    @returns void
*/
/**************************************************************************/
void loop() 
{
  switch(state)
  {
    case init_state:
      // Start
      if (Portal.begin()) 
      {
        Serial.println("WiFi connected: " + WiFi.localIP().toString());
        // clear display
        // use mdns for host name resolution
        if (!MDNS.begin(esp32_chipid.c_str())) { //http://<chipid>.local
          Serial.println("Error setting up MDNS responder!");
          errorCode |= 1 << 3;
        }
        errorCode &= ~(1 << 3);
        Serial.println("mDNS responder started!");
        u8g2.clearBuffer();
        state = check_first_sync;       
      }  
      break;
    case check_first_sync:
      if (WiFi.status() == WL_CONNECTED) 
      {
        u8g2.drawStr(0, 7, "WARTE AUF ");
        u8g2.drawStr(0, 15, "ZEIT-SYNC");
        u8g2.sendBuffer();

        //added from ezTime library to see whether it affects reboot issue.
        setInterval(21);
  
        Serial.println("wait for network time sync start");
        // update time from network
        // true if update was successful
        time_updated = waitForSync(NTP_TIMEOUT_WAIT);
        if (time_updated)
        {
          errorCode &= ~(1 << 4);
          Serial.println("got network time");

        }
        else
        {
          errorCode |= 1 << 4;
          Serial.println("sync network timeout");
        }

        u8g2.clearBuffer();
        state = idle_state;
      }
      break;
    case idle_state:
      //reboot daily at specific time
      if (time_updated && ENABLE_DAILY_REBOOT)
      {
        if ((getHourMinute() == TIMETOREBOOT) && (millis() > 60000))
        //if ((UTC.dateTime("i").toInt()%30)==0)
          resetModule();
      }
      //check wifi
      if (WiFi.status() == WL_CONNECTED)
      {
        //moved to lowerd state to fix overlap 
        state = wifi_found;
      }
      else if ((WiFi.status() == WL_CONNECT_FAILED) || (WiFi.status() == WL_CONNECTION_LOST) || (WiFi.status() == WL_DISCONNECTED))
        state = wifi_error;
      break;
    case wifi_found:
      state = handle_client;
      // check if time since last measurement is greater than sensor_interval
      if ((unsigned long)(millis() - previousMillis_sensor) >= SENSOR_INTERVAL) 
      {
        previousMillis_sensor = millis();
        
        if (!initialised_http) 
          state = check_server_status;
        else
          state = start_measurement_sds;
          
      }

      // send to madavi api after send2madavi_intervall milliseconds
      if (((unsigned long)(millis() - previousMillis_send2madavi) >= SEND2MADAVI_INTERVAL)  && !status_sds && ENABLE_SEND2MADAVI)
      {
        previousMillis_send2madavi = millis();
        state = send_to_madavi;
      }
      // send single sensor data to luftdaten api after send2luftdaten_intervall milliseconds
      if (((unsigned long)(millis() - previousMillis_send2luftdaten) >= SEND2LUFTDATEN_INTERVAL) && !status_sds && ENABLE_SEND2LUFTDATEN)
      {
        previousMillis_send2luftdaten = millis();
        state = send_to_luftdaten;
      }
      break;
    case check_server_status:
      gHttp.begin(String(BASE_URL)+"/Things");
      gHttpCode = gHttp.GET();
      gHttp.end();
      
      if (gHttpCode < 0) 
        http_error = true;
      else
      { 
        Serial.println("HTTP CODE:"+String(gHttpCode));
        if (gHttpCode != 200) 
          http_error = true;
        else 
          http_error = false;
      }
      if(http_error)
      {
        errorCode |= 1 << 0;
        Serial.println("Error on HTTP Server");
        state = start_measurement_sds;
      }
      else
      {        
        errorCode &= ~(1 << 0);
        Serial.println("Things Server Ready");
        initialised_http = true; 
        state = start_measurement_sds;
      }
      break;
    case start_measurement_sds:

      // read sensor and store results into global variables pm10, pm25
      // is 1 if failed, and 0 if suceeded
      
      //status_sds = sds.read(&pm25, &pm10);
      status_sds = sds.dataQueryCmd(&pm10, &pm25 );
      u8g2.begin();//fixed display problem while using sds.dataQueryCmd()
      
      timeofmeas_sds = getIsoTime();
      state = start_measurement_bme;
      break;
    case start_measurement_bme:
      // if bme280 sensor is connected, read values
      if (rhtSensor == "BME280") 
      {
        temp = bme.readTemperature();
        hum = bme.readHumidity();
        atm = bme.readPressure();
        atm /= 100;
        
        MY_DBG("[sensor] temperature: ");
        MY_DBGln(temp);
        MY_DBG("[sensor] humidity: ");
        MY_DBGln(hum);
        MY_DBG("[sensor] pressure: ");
        MY_DBGln(atm);
        
        timeofmeas_rht = getIsoTime();
        if (isnan(hum))
          use_bme280 = false;
      } 
      else if (rhtSensor == "DHT22") 
      {
        //    if dht22 is connected, read data
        use_dht22 = true;
        temp = dht.readTemperature() ;
        hum = dht.readHumidity();
        timeofmeas_rht = getIsoTime();
        
        MY_DBG("[sensor] temperature: ");
        MY_DBGln(temp);
        MY_DBG("[sensor] humidity: ");
        MY_DBGln(hum);
        
        if (isnan(hum))
          use_dht22 = false;
      }
      //Just to show statistics in case of Development NoN-Release code
      state = start_showing;
      
      if(show_QR && !busy)
      {
        //Generate and test a QR Code at location x,y
        u8g2.clearBuffer();
        generateQRCode(34,2,WiFi.localIP().toString().c_str());
        state = check_location_data;
      }
      show_QR=!show_QR;
      break;
    case start_showing:
      u8g2.clearBuffer();
      u8g2.drawStr(0, 7, "VERBUNDEN MIT:");
      u8g2.drawStr(0, 15, WiFi.SSID().c_str());
      u8g2.drawStr(0, 23, WiFi.localIP().toString().c_str());
      u8g2.sendBuffer();
      state = show_sds_values;
      #ifndef RELEASE
        countSensorInterval++;
        if ((countSensorInterval == STATS_INTERVAL) && !busy) {state = show_2nd_screen; countSensorInterval=0;}
      #endif
      break;
    case show_2nd_screen:
      if (errorCode != oldErrCode) set_integer("user_data", "warning", errorCode);
      u8g2.drawStr(0, 31, ("Reboot# "+String(resetCounter-1)).c_str());
      u8g2.drawStr(0, 39, ("Obsert# "+String(tObservations)).c_str());
      u8g2.drawStr(0, 47, ("Misses# "+String(nObservations)).c_str());
      u8g2.drawStr(0, 55, ("Begins# "+timeOfMeasure.substring(11)).c_str());
      u8g2.drawStr(0, 63, ("      "+timeOfMeasure.substring(0,10)).c_str());
      u8g2.sendBuffer();
      state = check_location_data;
      break;      
    case show_sds_values:
      if (!status_sds) 
      {
        errorCode &= ~(1 << 7);

        u8g2.drawStr(0, 31,  ("PM2.5: "+String(pm25,1)).c_str());
        u8g2.drawStr(0, 39,  ("PM10:  "+String(pm10,1)).c_str());

        MY_DBG("[sensor] PM2.5: ");
        MY_DBGln(String(pm25,1));
        MY_DBG("[sensor] PM10: ");
        MY_DBGln(String(pm10,1));
        
        
      }
      if (status_sds) 
      {
        errorCode |= 1 << 7;
        u8g2.drawStr(0, 31, "FEINSTAUBSENSOR ");
        u8g2.drawStr(0, 39, ":Bitte verbinden");
        
      }
      u8g2.sendBuffer();
      state = show_bme_values;
      break;
    case show_bme_values:
    
      if ((use_bme280 && !isnan(temp)) || (use_dht22 && !isnan(temp))) 
      {
        errorCode &= ~(1 << 6);
        errorCode &= ~(1 << 5);
        u8g2.drawStr(0, 47,  ("Temp:  "+String(temp,1)).c_str());
        u8g2.drawStr(0, 55,  ("Humid: "+String(hum,1)+"%").c_str());
      }

      if (!use_bme280 && !use_dht22) 
      {
        
        Serial.print("[sensor] Temp: Sensoren: ");
        Serial.println(rhtSensor);
        if ((rhtSensor == "") || (rhtSensor == "keiner"))
        {
          errorCode |= 1 << 5;
          u8g2.drawStr(0, 47, "TEMPERATURSENSOR");
          u8g2.drawStr(0, 55, "Bitte auswaehlen");
        }
        else 
        {
          errorCode |= 1 << 6;
          u8g2.drawStr(0, 47, "TEMPERATURSENSOR");
          u8g2.drawStr(0, 55, "Bitte verbinden");
        }
      }
      u8g2.drawStr(0, 63,  ("ID:"+esp32_chipid).c_str()); 
      char er[3];
      sprintf(er, "E%02X", errorCode);//print a error indicating icon
      u8g2.drawStr(103, 63, er); 
      u8g2.sendBuffer();
      state = check_location_data;
      
      break;
    case check_location_data:
      // only send if position data is given
      if (ENABLE_SEND2FROST &&  (get_value("user_data", "latitude") != ""))
      {
        errorCode &= ~(1 << 1);
        Serial.println("Position data is given, start sending");
        state = check_timesync;
      }else
      {
        errorCode |= 1 << 1;
        state = handle_client;//debugging: keep only this line
      }
      break;
    case check_timesync:
      if (!time_updated) // if no time sync, then retry
      {
        time_updated = waitForSync(NTP_TIMEOUT_WAIT);
        if (time_updated)
          Serial.println("got network time");
        else
        {
          Serial.println("sync network timeout");
          resetModule();
        }
      }
      // in case timesync is true, send to frost
      else 
      {
        if (!http_error)
        {
          if (!initialised_frost) 
            state = create_frost_entties;
          else
            state = send_sds_data;
        }
        else
          state = handle_client;
      }
      break;

    case create_frost_entties:
      //send data to FROST
      // initialse frost manager
      Serial.println("Initialze FROST manager (Server)...");      
      //disable IP address on display while FROST initiliasing

      u8g2.drawStr(0,23,"                ");
      u8g2.sendBuffer();
      u8g2.drawStr(0,23,"initialisiere..."); 
      u8g2.sendBuffer();
      
      //check if the creating entities are seccessful?
      busy = true;
      initialised_frost = createEntities();
      
      
      //clear and re-display the IP address again
      u8g2.drawStr(0,23,"                ");
      u8g2.sendBuffer();
      delay(10);
      u8g2.drawStr(0, 23, WiFi.localIP().toString().c_str());
      u8g2.sendBuffer();
      delay(10);
      
      if(initialised_frost)
      {
        errorCode &= ~(1 << 2);
        busy = false;
        Serial.println("Initialze FROST manager done!");
        timeOfMeasure = get_value("user_data", "tom");
        if (timeOfMeasure == "")
        {
          set_value("user_data", "tom",timeofmeas_sds);
          timeOfMeasure = timeofmeas_sds;
        }
        state = send_sds_data;
      }
      else
      {
        errorCode |= 1 << 2;
        Serial.println("Initialze FROST manager failed!\n!!!Check the HTTP Server...!!!");
        state = handle_client;
      }
      break;
    case send_sds_data:
      // send SDS data: send as often as a measurement occurs
      if (!status_sds) 
      {
        Serial.println("[sensor] Posting sds011 (PM10, PM2.5) values: ("+String(pm10)+","+String(pm25)+")");

        tObservations = tObservations+2;
        //post observation for datastream sds011 pm10
        postObservation(dsPM10, timeofmeas_sds, timeofmeas_sds, pm10);
        //post observation for datastream sds011 pm2.5
        postObservation(dsPM25, timeofmeas_sds, timeofmeas_sds, pm25);
      }
      state = send_bme_data;
      break;
    case send_bme_data:
      // send bme280 data: send hum and temp data
      if (use_bme280 && !isnan(temp)) 
      {
        Serial.println("[sensor] Posting bme280 (Temp, Hum, Atm) values: ("+String(temp)+","+String(hum)+","+String(atm)+")");
        tObservations = tObservations+3;
        set_integer("user_data", "tObservation", tObservations);
        set_integer("user_data", "nObservation", nObservations);
        //post observation for datastream bme280 temperature
        postObservation(dsTmBme, timeofmeas_rht, timeofmeas_rht, temp);
        //post observation for datastream bme280 humidity
        postObservation(dsHmBme, timeofmeas_rht, timeofmeas_rht, hum);
        //post observation for datastream bme280 pressure
        postObservation(dsPrBme, timeofmeas_rht, timeofmeas_rht, atm);
      }
      state = send_dht_data;
      break;
    case send_dht_data:
      // send dht22 data
      if (use_dht22 && !isnan(temp)) 
      {
        Serial.println("[sensor] Posting dht22 (Temp, Hum) values: ("+String(temp)+","+String(hum)+")");
        tObservations = tObservations+2;
        //post observation for datastream dht22 temperature
        postObservation(dsTmBme, timeofmeas_rht, timeofmeas_rht, temp);
        //post observation for datastream dht22 humidity
        postObservation(dsHmBme, timeofmeas_rht, timeofmeas_rht, hum);
      }
      state = handle_client;
      break;
    case send_to_madavi:
      // send all data to madavi api
      Serial.println("Sending all data to madavi");
      // send data to madavi
      sendData(json_madavi(status_sds, pm10, pm25, temp, hum, atm).c_str(), destination_madavi, "0");
      state = handle_client;
      break;
    case send_to_luftdaten:
      // send sds data to luftdaten if connected
      if (!status_sds) 
      {
        Serial.println("Sending sds data to luftdaten");
        sendData(json_sds(status_sds, pm10, pm25).c_str(), destination_luftdaten, "1");
      }
      // send bme280 data to luftdaten if connected
      if (use_bme280 && !isnan(temp)) 
      {
        Serial.println("Sending bme data to luftdaten");
        sendData(json_RHT(temp, hum, atm).c_str(), destination_luftdaten, "11");
      }
      // send dht22 data to luftdaten if connected
      else if (use_dht22 && !isnan(temp)) 
      {
        Serial.println("Sending dht data to luftdaten");
        sendData(json_RHT(temp, hum, atm).c_str(), destination_luftdaten, "7");
      }
      state = handle_client;
      break;
    case wifi_error:      
      u8g2.clearBuffer();
      u8g2.drawStr(0, 7,  "VERBINDUNG      ");
      u8g2.drawStr(0, 15, "FEHLGESCHLAGEN  ");
      u8g2.drawStr(0, 23, "STARTE NEU...   ");
      u8g2.drawStr(0, 31, "                ");
      u8g2.drawStr(0, 39, "                ");
      u8g2.drawStr(0, 47, "                ");
      u8g2.drawStr(0, 55, "                ");
      u8g2.drawStr(0, 63, ("ID:"+esp32_chipid).c_str());
      u8g2.sendBuffer();
      
      resetModule();
      break;

    case handle_client:
      state = idle_state;
      break;
  }
  //handle client outside state machine to fast its response
  if (state > idle_state) Portal.handleClient();
}
