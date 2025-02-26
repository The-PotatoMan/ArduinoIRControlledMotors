// Load 3rd party remote library included with Arduino IDE for uploading to the Arduino
#define DECODE_NEC
#include <IRremote.hpp>

// --- Pin Definitions ---
// Remote receiver pin
constexpr uint8_t RECV_PIN = 0;

/*
 * Motor Pins:
 * EN[ABCD] are the enable pins for speed control (using PWM).
 * IN[12345678] are the direction control pins.
 */

// Front Right Motor
constexpr uint8_t FRONT_RIGHT_ENA = 5;
constexpr uint8_t FRONT_RIGHT_IN1 = 7;
constexpr uint8_t FRONT_RIGHT_IN2 = 6;

// Front Left Motor
constexpr uint8_t FRONT_LEFT_ENB = 3;
constexpr uint8_t FRONT_LEFT_IN3 = 4;
constexpr uint8_t FRONT_LEFT_IN4 = 2;

// Rear Right Motor
constexpr uint8_t REAR_RIGHT_ENC = 11;
constexpr uint8_t REAR_RIGHT_IN5 = 13;
constexpr uint8_t REAR_RIGHT_IN6 = 12;

// Rear Left Motor
constexpr uint8_t REAR_LEFT_END = 9;
constexpr uint8_t REAR_LEFT_IN7 = 10;
constexpr uint8_t REAR_LEFT_IN8 = 8;

// --- Motor Control Settings ---
// Speed value (0-255).  Currently fixed at full speed (255).
// Note: There was a comment mentioning issues at certain speeds.
uint8_t speed = 255;

// --- Function Prototypes ---
void setupMotorPins();
void handleRemoteCommand(uint16_t command);
void rightForward();
void rightBackward();
void leftForward();
void leftBackward();
void runMotor(uint8_t highPin, uint8_t lowPin, uint8_t enablePin, uint8_t speed);
void stopMotor(uint8_t pin1, uint8_t pin2, uint8_t enablePin);
uint16_t irReceive();

// --- Setup ---
void setup() {
  // Initialize the IR receiver
  IrReceiver.begin(RECV_PIN);

  // Set motor control pins as OUTPUT
  setupMotorPins();
}

// --- Main Loop ---
void loop() {
  // Get the command from the remote control
  uint16_t command = irReceive();

  // Process the received command
  handleRemoteCommand(command);
}

// --- Function Implementations ---

// Sets the motor pins to OUTPUT mode.
void setupMotorPins() {
  pinMode(FRONT_RIGHT_ENA, OUTPUT);
  pinMode(FRONT_RIGHT_IN1, OUTPUT);
  pinMode(FRONT_RIGHT_IN2, OUTPUT);
  pinMode(FRONT_LEFT_ENB, OUTPUT);
  pinMode(FRONT_LEFT_IN3, OUTPUT);
  pinMode(FRONT_LEFT_IN4, OUTPUT);
  pinMode(REAR_RIGHT_ENC, OUTPUT);
  pinMode(REAR_RIGHT_IN5, OUTPUT);
  pinMode(REAR_RIGHT_IN6, OUTPUT);
  pinMode(REAR_LEFT_END, OUTPUT);
  pinMode(REAR_LEFT_IN7, OUTPUT);
  pinMode(REAR_LEFT_IN8, OUTPUT);
}
// Processes the command received from the remote.
void handleRemoteCommand(uint16_t command) {
  switch (command) {
    case 0x15: // Down button
      // Run all motors backward
      rightBackward();
      leftBackward();
      break;
    case 0x40: // OK button
      // Stop all motors
      stopMotor(FRONT_RIGHT_IN1, FRONT_RIGHT_IN2, FRONT_RIGHT_ENA);
      stopMotor(FRONT_LEFT_IN3, FRONT_LEFT_IN4, FRONT_LEFT_ENB);
      stopMotor(REAR_RIGHT_IN5, REAR_RIGHT_IN6, REAR_RIGHT_ENC);
      stopMotor(REAR_LEFT_IN7, REAR_LEFT_IN8, REAR_LEFT_END);
      break;
    case 0x43: // Right button
      // Right motors backward, left motors forward (turn right)
      rightBackward();
      leftForward();
      break;
    case 0x44: // Left button
      // Right motors forward, left motors backward (turn left)
      rightForward();
      leftBackward();
      break;
    case 0x46: // Up button
      // All motors forward
      rightForward();
      leftForward();
      break;
    default:
      // Ignore other buttons
      break;
  }
}

// Move the right motors forward.
void rightForward() {
  runMotor(FRONT_RIGHT_IN2, FRONT_RIGHT_IN1, FRONT_RIGHT_ENA, speed);
  runMotor(REAR_RIGHT_IN6, REAR_RIGHT_IN5, REAR_RIGHT_ENC, speed);
}

// Move the right motors backward.
void rightBackward() {
  runMotor(FRONT_RIGHT_IN1, FRONT_RIGHT_IN2, FRONT_RIGHT_ENA, speed);
  runMotor(REAR_RIGHT_IN5, REAR_RIGHT_IN6, REAR_RIGHT_ENC, speed);
}

// Move the left motors forward.
void leftForward() {
  runMotor(FRONT_LEFT_IN3, FRONT_LEFT_IN4, FRONT_LEFT_ENB, speed);
  runMotor(REAR_LEFT_IN8, REAR_LEFT_IN7, REAR_LEFT_END, speed);
}

// Move the left motors backward.
void leftBackward() {
  runMotor(FRONT_LEFT_IN4, FRONT_LEFT_IN3, FRONT_LEFT_ENB, speed);
  runMotor(REAR_LEFT_IN7, REAR_LEFT_IN8, REAR_LEFT_END, speed);
}

// Control a single motor's direction and speed.
// highPin: Pin that is set HIGH for a specific direction.
// lowPin: Pin that is set LOW for the same direction.
// enablePin: Pin used for speed control via Pulse Width Modulation (PWM).
// speed: Speed value (0-255).
void runMotor(uint8_t highPin, uint8_t lowPin, uint8_t enablePin, uint8_t speed) {
  digitalWrite(highPin, HIGH);
  digitalWrite(lowPin, LOW);
  analogWrite(enablePin, speed);
}

// Stop a single motor.
void stopMotor(uint8_t pin1, uint8_t pin2, uint8_t enablePin) {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  analogWrite(enablePin, 0);
}

// Receive and decode an IR command.
uint16_t irReceive() {
  uint16_t received = 0;

  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == NEC) {
      received = IrReceiver.decodedIRData.command;
    }
    IrReceiver.resume();
  }
  return received;
}