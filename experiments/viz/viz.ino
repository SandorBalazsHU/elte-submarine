// Lábra kötött szenzor és vezérlés
const int waterSensorPin = A0;
const int powerControlPin = D5;

// Érzékelési küszöb (ez a szenzortól függően változhat)
const int waterThreshold = 300;

void setup() {
  pinMode(powerControlPin, OUTPUT);
  digitalWrite(powerControlPin, HIGH); // Alapból bekapcsolt állapot
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(waterSensorPin);

  if (sensorValue > waterThreshold) {
    digitalWrite(powerControlPin, LOW);  // Áramot elvesz
    Serial.println("Van viz!!!");
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(powerControlPin, HIGH); // Áramot ad
    Serial.println("Nincs viz!!!");
    digitalWrite(LED_BUILTIN, HIGH); 
  }

  Serial.print("Vizszenzor ertek: ");
  Serial.println(sensorValue);

  delay(100);  // Frissítés 1 másodpercenként
}
