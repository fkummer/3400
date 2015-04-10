//Pins 3-9 will be used to output the bits corresponding to the coordinates
//7 bits because we broke the grid into 9x11, and use 0-99 as indices for a one-dimensional array
/*#define coord1 2
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

#define enable 2*/

//Most signifcant to least significant
int xPins[4] = {7, 6, 5, 4};

//Most signifcant to least significant
int yPins[4] = {A0, A1, A2, A3};

//Most(A2) to least(A3)
int conPins[2] = {3, 2};

int enable = A4;

int inputBuffer[2];

int byte1[8];

int byte2[8];

int maze[11][9] = {1, 2 ,0 ,1 ,0 ,0 ,0, 0, 0 ,0 ,0,
                   0, 0 ,0 ,0 ,0 ,0 ,0, 0, 0 ,0 ,0,
                   0, 0 ,0 ,0 ,0 ,0 ,0, 2, 0 ,0 ,0,
                   0, 0 ,0 ,0 ,0 ,0 ,0, 0, 0 ,0 ,0,
                   0, 0 ,0 ,2 ,0 ,0 ,0, 0, 1 ,0 ,0,
                   0, 0 ,0 ,0 ,0 ,1 ,0, 0, 0 ,0 ,0,
                   0, 0 ,1 ,0 ,0 ,0 ,2, 1, 0 ,1 ,0,
                   0, 0 ,0 ,0 ,0 ,0 ,0, 0, 0 ,0 ,0,
                   0, 0 ,0 ,0 ,0 ,0 ,0, 0, 0 ,0 ,0};


int count = 0;

int getPos(int x, int y){
  int col = x;
  int row = y;
  
  int content = maze[row][col];
  readIntoByte(byte1,7, col, 4);
  readIntoByte(byte1,3, row, 4);
  
  readIntoByte(byte2,7, content, 2);
  
  
 
}
void setup(){
  //Initialize x-coord pins as outputs
  for(int i = 0; i <= 3; i++){
    pinMode(xPins[i], OUTPUT);
  }
  
  //Initialize y-coord pins as outputs
  for(int i = 0; i <= 3; i++){
    pinMode(yPins[i], OUTPUT);
  }
  
  //Initialize content pins as outputs
  for(int i = 0; i <= 1; i++){
    pinMode(conPins[i], OUTPUT);
  }
  
  //Initalize enable pin as output
  pinMode(enable, OUTPUT);
  
  Serial.begin(9600);
}
int c = 0;
void loop(){
  if(Serial.available() > 0){
    Serial.read();
    for(int i = 10; i >= 0; i--){
     for(int j = 8; j >= 0; j--){
       getPos(i,j);
       printByteArray(byte1, "Byte 1:");
       printByteArray(byte2, "Byte 2:");
       outputData(xPins, 4, byte1, 7);
       outputData(yPins, 4, byte1, 3);
       outputData(conPins, 2, byte2, 1);
       while(!(Serial.available())){
         
       } 
       Serial.read();     
     }
   }
    
  }
  
 
    
    /*getPos(0,0);
    printByteArray(byte1, "Byte 1:");
    printByteArray(byte2, "Byte 2:");
    
    getPos(1,0);
    printByteArray(byte1, "Byte 1:");
    printByteArray(byte2, "Byte 2:");
    
    getPos(2,0);
    printByteArray(byte1, "Byte 1:");
    printByteArray(byte2, "Byte 2:");
    
    getPos(3,0);
    printByteArray(byte1, "Byte 1:");
    printByteArray(byte2, "Byte 2:");
    Serial.read();*/

  /*if(Serial.available() > 0){ //upon availability, do analysis
  
    inputBuffer[0] = possibleInputs[count]; //get this case input values
    inputBuffer[1] = possibleColors[count];
    
    for(int i = 0; i<8; i++){ //read bits
      
      byte1[i] = bitRead(inputBuffer[0], i);
      
      byte2[i] = bitRead(inputBuffer[1], i);
      
    } 
    
    printByteArray(byte1, "Byte 1:");
    
    printByteArray(byte2, "Byte 2:");
    
   outputData(xPins, 4, byte1, 7);
   outputData(yPins, 4, byte1, 3);
   outputData(conPins, 2, byte2, 1);
    
    digitalWrite(enable,HIGH); //set enable high for a single clock cycle
    digitalWrite(enable,LOW);
    if(count == 5){
      count = 0;
    }else{ 
      count = count++;
    }
    Serial.read();
 }*/

}  

//Prints an array of ints representing a byte, preceded by a message of your choice to identify the byte.
void printByteArray(int my_byte[], char name[]){
    Serial.print(name);
    for(int i = 7; i>=0; i--){
        Serial.print(my_byte[i], DEC);
    }
    
    Serial.println();
    return;
  
}

//Outputs a portion of byte, stored in my_byte, to the pins of the Arduino specified in the pins array.
//num indicates how many of the pins to output to, and startBit gives the bit to start from and then
//decrement from there.
void outputData(int pins[], int num, int my_byte[], int startBit){
    int currBit = startBit;
    for(int i = 0; i <= num-1; i++){
      digitalWrite(pins[i], my_byte[currBit]);
      currBit--;
    }
    digitalWrite(enable, HIGH);
    digitalWrite(enable, LOW);
}

void readIntoByte(int *my_byte, int startBit, int data , int num){
  int currBit = startBit;
  int currDataBit = num-1;
  for(int i = 0; i <= num - 1; i++){
    my_byte[currBit] = bitRead(data, currDataBit);
    currBit--;
    currDataBit--;
  }
}
