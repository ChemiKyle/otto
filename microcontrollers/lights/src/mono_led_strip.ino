#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define TRANSISTOR D1
#define MQTT_TOPIC "lights/SUBLIGHT" //SUBLIGHT should be replaced with your light location

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";

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
 // assign random client ID to avoid collisions
 // https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino
 String clientID = "ESP8266_client-";
 clientID += String(random(0xffff), HEX);

 if (client.connect(clientID.c_str())) {
  Serial.println("connected");
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
