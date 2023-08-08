#include<ESP8266WiFi.h>
#include <Arduino.h>
#include <Wire.h>
#include <ArduinoJson.h>

const char* ssid= "TXT"; // WiFi ssid
const char* password = "g00gle13"; //WiFi Password

WiFiServer server(80);

int analogPin = A0;
int val = 0;

void setup() {
  Serial.begin(9600);

  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

    Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  };

  server.begin();
  Serial.println("Server started");
  /* Print the IP address on serial monitor */
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    /* URL IP to be typed in mobile/desktop browser */
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {

  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  /* Wait until the client sends some data */
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  val = analogRead(analogPin); 
  if (val > 500) {
    Serial.println("Send notification to user");
  }
  else {
    Serial.println("Reading Normal!");
    Serial.print("val = "); 
    Serial.println(val);
  }

  

  /* Read the first line of the request */
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  StaticJsonDocument<200> data;
  data["PPM"] = val;
  String dataBody;
  serializeJson(data, dataBody);
  Serial.println(dataBody);

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json; charset=utf-8");
  client.println(""); /* do not forget this one */
  client.println(dataBody);
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");

  delay(1000);

}