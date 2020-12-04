#include <Arduino.h>
#include <DHT.h>
#include "WiFi.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

#define DHTPIN 13
#define LED_BUILTIN 2

//#define DHTTYPE DHT11
#define DHTTYPE DHT22

const char *ssid = "devopenspace";
const char *password = "devopenspace2020";

DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);

void setup()
{
  Serial.begin(9600);
  
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    float temperature = dht.readTemperature();
    Serial.println(temperature);
    char content[10] = "";
    sprintf(content, "%f", temperature);
    request->send(200, "application/json", content);
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    float humidity = dht.readHumidity();
    Serial.println(humidity);
    char content[10] = "";
    sprintf(content, "%f", humidity);
    request->send(200, "application/json", content);
  });

  server.begin();

  dht.begin();
}

void loop()
{
  
}
