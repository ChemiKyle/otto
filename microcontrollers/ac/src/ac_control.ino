/* Controls for my AC (unknown GE unit) were captured and listed using a fork of the IRremote library.
 * For more information, see:
 * https://github.com/markszabo/IRremoteESP8266/
 */

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const uint16_t kIrLed = D2;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

#define MQTT_TOPIC "ac"

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";

WiFiClient espClient;
PubSubClient client(espClient);


// Command data captured by IRrecvDumpV2.ino

uint16_t powerCycle[37] = {8462, 4282,  480, 1650,  484, 1624,  508, 1648,  456, 570,  510, 1648,  456, 598,  484, 1650,  482, 1648,  428, 4284,  510, 1624,  452, 1652,  508, 568,  510, 1648,  456, 568,  510, 568,  482, 598,  484, 570,  586};  // UNKNOWN B5E9B811

uint16_t coolMode[37] = {8436, 4308,  510, 1646,  430, 1648,  510, 1624,  478, 598,  484, 1646,  486, 568,  456, 1652,  508, 1626,  508, 4228,  510, 566,  512, 568,  456, 596,  510, 570,  510, 568,  456, 596,  512, 568,  510, 568,  590};  // UNKNOWN ABCD62D8

uint16_t fanMode[37] = {8460, 4280,  482, 1650,  484, 1622,  452, 1678,  482, 570,  510, 1622,  480, 570,  510, 1624,  478, 1650,  484, 4254,  480, 1626,  508, 568,  510, 570,  454, 596,  512, 568,  510, 568,  480, 598,  486, 568,  588};  // UNKNOWN E6F32BDF

uint16_t fanHigh[37] = {8466, 4284,  510, 1648,  458, 1648,  482, 1648,  430, 596,  512, 1648,  484, 568,  456, 1676,  484, 1648,  454, 4258,  510, 568,  510, 1650,  458, 568,  512, 568,  482, 596,  486, 568,  510, 568,  482, 598,  590};  // UNKNOWN C6F77A26

uint16_t fanLow[37] = {8466, 4284,  508, 1648,  458, 1626,  506, 1648,  430, 596,  510, 1650,  482, 568,  482, 1648,  486, 1646,  430, 4284,  510, 1648,  428, 598,  508, 570,  510, 1648,  432, 594,  508, 570,  510, 568,  456, 596,  586};  // UNKNOWN 3F3D09A6

uint16_t tempUp[37] = {8460, 4282,  492, 1664,  456, 1648,  484, 1646,  456, 568,  510, 1648,  480, 572,  484, 1648,  482, 1648,  430, 4280,  510, 568,  454, 624,  484, 1648,  484, 568,  454, 598,  510, 568,  510, 568,  454, 596,  586};  // UNKNOWN 224510A1

uint16_t tempDown[37] = {8462, 4278,  484, 1650,  484, 1648,  452, 1652,  484, 568,  510, 1646,  456, 570,  510, 1648,  480, 1652,  456, 4256,  480, 1650,  484, 568,  510, 1648,  428, 598,  510, 568,  512, 568,  454, 598,  510, 568,  590};  // UNKNOWN B3F44C1

void callback(char* topic, byte* payload, unsigned int length) {
  for (int i=0;i<length;i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);
  // TODO: dictionary instead of switch case?
    switch(receivedChar) {
    case '0':
      irsend.sendRaw(powerCycle, 37, 38);
      break;
    case 'c':
      irsend.sendRaw(coolMode, 37, 38);
      break;
    case 'f':
      irsend.sendRaw(fanMode, 37, 38);
      break;
    case 'h':
      irsend.sendRaw(fanHigh, 37, 38);
      break;
    case 'l':
      irsend.sendRaw(fanLow, 37, 38);
      break;
    case 'u':
      irsend.sendRaw(tempUp, 37, 38);
      break;
    case 'd':
      irsend.sendRaw(tempDown, 37, 38);
      break;
    }
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // assign random client ID to avoid collisions
    // https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino
    String clientID = "ESP8266_client-";
    clientID += String(random(0xffff), HEX);

    if (client.connect(clientID.c_str())) {
      Serial.println("connected");
      Serial.println("Client ID: ");
      Serial.print(clientID);
      // ... and subscribe to topic
      client.subscribe(MQTT_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  Serial.println("Connected to wifi");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
