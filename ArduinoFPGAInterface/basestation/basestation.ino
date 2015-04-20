
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example for Getting Started with nRF24L01+ radios.
 *
 * This is an example of how to use the RF24 class.  Write this sketch to two
 * different nodes.  Put one of the nodes into 'transmit' mode by connecting
 * with the serial monitor and sending a 'T'.  The ping node sends the current
 * time to the pong node, which responds by sending the value back.  The ping
 * node can then see how long the whole cycle took.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x000000001CLL, 0x000000001DLL };

// Array of maze is initially all unexplored with walls around it

int maze[9][11];

// Maze array converted to bits
unsigned long bit_maze;


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
                 

int count = 0;

int getPos(int x, int y){
  int col = x;
  int row = y;
  Serial.println(col);
  Serial.println(row);
  int content = maze[row][col];
  readIntoByte(byte1,7, col, 4);
  readIntoByte(byte1,3, row, 4);
  readIntoByte(byte2,7, content, 2);
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

void setup(void)
{
  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_HIGH);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

 
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
 

  //
  // Start listening
  //

  radio.startListening();
  
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



void loop(void)
{

  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //
    //if(radio.read(&bit_maze, sizeof(unsigned long))){
     //Serial.println("Radio Read");
    //}
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      Serial.println("Radio Available");
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        Serial.println("Reading");
        done = radio.read( &bit_maze, sizeof(unsigned long) );
        Serial.println(bit_maze);
      }
      
      // Take bits off integer to make back into original array
      int c = 0;
      for (int n = 8; n >= 0; n--){
        for(int m = 10; m >= 0; m--){
          //Serial.println(bit_maze & 0x3);
          if((bit_maze & 0x3) == 0){               // 0 = unexplored
            maze[n][m] = 0;
            bit_maze >> 2;
            Serial.println("Zero");
            
          }
          else if ((bit_maze & 0x3) == 1){         // 1 = no wall
            maze[n][m] = 1;
            bit_maze >> 2;
            Serial.println("One");
          }
          else if ((bit_maze & 0x3) == 2){         // 2 = wall
            maze[n][m] = 2;
            bit_maze >> 2;
            Serial.println("Two");
          }
          else if ((bit_maze & 0x3) == 3){
            maze[n][m] = 3;
            bit_maze >> 2;
            Serial.println("Three");
          }else{
            Serial.println("Bad Info");
          }
          c++;
          Serial.println(c);
        }
      }
      
      
       
          for(int i = 10; i >= 0; i--){
          for(int j = 8; j >= 0; j--){
             getPos(i,j);
             printByteArray(byte1, "Byte 1:");
             printByteArray(byte2, "Byte 2:");
             outputData(xPins, 4, byte1, 7);
             outputData(yPins, 4, byte1, 3);
             outputData(conPins, 2, byte2, 7);
           
          }
      
          
         }
      

  
    }
    
   /*
    //if(Serial.available() > 0){
      //Serial.read();
      for(int i = 10; i >= 0; i--){
      for(int j = 8; j >= 0; j--){
         getPos(i,j);
         printByteArray(byte1, "Byte 1:");
         printByteArray(byte2, "Byte 2:");
         outputData(xPins, 4, byte1, 7);
         outputData(yPins, 4, byte1, 3);
         outputData(conPins, 2, byte2, 7);
         //while(!(Serial.available())){
           
         //} 
         //Serial.read();     
      }
    //}
      
   }
  */


}
// vim:cin:ai:sts=2 sw=2 ft=cpp

