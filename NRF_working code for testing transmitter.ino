#include <SPI.h>
#include "RF24.h"

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);

const uint8_t address[6] = "1Node"; // Transmitter address
float payload = 0.0;

void setup() {
  Serial.begin(115200);

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!"));
    while (1) {}
  }

  radio.setPALevel(RF24_PA_LOW);   // Set power level
  radio.setPayloadSize(sizeof(payload)); // Set payload size
  radio.openWritingPipe(address);  // Set the writing pipe
  radio.stopListening();           // Set as transmitter
}

void loop() {
  unsigned long start_timer = micros(); 
  bool success = radio.write(&payload, sizeof(payload)); // Send the payload
  unsigned long end_timer = micros();

  if (success) {
    Serial.print(F("Sent: "));
    Serial.print(payload);
    Serial.print(F(" | Time: "));
    Serial.print(end_timer - start_timer);
    Serial.println(F(" us"));
    payload += 0.01; // Increment payload
  } else {
    Serial.println(F("Transmission failed."));
  }

  delay(1000); // Send data every second
}
