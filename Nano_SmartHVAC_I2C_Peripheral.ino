/* The following code reads I2C data from a controller
 * and adjusts motor PWM based on received data.
 * Tested using Arduino Nano for the Smart HVAC project.
 */

#include <Wire.h>

#define IN1A A1
#define IN2A A0
#define IN1B A2
#define IN2B A3
#define IN1C 8
#define IN2C 12
#define IN1D 7
#define IN2D 6

int pwmpin[4] = {11, 10, 9, 5};
byte room[4] = {};
int power[] = {0, 115, 145, 175, 205, 225, 255};

void setup() {
  Wire.begin(9);                  // join i2c bus with address #9
  Wire.onReceive(receiveEvent);   // register event
  Serial.begin(115200);           // start serial for output
  
  for (int i = 0; i < 4; i++){
    pinMode(pwmpin[i], OUTPUT);
  }
  pinMode(IN1A, OUTPUT);
  pinMode(IN2A, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(IN2B, OUTPUT);
  pinMode(IN1C, OUTPUT);
  pinMode(IN2C, OUTPUT);
  pinMode(IN1D, OUTPUT);
  pinMode(IN2D, OUTPUT);
  digitalWrite(IN1A, LOW);
  digitalWrite(IN2A, HIGH);
  digitalWrite(IN1B, HIGH);
  digitalWrite(IN2B, LOW);
  digitalWrite(IN1C, LOW);
  digitalWrite(IN2C, HIGH);
  digitalWrite(IN1D, HIGH);
  digitalWrite(IN2D, LOW);
}

void loop() {
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int numBytes) {
  if (numBytes >= 4) {
    for (int i = 0; i < 4; i++){
      room[i] = Wire.read();
      Serial.println(room[i]);
      analogWrite(pwmpin[i], power[room[i]]);
    }
    Serial.println("");
  }
}