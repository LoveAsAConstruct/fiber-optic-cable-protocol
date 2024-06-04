#define PHOTO_RESISTOR_PIN A0
#define THRESHOLD 512
#define BIT_DURATION 1000
#define SYNC_DURATION 2000

void setup() {
  pinMode(PHOTO_RESISTOR_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  int syncSignal = analogRead(PHOTO_RESISTOR_PIN);

  // Wait for the synchronization signal
  if (syncSignal > THRESHOLD) {
    unsigned long startTime = millis();
    while (millis() - startTime < SYNC_DURATION * 2) {
      // Wait through the duration of the sync signal
    }

    // Now start reading the message
    byte receivedByte = 0;
    bool lastState = LOW;

    for (int i = 7; i >= 0; i--) {
      int value = analogRead(PHOTO_RESISTOR_PIN);
      bool currentState = value > THRESHOLD;

      if (currentState != lastState) { // Check if the state has changed
        bitWrite(receivedByte, i, 1); // A change represents a '1'
      } else {
        bitWrite(receivedByte, i, 0); // No change represents a '0'
      }

      lastState = currentState;
      delay(BIT_DURATION);
    }

    // After constructing the byte, print it as a character
    Serial.print((char)receivedByte);

    // Optional: delay between readings of each byte
    delay(BIT_DURATION);
  }
}
