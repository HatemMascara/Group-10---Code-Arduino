// Pin setup
const int ldrPin = A0;
const int ledPin = 13;
const int trigPin = 11;
const int echoPin = 12;

// Lumen and Distance thresholds
const float lumenThreshold = 20.0;
const float distanceThreshold = 10.0; // cm

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // 1. Read LDR value
  int analogValue = analogRead(ldrPin);
  float voltage = analogValue * (5.0 / 1023.0);

  // Estimate resistance of LDR
  float resistance = (5.0 - voltage) * 10000.0 / voltage;

  // Estimate lux
  float lux = 500 / (resistance / 1000.0);

  // 2. Measure distance (with raw sensor)
  float measuredDistance = measureDistanceCM();

  // 3. Apply CURVE FITTING calibration
  float correctedDistance = 0.95 * measuredDistance - 0.5; 

  // 4. Debug output
  Serial.print("Lux: ");
  Serial.print(lux);
  Serial.print(" | Corrected Distance: ");
  Serial.print(correctedDistance);
  Serial.println(" cm");

  // 5. Logic for lighting up the LED
  if (lux < lumenThreshold && correctedDistance <= distanceThreshold) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(1000);
}

// Function to measure distance in cm using HC-SR04
float measureDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2.0; // Raw sensor distance
  return distance;
}
