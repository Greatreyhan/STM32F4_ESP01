char str[17];
void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.readBytes(str, 17);
  Serial.println(str);
  Serial.println(" Receiver");
  delay(1000);
}
