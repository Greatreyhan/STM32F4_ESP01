#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "infinergy"; 
const char *password = "okeokeoke";  

// MQTT Broker

#define mqtt_broker IPAddress(192, 168, 43, 37)
const char *topic = "astar/sensor";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const char *send = "sensors";
const char *get = "controls";
const int mqtt_port = 1883;

const char msg_sensor[] = {0xA5, 0x5A, 0x02, 0x41, 0x00, 0x32, 0x03, 0x25, 0x05, 0x62, 0x00, 0x32, 0x00, 0x12, 0x00, 0x42, 0x03};
const char* msg_control = "A55A0032001200420112";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }

  client.publish(topic, msg_control);
  client.subscribe(topic);

}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for(int i = 0; i< length; i++){ Serial.print(payload[i]); Serial.print("|");}
  Serial.println("-----------------------");
}
void loop() {
  client.loop();
}