/*
fft_adc_serial.pde
guest openmusiclabs.com 7.7.14
example sketch for testing the fft library.
it takes in data on ADC0 (Analog0) and processes them
with the fft. the data is sent out over the serial
port at 115.2kb.
*/

#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <FFT.h> // include the library

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter
}

void loop() {
  while(1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples

      float k = analogRead(A0);
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei();
    Serial.println("start");
    float j = 9;
    for (byte i = 9; i < FFT_N/2 ; i++) {
      Serial.println();
      Serial.print("Frequency \t");
      Serial.print(j*8200/256);
      Serial.print("\t"); 
      float amp = fft_log_out[i]/64.0;
      Serial.println(amp); // send out the data
      if(i >= 20 && i<= 22){
        if(amp >= .7){
          digitalWrite(13, HIGH);
        }
      }
      j++;

    }
    Serial.flush();
  }
}
