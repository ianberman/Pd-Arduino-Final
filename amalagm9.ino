#include <Key.h>
#include <Keypad.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>

#define trigPin 13
#define echoPin 12

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '&', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

int lastXAxis = 0;
int lastYAxis = 0;
int lastAccelx = 0;
int lastAccely = 0;
int lastMagx = 0;
int lastMagy = 0;

void setup() {
  Serial.begin(9600);
  //ULTRASONIC//
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //ACCEL//
  if (!accel.begin()) {
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while (1);
  }
  if (!mag.begin()) {
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }
  sensor_t sensor;
}

void loop(void) {
  //KEYPAD//
  char key = keypad.getKey();

  //JOYSTICK//
  int xAxis = analogRead(A0);
  int yAxis = analogRead(A1);

  //ULTRASONIC//
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  //ACCEL//
  sensors_event_t event;

  //SERIAL//
  Serial.print(1); //ULTRASONIC
  Serial.print(" ");
  if (distance >= 75 || distance <= 1) {
    Serial.print(0);
  } else {
    Serial.print(distance);
  }
  Serial.println(" ");
  Serial.print(2); //JOYSTICK
  Serial.print(" ");
  if (abs(xAxis - lastXAxis) < 2) {
    Serial.print(lastXAxis);
  } else {
    Serial.print(xAxis);
    lastXAxis = xAxis;
  }
  Serial.print(" ");
  if (abs(yAxis - lastYAxis) < 2) {
    Serial.print(lastYAxis);
  } else {
    Serial.print(yAxis);
    lastYAxis = yAxis;
  }
  Serial.println(" ");

  Serial.print(3); //ACCEL+MAG
  Serial.print(" ");

  accel.getEvent(&event);
  if ((abs(event.acceleration.x - lastAccelx)) < 1.0) {
    Serial.print(lastAccelx);
  } else {
    Serial.print(event.acceleration.x);
    lastAccelx = event.acceleration.x;
  }
  Serial.print(" ");
  if ((abs(event.acceleration.y - lastAccely)) < 1.0) {
    Serial.print(lastAccely);
  } else {
    Serial.print(event.acceleration.y);
    lastAccely = event.acceleration.y;
  }
  mag.getEvent(&event);
  Serial.print(" ");
  if (abs(event.magnetic.x - lastMagx) < 1) {
    Serial.print(lastMagx);
  } else {
    Serial.print(event.magnetic.x);
    lastMagx = event.magnetic.x;
  }
  Serial.print(" ");
  if (abs(event.magnetic.y - lastMagy) < 1) {
    Serial.print(lastMagy);
  } else {
    Serial.print(event.magnetic.y);
    lastMagy = event.magnetic.y;
  }
  Serial.println(" ");
  Serial.print(4); //KEYPAD
  Serial.print(" ");
  if (key) {
    Serial.print(key);
  } else {
    Serial.print(0);
  }
  Serial.println();
}
