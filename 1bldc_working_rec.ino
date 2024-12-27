#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// Create an RF24 object
RF24 radio(9, 10); // CE, CSN pins

const byte address[6] = "00001";

const int motorPin = 3; // Pin connected to the BLDC ESC (D3)
const int servoPin = 4; // Pin connected to the servo (D4)

Servo myServo;

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

  radio.openReadingPipe(0, address);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(110);
  radio.startListening();

  pinMode(motorPin, OUTPUT);
  analogWrite(motorPin, 0);

  myServo.attach(servoPin);
  myServo.write(90);

  Serial.println("Receiver initialized.");
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(data));

    // Process received data
    int throttleValue = constrain(data.throttleValue, 0, 255);
    analogWrite(motorPin, throttleValue);

    int servoValue = constrain(data.servoValue, 0, 180);
    myServo.write(servoValue);

    Serial.print("Received Throttle: ");
    Serial.print(throttleValue);
    Serial.print(", Servo: ");
    Serial.println(servoValue);
  } else {
    Serial.println("Not received.");
  }

  delay(10); // Avoid flooding Serial Monitor
}
