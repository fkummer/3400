int PINNAME = A0;
void setup(){
  Serial.begin(9600);
  pinMode(9, OUTPUT);
}

void loop(){
 
  int value = analogRead(PINNAME);
  Serial.println(value);
  int brightness = map(value, 0,1023, 0,255);
  analogWrite(9, brightness);
}
