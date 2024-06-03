void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  // if MQ2val > 500
  Serial.println(analogRead(A0));
  delay(1000);
}
