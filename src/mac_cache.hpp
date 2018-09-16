#ifndef MAC_CACHE_HPP
#define MAC_CACHE_HPP

#include <string>
#include <HardwareSerial.h>
#include <HTTPClient.h>

namespace tracer {

  #define WEBSERVICE_URL "http://192.168.1.3:8000/acessos"
  #define DATA_SIZE 100

  struct acesso {
    String mac;
    int rssi;
  };

  class MacCache {
  public:
    MacCache();
    ~MacCache();
    void add(String mac, int rssi);
    bool readyToSend();
    String toJson();
  private:
    long lastTimeSent;
    acesso *data;
    int curDataIndex;

    void sendMac();
    void resetData();
  };

  MacCache::MacCache() {
    data = new acesso[DATA_SIZE];
    lastTimeSent = millis();
    curDataIndex = 0;
  }

  MacCache::~MacCache() {
    delete[] data;
  }

  void MacCache::add(String mac, int rssi) {
    if(curDataIndex < DATA_SIZE) {
      data[curDataIndex].rssi = rssi;
      data[curDataIndex].mac = mac;
      curDataIndex++;
    } else {
      Serial.println("OVERFLOW");
    }

    if (readyToSend()) {
      sendMac();
      resetData();
    }
  }

  bool MacCache::readyToSend() {
    int timeSpent = millis() - lastTimeSent;
    return ( timeSpent > 10000 || curDataIndex == DATA_SIZE); //data.size() >= 100 ||
  }

  String MacCache::toJson() {
    String json = "[";

  	for (int i = 0; i < curDataIndex; i++) {
  		String mac = data[i].mac;
  		int rssi = data[i].rssi;
  		json += "{\"mac\":\""+mac+"\",";
  		json += "\"rssi\":"+String(rssi)+"},";
  	}

  	json[json.length()-1] = ']';

  	return json;
  }

  void MacCache::sendMac() {
    HTTPClient http;
    http.begin(String(WEBSERVICE_URL));
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(toJson());
    Serial.println(httpCode);
    http.end();
  }

  void MacCache::resetData() {
    curDataIndex = 0;
    lastTimeSent = millis();
    Serial.println("RESETED");
  }

}

#endif // MAC_CACHE_HPP
