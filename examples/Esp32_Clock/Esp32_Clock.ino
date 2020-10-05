#include <WSM-6879A.h>
#include <WiFi.h>

WSM6879A wsm(4,2,15,5);		// Change for your configuration

const char* ssid     = "<<SSID>>";
const char* password = "<<PASSWORD>>";
const char* ntpServer = "200.160.0.8";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;
long rssi;

void setup() {

    IPAddress ip;
    
    Serial.begin(115200);
    Serial.println("Iniciando");

    wsm.begin();

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        wsm.print("-");
        delay(500);
    }

    ip = WiFi.localIP();
    
    wsm.clear();
    wsm.showAntenna(WiFi.RSSI());
    wsm.printf("%d-%d-%d-%d",ip[0],ip[1],ip[2],ip[3]);

    delay(4000);
    
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    delay(1500);
}

void loop() {
    Serial.println("Loop...");

    wsm.clear();
    wsm.showAntenna(WiFi.RSSI());

    wsm.printLocalTime();

    wsm.writeBuffer();

    delay(1500);
}
