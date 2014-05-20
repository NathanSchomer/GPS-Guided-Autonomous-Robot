#define AVOID_ADJUST_MIN 0
#define AVOID_ADJUST_MAX 180
#define LEFT_IR_PIN 0
#define RIGHT_IR_PIN 1

void setup() {
}

void loop()
{
  double avoidLEFT = map(analogRead(LEFT_IR_PIN), 0, 1023, AVOID_ADJUST_MIN, AVOID_ADJUST_MAX);
  double avoidRIGHT = map(analogRead(RIGHT_IR_PIN), 0, 1023, AVOID_ADJUST_MIN, AVOID_ADJUST_MAX);
}
