
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

unsigned char maze[11][9];

// Maze array converted to bits
unsigned long bit_maze;

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
 
}



void loop(void)
{

  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &bit_maze, sizeof(unsigned long) );
      }
      
      // Take bits off integer to make back into original array
      for (int n = 11; n > 0; n--){
        for(int m = 9; m > 0; m--){
          if(bit_maze & 0x3 == 0){               // 0 = unexplored
            maze[n][m] = 0;
            bit_maze >> 2;
            
          }
          else if (bit_maze & 0x3 == 1){         // 1 = no wall
            maze[n][m] = 1;
            bit_maze >> 2;
           
          }
          else if (bit_maze & 0x3 == 2){         // 2 = wall
            maze[n][m] = 2;
            bit_maze >> 2;
            
          }
          else if (bit_maze & 0x3 == 3){
            maze[n][m] = 3;
            bit_maze >> 2;
            
          }
        }
      }
  

    }

}
// vim:cin:ai:sts=2 sw=2 ft=cpp

