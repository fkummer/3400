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

int maze[9][11] =
{
{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
};
//{
//{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
//{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
//{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
//{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
//{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
//{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
//{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
//{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
//};



// Maze array converted to bits
long bit_maze;




void setup(void)
{
  //
  // Print preamble
  //
    Serial.println("preamble");
 
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
  radio.setPALevel(RF24_PA_MAX);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);
  
  radio.setPayloadSize(25);

 

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)


    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
 
   Serial.begin(57600);


  //
  // Dump the configuration of the rf unit for debugging
  //
   radio.stopListening();

  radio.printDetails();
}

void loop(void)
{
  Serial.println("void loop begins");
  printf("void loop begins");
  //
  // Ping out role. Robot sending data to base station
  //
  
  for (int n = 0; n <= 8; n++){
        for(int m = 0; m <= 10; m++){
          maze[n][m] = 2;
          
        }
  }
  Serial.print("Two added");
  
  
  
  
  
  while(1){
//    bit_maze = 0;
//        // Take numbers off array and add onto integer two bits at a time
//    for (int n = 0; n <= 8; n++){
//        for(int m = 0; m <= 10; m++){
//          if (maze[n][m] == 0){          // 0 = unexplored
//            bit_maze = bit_maze << 2;
//            Serial.println(bit_maze | 0);    // add 00 onto the end of the bit_maze
//            bit_maze = bit_maze | 0;
//            Serial.println("Zero");
//          }
//          else if (maze[n][m] == 1){     // 1 = No wall
//            bit_maze = bit_maze << 2;
//            Serial.println(bit_maze | 1);    // add 01 onto the end of the bit_maze
//            bit_maze = bit_maze | 1;
//            Serial.println("One");
//          }
//          else if (maze[n][m] == 2){     // 2 = Wall
//            bit_maze = bit_maze << 2;
//            Serial.println(bit_maze | 2);    // add 10 onto the end of the bit_maze
//            bit_maze = bit_maze | 2;
//            Serial.println("Two");
//          }
//          else{                          // 3 is not a valid number
//            bit_maze = bit_maze << 2;    // add 11 onto the end of the bit_maze
//            bit_maze = bit_maze | 3;
//            Serial.println("Three");
//          }
//          
//        }
//    }
//    
//    for (int n = 8; n >= 0; n--){
//        for(int m = 10; m >= 0; m--){
//          Serial.print(n);
//            Serial.print(",");
//            Serial.println(m);
//            
//            Serial.println((bit_maze & 0x3));
//          if(bit_maze & 0x3 == 0){               // 0 = unexplored
//            Serial.print(n);
//            Serial.print(",");
//            Serial.println(m);
//            
//            Serial.println((bit_maze & 0x3));
//            
//            
//            bit_maze >> 2;
//            Serial.println("Zero");
//            
//          }
//          else if ((bit_maze & 0x3) == 1){         // 1 = no wall
//            bit_maze >> 2;
//            Serial.print(n);
//            Serial.print(",");
//            Serial.println(m);
//            
//            Serial.println((bit_maze & 0x3));
//            Serial.println("One");
//          }
//          else if ((bit_maze & 0x3) == 2){         // 2 = wall
//            bit_maze >> 2;
//            Serial.print(n);
//            Serial.print(",");
//            Serial.println(m);
//            
//            Serial.println((bit_maze & 0x3));
//            Serial.println("Two");
//          }
//          else if ((bit_maze & 0x3) == 3){
//            bit_maze >> 2;
//            Serial.print(n);
//            Serial.print(",");
//            Serial.println(m);
//            
//            Serial.println((bit_maze & 0x3));
//            Serial.println("Three");
//          } else{Serial.println("bad info");}        }
//      }
//  
//  
//   
    
      // keep sending message until recieved 
      Serial.println("Transmitting");
     bool ok=0;
    while (!ok){
      ok = radio.write(&maze, sizeof(maze) );
     
    } 
     Serial.println("Transmited");
//    for (int n = 0; n <= 8; n++){
//        for(int m = 0; m <= 10; m++){
//          maze[n][m] = 2;
//          Serial.print("Two");
//        }
//     }
   }

}
