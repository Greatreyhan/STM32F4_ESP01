char str[17];
void setup() {
  Serial.begin(115200);
}

void loop() {
  // // Receiver
  // if(Serial.available() > 0){
  // Serial.readBytes(str, 17);
  // Serial.println(str);
  // Serial.println(" Receiver");
  // }

  // Sender
  Serial.write("Hello too!");
  delay(1000);
}
