#include <AFMotor.h>

// IR sensors
#define IR_LEFT   A0
#define IR_RIGHT  A1

// Ultrasonic pins
#define TRIG_PIN  A2
#define ECHO_PIN  A3

// Motors
AF_DCMotor motorL1(1);
AF_DCMotor motorL2(2);
AF_DCMotor motorR1(3);
AF_DCMotor motorR2(4);

int motorSpeed = 200;
int obstacleDistance = 15; // cm

void setup()
{
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  motorL1.setSpeed(motorSpeed);
  motorL2.setSpeed(motorSpeed);
  motorR1.setSpeed(motorSpeed);
  motorR2.setSpeed(motorSpeed);

  stopMotors();
}

void loop()
{
  long distance = getDistance();

  // Obstacle detected → STOP
  if (distance > 0 && distance <= obstacleDistance)
  {
    stopMotors();
    return;
  }

  int leftIR  = digitalRead(IR_LEFT);
  int rightIR = digitalRead(IR_RIGHT);

  // Line follower logic
  if (leftIR == HIGH && rightIR == HIGH)
  {
    forward();        // both on black
  }
  else if (leftIR == LOW && rightIR == HIGH)
  {
    turnLeft();       // left off line
  }
  else if (leftIR == HIGH && rightIR == LOW)
  {
    turnRight();      // right off line
  }
  else
  {
    stopMotors();     // no line
  }
}

// ---------- Ultrasonic ----------

long getDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 25000);
  if (duration == 0) return -1;

  return duration * 0.034 / 2;
}

// ---------- Motor Control ----------

void forward()
{
  motorL1.run(FORWARD);
  motorL2.run(FORWARD);
  motorR1.run(FORWARD);
  motorR2.run(FORWARD);
}

void turnLeft()
{
  motorL1.run(RELEASE);
  motorL2.run(RELEASE);
  motorR1.run(FORWARD);
  motorR2.run(FORWARD);
}

void turnRight()
{
  motorL1.run(FORWARD);
  motorL2.run(FORWARD);
  motorR1.run(RELEASE);
  motorR2.run(RELEASE);
}

void stopMotors()
{
  motorL1.run(RELEASE);
  motorL2.run(RELEASE);
  motorR1.run(RELEASE);
  motorR2.run(RELEASE);
}
