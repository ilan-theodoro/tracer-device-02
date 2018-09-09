#include <HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "mac_handler_client.hpp"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

void setup() {
    Serial.begin(115200);
    mac_handler_client_init();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Conectando ao Wi-Fi...");
    }

    Serial.println("Conectado.");

}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    mac_handle();
  } else {
     Serial.println("Erro na conexão Wi-Fi.");
  }
}
