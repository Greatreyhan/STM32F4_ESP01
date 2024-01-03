#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "infinergy"; 
const char *password = "okeokeoke";  

// MQTT Broker
#define mqtt_broker IPAddress(192, 168, 43, 36)
const char *topic_sensor = "astar/sensor";
const char *topic_control = "astar/control";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const char *send = "sensors";
const char *get = "controls";
const int mqtt_port = 1883;

const char msg_sensor[] = {0xA5, 0x5A, 0x02, 0x41, 0x00, 0x32, 0x03, 0x25, 0x05, 0x62, 0x00, 0x32, 0x00, 0x12, 0x00, 0x42, 0x03};
const char* msg_control = "A55A0032001200420112";

WiFiClient espClient;
PubSubClient client(espClient);

// User Variable
char buffer[20];

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  // Connect to MQTT
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

  // Subscribe to topic Control
  client.subscribe(topic_control);
}

// Receive data from astar/control
void callback(char *topic, byte *payload, unsigned int length) {
  char message[length + 1];  // Create a character array to hold the message
  memcpy(message, payload, length);  // Copy the payload bytes into the message array
  message[length] = '\0';  // Add a null terminator to mark the end of the string

  // Now you have the message stored in the `message` variable
  Serial.print("Control: ");
  Serial.println(message);

  // Send Message to STM32
  Serial.write(message);
}
void loop() {
  // Callback received data from astar/control
  client.loop();

  // Send data to astar/sensor
  if(Serial.available() > 0){
    Serial.readBytes(buffer, 20);
    Serial.println(buffer);
    client.publish(topic_sensor, buffer);
  }

}