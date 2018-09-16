#ifndef MAC_CACHE_HPP
#define MAC_CACHE_HPP

#include <string>
#include <HardwareSerial.h>

namespace tracer {

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
    String toJsonAndReset();
  private:
    long lastTimeSent;
    acesso *data;
    int curDataIndex;

    void resetData();
  };

  MacCache::MacCache() {
    data = new acesso[100];
    lastTimeSent = millis();
    curDataIndex = 0;
  }

  MacCache::~MacCache() {
    delete[] data;
  }

  void MacCache::add(String mac, int rssi) {
    if(curDataIndex < 100) {
      acesso tmp;
      tmp.mac = mac;
      tmp.rssi = rssi;
      data[curDataIndex++] = tmp;
    } else {
      Serial.println("OVERFLOW");
    }
  }

  bool MacCache::readyToSend() {
    int timeSpent = millis() - lastTimeSent;
    return ( timeSpent > 10000 || curDataIndex == 100); //data.size() >= 100 ||
  }

  String MacCache::toJsonAndReset() {
    String json = "[";

  	for (int i = 0; i < curDataIndex; i++) {
  		String mac = data[i].mac;
  		int rssi = data[i].rssi;
  		json += "{\"mac\":\""+mac+"\",";
  		json += "\"rssi\":"+String(rssi)+"},";
  	}

  	json[json.length()-1] = ']';

    resetData();

  	return json;
  }

  void MacCache::resetData() {
      curDataIndex = 0;
    lastTimeSent = millis();
      Serial.println("RESETED");
  }

}

#endif // MAC_CACHE_HPP
