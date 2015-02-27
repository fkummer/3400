// To find out sampling frequency with analog read
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
}

void loop() {
   int time = millis(); //start timer
   int counter = 0;
   while(counter<1024){
     float a = analogRead(A0);
   }
  int period = millis()-time; //end timer after 1024 samples
  float actualPeriod = period/1024; //count period for one sample
  float samplingFrequency = 1/actualPeriod; //get frequency
  Serial.print(samplingFrequency); //print frequency
   
}
