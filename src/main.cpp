#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "mydht.h"
#include "myds18b20.h"
#include "relays.h"

#include "index.h" //Our HTML webpage contents with javascripts

const char *ssid = "digynet";
const char *password = "Dnpm7Ssgk8";

ESP8266WebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ru.pool.ntp.org", 5 * 3600, 6 * 60 * 60 * 1000);

void handleRoot()
{
  //String mess = "hello from esp8266!\r\n";
  //mess += timeClient.getFormattedTime();
  String mess = MAIN_page; //Read HTML contents
  server.send(200, F("text/html"), mess);
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

static const char xml1[] = "<?xml version = \"1.0\" ?><sensors><BoardHumi>";
static const char xml2[] = "</BoardHumi><BoardTemp>";
static const char xml3[] = "</BoardTemp><AmbientTemp>";
static const char xml4[] = "</AmbientTemp><EpraTemp>";
static const char xml5[] = "</EpraTemp><WaterTemp>";
static const char xml6[] = "</WaterTemp><BoardTime>";
static const char xml7[] = "</BoardTime></sensors>";
static const char xml8[] = "text/xml";
String adcValue = "";

void handleADC()
{
  adcValue = xml1;
  adcValue += String(dhtHumidity, 0);
  adcValue += xml2;
  adcValue += String(dhtTemp, 1);
  adcValue += xml3;
  adcValue += String(dsAmbientTemp, 1);
  adcValue += xml4;
  adcValue += String(dsEpraTemp, 1);
  adcValue += xml5;
  adcValue += String(dsWaterTemp, 1);
  adcValue += xml6;
  adcValue += timeClient.getFormattedTime();
  adcValue += xml7;

  server.send(200, xml8, adcValue); //Send ADC value only to client ajax request
}

void handleLED()
{
  String ledState = "OFF";                 //Feedback parameter
  String t_state = server.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LED="+lamp+"&LEDstate="+led, true);
  String t_lamp = server.arg("LED");       //Refer  xhttp.open("GET", "setLED?LED="+lamp+"&LEDstate="+led, true);
  Serial.println(t_state);
  if (t_state == NULL || t_lamp == NULL)
  {
    t_state = "";
    t_lamp = "";
  }
  byte led = 0;
  if (t_lamp == "1")
    led = 1;
  if (t_lamp == "2")
    led = 2;
  if (t_lamp == "3")
    led = 3;
  if (t_lamp == "5")
    led = 5;
  bool lampState = false;
  if (t_state == "1")
  {
    lampState = true;
    ledState = "ON";
  }
  else
  {
    lampState = false;
    ledState = "OFF";
  }

  if (led == 5)
    SetFun(lampState ? 255 : 0);
  else
    SetLampState(led, lampState);

  server.send(200, "text/plane", ledState); //Send web page
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
  server.on("/setLED", handleLED);
  server.on("/readADC", handleADC);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  timeClient.begin();

  DallasBegin();
  DhtBegin();

  //digitalWrite(16, LOW);
  //digitalWrite(12, LOW);
  //digitalWrite(13, LOW);
  //digitalWrite(14, LOW);
}
bool fun = false;
unsigned long lastUpdate = 0;
void loop(void)
{
  delay(10);

  if (WiFi.status() != WL_CONNECTED)
    return;

  server.handleClient();

  timeClient.update();

  if (millis() - lastUpdate > 1000)
  {
    lastUpdate = millis();
    CheckDallas();
    CheckDht();
  }
  //sample();
  /*if (fun)
    digitalWrite(16, HIGH);
  else
    digitalWrite(16, LOW);
  fun = !fun;*/
}