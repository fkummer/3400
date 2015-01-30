#include <Servo.h>

Servo s;

void setup(){
  s.attach(9);
}

void loop(){
  s.write(90);
}
