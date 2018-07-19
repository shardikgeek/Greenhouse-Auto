void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.print("Humedad: ");
  Serial.print(analogRead(A0));
  Serial.println("%%");
  delay(1000);
}
