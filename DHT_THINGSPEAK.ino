#include <WiFi.h>
#include "DHT.h"
#include "ThingSpeak.h"
char* ssid = "Woah"; //enter SSID
char* passphrase = "idontknow?"; // enter the password
WiFiServer server(80);
WiFiClient client;
unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "2AENL5MMRI9UHOSR";
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;
#define DHTPIN 27 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11
// Initializing the DHT11 sensor.
DHT dht(DHTPIN, DHTTYPE);
void setup()
{
Serial.begin(115200); //Initialize serial
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, passphrase);
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
// Print local IP address and start web server
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();
dht.begin();
ThingSpeak.begin(client); // Initialize ThingSpeak
}
void loop()
{
if ((millis() - lastTime) > timerDelay)
{
delay(2500);
float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t))
{
Serial.println(F("Failed to read from DHT sensor!"));
return;
}
Serial.print("Temperature (ºC): ");
Serial.print(t);
Serial.println("ºC");
Serial.print("Humidity");
Serial.println(h);
ThingSpeak.setField(3, t);
ThingSpeak.setField(4, h);
int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
if(x == 200)
{
Serial.println("Channel update successful.");
}
else
{
Serial.println("Problem updating channel. HTTP error code " + String(x));
}
lastTime = millis();
}
}
