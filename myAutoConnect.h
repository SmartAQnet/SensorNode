/**
 * \file myAutoConnect.h
 *
 * \brief Inherits the AutoConnect class and overrides few functions
 *
 * This is the files that contains the functions inherited from AutoConnect Class which needs some modfications to add OLED display code.
 * These functions are then over ridden here. Basically, when there is no wifi connected, it shows a message on serial port and on oled display that is searching for a Wifi.
 * Similarly, when no wifi is connected in sub period of time, it opens an access-point and shows another message with default SSID and password.
 * 
 * \date 19.08.2019
 */
#include "display.h" //OLED Display related stuff
#include <AutoConnect.h>

// Uncomment the following AC_DEBUG to enable debug output.
///enable debug output.
#define AC_DEBUG

/// Debug output destination can be defined externally with AC_DEBUG_PORT
#ifndef AC_DEBUG_PORT
#define AC_DEBUG_PORT Serial
#endif // !AC_DEBUG_PORT
#ifdef AC_DEBUG
/// Debug output destination can be defined externally with AC_DEBUG_PORT
#define AC_DBG_DUMB(...) do {AC_DEBUG_PORT.printf( __VA_ARGS__ );} while (0)
/// Debug output destination can be defined externally with AC_DEBUG_PORT
#define AC_DBG(...) do {AC_DEBUG_PORT.print("[AC] "); AC_DEBUG_PORT.printf( __VA_ARGS__ );} while (0)
#else
#define AC_DBG(...)
#define AC_DBG_DUMB(...)
#endif // !AC_DEBUGestablished

#if defined(ARDUINO_ARCH_ESP8266)
#define SOFT_RESET()  ESP.reset()
#define SET_HOSTNAME(x) do { WiFi.hostname(x); } while(0)
#elif defined(ARDUINO_ARCH_ESP32)
#define SOFT_RESET()  ESP.restart()
#define  SET_HOSTNAME(x) do { WiFi.setHostname(x); } while(0)
#endif


/// Externally defined OLED Display object
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
/// counter to count 500ms
unsigned char cntr;
/// show QR or Text for AP
boolean show_QR;

/// Timeout to search WiFi already configured. (150000ms=2.5min)
#define WIFI_REBOOT_TIMEOUT 150000
/// The interval to show dots while waiting for the searching Wifi (in ms)
#define SHOW_WAITING_DOTS_WIFI    300
/// The interval to show dots while waiting for the access point (in ms)
#define SHOW_WAITING_DOTS_AP    500
/// The interval to show QR or AP waiting screen   (in ms)
#define SHOW_QR_INTERVAL    10000
          
/// Just calculate 500ms interval while waiting the AP is configure to connect a WiFi
unsigned long previousMillis;
// initialise udp for network time

///myAutoConnect Class inherited from AutoConnect
class myAutoConnect : public AutoConnect 
{
public:

 /**
 *  myAutoConnect default constructor. This entry activates WebServer
 *  internally and the web server is allocated internal.
 */
   myAutoConnect(WebServerClass& webServer) : AutoConnect(webServer){
    }
    /**
     *  Wait for establishment of the connection until the specified time expires.
     *  @param  timeout Expiration time by millisecond unit.
     *  @return wl_status_t
     */
    wl_status_t _waitForConnect(unsigned long timeout) {
      wl_status_t wifiStatus;
    
      AC_DBG("Connecting");
      // show also on display
      
      u8g2.drawStr(0, 7, "SUCHE WIFI...   ");
      u8g2.sendBuffer();
      
      unsigned long st = millis();
      while ((wifiStatus = WiFi.status()) != WL_CONNECTED) {
        if (timeout) {
          if (millis() - st > timeout)
            break;
        }
        AC_DBG_DUMB("%c", '.');
        delay(SHOW_WAITING_DOTS_WIFI);
      }
      AC_DBG_DUMB("\n");
      AC_DBG_DUMB("%s IP:%s\n", wifiStatus == WL_CONNECTED ? "established" : "time out", WiFi.localIP().toString().c_str());
      return wifiStatus;
    }

/**
 *  Starts establishing WiFi connection without SSID and password.
 */
bool begin(void) {
  return begin(nullptr, nullptr,WIFI_REBOOT_TIMEOUT);//30000
  //return begin(nullptr, nullptr,30000);//
}

/**
 *  Starts establishing WiFi connection.
 *  Before establishing, start the Web server and DNS server for the captive
 *  portal. Then begins connection establishment in WIFI_STA mode. If
 *  connection can not established with the specified SSID and password,
 *  switch to WIFI_AP_STA mode and activate SoftAP.
 *  @param  ssid        SSID to be connected.
 *  @param  passphrase  Password for connection.
 *  @param  timeout     A time out value in milliseconds for waiting connection.
 *  @return true        Connection established, AutoConnect service started with WIFI_STA mode.
 *  @return false       Could not connected, Captive portal started with WIFI_AP_STA mode.
 */
    

  bool begin(const char* ssid, const char* passphrase, unsigned long timeout) {
    bool  cs;
    AC_DBG("Starting from the myAutoConnect\n");//1
    // Overwrite for the current timeout value.
    _connectTimeout = timeout;
  
    // Start WiFi connection with station mode.
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_STA);
    delay(100);
  
    // Set host name
    if (_apConfig.hostName.length())
      SET_HOSTNAME(_apConfig.hostName.c_str());
  
    // Start Ticker according to the WiFi condition with Ticker is available.
    if (_apConfig.ticker) {
      _ticker.reset(new AutoConnectTicker(_apConfig.tickerPort, _apConfig.tickerOn));
      if (WiFi.status() != WL_CONNECTED)
        _ticker->start(AUTOCONNECT_FLICKER_PERIODDC, (uint8_t)AUTOCONNECT_FLICKER_WIDTHDC);
    }
  
    // Advance configuration for STA mode. Restore previous configuration of STA.
    station_config_t  current;
    if (_getConfigSTA(&current)) {
      AC_DBG("Current:%.32s\n", current.ssid);
      _loadAvailCredential(reinterpret_cast<const char*>(current.ssid));
    }
    if (!_configSTA(_apConfig.staip, _apConfig.staGateway, _apConfig.staNetmask, _apConfig.dns1, _apConfig.dns2))
      return false;
  
    // If the portal is requested promptly skip the first WiFi.begin and
    // immediately start the portal.
    if (_apConfig.immediateStart) {
      cs = false;
      _apConfig.autoReconnect = false;
      AC_DBG("Start the portal immediately\n");
    }
    else {
      // Try to connect by STA immediately.
      if (ssid == nullptr && passphrase == nullptr)
        WiFi.begin();
      else {
        _disconnectWiFi(false);
        WiFi.begin(ssid, passphrase);
      }
      AC_DBG("WiFi.begin(%s%s%s)\n", ssid == nullptr ? "" : ssid, passphrase == nullptr ? "" : ",", passphrase == nullptr ? "" : passphrase);
      cs = _waitForConnect(_connectTimeout) == WL_CONNECTED;
    }
  
    // Reconnect with a valid credential as the autoReconnect option is enabled.
    if (!cs && _apConfig.autoReconnect && (ssid == nullptr && passphrase == nullptr)) {
      // Load a valid credential.
      if (_loadAvailCredential(nullptr)) {
        // Try to reconnect with a stored credential.
        char  ssid_c[sizeof(station_config_t::ssid) + sizeof('\0')];
        char  password_c[sizeof(station_config_t::password) + sizeof('\0')];
        *ssid_c = '\0';
        strncat(ssid_c, reinterpret_cast<const char*>(_credential.ssid), sizeof(ssid_c) - sizeof('\0'));
        *password_c = '\0';
        strncat(password_c, reinterpret_cast<const char*>(_credential.password), sizeof(password_c) - sizeof('\0'));
        AC_DBG("autoReconnect loaded SSID:%s\n", ssid_c);
        const char* psk = strlen(password_c) ? password_c : nullptr;
        _configSTA(IPAddress(_credential.config.sta.ip), IPAddress(_credential.config.sta.gateway), IPAddress(_credential.config.sta.netmask), IPAddress(_credential.config.sta.dns1), IPAddress(_credential.config.sta.dns2));
        WiFi.begin(ssid_c, psk);
        AC_DBG("WiFi.begin(%s%s%s)\n", ssid_c, psk == nullptr ? "" : ",", psk == nullptr ? "" : psk);
        cs = _waitForConnect(_connectTimeout) == WL_CONNECTED;
      }
    }
    _currentHostIP = WiFi.localIP();
  
    // End first begin process, the captive portal specific process starts here.
    if (cs) {
      // Activate AutoConnectUpdate if it is attached and incorporate it into the AutoConnect menu.
      if (_update)
        _update->enable();
    }
   // Rushing into the portal.
    else {
      // The captive portal is effective at the autoRise is valid only.
      if (_apConfig.autoRise) {
  
        // Change WiFi working mode, Enable AP with STA
        WiFi.setAutoConnect(false);
        _disconnectWiFi(false);
  
        // Activate the AP mode with configured softAP and start the access point.
        WiFi.mode(WIFI_AP_STA);
        while (WiFi.getMode() != WIFI_AP_STA) {
          delay(1);
          yield();
        }
  
        // Connection unsuccessful, launch the captive portal.
  #if defined(ARDUINO_ARCH_ESP8266)
        _config();
  #endif
        WiFi.softAP(_apConfig.apid.c_str(), _apConfig.psk.c_str(), _apConfig.channel, _apConfig.hidden);
        do {
          delay(100);
          yield();
        } while (!WiFi.softAPIP());
  #if defined(ARDUINO_ARCH_ESP32)
        _config();
  #endif
        if (_apConfig.apip) {
          do {
            delay(100);
            yield();
          } while (WiFi.softAPIP() != _apConfig.apip);
        }
        _currentHostIP = WiFi.softAPIP();
        AC_DBG("SoftAP %s/%s Ch(%d) IP:%s %s\n", _apConfig.apid.c_str(), _apConfig.psk.c_str(), _apConfig.channel, _currentHostIP.toString().c_str(), _apConfig.hidden ? "hidden" : "");
  
            u8g2.clearBuffer();
            u8g2.drawStr(0, 7, "BITTE           ");
            u8g2.drawStr(0, 15, "SENSORKNOTEN    ");
            u8g2.drawStr(0, 23, "EINRICHTEN      ");
            u8g2.drawStr(0, 31, "UNTER           ");
            u8g2.drawStr(0, 39, "SENSOR-WIFI     ");
            u8g2.drawStr(0, 47, ("SSID: "+esp32_chipid).c_str()); 
            u8g2.drawStr(0, 55, "PW: 12345678    ");
            u8g2.drawStr(0, 63, "IP:192.168.244.1");

            u8g2.sendBuffer();
            
        // Start ticker with AP_STA
        if (_ticker)
          _ticker->start(AUTOCONNECT_FLICKER_PERIODAP, (uint8_t)AUTOCONNECT_FLICKER_WIDTHAP);
  
        // Fork to the exit routine that starts captive portal.
        cs = _onDetectExit ? _onDetectExit(_currentHostIP) : true;
  
        // Start Web server when TCP connection is enabled.
        _startWebServer();
  
        // Start captive portal without cancellation by DetectExit.
        if (cs) {
          // Prepare for redirecting captive portal detection.
          // Pass all URL requests to _captivePortal to disguise the captive portal.
          _startDNSServer();
  
          // Start the captive portal to make a new connection
          bool  hasTimeout = false;
          _portalAccessPeriod = millis();
          
          AC_DBG("Trying the portal");//3
          while (WiFi.status() != WL_CONNECTED && !_rfReset) {
            unsigned long currentMillis = millis();//4
            if((currentMillis -previousMillis)>=SHOW_WAITING_DOTS_AP) 
            {
              previousMillis = currentMillis;
              AC_DBG_DUMB("%c", '.');
              cntr++;
              if(cntr == (SHOW_QR_INTERVAL/SHOW_WAITING_DOTS_AP)) {cntr = 0; show_QR=!show_QR;}
              if(show_QR)
              {
                //Generate and test a QR Code at location x,y
                u8g2.clearBuffer();
                generateQRCode(34,2,esp32_chipid.c_str());
                //u8g2.sendBuffer();
              }
              else
              {
                u8g2.clearBuffer();
                u8g2.drawStr(0, 7, "BITTE           ");
                u8g2.drawStr(0, 15, "SENSORKNOTEN    ");
                u8g2.drawStr(0, 23, "EINRICHTEN      ");
                u8g2.drawStr(0, 31, "UNTER           ");
                u8g2.drawStr(0, 39, "SENSOR-WIFI     ");
                u8g2.drawStr(0, 47, ("SSID: "+esp32_chipid).c_str()); 
                u8g2.drawStr(0, 55, "PW: 12345678    ");
                u8g2.drawStr(0, 63, "IP:192.168.244.1");
                u8g2.sendBuffer();
              }
            }
            handleClient();
            // Force execution of queued processes.
            yield();
            // Check timeout
            if ((hasTimeout = _hasTimeout(_apConfig.portalTimeout))) {
              AC_DBG("\nCP timeout exceeded:%ld\n", millis() - _portalAccessPeriod);//5
              break;
            }
          }
          cs = WiFi.status() == WL_CONNECTED;
  
          // If WLAN successfully connected, release DNS server.
          if (cs) {
            _dnsServer->stop();
            _dnsServer.reset();
            AC_DBG("\nDNS server stopped\n");//6
          }
          // Captive portal staying time exceeds timeout,
          // Close the portal if an option for keeping the portal is false.
          else if (hasTimeout) {
            if (_apConfig.retainPortal) {
              _purgePages();
              AC_DBG("Maintain portal\n");
            }
            else {
              _stopPortal();
            }
          }
        }
      }
      else {
        AC_DBG("Suppress autoRise\n");
      }
    }
  
    // It doesn't matter the connection status for launching the Web server.
    if (!_responsePage)
      _startWebServer();
  
    // Stop ticker
    if (cs)
      if (_ticker)
        _ticker->stop();
  
    return cs;
  }

};
