/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

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
/*{
  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
  {2, 1, 0, 3, 0, 0, 0, 0, 0, 0, 2},
  {2, 0, 1, 0, 3, 0, 0, 0, 0, 0, 2},
  {2, 0, 0, 1, 0, 3, 0, 0, 0, 0, 2},
  {2, 0, 0, 0, 1, 0, 3, 0, 0, 0, 2},
  {2, 0, 0, 0, 0, 1, 0, 3, 0, 0, 2},
  {2, 2, 2, 0, 0, 0, 1, 0, 3, 0, 2},
  {2, 3, 2, 0, 0, 0, 0, 1, 0, 3, 2},
  {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
};*/

int send_maze[11];


// Maze array converted to bits
int bit_maze;




void setup(void)
{
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)


    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);

  //
  // Dump the configuration of the rf unit for debugging
  //
   

  radio.printDetails();
}

//Transmits an array of 11 integers, with the bits of each representing the state of each position of the 11 columns of the maze.
//Since an integer is only 16 bits, only 8 rows can be sent. The last row will always be a wall though, so that information does not need to be sent.
void transmitMaze(void){
  radio.stopListening();
    //
    // Ping out role. Robot sending data to base station
    //
    
    // Create array to be sent to base station 
    for(int m = 0; m <= 10; m++){
     for(int n = 0; n <= 7; n++){
        if (maze[n][m] == 0){          // 0 = unexplored
          bit_maze = bit_maze << 2;    // add 00 onto the end of the bit_maze
          bit_maze = bit_maze | 0;
        }
        else if (maze[n][m] == 1){     // 1 = No wall
          bit_maze = bit_maze << 2;    // add 01 onto the end of the bit_maze
          bit_maze = bit_maze | 1;
        }
        else if (maze[n][m] == 2){     // 2 = Wall
          bit_maze = bit_maze << 2;    // add 10 onto the end of the bit_maze
          bit_maze = bit_maze | 2;
        }
        else{                          // 3 is not a valid number
          bit_maze = bit_maze << 2;    // add 11 onto the end of the bit_maze
          bit_maze = bit_maze | 3;
        }
     }
     
     send_maze[m] = bit_maze;
     bit_maze = 0;
    }
    
    //Transmit the array
    bool ok = radio.write(&send_maze, sizeof(send_maze));
  
    bit_maze = 0;
    /*for (int n = 0; n <= 7; n++){
      for(int m = 0; m <= 10; m++){
        maze[n][m] += 1;
        if (maze[n][m] == 4){
           maze [n][m] = 0;    
        } 
      }
    }*/
  delay(1000);
  radio.startListening(); 
  
}

void loop(void)
{
    Serial.println("Transmit");
    transmitMaze();

}
