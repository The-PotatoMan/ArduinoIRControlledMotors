// #include <IRremote.h>
// const int irPin = 7;
// IRrecv ir( irPin );
// decode_results results;

#define DECODE_NEC       // DECODE_NEC
#include <IRremote.hpp>  // Do not change header order.

constexpr uint8_t RECV_PIN{ 7 };

/*
 *  motor pins
 * ENA is the pin to determin speed
 * IN1 and IN2 determin which direction the motor spins
 * Copy these pins and change their names and which pins the other controller is connected to
 */
constexpr uint8_t ENA{ 9 };
constexpr uint8_t IN1{ 10 };
constexpr uint8_t IN2{ 11 };

constexpr uint8_t ENB{ 3 };
constexpr uint8_t IN3{ 2 };
constexpr uint8_t IN4{ 4 };

uint8_t speed = 255;  //0-255

constexpr uint16_t S1 = 0x38;  // Taste 5

void setup() {
  Serial.begin(9600);

  IrReceiver.begin(RECV_PIN);
  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(RECV_PIN);
//copy the pinMode for the new pins created above
  pinMode( ENA, OUTPUT );
  pinMode( IN1, OUTPUT );
  pinMode( IN2, OUTPUT );
  pinMode( ENB, OUTPUT );
  pinMode( IN3, OUTPUT );
  pinMode( IN4, OUTPUT );
}

void loop() {
  uint16_t cmd = irReceive();
  switch (cmd) {
    case 0x15:
      //run all motors backwards
      Serial.println("reverse");
      break;
    case 0x40: 
      //stop all motors
      Serial.println("stop");
      //copy this for the other motor
      stopMotor(IN1, IN2, ENA );
      stopMotor(IN3, IN4, ENB );
      break;
    case 0x43:
      //right motors reverse
      //left motors forward
      Serial.println("right");
      break;
    case 0x44:
      //right motors forward
      //left motors reverse
      Serial.println("left");
      break;
    case 0x46:
      //all motors forward
      //copy this for the other motor
      runMotor( IN1, IN2, ENA, speed );
      runMotor( IN3, IN4, ENB, speed );
      Serial.println("forward");
      break;
    // case 0x16://1
    //   setSpeed( 1 );
    //   break;
    // case 0x19://2
    // setSpeed( 2 );
    //   break;
    // case 0xD://3
    //   setSpeed( 3 );
    //   break;
    // case 0xC://4
    //   setSpeed( 4 );
    //   break;
    // case 0x18://5
    //   setSpeed( 5 );
    //   break;
    // case 0x5E://6
    //   setSpeed( 6 );
    //   break;
    // case 0x8://7
    //   setSpeed( 7 );
    //   break;
    // case 0x1C://8
    //   setSpeed( 8 );
    //   break;
    // case 0x5A://9
    //   setSpeed( 9 );
    //   break;
    default:
      // Serial.println( cmd, HEX );
      break;
  }
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
      Serial.println( received, HEX );
    }
    IrReceiver.resume();
  }
  return received;
}