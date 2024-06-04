#define PHOTO_RESISTOR_PIN A0 // Define the pin where the photoresistor is connected
#define THRESHOLD 512 // Threshold for determining whether the LED is ON or OFF
#define BIT_DURATION 1000 // Duration for each bit, should match the transmitter's duration
#define READ_INTERVAL 100 // Short interval for reading the sensor value to catch every change

void setup() {
  pinMode(PHOTO_RESISTOR_PIN, INPUT); // Set the photoresistor pin as an input
  Serial.begin(9600); // Start serial communication at 9600 baud rate
}

void loop() {
  byte receivedByte = 0; // This will store the reconstructed byte
  bool ledState;

  for (int i = 7; i >= 0; i--) { // Read bits from 7 to 0
    unsigned long startTime = millis();
    
    // Read the sensor multiple times within the bit duration to average out noise and ensure accurate reading
    int analogValue = 0;
    int samples = 0;
    while (millis() - startTime < BIT_DURATION) {
      analogValue += analogRead(PHOTO_RESISTOR_PIN);
      samples++;
      delay(READ_INTERVAL);
    }
    analogValue /= samples; // Average the readings

    // Determine the state of the LED from the average analog value
    if (analogValue > THRESHOLD) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    bitWrite(receivedByte, i, ledState); // Set the bit in the receivedByte
  }

  // After constructing the byte, print it as a character
  Serial.print((char)receivedByte);

  // Optional: delay between readings of each byte
  delay(BIT_DURATION);
}
