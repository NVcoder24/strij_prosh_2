#include <Servo.h>

// PINs CONFIG
#define M0_PWM 3
#define M1_PWM 5
#define M0_0 4
#define M0_1 2
#define M1_0 7
#define M1_1 6
#define CH1 8
#define CH2 9
#define SERVO 10
#define DEADZONE 50
// OTHER CONFIG
#define AUTOMUL 1

int servoCal = 5;

Servo myservo;

void setup() {
  // SERIAL
  Serial.begin(9600);
  // PINMODEs
  pinMode(M0_PWM, OUTPUT);
  pinMode(M1_PWM, OUTPUT);
  pinMode(M0_0, OUTPUT);
  pinMode(M0_1, OUTPUT);
  pinMode(M1_0, OUTPUT);
  pinMode(M1_1, OUTPUT);
  myservo.attach(SERVO);
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
}

// gets flysky pwm signal
int getCh(int pin) {
  int duration = pulseIn(pin, HIGH, 50000);
  int r = map(duration, 1000, 2000, -255, 255);
  if (r < -300 or r > 300) {
    return 0;
  }
  if (r > -20 and r < 20) {
    return 0;
  }
  return r;
}
bool sl = false;
bool sr = false;
int last_dir = 1;
void loop() {
  // COMPUTE X Y CONTROLS
  int x = -getCh(CH1);
  int y = -getCh(CH2);

  //Serial.print(x);
  //Serial.print(" ");
  //Serial.println(y);

  // MOTOR OUTPUT
  int d0 = y - x * .5;
  int d1 = y + x * .5;

  int m0 = constrain(d0, -255, 255);
  int m1 = constrain(d1, -255, 255);

  //Serial.print(m0);
  //Serial.print(" ");
  //Serial.println(m1);

  analogWrite(M0_PWM, abs(m0));
  digitalWrite(M0_1, m0 > 0);
  digitalWrite(M0_0, m0 < 0);

  analogWrite(M1_PWM, abs(m1));
  digitalWrite(M1_1, m1 > 0);
  digitalWrite(M1_0, m1 < 0);

  myservo.write(90 + 70 * ((float)x / (float)255) + servoCal);
}