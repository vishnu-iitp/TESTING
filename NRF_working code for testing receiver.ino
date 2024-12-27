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
  radio.openReadingPipe(1, address);  // Set the reading pipe
  radio.startListening();           // Set as receiver
}

void loop() {
  if (radio.available()) {
    radio.read(&payload, sizeof(payload)); // Read the payload
    Serial.print(F("Received: "));
    Serial.println(payload);
  }
}
