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

int n;
int x_coord = 5;  // Can range from 0 (all the way left) to 9 (all the way right)
int y_coord = 11; // Can range from 0 (top of maze) to 11 (bottom of maze)
int data = 5;    // Can range from 0 to 7 each number represents different wall configurations at a coordinate
int dir = 0;     // Can range from 0 to 3 each number represents a direction the robot is facing

unsigned long coord_info = 0; // The combined coordinates and information that are sent through the radio
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_pong_back;

void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

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

  if ( role == role_ping_out )
  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  }
  else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
}

void loop(void)
{
  //
  // Ping out role.  Repeatedly send the current time
  //
  
  // Add X coordinates to coord_info (4 bits)
  coord_info |= x_coord;
  coord_info << 4;
      
  coord_info |= y_coord;
  coord_info << 4;
      
  coord_info |= data;
  coord_info << 3;
  
  coord_info |= dir;
      
      
  /*for (n = 0; n <= 9; n++){
  if(x_coord == n){
      coord_info |= n;
      coord_info << 4;
    }
  }
  
  // Add Y coordinates to coord_info (4 bits)
  for (n = 0; n <= 11; n++){
  if(y_coord == n){
      coord_info |= n;
      coord_info << 4;
    }
  }
  
  // Add data to coord_info (3 bits)
  for (n = 0; n <= 7; n++){
  if(data == n){
      coord_info |= n;
      coord_info << 3;
    }
  }
  
  // Add direction information to coord_info (2 bits)
  for (n = 0; n <= 3; n++){
  if(dir == n){
      coord_info |= n;
      coord_info << 2;
    }
  } */
    
  if (role == role_ping_out)
  {
    // First, stop listening so we can talk.
    radio.stopListening();

    // Take the time, and send it.  This will block until complete
    printf("X coordinate %i ", x_coord);
    printf("Y coordinate %i ", y_coord);
    printf("data = %i ", data);
    printf("Direction = %i ", dir);
    printf("Now sending %lu...",coord_info);
    
    bool ok = radio.write( &coord_info, 13 );

    if (ok)
      printf("ok...");
    else
      printf("failed.\n\r");

    // Now, continue listening
    radio.startListening();

    // Wait here until we get a response, or timeout (250ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n\r");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      unsigned long got_time;
      radio.read( &coord_info, sizeof(unsigned long) );

      // Spew it
      printf("Got response %ul, round-trip delay: %lu\n\r",coord_info);
    }

    // Try again 1s later
    delay(1000);
  }

  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

  if ( role == role_pong_back )
  {
    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      unsigned long got_time;
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &coord_info, sizeof(unsigned long) );
      
        // Spew it
        printf("Got payload %lu...",coord_info);

        // Delay just a little bit to let the other unit
        // make the transition to receiver
        delay(20);

      }   
      
      coord_info =0;
      
      // Get the x coordinate off coord_info
      for (int n = 0; n <= 3; n++) { 
        if (coord_info & 0x11 == n) { 
          dir = n;
          coord_info >> 2; 
        }
      }
      
      // Get the y coordinate off coord_info
      for (int n = 0; n <= 7; n++) { 
        if (coord_info & 0x111 == n) { 
          data = n;
          coord_info >> 3; 
        }
      }
      
      // Get the data off coord_info
      for (int n = 0; n <= 11; n++) { 
        if (coord_info & 0x1111 == n) { 
          y_coord = n;
          coord_info >> 4; 
        }
      }
      
      // Get the direction off coord_info
      for (int n = 0; n <= 9; n++) { 
        if (coord_info & 0x1111 == n) { 
          x_coord = n;
          coord_info >> 4; 
        }
      }
      
      
      printf("X coordinate = %i ", x_coord);
      printf("Y coordinate = %i ", y_coord);
      printf("data = %i ", data);
      printf("direction = %i ", dir);
      
      // First, stop listening so we can talk
      radio.stopListening();

      // Send the final one back.
      radio.write( &coord_info, sizeof(unsigned long) );
      printf("Sent response.\n\r");

      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }
  }

  //
  // Change roles
  //

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back )
    {
      printf("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n\r");

      // Become the primary transmitter (ping out)
      role = role_ping_out;
      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1,pipes[1]);
    }
    else if ( c == 'R' && role == role_ping_out )
    {
      printf("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n\r");

      // Become the primary receiver (pong back)
      role = role_pong_back;
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1,pipes[0]);
    }
  }
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
