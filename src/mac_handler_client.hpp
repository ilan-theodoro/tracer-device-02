#define MAC_BEGIN_FLAG 128

#define WEBSERVICE_URL "http://192.168.1.2:8000/acesso"

HardwareSerial MySerial(1);

void mac_handler_client_init();
void mac_handle();
void send_mac(String, int);
void get_device_mac();

void mac_handler_client_init() {
  MySerial.begin(9600, SERIAL_8N1, 16, 17);
}

void mac_handle() {
  if (MySerial.available() > 0) {
      int c = MySerial.read();
      if(c == MAC_BEGIN_FLAG) {
        get_device_mac();
      }
  }
}

void send_mac(String mac, int rssi){
    HTTPClient http;
    char json[100];
    sprintf(json, "{\"mac\":\"%s\",\"rssi\":%d}",  mac.c_str(), rssi);
    //Serial.println(String(json));

    http.begin(String(WEBSERVICE_URL));
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(String(json));
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
    send_mac(macStr, -rssi);
}
