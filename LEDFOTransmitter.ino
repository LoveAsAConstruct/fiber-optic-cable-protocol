#define LED_PIN 13
#define BIT_DURATION 10
#define SYNC_DURATION BIT_DURATION*2 // Longer duration for sync signal

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    // Send sync signal
    digitalWrite(LED_PIN, HIGH);
    delay(SYNC_DURATION);
    digitalWrite(LED_PIN, LOW);
    delay(SYNC_DURATION);

    while (Serial.available() > 0) {
      char ch = Serial.read();
      int asciiValue = ch;

      bool lastState = LOW;
      digitalWrite(LED_PIN, lastState); // Start with LED OFF for reference

      for (int i = 7; i >= 0; i--) {
        bool bit = bitRead(asciiValue, i);
        bool newState = bit ? !lastState : lastState; // Toggle LED state on 1, keep on 0
        digitalWrite(LED_PIN, newState);
        lastState = newState;
        delay(BIT_DURATION);
      }
      digitalWrite(LED_PIN, LOW); // Ensure LED is OFF between characters
      delay(BIT_DURATION);
    }
  }
}
