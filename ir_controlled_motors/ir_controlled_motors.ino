// #include <IRremote.h>
// const int irPin = 7;
// IRrecv ir( irPin );
// decode_results results;

#define DECODE_NEC       // DECODE_NEC
#include <IRremote.hpp>  // Do not change header order.

constexpr uint8_t RECV_PIN{ 0 };

/*
 *  motor pins
 * ENA is the pin to determin speed
 * IN1 and IN2 determin which direction the motor spins
 * Copy these pins and change their names and which pins the other controller is connected to
 */

 //front right
constexpr uint8_t ENA{ 5 };
constexpr uint8_t IN1{ 7 };
constexpr uint8_t IN2{ 6 };
//front left
constexpr uint8_t ENB{ 3 };
constexpr uint8_t IN3{ 4 };
constexpr uint8_t IN4{ 2 };
//rear right
constexpr uint8_t ENC{ 11 };
constexpr uint8_t IN5{ 13 };
constexpr uint8_t IN6{ 12 };
//rear left
constexpr uint8_t END{ 9 };
constexpr uint8_t IN7{ 10 };
constexpr uint8_t IN8{ 8 };


uint8_t speed = 255;  //0-255

constexpr uint16_t S1 = 0x38;  // Taste 5

void setup() {
  //Serial.begin(9600);

  IrReceiver.begin(RECV_PIN);
  //Serial.print(F("Ready to receive IR signals at pin "));
  //Serial.println(RECV_PIN);
//copy the pinMode for the new pins created above
  pinMode( ENA, OUTPUT );
  pinMode( IN1, OUTPUT );
  pinMode( IN2, OUTPUT );
  pinMode( ENB, OUTPUT );
  pinMode( IN3, OUTPUT );
  pinMode( IN4, OUTPUT );
  pinMode( ENC, OUTPUT );
  pinMode( IN5, OUTPUT );
  pinMode( IN6, OUTPUT );
  pinMode( END, OUTPUT );
  pinMode( IN7, OUTPUT );
  pinMode( IN8, OUTPUT );
}

void loop() {
  uint16_t cmd = irReceive();
  switch (cmd) {
    case 0x15:
      //run all motors backwards
      //Serial.println("reverse");
      rightBackward();
      leftBackward();
      break;
    case 0x40: 
      //stop all motors
      //Serial.println("stop");
      //copy this for the other motor
      stopMotor(IN1, IN2, ENA );
      stopMotor(IN3, IN4, ENB );
      stopMotor(IN5, IN6, ENC );
      stopMotor(IN7, IN8, END );
      break;
    case 0x43:
      //right motors reverse
      rightBackward();
      //left motors forward
      leftForward();
      //Serial.println("right");
      break;
    case 0x44:
      //right motors forward
      rightForward();
      //left motors reverse
      leftBackward();
      //Serial.println("left");
      break;
    case 0x46:
      //all motors forward
      //copy this for the other motor
      rightForward();
      leftForward();
      //Serial.println("forward");
      break;
    default:
      // Serial.println( cmd, HEX );
      break;
  }
}

void rightForward(){
  runMotor( IN2, IN1, ENA, speed );
  runMotor( IN6, IN5, ENC, speed );
}

void rightBackward(){
  runMotor( IN1, IN2, ENA, speed );
  runMotor( IN5, IN6, ENC, speed );
}

void leftForward(){
  runMotor( IN3, IN4, ENB, speed );
  runMotor( IN8, IN7, END, speed );
}

void leftBackward(){
  runMotor( IN4, IN3, ENB, speed );
  runMotor( IN7, IN8, END, speed );
}

void runMotor( uint8_t highPin, uint8_t lowPin, uint8_t enablePin, uint8_t speed ){
  digitalWrite( highPin, HIGH );
  digitalWrite( lowPin, LOW );
  analogWrite( enablePin, speed );
}

void stopMotor( uint8_t pin1, uint8_t pin2, uint8_t enablePin ){
  digitalWrite( pin1, LOW );
  digitalWrite( pin2, LOW );
  analogWrite( enablePin, 0 );
}

uint16_t irReceive() {
  uint16_t received{ 0 };

  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == NEC) {
      received = IrReceiver.decodedIRData.command;
      //Serial.println( received, HEX );
    }
    IrReceiver.resume();
  }
  return received;
}