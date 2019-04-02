#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define TRANSISTOR D1

// Connect to the WiFi
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "192.168.x.yy";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
 for (int i=0;i<length;i++) {
  char receivedChar = (char)payload[i];
  Serial.print(receivedChar);
  if (receivedChar == '0'){  digitalWrite(TRANSISTOR, LOW);  }
  else if (receivedChar == '1') {  digitalWrite(TRANSISTOR, HIGH);  }
  }
  Serial.println();
}


void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266 Client")) {
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("lights/SUBLIGHT");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  delay(5000);
  }
 }
}

void setup()
{
 Serial.begin(9600);

 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);

 while (WiFi.status() != WL_CONNECTED) { delay(500); }
 Serial.println("Connected to wifi");
 Serial.println(WiFi.localIP());

 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);

 pinMode(TRANSISTOR, OUTPUT);
}

void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 client.loop();
}
