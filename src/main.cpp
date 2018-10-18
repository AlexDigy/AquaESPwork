#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "mydht.h"
#include "myds18b20.h"
#include "relays.h"

const char *ssid = "digynet";
const char *password = "Dnpm7Ssgk8";

ESP8266WebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ru.pool.ntp.org", 5 * 3600, 6 * 60 * 60 * 1000);

void handleRoot()
{
  String mess = "hello from esp8266!\r\n";
  mess += timeClient.getFormattedTime();

  server.send(200, "text/plain", mess);
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleOneWire()
{
  String mess = DallasSearch4web();
  server.send(200, "text/plain", mess);
}

void handleSensors()
{
  String mess = timeClient.getFormattedTime();
  mess += "\r\nOneWire:\r\n";
  mess += "Ambient: " + String(dsAmbientTemp, 1) + "\r\n";
  mess += "Epra: " + String(dsEpraTemp, 1) + "\r\n";
  mess += "Water: " + String(dsWaterTemp, 1) + "\r\n";

  mess += "DHT:\r\n";
  mess += "dht temp: " + String(dhtTemp, 1) + "\r\n";
  mess += "dht humidity: " + String(dhtHumidity, 0) + "\r\n";

  server.send(200, "text/plain", mess);
}

void setup(void)
{
  Serial.begin(115200);

  SetupRelays();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/onewire", handleOneWire);
  server.on("/sensors", handleSensors);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  timeClient.begin();

  DallasBegin();
  DhtBegin();
}

void loop(void)
{
  delay(1000);

  if (WiFi.status() != WL_CONNECTED)
    return;

  server.handleClient();

  timeClient.update();

  CheckDallas();
  CheckDht();
  sample();
}