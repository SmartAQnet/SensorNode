/**
 * \file main.h
 * \brief This the main page for the SMARTAQNET which is the core of the project. 
 * \mainpage Smart Air Quality Network (SMARTAQNET)
 * 
 * \section intro Introduction
 * SMARTAQNET project is aimed at development of a close-meshed network of local fine-dust data, which can be fed and used well by general public.
 * Further details can be checked at: http://www.smartaq.net.\n
 * This script uses a ESP32 HELTEC Board, a SDS011 particulate matter sensor and a BME280 sensor or a DHT22 sensor.
 * The \ref Fig-1 shows the main board having different sensors connected together. The ESP32 is also refered to 
 * as crowdsensor. After power on, it opens a Wifi access point, default Password is 12345678. The Wifi access point can be searched by a device 
 * (Smartphone, Tablet, Laptop or similar) using the ID shown on the OLED display. A webpage (also called "Captive Portal") opens after connection 
 * to this AP with Smartphone. If no webpage is opening automatically, it can be opend by typing "http://192.168.244.1" into the browser. By using 
 * this webpage, the crowdsensor can be connected to any of the available Wifi networks. The crowdsensors configuration page can then be accessed 
 * using http://[localIP]/ or using http://[SSD-ID].local/ e.g., http://192.168.0.39 or http://181712.local. The local IP and the SSD-ID are both 
 * printed on the screen of the crowd sensor. The location address, the thermal sensor type, the ID of the dust sensor SDS011, the owner of the data 
 * and more can be configure using the configuration page. The firware version of the code is hard-coded by the constant (#FIRMWARE_VERSION).
 * The detailed debugging can be controlled for the firmware using the constant RELEASE. Following is the details for differnt features and steps to start:\n
 * - Install the arduino, board and required libraies as explained in \ref arduinoide, \ref boardlib, \ref lib
 * - Connect the hardware as explained in \ref wiring, \ref hw_sec and \ref peri
 * - Configure the constants in the code as explained in \ref const.
 * - Compile and upload the cdes as explained in \ref upload and \ref dialout
 * - Configure the WiFi and other parameters as explained in \ref wifi and \ref configpage
 * - If everything is installed and configured perfectly, there should be no error i.e., E0 as explained in \ref errorenc
 * - If there is any errors please look into error or known bug section  as explained in \ref errorenc and \ref knownbugs
 * - For detailed debugging uncomment RELEASE constant. Some quick trick will also save the time as shown in \ref deb
 * \anchor Fig-1
 * \image html board.png "Figure 1. Sensor Board" width=500px \n
 * \image latex board.eps "Sensor Board" width=400px \n
 * \subsection oled OLED Display Usage
 * Built-in OLED, a SSD1306 based display, is used to display different type of information, messages, images and QR code. The display is connected using I2C interface using u8g2 library.
 *  - Hardware PINs: #RST (16) is reset, #OLED_SDA (4) is SDA and #OLED_SCL (15) is SCL pin of the display and #OLED_ADDR (0x3c) is the I2C address of the display.
 *  - ICONS and QR Codes: at the startup it displays logos and then a QR code
 *  - Which Screens to display and their meaning: There are two main screens. The \ref Fig-2 shows the main screen while the \ref Fig-3 shows second screen. During the execution, different screens are:
 *   -# When the firmware is started, OLED displays and it tries to search the previous configured WiFi as in \ref Fig-2. The last line shows the hardware ID of the ESP32.
 * \anchor Fig-2
 * \image html searchwifi.png "Figure 2. OLED Display: Searching Wifi" width=300px \n
 * \image latex searchwifi.eps "OLED Display: Searching Wifi" width=300px \n
 *   -# When the firmware could not find previous WiFi, it opens an AP and shows the screen as in \ref Fig-3. It shows the SSID and Password for AP and its IP address.
 * \anchor Fig-3
 * \image html apmode.png "Figure 3. OLED Display: Access Point WiFi" width=300px \n
 * \image latex apmode.eps "OLED Display: Access Point WiFi" width=300px \n
 *   -# When the WiFi is connected, it shows that it is waiting for the time to be synchronized, as in \ref Fig-4.
 * \anchor Fig-4
 * \image html syncwait.png "Figure 4. OLED Display: NTP Time Sync" width=300px \n
 * \image latex syncwait.eps "OLED Display: NTP Time Sync" width=300px \n
 *   -# When the WiFi is connected, temperature and dust sensor are attached and configured, the screen in \ref Fig-5 shows the sensed values, ID and static IP of the ESP32. ER shows the error code
 *   in hexadecimal. This is explained in the section below. ER of 0xC0 means the dust and temperature sensors are not connected.
 * \anchor Fig-5
 * \image html screen1.png "Figure 5. OLED Display Screen-1" width=300px \n
 * \image latex screen1.eps "OLED Display Screen-1" width=300px \n
 *   -# When the WiFi is connected, temperature and dust sensor are not attached, the screen in \ref Fig-6 shows the messages (i.e., FEINSTAUBSENSOR:Bitte verbinden and TEMPERATURSENSOR:Bitte verbinden,
 *   ID and static IP of the ESP32. When temperature and dust sensor are attached but temperature is not configured, screen shows the messages FEINSTAUBSENSOR:Bitte verbinden and TEMPERATURSENSOR:Bitte auswaehlen.
 * \anchor Fig-6
 * \image html screen1error.png "Figure 6. OLED Display: Screen-1 Error" width=300px \n
 * \image latex screen1error.eps "OLED Display: Screen-1 Error" width=300px \n
 *   -# When the WiFi is not connected or it is disconnected or time is not synchronized during the second trial, \ref Fig-7, shows the messages and ID of the ESP32. And it restarts.
 * \anchor Fig-7
 * \image html reboot.png "Figure 7. OLED Display: WiFi or NTP Error" width=300px \n
 * \image latex reboot.eps "OLED Display: WiFi or NTP Error" width=300px \n
 *   -# The \ref Fig-8 screen shows some statistical and informative messages, with the intervals of (#SENSOR_INTERVAL) in non-release mode.
 * \anchor Fig-8
 * \image html screen2.png "Figure 8. OLED Display: Screen-2" width=300px \n
 * \image latex screen2.eps "OLED Display: Screen-2" width=300px \n
 * The statistical screen contains the following. These are tentative, we can change the words and information.
 *    -# <b>Reboot</b> is indicating the total number of reboots. This will help us how many normal boots (@4AM) and how many accidental boots happened? Saved in the eeprom.
 *    -# <b>Obsert</b> is the total number of the observations since the beginning. Saved in the eeprom.
 *    -# <b>Misses</b> is the total number of the observations that are not posted correctly i.e., negative HTTP status codes. Saved in the eeprom.
 *    -# <b>E</b> (i.e. Errors) is the encoding of the errors in hexadecimal. (see below). Not saved in the eeprom.
 *    -# <b>Begins</b> is the timestamp of the first observation at the time entities were created for the first time. Saved in the eeprom.\n
 *   -# The QR screen is show at two occasions. First when there is no wifi configured and ESP32 opens an AP, it shows a screen (\ref Fig-3) and a QR code alternatively with an interval of #SHOW_QR_INTERVAL.
 *   Other screen after the WiFi is configured, is continuously with the text (\ref Fig-5) of and QR code with an interval of #SENSOR_INTERVAL.
 * 
 * \subsection errorenc Error Decoding
 * On the last line of the display, at the right most column indicated the different errors which are encoded in hexadecimal. If there is any error, corresponding bit is set to high.
 * The error encoding is explained below in the Table \ref Tab-1:
 * <center>
 * <table>
 * <caption id="Tab-1"> Error Enconding</caption>
 * <tr><th>Bit \n Location         <th>7  <th>6  <th>5  <th>4  <th>3  <th>2  <th>1  <th>0
 * <tr><td>Error \n Type         <td>SDS \n HW \n Error  <td>BME \n HW \n Error  <td>BME \n Select \n Error  <td>NTP \n First \n Error  <td>mDNS \n Error  <td>Create \n Entities \n Error  <td>Location \n Missing \n Error  <td>HTTP \n Server \n Error
 * </table>
 * </center>
 * -# <b>E00</b> indicates "NO ERROR".
 * -# <b>E01</b> indicates "HTTP Server Error", this happens when the HTTP Server is not responding.
 * -# <b>E02</b> indicates "Location Missing Error", this happens when the location is not configured on the configuration page.
 * -# <b>E04</b> indicates "Create Entities Error", this happens when the entities are not created properly.
 * -# <b>E08</b> indicates "mDNS Error", this happens when the mDNS has not started properly.
 * -# <b>E10</b> indicates "NTP First Error", this happens when the time is not sync for the first time.
 * -# <b>E20</b> indicates "BME Select ERROR", this happens  when the Temperature sensor is not configured from the configuration page.
 * -# <b>E40</b> indicates "BME HW ERROR", this happens when the Temperature sensor is not connected.
 * -# <b>E80</b> indicates "SDS HW ERROR", this happens when the dust sensor is not given.\n
 * There might b combination of above errors, for Example:
 *    -# Error E03 means: The comobination of E01 and E02.
 *    -# Error E21 means: The temperature sensor is not selected and the location is not configured.
 *    -# Error E01 means: There is a HTTP Server error.
 *   
 * \subsection nvs Non-Volatile Storage (NVS) Usage
 * NVS flash storage is used to save different information permanently. It saves latitude as "latitude", longitude as "longitude", SDS011 ID as "sds011id", human readable location as "address"
 * license name as "data_license, license legal notice as "data_owner", license url as "data_url", error codes as "warning",  starting timestamp as "tom", total number of observations posted as "tObservation",
 * total number of errorneous observations as "nObservation", total number of reset counter as "rCounter". The configurational parameteres are updated in the nvs when there is change from HTML configuration
 * page, while the statistical parameters are updated during different execution scenarios.
 * \subsection sdso11 SDS011 Dust Sensor Usage
 * - Hardware Pins: it uses transmit #SDS011_TXD (13) and recieve #SDS011_RXD  (17) pins
 * - SERIAL Port: it uses the HardwareSerial port 2 for #SDS_SERIAL
 * \subsection bme280 BME280 Sensor Usage
 * - Hardware Pins: it uses SDA #BME_SDA (21) and SCL #BME_SCL (22) pins
 * - I2C Port: to avoid any mismatch it uses Wire1 (default is Wire0) in bme.begin(BME_ADDR, &Wire1)
 * - I2C address: #BME_ADDR as 0x77 for Red, 0x76 for Purple (chinese)
 * 
 * \subsection wifi WiFi USage
 * Firstly, on power up ESP32 tries to connect to earlier configured WiFi untill (#WIFI_REBOOT_TIMEOUT), if not found it opens up a new access point. Any client can be connected using SSID and Passcode shown on the screen. 
 * If some connects to access point a configuration pages is automatically opened, if not, one can open it using http://[localIP]/ or using http://[SSD-ID].local/. Using this configuration page, any avaialable
 * WiFi arround you can be connected with using respective credentials. The ESP32 waits for some client to be connected untill (#APMODE_REBOOT_TIMEOUT), otherwise
 * it will restart and repeate the process. If some client is connected within timeout, ESP32 tries to synchronised with the network time untill (#NTP_TIMEOUT_WAIT), otherwise it skips this synchronization for the second
 * trial later. Later, before creating server entities it tries to synchronised with the network time untill (#NTP_TIMEOUT_WAIT) and restart if not seccessful.
 * 
 * \subsection configpage Configuration Page
 * A HTML configuration page is prepared using AutoConnect library without using Pagebuilder library. Different elements are defined like:   cfgtitle (ACText), cfgScript (ACElement), cfgBody (ACElement), cfgData (ACText), 
 * cfgBody2 (ACElement) which are combined using AutoConnectAux.  The function handleConfig() is used to handle the "/config" response of Webserver::on(...) after button submission, the function 
 * handleRoot() is used to handle the / response of AutoConnect::on(...), the function otaUpdateHandler() is used to handle the /otaUpdate HTTP_POST response of Webserver::on(...), the function 
 * updateHandler() is used to handle the /update HTTP_POST response of Webserver::on(...), the function updateHandlerResponse() is used to handle the /update HTTP_GET response of Webserver::on(...).
 * HTML script otaUpdate defines OTA uploading webpage using AutoConnectAux.
 * The HTML configuration element "inputAddress" defines to input the human readable address, "inputLon" defines to input location's longitude, "inputLat" defines to input location's latitude, 
 * "inputRhtsensor" defines to select  the temperature sensor out of "default", "BME280" and "DHT22"; "inputSds011id" defines to input the printed SDS011 serial number, "inputLicense" defines to 
 * select license type between "CC0 1.0" or "CC BY 4.0" and "inputOwner" defines to input the legal_notice of the license owner. The license "CC0 1.0" and "CC BY 4.0" points to " #data1_url_cc0 " 
 * and " #data1_url_ccby " respectively.\n
 * The \ref Fig-9 shows an example fo a configuration page. The configuration page has following information from top-bottom:
 * -# <b>Menus</b>: There are different menues which performs different actions
 * -# <b>Title</b>: It indicates the title of page and the firmware version.
 * -# <b>Map</b>: It indicates the google map of the location selected. Or you can choose any location from this map using the the cursor.
 * -# <b>Address</b>: One can input the human readable address for the location.
 * -# <b>Longitude</b>: One can input the longitude of the location.
 * -# <b>Latitude</b>: One can input the latitude of the location.
 * -# <b>Sensors</b>: One can chose the temperature sensor i.e., none, BME22, or DHT22.
 * -# <b>SDS011 ID</b>: One can input the hard-coded serial number from the sds011 dust sensor.
 * -# <b>Licese Name</b>: One can chose the license type here.
 * -# <b>Legal Notice</b>: One can chose the license owner of the data.
 * -# <b>Submit (Speichern)</b>: Button to submit the new configuration to ESP32, which will reset it and new entities will be created again.
 * 
 * The menu has the following items:
 * -# <b>HOME</b>: is the home of the configuration page. It shows the different parameters of the current connection.
 * -# <b>Update</b>: is used to update the firmware over OTA.
 * -# <b>Configure</b>: is the configuration page as shown in \ref Fig-9.
 * -# <b>Reset...</b>: this will reset the board.
 * -# <b>Disconnect</b>: will disconnect the system from the current WiFi.
 * -# <b>Open SSIDs</b>: shows all the used/configured WiFi SSIDs.
 * -# <b>Configure new AP</b>: used to connect to a new WiFi. There it show all the available WiFi routers and one can connect to one having SSID and Password.\n
 * <b>Note: The config page sometimes takes quite long to load (esp. on a mobile phone). Patience needed!!!</b>
 * <div style="page-break-after: always"></div>
 * \anchor Fig-9
 * \image html configr.png "Figure 9. Configuration Page" width=700px \n
 * \image latex configr.eps "Configuration Page" width=400px \n
 * 
 * \subsection mdns MDNS
 * A domain name service is used instead of typing IP. It is configure according to hardware ID of ESP32. One can open configuration page using http://[ID].local/.
 * 
 * \subsection dailyreboot Daily Reboot
 * There is a daily reboot at time defined by (#TIMETOREBOOT) if it is enabled by (#ENABLE_DAILY_REBOOT). This is incoporated as a precaution.
 * 
 * \subsection intervals Sensing and Sending Intervals
 * The temperature and dust values are sensed at an interval of (#STATS_INTERVAL), and posted at the interval of (#STATS_INTERVAL) if HTTP server is available, location data is given and if the server entities are created seccessfully.
 * The sensed values are also sent to madavi and send2luftdaten servers at the interval of (SEND2MADAVI_INTERVAL) and the interval of (#SEND2LUFTDATEN_INTERVAL), if (#ENABLE_SEND2MADAVI) and (#ENABLE_SEND2LUFTDATEN) is enabled respectively.
 * \subsection create Modelling and Creating Entities
 * Before posting the observations of dust and temperature sensors, different type of entities need to be created on the server using HTTP methods. Usually, the the entities which are created consists of:
 * Thing, Sensor, Location, Obervedproperty, Datastream and Observation. The nomenclature used for the creation can be seen in the presentation <a href="modelling.pdf" target="_blank"><b>Modelling the SmartAQnet Database</b></a>.
 * Some of the entities are created from scratch, while some are only patched if already exist. For example, the "Thing" is patched with location and properties. The "Datastream" is alse needs to be patched with the properties.
 * The sequence of the creation of different entities and how they are patched with required location and/or properties can be seen in the flowchart <a href="CreateEntitiesFlow.pdf" target="_blank"><b>Create Server Entities Flow</b></a>.
 * Entities are created using particular JSON messages which are transmitted using HTTP methods. The seccessfull creation indicates only the status code of 200 and 201, and based on this a boolean response is returned.
 * The observations are then posted at the interval of (#STATS_INTERVAL) and if (#ENABLE_SEND2FROST) is enabled.
 * The Software property version can be controlled by the constant (#sw_version) and timestamp is assign on the fly, while the harware revision date is controlled by using the constant (#hw_date).\n
 * The Table \ref Tab-2 shows which entities are to be created and which entities are patched? The entities are created if they does not exist and patched with different properties if they already exist.
 * 
 * <div style="page-break-after: always"></div>
 * <center>
 * <table>
 * <caption id="Tab-2"> Entities Creation & Patching</caption>
 * <tr><th>Entity                             <th>Type        <th>Patched?  <th>Patched With?
 * <tr><td>Thing                              <td>None        <td>Yes       <td>Location, Software Version
 * <tr><td>Location                           <td>None        <td>No        <td> None
 * <tr><td rowspan="3">Sensor                 <td>SDS011      <td>No        <td> None
 * <tr>                                       <td>BME280      <td>No        <td> None
 * <tr>                                       <td>DHT22       <td>No        <td> None
 * <tr><td rowspan="5">Observed Properties    <td>PM2.5       <td>No        <td> None
 * <tr>                                       <td>PM10        <td>No        <td> None
 * <tr>                                       <td>Temperature <td>No        <td> None
 * <tr>                                       <td>Pressure    <td>No        <td> None
 * <tr>                                       <td>Humidity    <td>No        <td> None
 * <tr><td rowspan="5">Observations           <td>PM2.5       <td>No        <td> None
 * <tr>                                       <td>PM10        <td>No        <td> None
 * <tr>                                       <td>Temperature <td>No        <td> None
 * <tr>                                       <td>Pressure    <td>No        <td> None
 * <tr>                                       <td>Humidity    <td>No        <td> None
 * <tr><td rowspan="5">Datastreams            <td>PM2.5       <td>Yes       <td> Software Version, License
 * <tr>                                       <td>PM10        <td>Yes       <td> Software Version, License
 * <tr>                                       <td>Temperature <td>Yes       <td> Software Version, License
 * <tr>                                       <td>Pressure    <td>Yes       <td> Software Version, License
 * <tr>                                       <td>Humidity    <td>Yes       <td> Software Version, License
 * </table>
 * </center>
 * 
 * \subsection flowchart FlowCharts
 * The project work-flow can be see in the <a href="StatemachineFlow.pdf" target="_blank"><b>flowchart</b></a>. 
 * Also there is a another flowchart and explanation regarding "Inside AutoConnect::begin" function at https://hieromon.github.io/AutoConnect/lsbegin.html .\n
 * There is a another flowchart and explanation regarding creating & patching different SAQN Database Entities on the server using JSON messages, as shown in <a href="CreateEntitiesFlow.pdf" target="_blank"><b>Create Server Entities Flow</b></a>.\n
 * There creation of entities implements OGC like API i.e. https://developers.sensorup.com/docs/#sensorthingsAPISensing).\n 
 * 
 * \subsection const Important Constants
 * Have a look at the following constants to configure them accordingly. These constants needs to be configured before the firmware is uploaded.
 * - in the file myAutoConnect.h 
 *  - #WIFI_REBOOT_TIMEOUT set Timeout to search WiFi already configured
 * - in the file create_json.h 
 *  - #SOFTWARE_VERSION Software version of the crowdsensor
 * - in the file sensor.h
 *  - #DHT_PIN 21, set DHT22 sensor communication pin
 *  - #BME_ADDR  0x77, set BME280 I2C Address 0x77 for Red, 0x76 for Purple 
 *  - #BME_SDA, set BME280 I2C SDA pin
 *  - #BME_SCL, set BME280 I2C SCL pin
 *  - #SDS011_TXD, set SDS011 TXD pin is connected at RXD of Serial2 Object
 *  - #SDS011_RXD, set SDS011 RXD pin is connected at TXD of Serial2 Object (no need to physically connect)
 * - in the file main.h
 *  - #FIRMWARE_VERSION controls the hard-coded firmware version of the code
 *  - #sw_version describes the software version of the Thing's software properties
 *  - #hw_date describes the hardware revision timestamp of the Thing's hardware properties
 *  - #BASE_URL HTTP Server Address
 *  - RELEASE controls the debugging prints and #MY_DEBUG
 *  - #ENABLE_SEND2FROST, set switch for sending to FROST teco server
 *  - #SENSOR_INTERVAL, set intervall of reading sensor data in ms
 *  - #STATS_INTERVAL, set intervall of showing statistical/informative screen multiple of (#SENSOR_INTERVAL)
 *  - #ENABLE_SEND2MADAVI, set switch for sending to madavi api
 *  - #ENABLE_SEND2LUFTDATEN, set switch for sending to luftdaten api
 *  - #SEND2MADAVI_INTERVAL, set intervall of sending to madavi api in ms 
 *  - #SEND2LUFTDATEN_INTERVAL, set intervall of sending to madavi api in ms 
 *  - #ENABLE_DAILY_REBOOT, set enable daily reboot at specific time
 *  - #TIMETOREBOOT, set time when to do daily reboot (hour of time of day for reboot goes here) //TODO set proper time
 *  - #APMODE_REBOOT_TIMEOUT, set intervall of auto reboot while in access point mode (config mode) in ms (6000000ms is 10min). autoreboot while in ap mode only occurs if no device is connected to the esp32
 *  - #NTP_TIMEOUT_WAIT, set Timeout for NTP timeout (s) in waitForSync()
 *  - #CREDENTIAL_OFFSET, Specified the NVS offset if the user data exists.
 *  - #data1_url_cc0 defines the Smart AQnet Licese for public domain
 *  - #data1_url_ccby defines the Smart AQnet Licese property
 * 
 * \section  prereq Pre-requisites
 * -# Arrange the required \ref hw_sec and carry out the \ref wiring.
 * -# Install the the \ref arduinoide
 * -# Install the \ref boardlib
 * -# Install the recommended \ref lib
 * 
 * \section hw_sec Hardware
 * It consists of ESP32 microcontrollers, which is "HelTec WiFi Kit 32" ----> https://heltec.org/project/wifi-kit-32/
 * Following board, hardware and libraries are used to develop this project.
 * 
 * \subsection peri Peripherals
 * -# OLED Built-in OLED Display ----> https://heltec.org/project/wifi-kit-32/
 * -# SDS011 Sensor ----> https://learn.watterott.com/sensors/sds011/ 
 * -# BME280 Sensor for Humidity and Temperature ----> https://www.ebay.de/itm/3-3V-Bosch-BME280-Barometer-Luftdruck-Luftfeuchte-Sensor-Modul-BMP280-SPI-I2C-/263548911232 or https://eckstein-shop.de/SparkFun-Barometric-Pressure-Sensor-Breakout-MPL115A1?curr=EUR&gclid=Cj0KCQjww7HsBRDkARIsAARsIT6QsuuszbyaQ4r1DLuYTOT5pgwMfpIhRTtwQKAA2vtHkuHGrBZa9JQaAo-gEALw_wcB
 * -# DHT22 Sensor for Temperature and Humidity ----> https://www.sparkfun.com/products/10167 
 * 
 * \subsection arduinoide Arduino IDE
 * The Arduino IDE Version 1.8 or later is needed. Please install from the official Arduino IDE download page. This step is not required if you already have the most recent version.
 * 
 * \subsubsection dialout Pyserial
 * If you use linux, you have to install the python pyserial package and add the current user to the dialout group:
 * <pre><code class="text">
 * sudo apt install python-pip
 * pip install pyserial
 * sudo /usr/sbin/usermod -a -G dialout \<MY_USER_NAME\>
 * </code></pre>
 * 
 * \subsubsection boardlib ESP32 Arduino core
 * Also, to apply AutoConnect to ESP32, the arduino-esp32 core provided by Espressif is needed. Stable 1.0.3 or required and the latest release is recommended.
 * Install third-party platform using the Boards Manager of Arduino IDE (for instructions see here: https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md). 
 * You can add multiple URLs into Additional Board Manager URLs field, separating them with commas. Package URL is https://dl.espressif.com/dl/package_esp32_index.json for ESP32.
 * 
 * \subsubsection lib Required Libraries
 * Following are the commonly used libraries, and should be added in the Arduino IDE, before the compiling.
 * -# FIRMWARE_VERSION 0.8.6.1 onwards
 *  - ESP32 Board by Espressif Systems v1.0.3 (https://github.com/espressif/arduino-esp32)
 *  - Adafruit Unified Sensor by Adafruit v1.0.3 (https://github.com/adafruit/Adafruit_Sensor)    
 *  - Adafruit BME280 Library by Adafruit v1.0.8 (https://github.com/adafruit/Adafruit_BME280_Library)    
 *  - ArduinoJson by Benuit Blanchon v 6.12.0 (https://arduinojson.org/?utm_source=meta&utm_medium=library.properties) (IMPORTANT: ArduinoIDE installs the version (v6), but it doesnt work with this one.)    
 *  - DHT sensor library by Adafruit v1.3.4 (https://github.com/adafruit/DHT-sensor-library)    
 *  - PageBuilder by Hieromon Ikasamo v1.3.4 (https://github.com/Hieromon/PageBuilder)    
 *  - eztime by Rop Gonggrijp v0.7.10 (https://github.com/ropg/ezTime)    
 *  - u8g2 by oliver v2.26.14 (https://github.com/olikraus/u8g2)  
 *  - QRCode by Richard Moore v0.0.1 (https://github.com/ricmoo/qrcode/)
 *  - AutoConnect by Hieromon Ikasamo v1.1.3 (https://github.com/Hieromon/AutoConnect.git)
 *  - name=SDS011 sensor Library by R. Zschiegner v0.0.6 (https://github.com/ricki-z/SDS011). This library is kept in src/lib/SDS011 folder. Even the library is updated on the github. But when tried to installed from library manager it is not installed properly for esp32.
 * -# FIRMWARE_VERSION 0.8.6 and older
 *  - Adafruit Unified Sensor by Adafruit v1.0.3 (https://github.com/adafruit/Adafruit_Sensor)    
 *  - Adafruit BME280 Library by Adafruit v1.0.8 (https://github.com/adafruit/Adafruit_BME280_Library)    
 *  - ArduinoJson by Benuit Blanchon v 5.13.5 (https://arduinojson.org/?utm_source=meta&utm_medium=library.properties) (IMPORTANT: ArduinoIDE installs the version (v6), but it doesnt work with this one.)    
 *  - DHT sensor library by Adafruit v1.3.4 (https://github.com/adafruit/DHT-sensor-library)    
 *  - PageBuilder by Hieromon Ikasamo v1.3.4 (https://github.com/Hieromon/PageBuilder)    
 *  - eztime by Rop Gonggrijp v0.7.10 (https://github.com/ropg/ezTime)    
 *  - u8g2 by oliver v2.25.10 (https://github.com/olikraus/u8g2)  
 *  - Autoconnect lib (is already in src folder, no need to install) [MODIFIED] [till FIRMWARE_VERSION   "0.8.6 pre"]
 *  - SDS011 sensor Library by R. Zschiegner (already in src folder, no need to install) [MODIFIED] [till FIRMWARE_VERSION   "0.8.6 pre"]  
 *  
 * \subsection upload Firmware Uploading
 * Following steps are recommended to upload the firmware before handing over the new crowd-sensor node.
 * -# To erase flash completely before delivering the product: "C:\Users\Sajjad\Documents\ArduinoData\packages\esp32\tools\esptool_py>esptool.py --chip esp32 erase_flash" from the command terminal.
 * -# Upload the firmware using the Ardiuno IDE
 * 
 * \section wiring  Wirings
 * -# OLED Display (built-in)\n
 *  The display is built-in already with the ESP32 board through I2C. The I2C address is 0x3c, SDA pin is 4, SCL pin is 15, and RST pin is 16. They are already connected. The custom fonts are generated using
 *  online Font Generatore from ----> http://oleddisplay.squix.ch/#/home. Don't need hardware wiring, just require pin definitions, as:
 *  - #RST, set Reset PIN of OLED Module
 *  - #OLED_ADDR, The I2C address of the SSD1306 OLDE 
 *  - #OLED_SDA, The SDA pin of the SSD1306 OLDE 
 *  - #OLED_SCL, The SCL pin of the SSD1306 OLDE 
 * -# SDS011 Dust Sensor\n
 * One has to:
 *  - connect TXD of Dust Sensor SDS011 to Pin 13 of ESP32 (ESP-RX)
 *  - connect 5V of Dust Sensor SDS011 to Pin 5V of ESP32
 *  - connect GND of Dust Sensor SDS011 to Pin GND of ESP32
 * -# BME280 Sensor for Humidity and Temperature\n
 *  - connect SCK of BME280 sensor to Pin 22 of ESP32 (ESP-SCL)
 *  - connect SDA of BME280 sensor to Pin 21 of ESP32 (ESP-SDA) 
 *  - connect VCC of BME280 sensor to 3V3 of ESP32 
 *  - connect GND of BME280 sensor to GND of ESP32
 *  - the I2C address for different BME280 (purple/red) can be configure in sensor.h using #BME_ADDR constant
 * -# DHT22 Sensor for Temperature and Humidity (if BME280 is not used)\n
 * Look at the sensor from the front (grid shows in your direction), put the pins facing down, start counting the pins from left (pin1) to right (pin4). pin 3 is not connected.\n One has to:
 *  - Connect pin 1 of DHT22 sensor (on the left) to 3V3 of ESP32
 *  - Connect pin 2 of DHT22 sensor to Pin 21 of ESP32 (data)
 *  - Connect pin 4 of DHT22 sensor (on the right) to GND of ESP32
 *  - Connect a resistor in range 4.7k to 10k from pin 2 (data) to pin 1 (power) of the sensor
 *  
 * \section knownbugs  Known Bugs
 * There are following bugs and discrepensies that needs to be take care off.
 * -# The config page sometimes takes quite long to load (esp. on a mobile phone). Patience needed!!!
 * -# Sometimes, the AutoConnect does not recover the earlier configured credentials after the reset. This is happend in rare situations with some internet routers. Similar issure reported at: https://github.com/espressif/arduino-esp32/issues/1464
 * -# Sometimes, the EZTime Library can not synchronize with the internet time. This is happend rarely. Sometime, increasing #NTP_TIMEOUT_WAIT constant's time, may solve the issue. Similar issue is reported at: https://github.com/ropg/ezTime/issues/40
 * -# Sometimes, while posting different observations to a server, there are some postings shows negative http status, like: HTTPC_ERROR_CONNECTION_REFUSED (-1) and HTTPC_ERROR_READ_TIMEOUT (-11) as traced in the HTTPClient library.
 * This means, when there is -1 HTTP Status, then the observation is not being posted. Following are some tests that shows the percentage of these errors.
 *  -# <b>Duration</b>:\n From 2020-01-19T03:02:38.000Z to 2020-01-20T04:14:02.000Z\n Result: 1 day, 1 hour, 11 minutes and 24 seconds= 90,684 seconds\n <b>Analysis</b>:\n Reset occurrences: 3 i.e. routine resets at 4:00\n [API] Post Observation... counts 43429 i.e. 8685.8 observation set of five\n [API] HTTP Code: -1x counts 339\n i.e. 339/43429x100=0.78%\n 
 *  -# <b>Duration</b>:\n From 2020-01-22T09:34:19.000Z to 2020-01-25T00:03:57.000Z\n Result: 2 days, 14 hours, 29 minutes and 38 seconds = 224,978 seconds\n <b>Analysis</b>:\n Reset occurrences: 2 i.e. routine resets at 4:00\n [API] Post Observation... counts 111655\n [API] HTTP Code: -1x counts 198\n i.e. 198/111655x100 = 0.17%\n 
 *  -# <b>Duration</b>:\n From 2020-01-26T22:17:08.000Z to 2020-01-27T20:01:32.000Z\n Result: 21 hours, 44 minutes and 24 seconds=78,264 seconds\n <b>Analysis</b>:\n Reset occurrences: 1 i.e. routine resets at 4:00\n [API] Post Observation... counts 38645\n [API] HTTP Code: -1x counts 149\n i.e. 149/38645x100 = 0.38%\n 
 *  -# <b>Duration</b>:\n From 2020-01-28T21:01:21.000Z to 2020-01-30T11:58:11.000Z\n Result: 1 day, 14 hours, 56 minutes and 50 seconds=140,210 seconds\n <b>Analysis</b>:\n Reset occurrences: 2 i.e. routine resets at 4:00\n [API] Post Observation... counts 69315\n [API] HTTP Code: -1x counts 225\n i.e. 225 / 69315 x 100 = 0.32%\n 
 *  -# <b>Duration</b>:\n From 2020-02-13T18:45:43.000Z to 2020-02-15T16:52:03.000Z\n Result: 1 day, 22 hours, 6 minutes and 20 seconds = 165,980 seconds\n <b>Analysis</b>:\n Reset occurrences: 2 i.e. routine resets at 4:00\n [API] Post Observation... counts 80725\n [API] HTTP Code: -1x counts 191\n i.e. 191 / 80725 x 100 = 0.22%\n
 * \subsection testing Some Observations
 * -# I tested it with long testing. And had the following observations (Ticket#2157).
 *  - From "phenomenonTime":"2019-12-31T01:29:44.000Z" to "phenomenonTime":"2019-12-31T17:37:14.000Z". There has been only 27 counts for "[API] HTTP Code: -"
 *  - Then, there is an interruption on the internet. I noticed that my browser was not responding and there continuous -1 or -11 error. 
 *  - There has been 64 counts for "[API] HTTP Code: -" i.e. another 37 counts just between "phenomenonTime":"2019-12-31T17:37:24.000Z" and "phenomenonTime":"2019-12-31T17:43:13.000Z".
 *  - The queries [#BASE_URL]/Datastreams('saqn%3Ads%3Ace4fe57')/Observations?$count=True&$filter=phenomenonTime%20ge%202019-12-31T01:29:44.000Z%20and%20phenomenonTime%20le%202019-12-31T02:03:53.000Z&$orderby=phenomenonTime%20desc gives 205 count.
 *  - The queries [#BASE_URL]/Datastreams('saqn%3Ads%3A6d3c4e7')/Observations?$count=True&$filter=phenomenonTime%20ge%202019-12-31T01:29:44.000Z%20and%20phenomenonTime%20le%202019-12-31T02:03:53.000Z&$orderby=phenomenonTime%20desc gives 204 count, there was [API] HTTP Code: -1.
 *  - This means, when there is -1 HTTP Status, then the observation is not being posted. When traced the HTTPClient library for -1 or -11, it shows:
 *   -# HTTPC_ERROR_CONNECTION_REFUSED  (-1)
 *   -# HTTPC_ERROR_READ_TIMEOUT        (-11)
 * -# Just carried the test and judged how many times there is HTTP CODE: -1 (Ticket#2185).
 *  - Duration:
 *   - Start: 2020-01-19T03:02:38 and End: 2020-01-20T04:14:02
 *   - Alternative time units: 1 day, 1 hour, 11 minutes and 24 seconds can be converted to one of these units: 90,684 seconds
 *  - Analysis:
 *   - Reset occurrences: 3 i.e. routine resets at 4:00
 *   - ideally it should be around 9068 observation set of five, searchig the following in an Editor shows:
 *    -# "[API] Post Observation..." counts 43429 i.e. 8685.8 observation set of five
 *    -# "[API] HTTP Code: -1" counts 339 i.e. 339/43429x100=0.0078%  
 *  
 * \section changelog  Change Log
 * \subsection changelog2 FIRMWARE_VERSION 0.8.6.1 onwards
 * - [1.1]  Mar. 27, 2021
 *  - There is a bug in display while using our its own sds011lib. Tried different variables to rename, but didnot work. Finally, found the trick to
 *  call begin() just after calling sds.dataQueryCmd() function. Also changed the version to 1.1
 * - [1.0]  Jan. 28, 2021
 *  - add its own sds011 library code
 * - [0.8.7.0]  Mar. 28, 2020
 *  - add an extra html page that is indirectly accessed using http://[HOME-IP]/dummy. This is not visible in the menues.
 * - [0.8.7.0]  Mar. 11, 2020
 *  - Showing error as E00. Also separate Error Decoding section is added explaining different errors.
 *  - software version is not now saved in eeprom.
 *  - license patching bug fix. now it is implemented as: if any of "name" and "legal_notice" is changed, the license is patched with new "name", "legal_notice" and "url".
 *  - get_value("user_data", "rht_sensor") is used to read rht_sensor values frequently, just dit this at the begining and save it to "rhtSensor"
 *  - more introduction text, added logo to Latex title page by adding custom headers, removal of "Generated by Doxygen" from refman.tex manually
 * - [0.8.7.0]  Mar. 07, 2020
 *  - #SOFTWARE_VERSION in create_json.h is updated and sync with #FIRMWARE_VERSION
 *  - updated the doxygen with Known bugs
 *  - license patching is implemented now. If the License is not there or if it is in the old form, it is patched.
 *  - use the current timestamp (=date of installation or date of first booting) for software.version
 *  - adjust error code  and ID on the same line
 *  - bug fix to show QR while creating entities and whether it is successful or not
 * - [0.8.7.0]  Mar. 01, 2020
 *  - handle client is bring out of the statemachine, Alternative QR code screens for ID (in AP mode) and for IP (in Wifi connected), interval can be controlled using different constants. Contrast is controlled.
 * - [0.8.7.0]  Feb. 24, 2020
 *  - changing bit position of errors according to harwdare/software types, use multiple screens for release version only, 
 *  - cleaning code and removing extra variables and expressions etc. update the flowchart and documentation
 * - [0.8.7.0]  Feb. 20, 2020
 *  - adding the recent features into SM Flowchart and into create entities flowchart.
 *  - avoid using long paths, and setting it to use relative path in doxygen html/latex (STRIP_FROM_PATH).
 *  - License type=>name, owner=>legal_notice, metadata=>url in the json messages, and Lizenz=> Lizenz Name, Name=>Legal Notice in the html configuration page
 *  - fixing "Lizenz Name", "Legal Notice" split on different lines. bold, italic, strong
 * - [0.8.7.0]  Feb. 18, 2020
 *  - cleaned extra constant and variables from timer_definition.h, made interrupt based inetervals to balance screen timings
 *  - added images into doxygen html, generated doxygen latex and then pdf
 * - [0.8.7.0]  Feb. 14, 2020
 *  - ESP32 board version 1.0.4 downgraded to 1.0.3, as the earlier was having problem with mDNS.
 *  - added integer save and read function from eeprom
 *  - There are two screens that are toggled with the interval of 5sec: 1.Normal Screen 2. Statistical Screen
 * - [0.8.7.0]  Jan. 19, 2020
 *  - modeling new nomenclature, blinking HTTP server error but only at the begining, patching things properties, posting observations with /Datastreams('iotId')/Observations
 * - [0.8.7.0]  Jan. 12, 2020
 *  - adding 3rd parameter to createOrUpdateEntities(), if true will patch the created entities otherwise just create it. only datastreams are patched.
 *  - instead of using repeated checks for 200 & 201 code, just copy it inside individiual funstions.
 *  - Changes the QRCode=ID, Changes Frost Manager Initialising, Updating AutoConnect 1.1.3
 * - [0.8.7.0]  Jan. 3, 2020
 *  - integrating u8g2 and u8x8 unseccessful, using u8g2 for suitable text FONT and QR code generation, including logos KIT,TECO, showing QR code.
 *  - using alternative oled and QR code libraries, u8g2: (https://github.com/olikraus/u8g2), QRcode: (https://github.com/ricmoo/qrcode/).
 * - [0.8.7.0]  Dec. 29, 2019
 *  - introducing the common DEBUG controll using MY_DBG()
 *  - for HTTP Error Code 404, making consistent check for different "Nothing found." response from different servers
 * - [0.8.7.0]  Dec. 24, 2019
 *  - first version of deFrosting the esp32-thingsapi, looking into API code and catching up all JSON message and converting into strings without using ArduinoJSON.
 *  - fixing http error for different entities while creating and posting and gaurentying no error, test different Things and Locations and how they are creating/patched.
 *  - added flow chart for creating entities, removed all esp32-thingsapi calls/libraries
 * - [0.8.7.0]  Dec. 2, 2019
 *  - added RELEASE_THINGSAPI1 & RELEASE_THINGSAPI2 in esp32_thingsapi/src/workshop_instances/Release-ThingsAPI.h
 *  - in esp32-thingsapi, all prints are precded by [API] to differtiate from other debugging statements
 *  - updated eztime library to 0.8.2, used eztime setInterval(10) function just before waitsync().
 * - [0.8.7.0]  Nov. 1, 2019
 *  - Updated Espressif from 1.0.2 to 1.0.4 and AutoConnect from 1.0.2 to 1.1.2 (Reuires changes in myAutoConnect.h)
 *  - added initial screens around FROST initialisation
 *  - used "master" branch instead of "development" of submodule esp32_thingsapi
 * - [0.8.6.1]  Sept. 19, 2019
 *  - Adding configuration  & OTA pages using AutoConnectAux without Pagebuilder and fixing different issues (Sajjad)
 *  - cleaning all the unnessary comments, defining HW pins constants, addresses etc and documenting them. (Sajjad)
 *  - moved to JSON6 (by Jan)
 *  - fixed graphic OLED overlapping bugs because it don't have clearline() function and it clears whole display (Sajjad)
 *  - using BME280 RED/PURPLE by just changing address in bme(address, sda, scl) (Sajjad)
 *  - fixed BME280 not found problem by using another TwoWire instance Wire1 (Sajjad)
 *  - started doxygen documentations (Sajjad).
 *  - graphic based oled with qr support (Sajjad).
 *  - converted to state machine (Sajjad).
 *  - inherited AutoConnect to use custom code (Sajjad).
 *  - added NTP sync timeout (Sajjad).
 *  .
 * \subsection log1 FIRMWARE_VERSION 0.8.6 and older
 * change history (since version 0.8)
 * - [0.8.1]
 *  - bugfix for coordinates: switched to coordinates[]={long, lat...}
 * - [0.8.2]
 *  - added hack for HTTP patch for location
 *  - removed all libs from src folder that are used in its original state and included the libs directly in arduino
 *  - updated SDS011 lib to newest version
 *  - changed display font
 *  - added over the air update support
 *  - added mDNS: now esp32.local can be typed into browser to connect to esp32
 * - [0.8.3]
 *  - bugfix for error that no reboot occured after timeout in ap mode
 *  - bugfix for wrong display message if no RH T sensor is configured
 *  - bugfix for wrong display message if Wifi is searched
 *  - improved display messages
 *  - bugfix for not working geocode button
 * - [0.8.4]
 *  - added properties and webinterface inputs for data license
 *  - geocoding button does not work again, previous bugfix did not work
 * - [0.8.5]
 *  - (hopefully) permanent bugfix for not working geocode button
 *  - added license url to webinterface
 *  - added RELEASE macro that can be used to switch between development and release versions (switches server addresses and supressses some serial output)
 *  - display shows FW version for 2 sec on startup
 *  - webinterface shows map to pick location
 *  - added reboot after saving configuration, so that thing and datastreams are patched in new setup()
 *  
 * \section deb Quick Debugging
 * The detailed debugging can be controlled for the firmware using the constant RELEASE.
 * -# ESP32-THINGSAPI DEBUGGING
 * For quick debugging of different functionality and parts of the code, we can use following quick tricks for debugging.
 * Defined a constant i.e. RELEAS_THINGSAPI1 & RELEAS_THINGSAPI2 in the file main.h, for more detailed or more important respectively.\n
 * uncomment #MY_DEBUG to supress debugg messages (otherwise will print more detailed)\n
  * All prints are precded by [API] to differtiate from other debugging statements 
 * -# Use Short Timeouts
 *  - #APMODE_REBOOT_TIMEOUT 3600000=10min (default).  This is intervall of auto reboot while in access point mode. Try with 1min for debugging
 *  - #NTP_TIMEOUT_WAIT=60s. Timeout for NTP timeout in waitForSync(), try 20s
 *  - #WIFI_REBOOT_TIMEOUT=150000=2.5min, Timeout to search WiFi already configured.  Try with 1min for debugging
 * -# Sending data without JSON error: To test some functionality, like AutoConnect, Wifi, EEPROM, Timeouts etc, uploading is not required, one can skip it.\n
 * <pre><code class="c">
 *    case check_location_data:
 *       // only send if position data is given
 *       //if (enable_send2frost &&  (get_value("user_data", "latitude") != ""))
 *       //{
 *       //  Serial.println("position data is given, start sending");
 *       //  state = check_timesync;
 *       //}else
 *         state = handle_client;
 *       break;
 * </code></pre>
 * -# ByPassing Save Credentials: One can use the manual WiFi to quickly run the code. Put the following in the setup()\n
 * <pre><code class="c">
 *  WiFi.begin("WG21", "Pakistan1");//Sajjad Debugging
 * </code></pre>
 * -# Testing Webpage & Avoid changing data: One can fix the eeprom data by putting the following code in the setup()\n
 * <pre><code class="c">
 * set_value("user_data", "address", "");       //inputAddress
 * set_value("user_data", "latitude", "");       //inputLon
 * set_value("user_data", "longitude", "");      //inputLat
 * set_value("user_data", "rht_sensor", "default");  //inputRhtsensor
 * set_value("user_data", "sds011id", "");       //inputSds011id
 * set_value("user_data", "data_license", "CC BY 4.0");//inputLicense
 * set_value("user_data", "data_owner", "");     //inputOwner
 * </code></pre>
 * -# If the storage space is too small when compiling, change it in arduino IDE:\n
 * Tools -> board -> select ESP32 DEV Module\n
 * Tools -> partition scheme -> No OTA (large APP)\n
 *
 *\section author Author(s)
 * - Jan Formanek, 
 * - Sajjad Hussain, 
 * - Matthias Budde, 
 *
 * The team at TECO (www.teco.edu) and at Chair for Pervasive Computing Systems (https://pcs.tm.kit.edu).
 *
 * \section license License
 * This code belongs to TECO www.teco.edu and must be reffered when used. BSD license, all text here must be included in any redistribution.
 * \subsection ack Acknowledgments
 * The following libraries were used:\n
 * - Espressif Systems: Licenses and Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
 * - ArduinoJson is licensed under the MIT License. https://arduinojson.org/
 * - Adafruit BME280 is licensed under Adafruit. https://adafruit.com/
 * - DHT sensor is licensed under Adafruit. https://adafruit.com/
 * - AutoConnect is licensed under the MIT License. https://github.com/Hieromon/AutoConnect
 * - QRCode by Richard Moore is licensed under the MIT License https://github.com/ricmoo/QRCode/blob/master/LICENSE.txt
 * - ezTime is licensed under the MIT License. https://github.com/ropg/ezTime
 * - PageBuilder is licensed under the MIT License. https://github.com/Hieromon/PageBuilder
 * - U8g2 is licensed under the MIT License. https://github.com/olikraus/u8g2
 * - SDS011 sensor Library is licensed under the MIT License. (https://github.com/ricki-z/SDS011)
 * 
 * \subsection mit MIT License
 * 
 *        Copyright (c) 2017-2019 Hieromon Ikasamo
 * 
 *        Permission is hereby granted, free of charge, to any person obtaining a copy
 *        of this software and associated documentation files (the "Software"), to deal
 *        in the Software without restriction, including without limitation the rights
 *        to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *        copies of the Software, and to permit persons to whom the Software is
 *        furnished to do so, subject to the following conditions:
 * 
 *        The above copyright notice and this permission notice shall be included in all
 *        copies or substantial portions of the Software.
 * 
 *        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *        LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *        OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *        SOFTWARE.
 * 
 * 
 * \subsection u8gLic U8g2lib License
 * 
 *        The U8g2lib code (http://code.google.com/p/u8g2/) is licensed under the terms of 
 *        the new-bsd license (two-clause bsd license).
 *        See also: http://www.opensource.org/licenses/bsd-license.php
 * 
 *        The repository and optionally the releases contain icons, which are
 *        derived from the WPZOOM Developer Icon Set:
 *        http://www.wpzoom.com/wpzoom/new-freebie-wpzoom-developer-icon-set-154-free-icons/
 *        WPZOOM Developer Icon Set by WPZOOM is licensed under a Creative Commons 
 *        Attribution-ShareAlike 3.0 Unported License.
 * 
 *        Fonts are licensed under different conditions.
 *        See  https://github.com/olikraus/u8g2/wiki/fntgrp
 *        for detailed information on the licensing conditions for each font. 
 * 
 * 
 * \subsection EspressifLic Espressif Systems License
 * 
 *        Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
 *        
 *        Licensed under the Apache License, Version 2.0 (the "License");    
 *        you may not use this file except in compliance with the License.    
 *        You may obtain a copy of the License at    
 *        http://www.apache.org/licenses/LICENSE-2.0    
 *        Unless required by applicable law or agreed to in writing, software    
 *        distributed under the License is distributed on an "AS IS" BASIS,    
 *        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    
 *        See the License for the specific language governing permissions and    
 *        limitations under the License.
 *        
 *        
 *\date 19.08.2019
 */



//******************************************************** constants ***************************

/// firmware version, (for intermediate versions being actively worked on, append "pre" to number, indicating that it is "not yet" the specified number)
#define FIRMWARE_VERSION "1.1"
/// Software.version Name property for Thing and Datastreams 
#define sw_version FIRMWARE_VERSION
/// Hardware.Revision Date property for Thing and Datastreams
#define hw_date "2020-01-17T12:00:00.000Z"

/// Debugging, uncomment to switch servers to release version, enable sending to madavi and luftdaten.info, and supress some debug output
#define RELEASE 

// Setting the servers and debugging control using the RELEASE
#ifndef RELEASE
  ///Non-Release Version: Frost Server Base address used in Frost_Server.cpp
  #define BASE_URL "http://xxx.xxx.xx.xxx:xxxx"
  
  ///Define MYDEBUG for controlling debugging
  #define MY_DEBUG
  
#else
  ///Release Version: Frost Server Base address used in Frost_Server.cpp
  #define BASE_URL "http://api.smartaq.net/v1.0"
  
  ///unDefine MYDEBUG for controlling debugging
  #undef MY_DEBUG
  
#endif 

#ifdef MY_DEBUG
  ///Define MY_DEB() for controlling serial.print debugging
  #define MY_DBG(...) do {Serial.print( __VA_ARGS__ );} while (0)
  ///Define MY_DEBln() for controlling serial.println debugging
  #define MY_DBGln(...) do {Serial.println( __VA_ARGS__ );} while (0)
#else
  ///UnDefine MY_DEB() for controlling serial.print debugging
  #define MY_DBG(...)
  ///UnDefine MY_DEBln() for controlling serial.println debugging
  #define MY_DBGln(...)
#endif // !MY_DEBUG

/// switch for sending to FROST teco server
#define ENABLE_SEND2FROST true 
/// intervall of reading sensor data in ms
#define SENSOR_INTERVAL 10000 
/// intervall of showing statistics informations, it is multiple of SENSOR_INTERVAL i.e. 10 mean  100s
#define STATS_INTERVAL 10 

#ifdef RELEASE
  /// switch for sending to madavi api
  #define ENABLE_SEND2MADAVI true  
  /// switch for sending to luftdaten api
  #define ENABLE_SEND2LUFTDATEN true  
#else
  /// switch for sending to madavi api
  #define ENABLE_SEND2MADAVI false 
  /// switch for sending to luftdaten api
  #define ENABLE_SEND2LUFTDATEN false
#endif
/// intervall of sending to madavi api in ms 
#define SEND2MADAVI_INTERVAL 145000
/// intervall of sending to madavi api in ms 
#define SEND2LUFTDATEN_INTERVAL 145000
///enable daily reboot at specific time
#define ENABLE_DAILY_REBOOT true 
/// set time when to do daily reboot (hour of time of day for reboot goes here) //TODO set proper time
//#define TIMETOREBOOT "0400" 
#define TIMETOREBOOT "2245" 
/// intervall of auto reboot while in access point mode (config mode) in ms (600000ms is 10min). autoreboot while in ap mode only occurs if no device is connected to the esp32
#define APMODE_REBOOT_TIMEOUT 600000 
/// Timeout for NTP timeout (s) in waitForSync()
#define NTP_TIMEOUT_WAIT 120 


// The following two lines define the boundalyOffset value to be supplied to
// AutoConnectConfig respectively. It may be necessary to adjust the value
// accordingly to the actual situation.
/// Specified the offset if the user data exists.
#define CREDENTIAL_OFFSET 0
//#define CREDENTIAL_OFFSET 64

/// url for public domain
#define data1_url_cc0 "https://creativecommons.org/publicdomain/zero/1.0/deed.de"
/// url for license property
#define data1_url_ccby "https://creativecommons.org/licenses/by/4.0/deed.de"


/// Reset PIN of OLED Module
#define RST 16
/// The I2C address of the SSD1306 OLDE 
#define OLED_ADDR 0x3c
/// The SDA pin of the SSD1306 OLDE 
#define OLED_SDA 4
/// The SCL pin of the SSD1306 OLDE 
#define OLED_SCL 15

//Following Selecrors are used in postObservation() function
/// Post Observation Selector for SDS011 PM25
#define dsPM25 1
/// Post Observation Selector for SDS011 PM10
#define dsPM10 2
/// Post Observation Selector for BME280 Humidity
#define dsHmBme 3
/// Post Observation Selector for BME280 Temperature
#define dsTmBme 4
/// Post Observation Selector for BME280 Pressure
#define dsPrBme 5
/// Post Observation Selector for DHT22 Humidity
#define dsHmDht 6
/// Post Observation Selector for DHT22 Temperature
#define dsTmDht 7

 
/// state machine initial state
#define init_state          1
/// state machine check NTP sync state
#define check_first_sync      2
/// state machine idle state
#define idle_state          3
/// state machine WiFi found state
#define wifi_found        4
/// state machine start measurement state
#define start_measurement_sds     5
/// state machine check sensor selection state
#define start_measurement_bme   6
/// state machine show valid SDS values state
#define show_sds_values       7
/// state machine show valid BME values state
#define show_bme_values       8
/// state machine check if position data is given state
#define check_location_data     9
/// state machine check NTP sync again state
#define check_timesync        10
/// state machine check if frost is initialized state
#define create_frost_entties       11
/// state machine send sds data state
#define send_sds_data       12
/// state machine send bme data state
#define send_bme_data       13
/// state machine send dht data state
#define send_dht_data       14
/// state machine send madavi data state
#define send_to_madavi        15
/// state machine send luftdaten data state
#define send_to_luftdaten     16
/// state machine WiFi error state
#define wifi_error          17
/// state machine handle client state
#define handle_client       18
/// state machine check server response state
#define check_server_status  19
/// state to show the 2nd screen
#define show_2nd_screen   20
/// state to start showing screens
#define start_showing   21

//******************************************************** includes ***************************

//#include "src/lib/SDS011/SDS011.h"
#include "sds011lib.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
//#include <Update.h>
//#include <AutoConnect.h>
//#include <PageBuilder.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
//#include <time.h>
#include <ezTime.h>
#include <HTTPClient.h>
#include "nvs_storage.h"
#include "sensor.h"
#include "timer_definitions.h"
#include "send_data.h"
#include "create_json.h"
#include "myAutoConnect.h"//inherited the AutoConnect class and overridden the begin() function.

//******************************************************** variables ***************************
//Variables for SAQNET Entities Modelling and Nomenclature
/// Precision for location coordinates
const int precision = 6;
//SAQN Database Modeling: Firmware & License
/// License Type
String licType;
/// License Owner
String licOwner;
/// License Url
String licUrl;
//Location
//--------
//SAQN Database Modeling: Location
/// Location Name
String locName;
/// Location Description
String locDesc;
/// Location Encoding Type
String locEnc;
/// Location Type
String locTyp;
/// Location IOT ID
String locIotId;
/// Location Coordinates
String locCord;
/// Location JSON message
String locJson;
//THING
//--------
//SAQN Database Modeling: Thing
/// Thing Name
String thName;
/// Thing Description
String thDesc;
/// Thing Hardware Revision Date
String thHwDate;
/// Thing Hardware Revision Version
String thHwVersion;
/// Thing Software Version Date
String thSwDate;
/// Thing Software Version
String thSwVersion;
/// Thing Hardware ID
String thHwId;
/// Thing Short Name
String thShortName;
/// Thing Operater Name
String thOpDomain;
/// Thing Simple ID
String thId;
/// Thing IOT ID
String thIotId;
/// Thing JSON Message
String thJson;
/// Thing Properties JSON message
String prJson;
//SENSORS
//--------
//SAQN Database Modeling: Sensor SDS011
/// SDS011 Serial No.
String sdsSerial;
/// SDS011 Name
String sdsName;
/// SDS011 Description
String sdsDesc;
/// SDS011 Encoding Type
String sdsEnc;
/// SDS011 DataSheet URL
String sdsDataUrl;
/// SDS011 Short Name
String sdsShortName;
/// SDS011 Menufacture Name
String sdsManDomain;
/// SDS011 Simple ID
String sdsId;
/// SDS011 IOT ID
String sdsIotId;
/// SDS011 Meta DATA
String sdsMeta;
/// SDS011 JSON Message
String sdsJson;
//SAQN Database Modeling: Sensor BME280
/// BME280 Serial No.
String bmeSerial;
/// BME280 Name
String bmeName;
/// BME280 Description
String bmeDesc;
/// BME280 Encoding Type
String bmeEnc;
/// BME280 Datasheet Url 
String bmeDataUrl;
/// BME280 Short Name
String bmeShortName;
/// BME280 Manufacture Domain
String bmeManDomain;
/// BME280 Simple ID
String bmeId;
/// BME280 IOT ID
String bmeIotId;
/// BME280 MetaData
String bmeMeta;
/// BME280 JSON Message
String bmeJson;
//SAQN Database Modeling: Sensor DHT22
/// DHT22 Serial No.
String dhtSerial;
/// DHT22 Name
String dhtName;
/// DHT22 Description
String dhtDesc;
/// DHT22 Encoding Type
String dhtEnc;
/// DHT22 Datasheet Url
String dhtDataUrl;
/// DHT22 Short Name
String dhtShortName;
/// DHT22 Manufacture Domain
String dhtManDomain;
/// DHT22 SImple ID
String dhtId;
/// DHT22 IOT ID
String dhtIotId;
/// DHT22 Metadata
String dhtMeta;
/// DHT22 JSON Message
String dhtJson;
//OBSERVER PROPERTIES
//-------------------
//SAQN Database Modeling: ObservedProperty PM10
/// ObservedProperty PM10 Name 
String opPM10Name;
/// ObservedProperty PM10 Description
String opPM10Desc;
/// ObservedProperty PM10 Definition
String opPM10Defi;
/// ObservedProperty PM10 Short Definition
String opPM10ShortDefi;
/// ObservedProperty PM10 Short Name
String opPM10ShortName;
/// ObservedProperty PM10 Unit of Measurement
String opPM10UoM;
/// ObservedProperty PM10 UoM Symbol
String opPM10Sym;
/// ObservedProperty PM10 UoM Convention Definition
String opPM10ConDef;
/// ObservedProperty PM10 Simple ID
String opPM10Id;
/// ObservedProperty PM10 IoT ID
String opPM10IotId;
/// ObservedProperty PM10 JSON message
String opPM10Json;
//SAQN Database Modeling: ObservedProperty PM2_5
/// ObservedProperty PM2.5 Name
String opPM25Name;
/// ObservedProperty PM2.5 Description
String opPM25Desc;
/// ObservedProperty PM2.5 Definition
String opPM25Defi;
/// ObservedProperty PM2.5 Short Definition
String opPM25ShortDefi;
/// ObservedProperty PM2.5 Short Name
String opPM25ShortName;
/// ObservedProperty PM2.5 Unit of Measurement
String opPM25UoM;
/// ObservedProperty PM2.5 UoM Symbol
String opPM25Sym;
/// ObservedProperty PM2.5 UoM Convention Definition
String opPM25ConDef;
/// ObservedProperty PM2.5 Simple ID
String opPM25Id;
/// ObservedProperty PM2.5 IoT ID
String opPM25IotId;
/// ObservedProperty PM2.5 JSON Message
String opPM25Json;
//SAQN Database Modeling: BME280/DHT22 ObservedProperty Humidity
/// ObservedProperty Humidity Name
String opHmName;
/// ObservedProperty Humidity Description
String opHmDesc;
/// ObservedProperty Humidity Definition
String opHmDefi;
/// ObservedProperty Humidity Short Definition
String opHmShortDefi;
/// ObservedProperty Humidity Short Name
String opHmShortName;
/// ObservedProperty Humidity Unit of Measurement
String opHmUoM;
/// ObservedProperty Humidity UoM Symbol
String opHmSym;
/// ObservedProperty Humidity UoM Convention Definition
String opHmConDef;
/// ObservedProperty Humidity Simple ID
String opHmId;
/// ObservedProperty Humidity IoT ID
String opHmIotId;
/// ObservedProperty Humidity JSON Message
String opHmJson;
//SAQN Database Modeling: BME280/DHT22 ObservedProperty Temperature
/// ObservedProperty Temperature Name
String opTmName;
/// ObservedProperty Temperature Description
String opTmDesc;
/// ObservedProperty Temperature Definition
String opTmDefi;
/// ObservedProperty Temperature Short Definition
String opTmShortDefi;
/// ObservedProperty Temperature Short Name
String opTmShortName;
/// ObservedProperty Temperature Unit of Measurement
String opTmUoM;
/// ObservedProperty Temperature UoM Symbol
String opTmSym;
/// ObservedProperty Temperature UoM Convention Definition
String opTmConDef;
/// ObservedProperty Temperature Simple ID
String opTmId;
/// ObservedProperty Temperature IoT ID
String opTmIotId;
/// ObservedProperty Temperature JSON Message
String opTmJson;
//SAQN Database Modeling: BME280 ObservedProperty Pressure
/// ObservedProperty Pressure Name 
String opPrName;
/// ObservedProperty Pressure Description
String opPrDesc;
/// ObservedProperty Pressure Definition
String opPrDefi;
/// ObservedProperty Pressure Short Definition
String opPrShortDefi;
/// ObservedProperty Pressure Short Name
String opPrShortName;
/// ObservedProperty Pressure Unit of Measurement
String opPrUoM;
/// ObservedProperty Pressure UoM Symbol
String opPrSym;
/// ObservedProperty Pressure UoM Convention Definition
String opPrConDef;
/// ObservedProperty Pressure Simple ID
String opPrId;
/// ObservedProperty Pressure IoT ID
String opPrIotId;
/// ObservedProperty Pressure JSON Message
String opPrJson;
// DATASTREAMS
//------------  
//SAQN Database Modeling: Datastream PM10
/// Datastream BME280 PM10 Name
String dsPM10Name;
/// Datastream BME280 PM10 Description
String dsPM10Desc;
/// Datastream BME280 PM10 Type
String dsPM10ObType;
/// Datastream BME280 PM10 Unit of Measurement
String dsPM10UoMName;
/// Datastream BME280 PM10 UoM Symbol
String dsPM10UoMSym;
/// Datastream BME280 PM10 UoM Definition
String dsPM10UoMDef;
/// Datastream BME280 PM10 Simple ID
String dsPM10Id;
/// Datastream BME280 PM10 IoT ID
String dsPM10IotId;
/// Datastream BME280 PM10 JSON Message
String dsPM10Json;
//SAQN Database Modeling: Datastream PM2_5
/// Datastream BME280 PM2.5 Name
String dsPM25Name;
/// Datastream BME280 PM2.5 Description
String dsPM25Desc;
/// Datastream BME280 PM2.5 Type
String dsPM25ObType;
/// Datastream BME280 PM2.5 Unit of Measurement
String dsPM25UoMName;
/// Datastream BME280 PM2.5 UoM Symbol
String dsPM25UoMSym;
/// Datastream BME280 PM2.5 UoM Definition
String dsPM25UoMDef;
/// Datastream BME280 PM2.5 Simple ID
String dsPM25Id;
/// Datastream BME280 PM2.5 IoT ID
String dsPM25IotId;
/// Datastream BME280 PM2.5 JSON Message
String dsPM25Json;
//SAQN Database Modeling: BME280 Datastream Humidity
/// Datastream BME280 Humidity Name
String dsHmBmeName;
/// Datastream BME280 Humidity Description
String dsHmBmeDesc;
/// Datastream BME280 Humidity Type
String dsHmBmeObType;
/// Datastream BME280 Humidity Unit of Measurement Name
String dsHmBmeUoMName;
/// Datastream BME280 Humidity UoM Symbol
String dsHmBmeUoMSym;
/// Datastream BME280 Humidity UoM Definition
String dsHmBmeUoMDef;
/// Datastream BME280 Humidity Simple ID
String dsHmBmeId;
/// Datastream BME280 Humidity IoT ID
String dsHmBmeIotId;
/// Datastream BME280 Humidity JSON MEssage
String dsHmBmeJson;
//SAQN Database Modeling: BME280 Datastream Temperature
/// Datastream BME280 Temperature Name
String dsTmBmeName;
/// Datastream BME280 Temperature Description
String dsTmBmeDesc;
/// Datastream BME280 Temperature Type
String dsTmBmeObType;
/// Datastream BME280 Temperature Unit of Measurement Name
String dsTmBmeUoMName;
/// Datastream BME280 Temperature UoM Symbol
String dsTmBmeUoMSym;
/// Datastream BME280 Temperature UoM Definition
String dsTmBmeUoMDef;
/// Datastream BME280 Temperature Simple ID
String dsTmBmeId;
/// Datastream BME280 Temperature IoT ID
String dsTmBmeIotId;
/// Datastream BME280 Temperature JSON Message
String dsTmBmeJson;
//SAQN Database Modeling: BME280 Datastream Pressure
/// Datastream BME280 Pressure Name
String dsPrBmeName;
/// Datastream BME280 Pressure Description
String dsPrBmeDesc;
/// Datastream BME280 Pressure Type
String dsPrBmeObType;
/// Datastream BME280 Pressure Unit of MEasurement Name
String dsPrBmeUoMName;
/// Datastream BME280 Pressure UoM Symbol
String dsPrBmeUoMSym;
/// Datastream BME280 Pressure UoM Definition
String dsPrBmeUoMDef;
/// Datastream BME280 Pressure Simple ID
String dsPrBmeId;
/// Datastream BME280 Pressure IoT ID
String dsPrBmeIotId;
/// Datastream BME280 Pressure JSON Message
String dsPrBmeJson;
//SAQN Database Modeling: DHT22 Datastream Humidity
/// Datastream DHT22 Humidity Name
String dsHmDhtName;
/// Datastream DHT22 Humidity Description
String dsHmDhtDesc;
/// Datastream DHT22 Humidity Type
String dsHmDhtObType;
/// Datastream DHT22 Humidity Unit of Measurement Name
String dsHmDhtUoMName;
/// Datastream DHT22 Humidity UoM Symbol
String dsHmDhtUoMSym;
/// Datastream DHT22 Humidity UoM Definition
String dsHmDhtUoMDef;
/// Datastream DHT22 Humidity Simple ID
String dsHmDhtId;
/// Datastream DHT22 Humidity IoT ID
String dsHmDhtIotId;
/// Datastream DHT22 Humidity JSON Message
String dsHmDhtJson;
//SAQN Database Modeling: DHT22 Datastream Temperature
/// Datastream DHT22 Temperature Name
String dsTmDhtName;
/// Datastream DHT22 Temperature Description
String dsTmDhtDesc;
/// Datastream DHT22 Temperature Type
String dsTmDhtObType;
/// Datastream DHT22 Temperature Unit of Measurement Name
String dsTmDhtUoMName;
/// Datastream DHT22 Temperature UoM Symbol
String dsTmDhtUoMSym;
/// Datastream DHT22 Temperature UoM Definition
String dsTmDhtUoMDef;
/// Datastream DHT22 Temperature Simple ID
String dsTmDhtId;
/// Datastream DHT22 Temperature IoT ID
String dsTmDhtIotId;
/// Datastream DHT22 Temperature JSON Message
String dsTmDhtJson;

// do not change this vars
/// tracking if BME280 sensor is selected
bool use_bme280 = false;
/// tracking if DHT22 sensor is selected
bool use_dht22 = false;
/// if frost manager is initialized, i.e. entities are createdd on the server seccessfully
bool initialised_frost = false;
/// indicated the busy status while entities are being created on the server
bool busy;
/// if HTTP Server manager is initialized
bool initialised_http = false;
/// if wifi is connected
bool wifi_connected;
/// if NTP time is synced
bool time_updated;
/// total number of observations posted
unsigned  int tObservations;
/// total number of observations not posted successfully
unsigned  int nObservations;
/// No. of reboots
unsigned  int resetCounter;
/// the time of the measurement of the first observation
String timeOfMeasure;
/// the error encodings
unsigned int errorCode;
/// the error encodings last saved
unsigned int oldErrCode;
/// count the intervals of sensor measurement
unsigned int countSensorInterval;
/// Add of the BME280 I2C, dynamic fetched from BME_ADR
unsigned char bmeAddress;
/// vars to store sensor results, sensor results for dust sensor pm10
float pm10;
/// sensor results for dust sensor pm25
float pm25;
/// temperature values
float temp;
/// humidity values
float hum;
/// pressure values
float atm;
/// time of measurement for sds reading
String timeofmeas_sds;
/// time of measurement for rht reading
String timeofmeas_rht;
/// sds status
int status_sds;
/// set display contrast
//boolean contrast;
/// HTTP Server error
boolean http_error;
/// state machine tracking variable
unsigned char state;
/// Location Latitude Point
float latitude;
/// Location longitude Point
float longitude;
/// Location abovenn Point
float abovenn;
/// Location Estimated abovenn Point
float abovennEstimated;
/// Human Readable Name of the street location
String humanReadableLocation;
/// rht sensor global variable
String rhtSensor;
/// Possible additional sensors other than SDS011
//enum AdditionalSensor {NO = 0,BME280 = 1,DHT22 = 2,};
/// additional sensors tracking variable
//AdditionalSensor additionalSensor;
/// License Type for License property
String license_type;
/// License Owner for License property
String license_owner;
/// License Meta for License property
String license_url;
/// Chip ID for the ESp32
String device_Serial;
/// ID written of SDS011 Sensor
String SDS011_Serial;
/// used to store mac address
byte mac[6];
/// Chip ID based on MAC
String esp32_chipid;

/// host to send data madavi API
//char destination_madavi[] = "http://192.168.12.67/data.php"; //TODO set proper url
//char destination_luftdaten[] = "http://192.168.12.67/data.php"; //TODO set proper url
char destination_madavi[] = "https://api-rrd.madavi.de/data.php";
/// host to send data luftdaten API
char destination_luftdaten[] = "https://api.luftdaten.info/v1/push-sensor-data/";

///HTTP Client Variable Global
HTTPClient gHttp;
///HTTP Client Response Global
int gHttpCode;


/// initialise udp for network time
WiFiUDP ntpUDP;
/// initialise Webserver
WebServer Server; 
/// initialise AutoConnect
//AutoConnect Portal(Server); 
myAutoConnect Portal(Server);
/// initialise AutoConnectConfig
AutoConnectConfig  Config;
/// initialise the OLED which is used. OLED Module Object with I2C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 15, /* data=*/ 4);

//******************************************************** AutoConnect Scripts ***************************

/// String to hold temporary html scripts for AutoConnect
String  cfgTemp;

/// Crowdsensor html page script
ACElement(cfgScript,  
"  <script src=\"http://www.openlayers.org/api/OpenLayers.js\"></script>\n"
"  <script>\n"
"    //global vars\n"
"    var latitude;\n"
"    var longitude;\n"
"    //var elevation;\n"
"    var address            = \"\";\n"
"    var wgsProjection      = new OpenLayers.Projection(\"EPSG:4326\");   // WGS 1984 Projection\n"
"    var mercatorProjection = new OpenLayers.Projection(\"EPSG:900913\"); // Spherical Mercator Projection\n"
"    var initialposition    = new OpenLayers.LonLat(8.424171,49.013034).transform(wgsProjection, mercatorProjection);\n"
"    var initialzoom        = 3;\n"
"    var position           = initialposition;\n"
"    var zoom               = 18;\n"
"    var mapnik             = new OpenLayers.Layer.OSM();\n"
"    var markers            = new OpenLayers.Layer.Markers(\"Markers\");\n"
"    var marker             = new OpenLayers.Marker(position);\n"
"    var touchNavigation    = new OpenLayers.Control.TouchNavigation({\n"
"                                 defaultClickOptions:{\n"
"                                     pixelTolerance: 10\n"
"                                 }\n"
"                             });\n"
"\n  "
"    // function to geocode: look up coordinates from address text field, populate lon and lat fields and update map \n"
"    function geolookup() {\n"
"      address = document.getElementById('inputAddress').value;\n"
"      var request = new XMLHttpRequest();\n"
"      var url = \"https://nominatim.openstreetmap.org/search/'\" + encodeURI(address) + \"'?format=json&limit=1\";\n"
"      request.open('GET', url, true);\n"
"      request.onload = function() {\n"
"        if (this.status >= 200 && this.status < 300) {\n"
"          var response = JSON.parse(this.responseText);\n"
"          if (response[0]) {\n"
"            latitude = Number(response[0].lat).toFixed(6);\n"
"            longitude = Number(response[0].lon).toFixed(6);\n"
"            document.getElementById(\"inputLon\").value = longitude;\n"
"            document.getElementById(\"inputLat\").value = latitude;\n"
"            updateMarker();\n"
"          } else {\n"
"            alert(\"Keine Koordinaten gefunden, bitte Adress-Anfrage ändern.\");\n"
"          }\n"
"        } \n"
"      };\n"
"      request.send();\n"
"    }\n"
"\n  "
"    // function to \"move\" marker (avoiding marker.moveTo(), as it is an \"unofficial\" function)\n"
"    function replaceMarker() {\n"
"      markers.clearMarkers();\n"
"      position = new OpenLayers.LonLat(longitude,latitude).transform(wgsProjection, mercatorProjection);\n"
"      marker = new OpenLayers.Marker(position);\n"
"      markers.addMarker(marker);\n"
"    }\n"
"\n  "
"\n  "
"    // function to update marker based on inputs and center on map\n"
"    function updateMarker() {\n"
"      longitude = document.getElementById(\"inputLon\").value;\n"
"      latitude = document.getElementById(\"inputLat\").value;  \n"
"\n  "
"      if (typeof map !== 'undefined') {\n"
"        if (latitude && longitude) {\n"
"          replaceMarker();\n"
"          map.setCenter(position, zoom);\n"
"        } else {\n"
"          markers.clearMarkers();\n"
"          map.setCenter(initialposition, initialzoom);\n"
"        }\n"
"      }\n"
"    }\n"
"\n  "
"    // function to \"move\" marker on click in map\n"
"    OpenLayers.Util.extend(touchNavigation, {\n"
"      defaultClick: function(evnt) {\n"
"        var lonlat = map.getLonLatFromViewPortPx(evnt.xy);\n"
"\n"
"        latitude =  lonlat.transform(mercatorProjection, wgsProjection).lat.toFixed(6);\n"
"        longitude = lonlat.lon.toFixed(6);\n"
"\n"
"        document.getElementById(\"inputLon\").value = longitude;\n"
"        document.getElementById(\"inputLat\").value = latitude;\n"
"\n"
"        replaceMarker();\n"
"      }\n"
"    });\n"
"\n"
"    // function to unfocus current form element when enter key was pressed\n"
"    function unfocusForm() {\n"
"      if (event.key == 'Enter') {\n"
"        document.activeElement.blur();\n"
"        return false;\n"
"      } else {\n"
"        return true;\n"
"      }\n"
"    }\n"
"\n  "
"    // execute the following once all DOM elements are loaded\n"
"    document.addEventListener('DOMContentLoaded', function(){\n"
"\n  "
//"      if(\"{{WIFICONNECTIVITY}}\"== \"true\"){\n"
"        // add geocoding button\n"
"        var findbutton = document.createElement(\"button\");\n"
"        findbutton.setAttribute(\"id\", \"geocode\");\n"
"        findbutton.setAttribute(\"type\", \"button\");\n"
"        findbutton.innerHTML = \"Suche Koordinaten\";\n"
"        findbutton.addEventListener (\"click\", function() {\n"
"          geolookup();\n"
"        });\n"
"        document.getElementById(\"pgeocode\").appendChild(findbutton);\n"
"\n  "
"        // init OpenLayers map\n"
"        map = new OpenLayers.Map(\"mapDiv\");\n"
"        map.addLayer(mapnik);\n"
"        map.addLayer(markers);\n"
"\n  "
"        updateMarker();\n"
"\n  "
"        // register function to \"move\" marker\n"
"        map.addControl(touchNavigation);   \n"
"        touchNavigation.activate();\n"
"\n  "
"        document.getElementById(\"inputLon\").addEventListener(\"change\", updateMarker);\n"
"        document.getElementById(\"inputLat\").addEventListener(\"change\", updateMarker);\n"
"\n  "
"\n  "
//"      }\n"
"    });\n"
"  </script>\n");

/// Crowdsensor temporary ACText Element
ACText(cfgData, " ");
/// Crowdsensor main page html body
ACElement(cfgBody,  
"<style type=\"text/css\"> \n"
"  input[type=number], input#owner { font-weight: 300; width: calc(100% - 124px); background-color: #fff; border: 1px solid #ccc; border-radius: 2px; color: #444; margin: 8px 0 8px 0; padding: 10px; }\n"
"  button[type=button] { font-weight: normal; padding: 8px 14px; margin: 12px; width: auto; outline: none; text-decoration: none; background-color: #1b5e20; border-color: #1b5e20; letter-spacing: 0.8px; color: #fff; border: 1px solid; border-radius: 2px; font-size: 0.9em; }\n"
"  div#OpenLayers_Control_Attribution_16 { bottom: .2em; }\n"
//    ".menu > a:link {"
//    "position: absolute;"
//    "display: inline-block;"
//    "right: 12px;"
//    "padding: 0 6px;"
//    "text-decoration: none;"
//    "}"
"</style>\n"
"\n"
//"<div class=\"menu\">" AUTOCONNECT_LINK(BAR_32) "</div>\n"
//"<h1 style=\"width: 90%\">Crowdsensor Konfiguration Firmware:FIRMWARE_VERSION</h1>\n"
//"  <label for \"inputFW\" style=\"width: 90%\">Crowdsensor Konfiguration Firmware: </label>\n"
//"  <input type=\"text\" name=\"inputFW\" id=\"inputFW\"  placeholder=\"FW\" style=\"border: none;background-color:none;outline:0;font-size: 50px;width: 1200px;height:50px; font-weight:bold;color:#000;\" readOnly value=\"\">\n"
//"\n"
"  <div id=\"mapDiv\" style=\"height:250px\"><br/></div><br/>\n"
"\n"
"  <h3>Standort des Sensors</h3>\n"
//"  <form>\n"
//"  <form onsubmit=\"return confirm('Sind die Daten korrekt?');\" onkeydown=\"return unfocusForm()\" action=\"/config\" method=\"POST\">\n"
"  <label for=\"inputAddress\">Adresse, Format: Straße Hausnummer, Stadt </label>\n"
"  <br />\n"
"  <input type=\"text\" name=\"inputAddress\" id=\"inputAddress\" maxlength=\"160\" placeholder=\"Adresse\" value=\"\"  style=\"width: calc(100% - 124px);\">\n"
"  <br />\n"
"  <span id=\"pgeocode\"></span>\n"
"  <br />\n"
"  <label for=\"inputLon\"><b>Länge</b>, Format: -3.703790 (für Europa im Bereich -15 bis 35):</label>\n"
"  <br />\n"
"  <input type=\"number\" name=\"inputLon\" id=\"inputLon\" min=\"-180\" max=\"180\" step=\"0.000001\" placeholder=\"z.B. -3.703790\" value=\"\" required onchange=\"updateMarker()\">\n"
"  <br />\n"
"  <br />\n"
"  <label for=\"inputLat\"><b>Breite</b>, Format: 40.416775 (für Europa im Bereich 35 bis 75):</label>\n"
"  <br />\n"
"  <input type=\"number\" name=\"inputLat\" id=\"inputLat\" min=\"-90\" max=\"90\" step=\"0.000001\" placeholder=\"z.B. 40.416775\" value=\"\" required onchange=\"updateMarker()\"> \n"
"  <br />\n"
"  <br />\n"
"  <h3>Sensoren</h3>\n"
"  <label for=\"inputRhtsensor\">Temperatursensor, der benutzt werden soll</label>\n"
"  <br />\n"
"  <select name=\"inputRhtsensor\" id=\"inputRhtsensor\">\n"
"  <option value=\"default\">(keiner)</option>\n"
"  <option value=\"BME280\">BME280</option>\n"
"  <option value=\"DHT22\">DHT22</option>\n"
"  </select>\n"
"  <br />\n"
"  <br />\n"
//"  <p><b>ID des SDS011 </b>Feinstaub-Sensors (XXXX-XXXX)</p>\n"
"  <label for=\"inputSds011id\">ID des SDS011, Feinstaub-Sensors (XXXX-XXXX)</label>\n"
"  <br />\n"
"  <input type=\"text\"  name=\"inputSds011id\" id=\"inputSds011id\" maxlength=\"10\" placeholder=\"XXXX-XXXX\" value=\"\">\n"
"  <br />\n"
"  <br />\n"
"  <h3>Datenlizenz</h3>\n"
"  <h4>Lizenz Name</h4>\n"
"  <label for=\"inputLicense\">Unter der die von deinem Sensor aufgenommenen Daten im SmartAQnet veröffentlicht werden sollen</label>\n"
"  <br />\n"
"  <select name=\"inputLicense\" id=\"inputLicense\" onchange=\"set_required()\">\n"
"  <option value=\"CC0 1.0\">CC0 1.0 Universell</option>\n"
"  <option value=\"CC BY 4.0\">CC BY 4.0 Namensnennung International</option>\n"
"  </select>\n"
"  <br />\n"
"  <span style=\"font-size: 75%\"> Weitere Informationen: <a href=\"" data1_url_cc0 "\" id=\"license_url\">" data1_url_cc0 "</a></span>\n"
"  <script>\n"
"  function set_required() {\n"
"    var lic = document.getElementById('inputLicense').value;\n"
"      if (lic == \"CC BY 4.0\") {\n"
"        document.getElementById(\"inputOwner\").required = true;\n"
"        document.getElementById(\"license_url\").innerHTML = \"" data1_url_ccby "\";\n"
"        document.getElementById(\"license_url\").href = \"" data1_url_ccby "\";\n"
"      } else {\n"
"        document.getElementById(\"inputOwner\").required = false;\n"
"        document.getElementById(\"license_url\").innerHTML = \"" data1_url_cc0 "\";\n"
"        document.getElementById(\"license_url\").href = \"" data1_url_cc0 "\";\n"
"      }\n"
"  }\n"
"  </script>\n"
"  <br />\n"
"  <br />\n"
"  <h4>Legal Notice</h4>\n"
"  <label for=\"inputOwner\">Für Namensnennung in Lizenz (optional bei CC0)</label>\n"
"  <br />\n"
"  <input type=\"text\" name=\"inputOwner\" id=\"inputOwner\" maxlength=\"40\" placeholder=\"Name (optional bei CC0)\" value=\"\">\n"
//"  <br />\n"
"  <br />\n"
"  <input type=\"button\" value=\"Speichern\" name=\"cfgButton\" onclick = \"if (confirm('Daten speichern?')) _sa('/config')\" method=\"post\">\n"
//"  <input type=\"submit\" value=\"Speichern\" onclick = \"ConfirmBox1();\" method=\"post\">\n"
//"  <input type=\"submit\" value=\"Speichern\" onclick = \"return confirm('Daten speichern?');\" window.location.href=\"_sa('/config')\">\n"
//"  <form onsubmit=\"return confirm('Sind die Daten korrekt?');\" onkeydown=\"return unfocusForm()\" action=\"/config\" method=\"POST\">\n"
//"  <input type=\"submit\" value=\"Speichern\">\n"
//"</form>\n"
"<script>\n"
"function ConfirmBox1() {\n"
"  if (confirm('Daten speichern1?')) window.location.href = \"/config\";\n"
"}\n"
"</script>\n"
"<br />\n"
);
///Crowdsensor main page html body remaining part
ACElement(cfgBody2,
"<p>Hinweise: <br />\n"
"<ul>\n"
"  <li>Die Daten werden nur gespeichert, wenn sie den \"Speichern\" Button klicken.</li>\n"
"  <li>Die Eingabe neuer Daten überschreibt die alten Daten (z.B. für Standortwechsel).</li>\n"
"  <li>Alle von Ihnen eingegeben Informationen werden offen in der Datenbank abrufbar sein.</li>\n"
"</ul>\n"
"</p>\n"
"<script>\n"
"  window.addEventListener('load', function(){\n"
"    document.getElementById(\"_aux\").onkeydown = \"return unfocusForm()\"\n"
"  });\n"
"</script>\n"
"");

/// Page for OTA Programming html
const char* otaUpdate = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')" 
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

///Crowdsensor main page html main title as ACText element
ACText(cfgtitle, "", "text-align:center;color:#2f4f4f;");
///Crowdsensor main page cfgSensorPageAux based on different  AutoConnect Elements
AutoConnectAux cfgSensorPageAux("/", "Configure", true, {
  cfgtitle,
  cfgScript,
  cfgBody,
  cfgData,
  cfgBody2
});
///Crowdsensor OTA page html script
ACElement(otaScript, otaUpdate);
///Crowdsensor OTA page cfgSensorPageAux based on different  AutoConnect Elements
AutoConnectAux cfgUpdatePageAux("/otaUpdate", "Update", true, {
  otaScript
});


//******************************************************** FUNCTIONS ***************************

/// Get Standard time
String getIsoTime() {
  return (UTC.dateTime("Y-m-d\\TH:i:s") + ".000Z");
}

/// Get hours from the time
String getHourMinute() {
  return (UTC.dateTime("Hi"));
}

/// get chip id from MAC only 3-bytes
String getChipId() 
{
  // save mac address and convert to char array.
  // only last 3 bytes of mac address are used, because the first 3 bytes are fixed and always the same on every board

  // get mac address
  // wasting memory for conversion last 3 bytes of byte mac into string mac. should be done better
  esp_efuse_mac_get_default(mac);

  char mac_str[18];
  // convert to string
  snprintf(mac_str, sizeof(mac_str), "%02x%02x%02x", mac[3], mac[4], mac[5]);
  unsigned long mac_decimal =  strtoul(mac_str, NULL, 16);
  esp32_chipid = String(mac_decimal);//"181721";//
  return esp32_chipid;
}

/// Function to handle the http://[IPAddress]/dummy response of Webserver::on(...). This is a dummy url for hidden tests.

void handleDummy() {
  String page = PSTR(
"<html>"
"</head>"
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
  "<style type=\"text/css\">"
    "body {"
    "-webkit-appearance:none;"
    "-moz-appearance:none;"
    "font-family:'Arial',sans-serif;"
    "text-align:center;"
    "}"
    ".menu > a:link {"
    "position: absolute;"
    "display: inline-block;"
    "right: 12px;"
    "padding: 0 6px;"
    "text-decoration: none;"
    "}"
    ".button {"
    "display:inline-block;"
    "border-radius:7px;"
    "background:#1b5e20;"
    "margin:0 10px 0 10px;"
    "padding:10px 20px 10px 20px;"
    "text-decoration:none;"
    "color:#fff;"
    "}"
  "</style>"
"</head>"
"<body>"
  "<div class=\"menu\">" AUTOCONNECT_LINK(BAR_24) "</div>"
  "<h3>INTERNAL TESTING</h3><br>"
  "<hr>"
  "To set the BME280 I2C Address, use the following buttons.<br>");
  page += String(F("<span style=\"font-weight:normal;color:"));
  page += String("Black\" >Current Address is:");
  page += String(F("</span>"));
  page += String(F("<span style=\"font-weight:bold;color:"));  
  page += (bmeAddress == 0x77) ? String("Red\">Red=0x77") : String("Purple\">Purple=0x76");
  page += String(F("</span>"));
  page += String(F("<p><a class=\"button\" href=\"/dummyResult?v=0x76\">Purple</a><a class=\"button\" href=\"/dummyResult?v=0x77\">Red</a></p>"));
  
  page += String(F("<hr>"));
  page += String(F("<span style=\"font-weight:normal;color:"));
  page += String("Black\" >To set statistics to initial values, use the following button.");
  page += String(F("</span>"));
  page += String(F("<p><a class=\"button\" href=\"/dummyResult?v=0x17\">Init</a></p>"));
  page += String(F("<hr>"));
  page += String(F("</body></html>"));
  Server.send(200, "text/html", page);
}

/// Function to handle the http://[IPAddress]/dummyResult response of Webserver::on(...). This is a dummy url which is response for /dummy.
void handleDummyResult() 
{
  HTTPClient  httpClient;
  WiFiClient  client;
  AutoConnectCredential  ac(CREDENTIAL_OFFSET);
  MY_DBGln("Dummy Test....");
  if (Server.arg("v") == "0x76")
  {
    bmeAddress =0x76;
  } 
  else if (Server.arg("v") == "0x77")
  {
    bmeAddress =0x77;
  }
  else if (Server.arg("v") == "0x17")
  {
    //Set the statistical parameters to initial values
    set_value("user_data", "tom",timeofmeas_sds);
    set_integer("user_data", "tObservation",0);
    set_integer("user_data", "nObservation",0);
    set_integer("user_data", "rCounter",0);
    //set some WiFi explicitly
    //WiFi.begin("WG21", "Pakistan1");//Sajjad Debugging
    //Set the configuration parameters to some test values
    /*set_value("user_data", "address", "Georg-Friedrich-Str. 34");
    set_value("user_data", "longitude", "8.423494");
    set_value("user_data", "latitude", "49.008801");
    set_value("user_data", "rht_sensor", "BME280");//BME280//default
    set_value("user_data", "sds011id", "5002-664f");
    set_value("user_data", "data_owner", "Sajjad");
    set_value("user_data", "data_license", "CC BY 4.0");
    set_value("user_data", "data_url", String(data1_url_ccby));*/    
  }
  // Returns the redirect response.
  WebServer&  webServer = Portal.host();
  webServer.sendHeader("Location", String("http://") + webServer.client().localIP().toString() + String("/"));
  webServer.send(302, "text/plain", "");
  webServer.client().flush();
  webServer.client().stop();

  // RESTART ESP TODO: test this!!!
  delay(2000);
  Serial.println("Rebooting...\n");
  delay(1000);
  ESP.restart();
  
}

/// Function to handle the http://[IPAddress]/config response of Webserver::on(...) after button submission
void handleConfig() 
{
  HTTPClient  httpClient;
  WiFiClient  client;
  AutoConnectCredential  ac(CREDENTIAL_OFFSET);
  //all these values are updates in the eeprom but are re-assigned to global variables after reset
  if (Server.hasArg("inputAddress") && (Server.arg("inputAddress").length() > 0))
  {
    set_value("user_data", "address", Server.arg("inputAddress").c_str());
  }

  if (Server.hasArg("inputLat") && (Server.arg("inputLat").length() > 0))
  {
    set_value("user_data", "latitude", Server.arg("inputLat").c_str());
  }

  if (Server.hasArg("inputLon") && (Server.arg("inputLon").length() > 0))
  {
    set_value("user_data", "longitude", Server.arg("inputLon").c_str());
  }

  if (Server.hasArg("inputRhtsensor"))
  {
    set_value("user_data", "rht_sensor", Server.arg("inputRhtsensor").c_str());
  }

  if (Server.hasArg("inputSds011id"))
  {
    set_value("user_data", "sds011id", Server.arg("inputSds011id").c_str());
  }

  if (Server.hasArg("inputLicense"))
  {
    String cur_license = Server.arg("inputLicense").c_str();

    set_value("user_data", "data_license", Server.arg("inputLicense").c_str());

    if (cur_license == "CC0 1.0") { 
      set_value("user_data", "data_url", String(data1_url_cc0));
    } 
    else {
      set_value("user_data", "data_url", String(data1_url_ccby));
    } 
  }

  if (Server.hasArg("inputOwner"))
  {
    set_value("user_data", "data_owner", Server.arg("inputOwner").c_str());
  }

  // Returns the redirect response.
  WebServer&  webServer = Portal.host();
  webServer.sendHeader("Location", String("http://") + webServer.client().localIP().toString() + String("/"));
  webServer.send(302, "text/plain", "");
  webServer.client().flush();
  webServer.client().stop();

  // RESTART ESP TODO: test this!!!
  delay(2000);
  Serial.println("Rebooting...\n");
  delay(1000);
  ESP.restart();

}

/// Function to handle the http://[IPAddress]/ response of AutoConnect::on(...)
String handleRoot(AutoConnectAux& aux, PageArgument& args) 
{

  String tempLicense = get_value("user_data", "data_license");
  cfgTemp = "<script> \n"
             "  document.addEventListener('DOMContentLoaded', function(){\n"
             //"    document.getElementById(\"inputFW\").value =\"Crowdsensor Konfiguration Firmware: " + String(FIRMWARE_VERSION) + "\";\n"
             "    document.getElementById(\"inputAddress\").value =\"" + get_value("user_data", "address") + "\";\n"
             "    document.getElementById(\"inputLon\").value =\"" + get_value("user_data", "longitude") + "\";\n"
             "    document.getElementById(\"inputLat\").value =\"" + get_value("user_data", "latitude") + "\";\n"
             "    document.getElementById(\"inputRhtsensor\").value =\"" + get_value("user_data", "rht_sensor") + "\";\n"
             "    document.getElementById(\"inputSds011id\").value =\"" + get_value("user_data", "sds011id") + "\";\n"
             "    document.getElementById(\"inputLicense\").value =\"" + ((tempLicense == NULL) ? "CC0 1.0" : tempLicense) + "\";\n"
             "    document.getElementById(\"inputOwner\").value =\"" + get_value("user_data", "data_owner") + "\";\n"
             "  });\n"
             "  window.addEventListener('load', function(){\n"
             "    updateMarker();"
             "  });\n"
             "</script>";

//#ifndef RELEASE
//  Serial.println(cfgTemp);
//#endif
  MY_DBGln(cfgTemp);
  
  AutoConnectText& ac_scriptdiv = aux.getElement<AutoConnectText>("cfgData");
  ac_scriptdiv.value = cfgTemp.c_str();
  return String();  
}

/// Function to handle the http://[IPAddress]/otaUpdate HTTP_POST response of Webserver::on(...)
void otaUpdateHandler(void)
{
  Server.sendHeader("Connection", "close");
  Server.send(200, "text/html", otaUpdate);
  delay(2000);
  Server.sendHeader("Location","/");
}

/// Function to handle the http://[IPAddress]/update HTTP_POST response of Webserver::on(...)
void updateHandler(void)
{
  Server.sendHeader("Connection", "close");
  Server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
  delay(2000);
  ESP.restart();
}

/// Function to handle the http://[IPAddress]/update HTTP_GET response of Webserver::on(...)
void updateHandlerResponse(void)
{
  WebServer* _server = &Server;
  HTTPUpload& upload = _server->upload();
  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("Update: %s\n", upload.filename.c_str());
    if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    // flashing firmware to ESP
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) { //true to set the size to the current progress
      Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      delay(100);
      _server->client().stop();
      ESP.restart();
    } else {
      Update.printError(Serial);
    }
  }
}

//******************************************************** Frost Manager Functions ***************************

/**************************************************************************/
/*!
    @brief  create a HASH using SHA-1
    @param serial String values from where HASH to be calculated
    @returns sha1hex String HASH output
*/
/**************************************************************************/
String toHEXSHA(String serial) 
{
  int serialLength = serial.length();
  char* serialAsChar = new char[serialLength+1];
  serial.toCharArray(serialAsChar, serialLength+1);

  byte shaResult[20]; //sha1 result has always 160bits
  
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA1;        
 
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *) serialAsChar, serialLength);
  mbedtls_md_finish(&ctx, shaResult);
  mbedtls_md_free(&ctx);
 
  //print to HEX characters
  char sha1hex[2*sizeof(shaResult)+1];
  for(int i= 0; i< sizeof(shaResult); i++){
      sprintf(&sha1hex[i*2], "%02x", (int)shaResult[i]);
  }
  delete serialAsChar;
  return String(sha1hex);
}

/**************************************************************************/
/*!
    @brief  Create (HTTP Post) entities 
    @param url String that contains the address of entities to be created/patched
    @param contents String that contains the JSON message for the server
    @returns httpCode int indicating HTTPClient Code Error
*/
/**************************************************************************/
boolean createEntity(String url, String contents) 
{
  HTTPClient http;
  int httpCode;
  
  MY_DBG("[API] URL: ");
  MY_DBGln(url);
  MY_DBGln("[API] JSON_buffer:");
  MY_DBGln(contents);

  //http.setTimeout(9000);//default was 5000ms
  //http.setConnectTimeout(9000);
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  httpCode = http.POST(contents);

  http.end();
  if(httpCode < 0) 
  {
    MY_DBGln("[API] Error on HTTP post");
    MY_DBGln("[API] HTTP Code: "+String(httpCode));  
    
  }
  else 
  {
    MY_DBGln("[API] HTTP Code: "+String(httpCode));
    //MY_DBGln("[API] response: " + response);
  }
  
  if(httpCode != 200 && httpCode != 201) return (false);
  else return (true);

}

/**************************************************************************/
/*!
    @brief  Patch (HTTP Patch) entities 
    @param url String that contains the address of entities to be created/patched
    @param contents String that contains the JSON message for the server
    @returns httpCode int indicating HTTPClient Code Error
*/
/**************************************************************************/
boolean patchEntity(String url, String contents) 
{ 
  HTTPClient http;
  int httpCode;
  
  MY_DBG("[API] URL: ");
  MY_DBGln(url);
  MY_DBGln("[API] JSON_buffer:");
  MY_DBGln(contents);

  //http.setTimeout(9000);//default was 5000ms
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  httpCode = http.PATCH(contents);
  http.end();
  if(httpCode < 0) 
  {
    MY_DBGln("[API] Error on HTTP patch");
    MY_DBGln("[API] HTTP Code "+String(httpCode));
  }
  else 
  {
    MY_DBGln("[API] HTTP Code: "+String(httpCode));
  }

  if(httpCode != 200 && httpCode != 201) return (false);
  else return (true);
}


/**************************************************************************/
/*!
    @brief  Get the HTTP code  (HTTP Get) from a server and print the response
    @param url String that contains the address of entities to be created/patched
    @returns response String indicating HTTPClient Code response
*/
/**************************************************************************/
int getEntity(String url) 
{
  HTTPClient http;
  int httpCode;
 
  MY_DBG("[API] URL: ");
  MY_DBGln(url);

  //http.setTimeout(9000);//default was 5000ms
  http.begin(url);
    
  httpCode = http.GET();
      
  String response = http.getString();
  http.end(); 
  if(httpCode < 0) 
  {
    MY_DBGln("[API] Error on HTTP get");
    MY_DBGln("[API] HTTP Code "+String(httpCode));
  }
  else 
  {
    MY_DBGln("[API] HTTP Code: "+ String(httpCode));
    MY_DBGln("[API] response:\n" + response);
  }

  return (httpCode);
}

/**************************************************************************/
/*!
    @brief  Get the HTTP response  (HTTP Get) from a server and print the response
    @param url String that contains the address of entities to be created/patched
    @returns response String indicating HTTPClient Code response
*/
/**************************************************************************/
String getEntityResponse(String url) 
{
  HTTPClient http;
  int httpCode;
 
  MY_DBG("[API] URL: ");
  MY_DBGln(url);

  //http.setTimeout(9000);//default was 5000ms
  http.begin(url);
    
  httpCode = http.GET();
      
  String response = http.getString();
  http.end();
  if(httpCode < 0) 
  {
    MY_DBGln("[API] Error on HTTP get");
    MY_DBGln("[API] HTTP Code "+String(httpCode));
  }
  else 
  {
    MY_DBGln("[API] HTTP Code: "+ String(httpCode));
    MY_DBGln("[API] response:\n" + response);
  }
  return (response);
}

/**************************************************************************/
/*!
    @brief  Create  (HTTP Post) if entities does exist and patch  (HTTP Patch) if it already exists
    @param url String that contains the address of entities to be created/patched
    @param contents String that contains the JSON message for the server
    @param id String that indicate the iot.id 
    @param doPatch bool if true then create & patch, if false then only create.
    @returns httpCode int indicating HTTPClient Code Error
*/
/**************************************************************************/
boolean createOrUpdateEntities(String url , String contents,String id, boolean doPatch) 
{ 
  boolean httpCode;
  String tempURL = url+"('"+id+"')"; 

  int hCode = getEntity(tempURL);

  if (hCode == 404) //DS does not exist yet
  { 
    MY_DBGln("[API] Creating Entity...");
    httpCode = createEntity(url, contents);
  } 
  else //exist
  {
    if(hCode != 200 && hCode != 201) httpCode = (false);
    else httpCode =  (true);
    if(doPatch)
    {
      MY_DBGln("[API] Patching Entity...");
      httpCode = patchEntity(tempURL, contents);    
    }
  }
  return httpCode;
}

/**************************************************************************/
/*!
    @brief  Create  (HTTP Post) if entities does exist and patch  (HTTP Patch) the properties if it already exists
    @param entity String that contains the address of entities to be created/patched
    @param json String that contains the JSON message for the server
    @param iotId String that indicate the iot.id 
    @returns httpCode boolean indicating HTTPClient Code Error
*/
/**************************************************************************/
boolean createOrPatchEntities(String entity , String json, String iotId)
{
  boolean tempCode;
  
  String entityUrl = String(BASE_URL) + "/"+entity+"('"+iotId+"')";
  
  MY_DBGln("[API] Attempting to retrieve "+entity+" from server...");

  int hCode = getEntity(entityUrl);
      
  if (hCode == 404)  //Entity (i.e. Thing) does not exist yet
  {
   MY_DBGln("[API] "+entity+" does not exist yet=>Create it...");
   
   tempCode = createEntity(String(BASE_URL) + "/"+entity, json);
   if(!tempCode) return (false);
  } 
  else // Entity (i.e. Thing) exists
  {
    //Check if the Properties are identical 
    //Get the properties
    String propUrl = entityUrl+"/properties";
    int hCode = getEntity(propUrl);
    MY_DBGln("[API] "+entity+" exist=>Check properties...");
    if (hCode == 204) // Entity exists, but properties does not exist on server
    { 
      MY_DBGln("[API] Pending Properties not yet on server=> Patch it...");
      tempCode = patchEntity(entityUrl, prJson);
      if(!tempCode) return (false);
    }
    else if  (hCode == 200 || hCode == 201 )
    {
      //Checking the properties i.e. software.version
      String propResponse = getEntityResponse(propUrl);
      DynamicJsonDocument doc(1024);
      Serial.println(propResponse);
      deserializeJson(doc, propResponse);
      JsonObject obj1 = doc.as<JsonObject>();
      JsonObject obj2 = obj1["properties"]["software.version"];
      //Iterate through JSON Pair
      String keyField,valueField, preKey, recent1, recent2;
      for (JsonPair pair : obj2)
      {
        keyField = pair.key().c_str();        //Date key
        valueField = pair.value().as<char*>(); //Version key
        //find the recent TimeStamp
        if(keyField > preKey)
        {
          recent1 = keyField;//Recent Date
          recent2 = valueField;//Recent Version
        } 
        preKey = keyField;//Swap Date
      }
      //Update the software version properties based on the above comparision
      if (thSwVersion == recent2) //identical
      {
        MY_DBGln("[API] Properties \"Software.version\" are indentical...");
      }
      else
      {
        MY_DBGln("[API] Properties \"Software.version\" are not indentical...");
        thSwDate = getIsoTime();//update the timestampe
        obj2[thSwDate] = thSwVersion;//update the software version properties
        String propUpdated;
        serializeJson(doc, propUpdated);
        tempCode = patchEntity(entityUrl, propUpdated);
        if(!tempCode) return (false);
      }
      
      //Checking the properties i.e. license for the datastream etities
      if (entity=="Datastreams")
      {
        obj1 = doc.as<JsonObject>();
        obj2 = obj1["properties"]["license"];
        //Iterate through JSON Pair
        for (JsonPair pair : obj2)
        {
          keyField = pair.key().c_str();        //first field of the pair
          valueField = pair.value().as<char*>(); //Version key
          //find the any pair value is equal to license owner or type
          if(keyField == "legal_notice")
            recent1 = valueField;// assign recent pair value i.e. "legal_notice"
          if(keyField == "name")
            recent2 = valueField;// assign recent pair value i.e. "name"
        }
        //Update the license properties based on the above comparision
        if (recent1 == license_owner && recent2 == license_type) //identical
        {
          MY_DBGln("[API] Properties \"License\" are indentical...");
        }
        else
        {
          MY_DBGln("[API] Properties \"License\" are not indentical...");

          String propUpdated ="";
          obj2["name"] = license_type;//update the license properties
          obj2["legal_notice"] = license_owner;
          if (license_type == "CC0 1.0") 
            license_url = data1_url_cc0;
          else
            license_url = data1_url_ccby;
          obj2["url"] = license_url;
          serializeJson(doc, propUpdated);
          tempCode = patchEntity(entityUrl, propUpdated);
          if(!tempCode) return (false);
        }
      }
    }
  }
}

/**************************************************************************/
/*!
    @brief  create different entities like Thing, Sensor, Datastream, ObservedProperties, & Location and Patch if not already linked
    @returns created bool variable to indicated if the all the entities are seccessfully created
*/
/**************************************************************************/
bool createEntities() 
{
  String tempId;
  boolean tempCode;
  
  //SAQN Database Modeling: Firmware & License
  licType = license_type;
  licOwner = license_owner;
  licUrl = license_url;
  
  //Location
  //--------
  //SAQN Database Modeling: Location
  locName = humanReadableLocation;
  locDesc = "no description";
  locEnc = "application/vnd.geo+json";
  locTyp = "Point";
  locIotId = "geo:" + String(latitude,precision)+","+String(longitude,precision);
  locCord = "[" + String(longitude,precision)+"," + String(latitude,precision)+"]";
  locJson = "{\"name\":\""+locName+"\",\"description\":\""+locDesc+"\",\"encodingType\":\""+locEnc+"\",\"location\":{\"type\":\""+locTyp+"\",\"coordinates\":"+locCord+"},\"@iot.id\":\""+locIotId+"\"}";
  //MY_DBGln(locJson);
  
  //THING
  //--------
  //SAQN Database Modeling: Thing
  thName = "Crowdsensing "+ device_Serial;
  thDesc = "A Low Cost Node Measuring Particulate Matter";
  thHwDate = hw_date;//getIsoTime();
  thHwVersion = "https://www.teco.edu/wp-content/grand-media/application/Anleitung_Crowdsensor_V085b.pdf";
  thSwDate = getIsoTime();
  thSwVersion = "v"+String(FIRMWARE_VERSION);
  thHwId = device_Serial;
  thShortName = "crowdsensor";
  thOpDomain = "teco.edu";
  thId = thOpDomain+":"+thShortName+":"+device_Serial;
  thIotId = "saqn:t:"+thId;
  thJson = "{\"name\":\"" + thName + "\",\"description\":\""+thDesc+"\",\"properties\":{\"software.version\":{\"" + thSwDate + "\":\"" + thSwVersion + "\"},\"hardware.revision\":{\"" + thHwDate + "\":\"" + thHwVersion + "\"},\"hardware.id\": \""+thHwId+"\",\"shortname\": \""+thShortName+"\",\"operator.domain\": \""+thOpDomain+"\"},\"@iot.id\":\""+thIotId+"\"}";
  prJson = "{\"properties\":{\"software.version\":{\"" + thSwDate + "\":\"" + thSwVersion + "\"},\"hardware.revision\":{\"" + thHwDate + "\":\"" + thHwVersion + "\"},\"hardware.id\": \""+thHwId+"\",\"shortname\": \""+thShortName+"\",\"operator.domain\": \""+thOpDomain+"\"}}";
  //MY_DBGln(thJson);
  
  //SENSORS
  //--------
  //SAQN Database Modeling: Sensor SDS011
  sdsSerial = SDS011_Serial;
  sdsName = "Nova SDS011";
  sdsDesc = "A type of low-cost sensor measuring particulate matter";
  sdsEnc = "application/json";
  sdsDataUrl =   "https://web.archive.org/web/20191111144451/https://www.watterott.com/media/files_public/fwyjbmbnj/SDS01.pdf";
  sdsShortName = "sds011";
  sdsManDomain = "inovafitness.com";
  sdsId = sdsManDomain+":"+sdsShortName;
  sdsIotId = "saqn:s:"+sdsId;
  sdsMeta = String(BASE_URL) + "/Sensors"+"('"+sdsIotId+"')/properties";
  sdsJson = "{\"name\":\""+sdsName+"\",\"description\":\""+sdsDesc+"\",\"encodingType\":\""+sdsEnc+"\",\"metadata\":\""+sdsMeta+"\",\"properties\":{\"datasheet.url\": \""+sdsDataUrl+"\",\"shortname\": \""+sdsShortName+"\", \"manufacturer.domain\": \""+sdsManDomain+"\"},\"@iot.id\":\""+sdsIotId+"\"}";
  //MY_DBGln(sdsJson);

  //SAQN Database Modeling: Sensor BME280
  bmeSerial = "";
  bmeName = "Bosch BME280";
  bmeDesc = "A sensor measuring temperature, atmospheric pressure and humidity";
  bmeEnc = "application/pdf";
  bmeDataUrl =   "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280-DS002.pdf";
  bmeShortName = "bme280";
  bmeManDomain = "bosch-sensortec.com";
  bmeId = bmeManDomain+":"+bmeShortName;
  bmeIotId = "saqn:s:"+bmeId;
  bmeMeta = String(BASE_URL) + "/Sensors"+"('"+bmeIotId+"')/properties";
  bmeJson = "{\"name\":\""+bmeName+"\",\"description\":\""+bmeDesc+"\",\"encodingType\":\""+bmeEnc+"\",\"metadata\":\""+bmeMeta+"\",\"properties\":{\"datasheet.url\": \""+bmeDataUrl+"\",\"shortname\": \""+bmeShortName+"\", \"manufacturer.domain\": \""+bmeManDomain+"\"},\"@iot.id\":\""+bmeIotId+"\"}";
  //MY_DBGln(bmeJson);

  //SAQN Database Modeling: Sensor DHT22
  dhtSerial = "";
  dhtName = "DHT22";
  dhtDesc = "A sensor measuring temperature and humidity";
  dhtEnc = "application/pdf";
  dhtDataUrl = "https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf";
  dhtShortName = "dht22";
  dhtManDomain = "adafruit.com";
  dhtId = dhtManDomain+":"+dhtShortName;
  dhtIotId = "saqn:s:"+dhtId;
  dhtMeta = String(BASE_URL) + "/Sensors"+"('"+dhtIotId+"')/properties";
  dhtJson = "{\"name\":\""+dhtName+"\",\"description\":\""+dhtDesc+"\",\"encodingType\":\""+dhtEnc+"\",\"metadata\":\""+dhtMeta+"\",\"properties\":{\"datasheet.url\": \""+dhtDataUrl+"\",\"shortname\": \""+dhtShortName+"\", \"manufacturer.domain\": \""+dhtManDomain+"\"},\"@iot.id\":\""+dhtIotId+"\"}";
  //MY_DBGln(dhtJson);
  
  //OBSERVER PROPERTIES
  //-------------------
  //SAQN Database Modeling: ObservedProperty PM10
  opPM10Name = "PM10 Mass Concetration";
  opPM10Desc = "Mass concentration of Particulate Matter with a diameter of equal or less than 10 micrometers in air.";
  opPM10Defi = "http://cfconventions.org/Data/cf-standard-names/63/build/cf-standard-name-table.html#mass_concentration_of_pm10_ambient_aerosol_particles_in_air";
  opPM10ShortDefi = "http://www.uc2-program.org/uc2_table_A1.pdf";
  opPM10ShortName = "mcpm10";
  opPM10UoM = "microgram per cubic meter";
  opPM10Sym = "ug/m3";
  opPM10ConDef = "http://unitsofmeasure.org/ucum.html";
  opPM10Id = opPM10ShortName;
  opPM10IotId = "saqn:op:"+opPM10Id;
  opPM10Json = "{\"name\": \""+opPM10Name+"\",\"description\": \""+opPM10Desc+"\",\"definition\": \""+opPM10Defi+"\",\"properties\": {\"conventions\": {\"unitOfMeasurement\": {\"name\": \""+opPM10UoM+"\",\"symbol\": \""+opPM10Sym+"\",\"definition\": \""+opPM10ConDef+"\"},\"fixedPoints\": {\"0\": \"#0f8a0f\",\"5\": \"#2db00c\",\"10\": \"#59d408\",\"15\": \"#8fec04\",\"20\": \"#c7f901\",\"25\": \"#ffff00\",\"30\": \"#f9c701\",\"40\": \"#ec8f04\",\"50\": \"#d45908\",\"100\": \"#b02d0c\",\"2147483647\": \"#8a0f0f\"}},\"shortname\": \""+opPM10ShortName+"\",\"shortname.definition\": \""+opPM10ShortDefi+"\"},\"@iot.id\": \""+opPM10IotId+"\"}";
  //MY_DBGln(opPM10Json);

  //SAQN Database Modeling: ObservedProperty PM2_5
  opPM25Name = "PM2.5 Mass Concetration";
  opPM25Desc = "Mass concentration of Particulate Matter with a diameter of equal or less than 2.5 micrometers in air.";
  opPM25Defi = "http://cfconventions.org/Data/cf-standard-names/63/build/cf-standard-name-table.html#mass_concentration_of_pm2p5_ambient_aerosol_particles_in_air";
  opPM25ShortDefi = "http://www.uc2-program.org/uc2_table_A1.pdf";
  opPM25ShortName = "mcpm2p5";
  opPM25UoM = "microgram per cubic meter";
  opPM25Sym = "ug/m3";
  opPM25ConDef = "http://unitsofmeasure.org/ucum.html";
  opPM25Id = opPM25ShortName;
  opPM25IotId = "saqn:op:"+opPM25Id;
  opPM25Json = "{\"name\": \""+opPM25Name+"\",\"description\": \""+opPM25Desc+"\",\"definition\": \""+opPM25Defi+"\",\"properties\": {\"conventions\": {\"unitOfMeasurement\": {\"name\": \""+opPM25UoM+"\",\"symbol\": \""+opPM25Sym+"\",\"definition\": \""+opPM25ConDef+"\"},\"fixedPoints\": {\"0\": \"#0f8a0f\",\"5\": \"#2db00c\",\"10\": \"#59d408\",\"15\": \"#8fec04\",\"20\": \"#c7f901\",\"25\": \"#ffff00\",\"30\": \"#f9c701\",\"40\": \"#ec8f04\",\"50\": \"#d45908\",\"100\": \"#b02d0c\",\"2147483647\": \"#8a0f0f\"}},\"shortname\": \""+opPM25ShortName+"\",\"shortname.definition\": \""+opPM25ShortDefi+"\"},\"@iot.id\": \""+opPM25IotId+"\"}";
  //MY_DBGln(opPM25Json);

  //SAQN Database Modeling: BME280/DHT22 ObservedProperty Humidity
  opHmName = "Relative Humidity";
  opHmDesc = "Relative humidity is the ratio of the partial pressure of water vapor to the equilibrium vapor pressure of water at a given temperature.";
  opHmDefi = "http://cfconventions.org/Data/cf-standard-names/63/build/cf-standard-name-table.html#relative_humidity";
  opHmShortDefi = "http://www.uc2-program.org/uc2_table_A1.pdf";
  opHmShortName = "hur";
  opHmUoM = "percent";
  opHmSym = "%";
  opHmConDef = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#Percent";
  opHmId = opHmShortName;
  opHmIotId = "saqn:op:"+opHmId;
  opHmJson = "{\"name\": \""+opHmName+"\",\"description\": \""+opHmDesc+"\",\"definition\": \""+opHmDefi+"\",\"properties\": {\"conventions\": {\"unitOfMeasurement\": {\"name\": \""+opHmUoM+"\",\"symbol\": \""+opHmSym+"\",\"definition\": \""+opHmConDef+"\"},\"fixedPoints\": {\"0\": \"#0f8a0f\",\"5\": \"#2db00c\",\"10\": \"#59d408\",\"15\": \"#8fec04\",\"20\": \"#c7f901\",\"25\": \"#ffff00\",\"30\": \"#f9c701\",\"40\": \"#ec8f04\",\"50\": \"#d45908\",\"100\": \"#b02d0c\",\"2147483647\": \"#8a0f0f\"}},\"shortname\": \""+opHmShortName+"\",\"shortname.definition\": \""+opHmShortDefi+"\"},\"@iot.id\": \""+opHmIotId+"\"}";
  //MY_DBGln(opHmJson);

  //SAQN Database Modeling: BME280/DHT22 ObservedProperty Temperature
  opTmName = "Air Temperature";
  opTmDesc = "Air temperature is the bulk temperature of the air, not the surface (skin) temperature.";
  opTmDefi = "http://cfconventions.org/Data/cf-standard-names/63/build/cf-standard-name-table.html#air_temperature";
  opTmShortDefi = "http://www.uc2-program.org/uc2_table_A1.pdf";
  opTmShortName = "ta";
  opTmUoM = "Degree Celsius";
  opTmSym = "degC";
  opTmConDef = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#DegreeCelsius";
  opTmId = opTmShortName;
  opTmIotId = "saqn:op:"+opTmId;
  opTmJson = "{\"name\": \""+opTmName+"\",\"description\": \""+opTmDesc+"\",\"definition\": \""+opTmDefi+"\",\"properties\": {\"conventions\": {\"unitOfMeasurement\": {\"name\": \""+opTmUoM+"\",\"symbol\": \""+opTmSym+"\",\"definition\": \""+opTmConDef+"\"},\"fixedPoints\": {\"0\": \"#0f8a0f\",\"5\": \"#2db00c\",\"10\": \"#59d408\",\"15\": \"#8fec04\",\"20\": \"#c7f901\",\"25\": \"#ffff00\",\"30\": \"#f9c701\",\"40\": \"#ec8f04\",\"50\": \"#d45908\",\"100\": \"#b02d0c\",\"2147483647\": \"#8a0f0f\"}},\"shortname\": \""+opTmShortName+"\",\"shortname.definition\": \""+opTmShortDefi+"\"},\"@iot.id\": \""+opTmIotId+"\"}";
  //MY_DBGln(opTmJson);

  //SAQN Database Modeling: BME280 ObservedProperty Pressure
  opPrName = "Air Pressure";
  opPrDesc = "Air pressure is the force per unit area which would be exerted when the moving gas molecules of which the air is composed strike a theoretical surface of any orientation.";
  opPrDefi = "http://cfconventions.org/Data/cf-standard-names/63/build/cf-standard-name-table.html#air_pressure";
  opPrShortDefi = "http://www.uc2-program.org/uc2_table_A1.pdf";
  opPrShortName = "plev";
  opPrUoM = "Hectopascal";
  opPrSym = "hPa";
  opPrConDef = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#Pascal";
  opPrId = opPrShortName;
  opPrIotId = "saqn:op:"+opPrId;
  opPrJson = "{\"name\": \""+opPrName+"\",\"description\": \""+opPrDesc+"\",\"definition\": \""+opPrDefi+"\",\"properties\": {\"conventions\": {\"unitOfMeasurement\": {\"name\": \""+opPrUoM+"\",\"symbol\": \""+opPrSym+"\",\"definition\": \""+opPrConDef+"\"},\"fixedPoints\": {\"0\": \"#0f8a0f\",\"5\": \"#2db00c\",\"10\": \"#59d408\",\"15\": \"#8fec04\",\"20\": \"#c7f901\",\"25\": \"#ffff00\",\"30\": \"#f9c701\",\"40\": \"#ec8f04\",\"50\": \"#d45908\",\"100\": \"#b02d0c\",\"2147483647\": \"#8a0f0f\"}},\"shortname\": \""+opPrShortName+"\",\"shortname.definition\": \""+opPrShortDefi+"\"},\"@iot.id\": \""+opPrIotId+"\"}";
  //MY_DBGln(opPrJson);


  // DATASTREAMS
  //------------
  
  //SAQN Database Modeling: Datastream PM10
  dsPM10Name = "PM10 Datastream of Crowdsensing Node (SDS011, "+device_Serial+")";
  dsPM10Desc = "Datastream for recording Particulate Matter";
  dsPM10ObType = "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement";
  dsPM10UoMName = "microgram per cubic meter";
  dsPM10UoMSym = "ug/m^3";
  dsPM10UoMDef = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#KilogramPerCubicMeter";
  dsPM10Id = thId+":"+sdsId+":"+sdsSerial+":"+opPM10Id;
  dsPM10IotId = "saqn:ds:"+toHEXSHA(dsPM10Id).substring(0,7);
  dsPM10Json = "{\"name\":\""+dsPM10Name+"\",\"description\":\""+dsPM10Desc+"\",\"observationType\":\""+dsPM10ObType+"\",\"unitOfMeasurement\":{\"name\":\""+dsPM10UoMName+"\",\"symbol\":\""+dsPM10UoMSym+"\",\"definition\":\""+dsPM10UoMDef+"\"},\"properties\":{\"software.version\":{\"" + thSwDate + "\":\"" + thSwVersion + "\"},\"hardware.serial_number\": \""+sdsSerial+"\",\"operator.domain\": \""+thOpDomain+"\",\"license\":{\"name\":\"" + licType + "\",\"legal_notice\":\"" + licOwner + "\",\"url\":\"" + licUrl + "\"}},\"Thing\":{\"@iot.id\":\""+thIotId+"\"},\"Sensor\":{\"@iot.id\":\""+sdsIotId+"\"},\"ObservedProperty\":{\"@iot.id\":\""+opPM10IotId+"\"},\"@iot.id\":\""+dsPM10IotId+"\"}";
  //MY_DBGln(dsPM10Json);
  
  //SAQN Database Modeling: Datastream PM2_5
  dsPM25Name = "PM2.5 Datastream of Crowdsensing Node (SDS011, "+device_Serial+")";
  dsPM25Desc = "Datastream for recording Particulate Matter";
  dsPM25ObType = "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement";
  dsPM25UoMName = "microgram per cubic meter";
  dsPM25UoMSym = "ug/m^3";
  dsPM25UoMDef = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#KilogramPerCubicMeter";
  dsPM25Id = thId+":"+sdsId+":"+sdsSerial+":"+opPM25Id;
  dsPM25IotId = "saqn:ds:"+toHEXSHA(dsPM25Id).substring(0,7);
  dsPM25Json = "{\"name\":\""+dsPM25Name+"\",\"description\":\""+dsPM25Desc+"\",\"observationType\":\""+dsPM25ObType+"\",\"unitOfMeasurement\":{\"name\":\""+dsPM25UoMName+"\",\"symbol\":\""+dsPM25UoMSym+"\",\"definition\":\""+dsPM25UoMDef+"\"},\"properties\":{\"software.version\":{\"" + thSwDate + "\":\"" + thSwVersion + "\"},\"hardware.serial_number\": \""+sdsSerial+"\",\"operator.domain\": \""+thOpDomain+"\",\"license\":{\"name\":\"" + licType + "\",\"legal_notice\":\"" + licOwner + "\",\"url\":\"" + licUrl + "\"}},\"Thing\":{\"@iot.id\":\""+thIotId+"\"},\"Sensor\":{\"@iot.id\":\""+sdsIotId+"\"},\"ObservedProperty\":{\"@iot.id\":\""+opPM25IotId+"\"},\"@iot.id\":\""+dsPM25IotId+"\"}";
  //MY_DBGln(dsPM25Json);
    
  //SAQN Database Modeling: BME280 Datastream Humidity
  dsHmBmeName = "Relative Humidity Datastream of Crowdsensing Node (BME280, "+device_Serial+")";
  dsHmBmeDesc = "Datastream for recording relative humidity";
  dsHmBmeObType = "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement";
  dsHmBmeUoMName = "percent";
  dsHmBmeUoMSym = "%";
  dsHmBmeUoMDef = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#Percent";
  dsHmBmeId = thId+":"+bmeId+":"+bmeSerial+":"+opHmId;
  dsHmBmeIotId = "saqn:ds:"+toHEXSHA(dsHmBmeId).substring(0,7);
  dsHmBmeJson = "{\"name\":\""+dsHmBmeName+"\",\"description\":\""+dsHmBmeDesc+"\",\"observationType\":\""+dsHmBmeObType+"\",\"unitOfMeasurement\":{\"name\":\""+dsHmBmeUoMName+"\",\"symbol\":\""+dsHmBmeUoMSym+"\",\"definition\":\""+dsHmBmeUoMDef+"\"},\"properties\":{\"software.version\":{\"" + thSwDate + "\":\"" + thSwVersion + "\"},\"hardware.serial_number\": \""+bmeSerial+"\",\"operator.domain\": \""+thOpDomain+"\",\"license\":{\"name\":\"" + licType + "\",\"legal_notice\":\"" + licOwner + "\",\"url\":\"" + licUrl + "\"}},\"Thing\":{\"@iot.id\":\""+thIotId+"\"},\"Sensor\":{\"@iot.id\":\""+bmeIotId+"\"},\"ObservedProperty\":{\"@iot.id\":\""+opHmIotId+"\"},\"@iot.id\":\""+dsHmBmeIotId+"\"}";
  //MY_DBGln(dsHmBmeJson);
  
  //SAQN Database Modeling: BME280 Datastream Temperature
  dsTmBmeName = "Air Temperature Datastream of Crowdsensing Node (BME280, "+device_Serial+")";
  dsTmBmeDesc = "Datastream for recording temperature";
  dsTmBmeObType = "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement";
  dsTmBmeUoMName = "Degree Celsius";
  dsTmBmeUoMSym = "degC";
  dsTmBmeUoMDef = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#DegreeCelsius";
  dsTmBmeId = thId+":"+bmeId+":"+bmeSerial+":"+opTmId;
  dsTmBmeIotId = "saqn:ds:"+toHEXSHA(dsTmBmeId).substring(0,7);
  dsTmBmeJson = "{\"name\":\""+dsTmBmeName+"\",\"description\":\""+dsTmBmeDesc+"\",\"observationType\":\""+dsTmBmeObType+"\",\"unitOfMeasurement\":{\"name\":\""+dsTmBmeUoMName+"\",\"symbol\":\""+dsTmBmeUoMSym+"\",\"definition\":\""+dsTmBmeUoMDef+"\"},\"properties\":{\"software.version\":{\"" + thSwDate + "\":\"" + thSwVersion + "\"},\"hardware.serial_number\": \""+bmeSerial+"\",\"operator.domain\": \""+thOpDomain+"\",\"license\":{\"name\":\"" + licType + "\",\"legal_notice\":\"" + licOwner + "\",\"url\":\"" + licUrl + "\"}},\"Thing\":{\"@iot.id\":\""+thIotId+"\"},\"Sensor\":{\"@iot.id\":\""+bmeIotId+"\"},\"ObservedProperty\":{\"@iot.id\":\""+opTmIotId+"\"},\"@iot.id\":\""+dsTmBmeIotId+"\"}";
  //MY_DBGln(dsTmBmeJson);
  
  //SAQN Database Modeling: BME280 Datastream Pressure
  dsPrBmeName = "Atmospheric Pressure Datastream of Crowdsensing Node (BME280, "+device_Serial+")";
  dsPrBmeDesc = "Datastream for recording pressure";
  dsPrBmeObType = "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement";
  dsPrBmeUoMName = "Hectopascal";
  dsPrBmeUoMSym = "hPa";
  dsPrBmeUoMDef = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#Pascal";
  dsPrBmeId = thId+":"+bmeId+":"+bmeSerial+":"+opPrId;
  dsPrBmeIotId = "saqn:ds:"+toHEXSHA(dsPrBmeId).substring(0,7);
  dsPrBmeJson = "{\"name\":\""+dsPrBmeName+"\",\"description\":\""+dsPrBmeDesc+"\",\"observationType\":\""+dsPrBmeObType+"\",\"unitOfMeasurement\":{\"name\":\""+dsPrBmeUoMName+"\",\"symbol\":\""+dsPrBmeUoMSym+"\",\"definition\":\""+dsPrBmeUoMDef+"\"},\"properties\":{\"software.version\":{\"" + thSwDate + "\":\"" + thSwVersion + "\"},\"hardware.serial_number\": \""+bmeSerial+"\",\"operator.domain\": \""+thOpDomain+"\",\"license\":{\"name\":\"" + licType + "\",\"legal_notice\":\"" + licOwner + "\",\"url\":\"" + licUrl + "\"}},\"Thing\":{\"@iot.id\":\""+thIotId+"\"},\"Sensor\":{\"@iot.id\":\""+bmeIotId+"\"},\"ObservedProperty\":{\"@iot.id\":\""+opPrIotId+"\"},\"@iot.id\":\""+dsPrBmeIotId+"\"}";
  //MY_DBGln(dsPrBmeJson);

  //SAQN Database Modeling: DHT22 Datastream Humidity
  dsHmDhtName = "Relative Humidity Datastream of Crowdsensing Node (DHT22, "+device_Serial+")";
  dsHmDhtDesc = "Datastream for recording relative humidity";
  dsHmDhtObType = "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement";
  dsHmDhtUoMName = "percent";
  dsHmDhtUoMSym = "%";
  dsHmDhtUoMDef = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#Percent";
  dsHmDhtId = thId+":"+dhtId+":"+dhtSerial+":"+opHmId;
  dsHmDhtIotId = "saqn:ds:"+toHEXSHA(dsHmDhtId).substring(0,7);
  dsHmDhtJson = "{\"name\":\""+dsHmDhtName+"\",\"description\":\""+dsHmDhtDesc+"\",\"observationType\":\""+dsHmDhtObType+"\",\"unitOfMeasurement\":{\"name\":\""+dsHmDhtUoMName+"\",\"symbol\":\""+dsHmDhtUoMSym+"\",\"definition\":\""+dsHmDhtUoMDef+"\"},\"properties\":{\"software.version\":{\"" + thSwDate + "\":\"" + thSwVersion + "\"},\"hardware.serial_number\": \""+dhtSerial+"\",\"operator.domain\": \""+thOpDomain+"\",\"license\":{\"name\":\"" + licType + "\",\"legal_notice\":\"" + licOwner + "\",\"url\":\"" + licUrl + "\"}},\"Thing\":{\"@iot.id\":\""+thIotId+"\"},\"Sensor\":{\"@iot.id\":\""+dhtIotId+"\"},\"ObservedProperty\":{\"@iot.id\":\""+opHmIotId+"\"},\"@iot.id\":\""+dsHmDhtIotId+"\"}";
  //MY_DBGln(dsHmDhtJson);
  
  //SAQN Database Modeling: DHT22 Datastream Temperature
  dsTmDhtName = "Air Temperature Datastream of Crowdsensing Node (DHT22, "+device_Serial+")";
  dsTmDhtDesc = "Datastream for recording temperature";
  dsTmDhtObType = "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement";
  dsTmDhtUoMName = "Degree Celsius";
  dsTmDhtUoMSym = "degC";
  dsTmDhtUoMDef = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#DegreeCelsius";
  dsTmDhtId = thId+":"+dhtId+":"+dhtSerial+":"+opTmId;
  dsTmDhtIotId = "saqn:ds:"+toHEXSHA(dsTmDhtId).substring(0,7);
  dsTmDhtJson = "{\"name\":\""+dsTmDhtName+"\",\"description\":\""+dsTmDhtDesc+"\",\"observationType\":\""+dsTmDhtObType+"\",\"unitOfMeasurement\":{\"name\":\""+dsTmDhtUoMName+"\",\"symbol\":\""+dsTmDhtUoMSym+"\",\"definition\":\""+dsTmDhtUoMDef+"\"},\"properties\":{\"software.version\":{\"" + thSwDate + "\":\"" + thSwVersion + "\"},\"hardware.serial_number\": \""+dhtSerial+"\",\"operator.domain\": \""+thOpDomain+"\",\"license\":{\"name\":\"" + licType + "\",\"legal_notice\":\"" + licOwner + "\",\"url\":\"" + licUrl + "\"}},\"Thing\":{\"@iot.id\":\""+thIotId+"\"},\"Sensor\":{\"@iot.id\":\""+dhtIotId+"\"},\"ObservedProperty\":{\"@iot.id\":\""+opTmIotId+"\"},\"@iot.id\":\""+dsTmDhtIotId+"\"}";
  //MY_DBGln(dsTmDhtJson);
  
  
  MY_DBGln("[API] Start Create Entities...");
  MY_DBGln("[API] Things Create Or Update Entity...\n-------------------------------------------");
  String thingUrl = String(BASE_URL) + "/Things('"+thIotId+"')";
  
  tempCode = createOrPatchEntities("Things", thJson, thIotId);
  
  MY_DBGln("[API] Attempting to check pending Locations on server...");
      
  String locationUrl = String(BASE_URL) + "/Locations('"+ locIotId + "')";
      
  int locResponse = getEntity(locationUrl);
  //Patch the Thing with the properties
  String thJsonPatch = thJson.substring(0,thJson.length()-1)+",\"Locations\": [{\"@iot.id\":\""+locIotId+"\"}]}";

  if (locResponse == 404) // New location does not exist on server
  { 
    MY_DBGln("[API] Pending location not yet on server=>Create Pending Location & Patch...");
    
    tempCode = createEntity(String(BASE_URL) + "/Locations", locJson);
    if(!tempCode) return (false);
    
    MY_DBGln("[API] Patching Thing with Location...");

    tempCode = patchEntity(thingUrl, thJsonPatch);
    if(!tempCode) return (false);
  }
  else // New location also already exists on server
  { 
    //check if new location is linked from thing

    MY_DBGln("[API] Check if thing points to pending location...");

    String thingLocationQueryURL = String(BASE_URL) + "/Things('"+thIotId+"')/Locations"+"?$filter=@iot.id"+"%20"+ "eq"+"%20"+"'"+locIotId+"'";

    String thingLocationQueryResult = getEntityResponse(thingLocationQueryURL);
   
    //determine size of string if < 50 location is not pendingLocation in thing  
    MY_DBG("[API] thingLocationQueryResult.length: ");
    MY_DBGln(thingLocationQueryResult.length());

    if (thingLocationQueryResult.length()<50)
    {
      MY_DBGln("[API] Pending Location not found in Thing=>Patching Thing...");
      tempCode = patchEntity(thingUrl, thJsonPatch);
      if(!tempCode) return (false);
    }
    else // Current location in server is already correctly set
    { 
      //do nothing
      MY_DBGln("[API] Data already up-to-date.");
    } 
  }
  
  MY_DBGln("[API] SDS011 Sensor Create Or Update Entity...\n-------------------------------------------");
  tempCode = createOrUpdateEntities(String(BASE_URL) + "/Sensors", sdsJson,sdsIotId, false);
  if(!tempCode) return (false);
  
  MY_DBGln("[API] ObservedProperty PM2_5 Create Or Update Entity...\n-------------------------------------------");
  tempCode = createOrUpdateEntities(String(BASE_URL) + "/ObservedProperties", opPM25Json, opPM25IotId, false);
  if(!tempCode) return (false);
  
  MY_DBGln("[API] ObservedProperty PM10 Create Or Update Entity...\n-------------------------------------------");
  tempCode = createOrUpdateEntities(String(BASE_URL) + "/ObservedProperties", opPM10Json, opPM10IotId, false);
  if(!tempCode) return (false);
  
  MY_DBGln("[API] ObservedProperty Humidity Create Or Update Entity...\n-------------------------------------------");
  tempCode = createOrUpdateEntities(String(BASE_URL) + "/ObservedProperties", opHmJson,opHmIotId, false);
  if(!tempCode) return (false);
  
  MY_DBGln("[API] ObservedProperty Temperature Create Or Update Entity...\n-------------------------------------------");
  tempCode = createOrUpdateEntities(String(BASE_URL) + "/ObservedProperties", opTmJson, opTmIotId, false);
  if(!tempCode) return (false);

  
  MY_DBGln("[API] Datastream PM2_5 Create Or Update Datastream...\n-------------------------------------------");
  tempCode = createOrPatchEntities("Datastreams", dsPM25Json, dsPM25IotId);
  if(!tempCode) return (false);

  MY_DBGln("[API] Datastream PM10 create Or Update Datastream...\n-------------------------------------------");
  tempCode = createOrPatchEntities("Datastreams", dsPM10Json, dsPM10IotId);
  if(!tempCode) return (false);
  
  //Distinguish between DHT22, BME280 or no Sensors
  if(rhtSensor == "BME280") 
  {
    MY_DBGln("[API] BME280 Sensor Create Or Update Entity...\n-------------------------------------------");
    tempCode = createOrUpdateEntities(String(BASE_URL) + "/Sensors", bmeJson, bmeIotId, false);
    if(!tempCode) return (false);
  
    MY_DBGln("[API] ObservedProperty Pressure Create Or Update Entity...\n-------------------------------------------");
    tempCode = createOrUpdateEntities(String(BASE_URL) + "/ObservedProperties",opPrJson,opPrIotId, false);
    if(!tempCode) return (false);
    
    MY_DBGln("[API] Datastream Humidity BME280 Create Or Update Datastream...\n-------------------------------------------");
    tempCode = createOrPatchEntities("Datastreams", dsHmBmeJson, dsHmBmeIotId);
    if(!tempCode) return (false);
    
    MY_DBGln("[API] Datastream Pressure BME280 create Or Update Datastream...\n-------------------------------------------");
    tempCode = createOrPatchEntities("Datastreams", dsPrBmeJson, dsPrBmeIotId);
    if(!tempCode) return (false);

    MY_DBGln("[API] Datastream Temperature BME280 Create Or Update Datastream...\n-------------------------------------------");
    tempCode = createOrPatchEntities("Datastreams", dsTmBmeJson, dsTmBmeIotId);
    if(!tempCode) return (false);
  }
  else if(rhtSensor == "DHT22") 
  {
    MY_DBGln("[API] DHT22 Sensor Create Or Update Entity...\n-------------------------------------------");
    tempCode = createOrUpdateEntities(String(BASE_URL) + "/Sensors", dhtJson, dhtIotId, false);
    if(!tempCode) return (false);
  
    MY_DBGln("[API] Datastream Humidity DHT22 Create Or Update Datastream...\n-------------------------------------------");
    tempCode = createOrPatchEntities("Datastreams", dsHmDhtJson, dsHmDhtIotId);
    if(!tempCode) return (false);

    MY_DBGln("[API] Datastream Temperature DHT22 Create Or Update Datastream...\n-------------------------------------------");
    tempCode = createOrPatchEntities("Datastreams", dsTmDhtJson, dsTmDhtIotId);
    if(!tempCode) return (false);
    
  }
  else if(rhtSensor == "") 
  {
    //nothing sensor
  }
  
  MY_DBGln("[API] Creating Entities Done!");

  return(true);

}


/**************************************************************************/
/*!
    @brief  Post Observation for sensed vaules of PM25, PM10, humidity, temperature and Pressure (HTTP Post) 
    @param datastream int selector for PM25, PM10, humidity, temperature and Pressure etc.
    @param phenomenonTime String that contains phenomenon/occurance Time
    @param resultTime String that contains result/finish Time
    @param result double that contains values for PM25, PM10, humidity, temperature and Pressure etc. from SDS011, BME280, DHT22 etc.
    @returns void none value
*/
/**************************************************************************/

void postObservation(int datastream, String phenomenonTime, String resultTime, double result)
{
  String IotId;
  boolean errorCode;
  switch(datastream)
  {
    case 1: IotId = dsPM25IotId; break;
    case 2: IotId = dsPM10IotId; break;
    case 3: IotId = dsHmBmeIotId; break;
    case 4: IotId = dsTmBmeIotId; break;
    case 5: IotId = dsPrBmeIotId; break;
    case 6: IotId = dsHmDhtIotId; break;
    case 7: IotId = dsTmDhtIotId; break;
  }

  String obsJson = "{\"phenomenonTime\":\""+phenomenonTime+"\",\"resultTime\":\""+resultTime+"\",\"result\":"+result+"}";
  MY_DBGln("[API] Post Observation...");
  
  errorCode = createEntity(String(BASE_URL) + "/Datastreams('"+IotId+"')/Observations", obsJson);
  if(!errorCode) nObservations++;
}
