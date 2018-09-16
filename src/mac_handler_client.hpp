#ifndef MAC_HANDLER_CLIENT_HPP
#define MAC_HANDLER_CLIENT_HPP


#include <vector>

#include "mac_cache.hpp"

namespace tracer {

  #define MAC_BEGIN_FLAG 128

  #define WEBSERVICE_URL "http://192.168.1.3:8000/acessos"

  HardwareSerial MySerial(1);

  MacCache macCache;

  void mac_handler_client_init();
  void mac_handle();
  void send_mac(String);
  void get_device_mac();

  void mac_handler_client_init() {
    MySerial.begin(9600, SERIAL_8N1, 16, 17);
  }

  void mac_handle() {
    if (MySerial.available() > 0) {
        int c = MySerial.read();
        if(c == MAC_BEGIN_FLAG) {
          get_device_mac();
          if (macCache.readyToSend()) {
              send_mac(macCache.toJsonAndReset());
          }
        }
    }
  }

  void send_mac(String json){
      HTTPClient http;
      //Serial.println(String(json));

      http.begin(String(WEBSERVICE_URL));
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(json);
      Serial.println(httpCode);
      http.end();
  }

  void get_device_mac() {
    int mac[6];
    String macStr = String("");
    int rssi;
    for (int i = 0; i < 6; i++) {
      mac[i] = MySerial.read();
      char hex_aux[5];
      sprintf(hex_aux, "%x", mac[i]);
      macStr = macStr + String(hex_aux);
      if (i != 5) {
        macStr = macStr + String(".");
      }
    }
    rssi = MySerial.read();
    //if (macStr != String("58.90.43.7a.81.88"))
      macCache.add(macStr, -rssi);
  }

}

#endif // MAC_HANDLER_CLIENT_HPP
