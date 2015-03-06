//Pins 3-9 will be used to output the bits corresponding to the coordinates
//7 bits because we broke the grid into 9x11, and use 0-99 as indices for a one-dimensional array
#define coord1 2
#define coord2 3
#define coord3 4
#define coord4 5
#define coord5 6
#define coord6 7
#define coord7 8
#define coord8 9

//Pins 10 and 11 will output the bits corresponding to the color of the square
//Possible states are wall
#define color1 10
#define color2 11

//Pin 12 is used for the parity bit
//#define parity 12

#define enable 12

int inputBuffer[2];
int byte1[8];
int byte2[8];

void setup(){
  pinMode(coord1, OUTPUT);
  pinMode(coord2, OUTPUT);
  pinMode(coord3, OUTPUT);
  pinMode(coord4, OUTPUT);
  pinMode(coord5, OUTPUT);
  pinMode(coord6, OUTPUT);
  pinMode(coord7, OUTPUT);
  pinMode(coord8, OUTPUT);
  pinMode(color1, OUTPUT);
  pinMode(color2, OUTPUT);
  //pinMode(OUTPUT, parity);
  pinMode(enable, OUTPUT);
  Serial.begin(9600);
}

void loop(){

  if(Serial.available() > 1){
    
 
    inputBuffer[0] = 0x54; 
    inputBuffer[1] = 0x05;
    
    //byte mask = 0x01;
    for(int i = 0; i<8; i++){
      
      byte1[i] = bitRead(inputBuffer[0], i);
      
      byte2[i] = bitRead(inputBuffer[1], i);
      
    }
    
   Serial.print("Byte 1:");
    for(int i = 7; i>=0; i--){
      
      Serial.print(byte1[i], DEC);
      
    }
    Serial.println();
    Serial.print("Byte 2:");
    for(int i = 7; i>=0; i--){
      
     Serial.print(byte2[i], DEC);
      
    }
    Serial.println();
    
    inputBuffer[0] = 0x00;
    inputBuffer[1] = 0x00;
    Serial.read();
    Serial.read();
    
    int currPin = 2;
  for(int i = 0; i<8; i++){
    Serial.println(currPin);
    if(byte1[i] == 1){
      digitalWrite(currPin, HIGH);
    }else{
      digitalWrite(currPin, LOW);
    }
    currPin++;
  }
  
  for(int j = 0; j<3; j++){
    Serial.println(currPin);
    if(byte2[j] == 1){
      digitalWrite(currPin, HIGH);
    }else{
      digitalWrite(currPin, LOW);
    }
    currPin++;
  }
  }
  
  
  
    

}  
