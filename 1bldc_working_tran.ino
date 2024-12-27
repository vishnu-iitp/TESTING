#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN pins

const byte address[6] = "00001";

struct DataPacket {
  int throttleValue;
  int servoValue;
};

DataPacket data;

void setup() {
  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println("Radio initialization failed!");
    while (1); // Stop the program if initialization fails
  }

  radio.openWritingPipe(address);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(110);
  radio.stopListening();

  Serial.println("Transmitter initialized.");
}

void loop() {
  int potThrottle = analogRead(A0);
  data.throttleValue = map(potThrottle, 0, 1023, 0, 255);

  int potServo = analogRead(A1);
  data.servoValue = map(potServo, 0, 1023, 0, 180);

  bool success = radio.write(&data, sizeof(data));
  if (success) {
    Serial.print("Data sent successfully: Throttle=");
    Serial.print(data.throttleValue);
    Serial.print(", Servo=");
    Serial.println(data.servoValue);
  } else {
    Serial.println("Data transmission failed!");
  }

  delay(10);
}
