/* 
 * This is the function responsible for the general line following logic.
 * It first reads the sensor values, determines the error based on our target
 * sensor reading by using a linear mapping function that maps the difference
 * between sensor readings to a range from -4 to 4. Additional logic at the end
 * of the function resposible for handling intersections has been omitted
 */
#include <Servo.h>
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */


#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"


Servo myservo1;  // left wheel
Servo myservo2; // right wheel

int right_direction;
int left_direction;

  // Variables for reading the line sensor values:
char qti_left;
char qti_right;
int left_side;
int right_side;

//nav algorithm vars
int adjacent_vis[3];
int visited[99];


int firstInt = LOW;
  // The relative error between our current position over the line and our
  // desired position (ranges from -4 to 4):
int error;

  // "I" and "D" terms for the equation:
  // (Note: Though we kept track of the "I" term, we did not actually make use
  // of it)
float err_sum = 0;
float err_diff = 0;
float prev_err = 0; // this is used for determining the error difference

char counter; //counter for identifying which signal is read from the mux

/*This code is for the IR sensor*/

#define UNEXPLORED 0
#define WALL 2
#define NO_WALL 1
#define UNREACHABLE 3

#define THRESH_1 1500
#define THRESH_2 1500


int maze [9][11];
byte currDirection = 0;
byte currX = 1;
byte currY = 1;
#define front_sensor A1
#define left_sensor A2
#define right_sensor A0
#define led  13
byte left_wall;
byte right_wall;
byte front_wall;


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
int send_maze[11];


// Maze array converted to bits
int bit_maze;

#define button 7
int started = LOW;
int startup = 0;

void updateMaze(){
  for(int y=2; y<7; y+=2){
    for(int x=2; x<9;  x+=2){
     if(maze[x+2][y+2]==WALL || maze[x+2][y+2]==WALL ||
 maze[x+2][y+2]==WALL || maze[x+2][y+2]==WALL){
       maze[x][y]=WALL;
     }
     else if(maze[x+2][y+2]==NO_WALL && maze[x+2][y+2]==NO_WALL &&
 maze[x+2][y+2]==NO_WALL && maze[x+2][y+2]==NO_WALL){
        maze[x][y]=NO_WALL;
     }
    }
  } 
}


void outputMaze(){
  for(int y=0; y<9; y++){
    Serial.print("[");
    for(int x=0; x<11; x++){
      Serial.print(maze[y][x]);
      Serial.print(", "); 
    }
    Serial.println("]");
  } 
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
  //delay(1000);
  radio.startListening(); 
  
}



void setup(){
  int iter;
  for(iter = 0; iter<99; iter ++){
	visited[iter] = 0;
  }
  
  
  Serial.begin(9600);
  //pinMode(front_sensor, INPUT);
  //pinMode(left_sensor,INPUT);
  //pinMode(right_sensor,INPUT);
  pinMode(led, OUTPUT);
  
  myservo1.attach(5);  // left wheel
  myservo2.attach(6); // right wheel
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);// A4 and A5 are for the front line sensors
  pinMode(A5, INPUT);
  
  pinMode(button, INPUT);
  pinMode(4,OUTPUT); //the following 3 pins work for the mux
  pinMode(3,OUTPUT);
  pinMode(2,OUTPUT);
  
  //qti_left = analogRead(A4); // left front sensor
  //qti_right = analogRead(A5); //right fron sensor
  //left_side = analogRead(A3); //left sensor
  //right_side = analogRead(A2); //right sensor
  myservo1.write(90);   //left sensor start speed
  myservo2.write(90);    //right sensor start speed 
  //delay(100000); //for testing
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
  started = digitalRead(button);
}

void loop(){
    //Serial.println("hello world");
    //Serial.println(started);
    if(started){
      if(startup == LOW){
        myservo1.write(100);
        myservo2.write(80);
        startup = HIGH;
      }
      //Serial.println("started");
      //mux();
     // First we read the line sensor values:
      //update_qtis(&qti_left, &qti_right);
      qti_left = analogRead(A4);
      qti_right = analogRead(A5);
      
      digitalWrite(4, HIGH); // assigns input digits to the mux
      digitalWrite(3, LOW);
      digitalWrite(2, HIGH);
      //delay(1000);
      left_side = analogRead(A3);
      right_side = analogRead(A2);
    
      error = qti_left - qti_right; 
      //myservo1.write(101);
      //myservo2.write(81);
      
      if (left_side > 750 && right_side > 750){
          delay(210);
          myservo1.write(90);
          myservo2.write(90);
          if(!firstInt){
            firstInt = HIGH;
          }else{
            updatePosition();
          }
          wallSense();
          transmitMaze();
          navigate();
          /*
          if(front_wall == 1){
            if (right_wall == 0){  
                turnRight();
                //delay(75);
                prev_err = 0;
            }else{
              if(left_wall == 0){
                turnLeft();
                //delay(30);
                prev_err = 0;
              }else{
                turnRight();
                //delay(50);
                turnRight();
                //delay(50);
                prev_err = 0;
              }
            }
          }
          */
        }  
      
      else{
        //Serial.println(error);
        
        if (error < 100 && error > -100) { 
          error = 0; 
          right_direction = (81 + 0.08*error);
          left_direction = (100 + 0.08*error);
        }
        else if (error > 100) { //veering to the right
          //right_direction = 80 + 0.1*error;
          //left_direction = 100 + 0.1*error;
          right_direction = (81 + 0.05*error + 0.005*err_diff);
          left_direction = (101 + 0.05*error + 0.005*err_diff);
        }
        else { //veering to the right
          right_direction = (81 + 0.055*error + 0.0055*err_diff);
          left_direction = (101 + 0.055*error + 0.0055*err_diff);
        }
      }
      //right_direction = 90;  //for tuning the servos
      //left_direction = 90;
      
      //Serial.222222println(error);
      myservo1.write(left_direction); //uncomment after adjusting servos
      myservo2.write(right_direction); 
      
      //myservo1.write(90); //for adjusting servos
      //myservo2.write(90);
      err_diff = error - prev_err;
      prev_err = error;  // Save this "P" term to determine the next "D" term
      
      
   
    //delay(500);*/
   }else{
    started = digitalRead(button); 
    //Serial.println("waiting"); 
   }
}



void turnLeft(){
        //delay(400);
        myservo1.write(83);
        myservo2.write(83);
        delay(590);
        //myservo1.write(90);
        //myservo2.write(90);
        while (analogRead(A5) < 750); 
        while (analogRead(A4) > 750);
        /*  qti_right = analogRead(A5);
          Serial.println((int)qti_right);
        } */
        //delay(50);
        //left_direction = 90;
        //right_direction = 90;
        //delay(500);
        currDirection = (currDirection+3)%4;
        //Serial.println("direction");
        //Serial.println(currDirection);
        prev_err=0;
}

void turnRight() { 
        //delay(400);
        myservo1.write(97);
        myservo2.write(97);
        delay(590);
        //myservo1.write(90);
        //myservo2.write(90);
        while (analogRead(A4) < 750);
        while (analogRead(A5) > 750);
        /*  qti_left = analogRead(A4);
          Serial.println((int)qti_left);
        } */
        //delay(100);
        //left_direction = 90;
        //right_direction = 90;
        //delay(500);
        currDirection = (currDirection+1)%4;
        //Serial.println("direction");
        //Serial.println(currDirection);
        prev_err=0;
}

void updatePosition(){
 if (currDirection == 0){
  currY -= 2;
 } 
 else if(currDirection == 1){
  currX += 2; 
 }
 else if(currDirection == 2){
  currY += 2; 
 }
 else if(currDirection == 3){
   currX -= 2;
 } 
}


void wallSense(){
    digitalWrite(4, HIGH); // assigns input digits to the mux
    digitalWrite(3, HIGH);
    digitalWrite(2, HIGH);
    //delay(1000);
    int left = 0;
    int right = 0;
    int front = 0;
    for (int avsd=0;avsd<10;avsd++) {
    left += analogRead(left_sensor);
    right += analogRead(right_sensor);
    front += analogRead(front_sensor);
    //Serial.println(analogRead(left_sensor));
    //Serial.println(analogRead(right_sensor));
    //Serial.println(analogRead(front_sensor));
    //closest wall 82
    //further wall 40
    }
    maze[currY][currX]=NO_WALL;
    switch (currDirection) {
      case 0: //robot facing up
        if(left>THRESH_2){
          if(left>THRESH_1){
            maze[currY][currX-1]=WALL;
          } else{
            maze[currY][currX-1]=NO_WALL;
            //maze[currY][currX-3]=WALL;
          }
        }else{
          maze[currY][currX-1]=NO_WALL;
        }
        
        if(right>THRESH_2){
          if(right>THRESH_1){
            maze[currY][currX+1]=WALL;
          }else{
            maze[currY][currX+1]=NO_WALL;
            //maze[currY][currX+3]=WALL;
          }
        }else{
          maze[currY][currX+1]=NO_WALL;
        }
        
        if(front>THRESH_2){
          if(front>THRESH_1){
            maze[currY-1][currX]=WALL;
          }else{
            maze[currY-1][currX]=NO_WALL;
            //maze[currY-3][currX]=WALL;
          }
        }else{
          maze[currY-1][currX]=NO_WALL;
        }
        break;
        
        
      case 1: //robot facing right
      if(left>THRESH_2){
          if(left>THRESH_1){
            maze[currY-1][currX]=WALL;
          } else{
            maze[currY-1][currX]=NO_WALL;
            //maze[currY-3][currX]=WALL;
          }
        }else{
          maze[currY-1][currX]=NO_WALL;
        }
        
        if(right>THRESH_2){
          if(right>THRESH_1){
            maze[currY+1][currX]=WALL;
          }else{
            maze[currY+1][currX]=NO_WALL;
            //maze[currY+3][currX]=WALL;
          }
        }else{
          maze[currY+1][currX]=NO_WALL;
        }
        
        if(front>THRESH_2){
          if(front>THRESH_1){
            maze[currY][currX+1]=WALL;
          }else{
            maze[currY][currX+1]=NO_WALL;
            //maze[currY][currX+3]=WALL;
          }
        }else{
          maze[currY][currX+1]=NO_WALL;
        }
        break;
        
        
      case 2: //robot facing down
      if(left>THRESH_2){
          if(left>THRESH_1){
            maze[currY][currX+1]=WALL;
          }else{
            maze[currY][currX+1]=NO_WALL;
            //maze[currY][currX+3]=WALL;
          }
        }else{
          maze[currY][currX+1]=NO_WALL;
        }
        
        if(right>THRESH_2){
          if(right>THRESH_1){
            maze[currY][currX-1]=WALL;
          }else{
            maze[currY][currX-1]=NO_WALL;
            //maze[currY][currX-3]=WALL;
          }
        }else{
          maze[currY][currX-1]=NO_WALL;
        }
        
        if(front>THRESH_2){
          if(front>THRESH_1){
            maze[currY+1][currX]=WALL;
          }else{
            maze[currY+1][currX]=NO_WALL;
            //maze[currY+3][currX]=WALL;
          }
        }else{
          maze[currY+1][currX]=NO_WALL;
        }
        break;
      
      case 3: //robot facing left
      
      
      if(left>THRESH_2){
          if(left>THRESH_1){
            maze[currY+1][currX]=WALL;
          }else{
            maze[currY+1][currX]=NO_WALL;
            //maze[currY+3][currX]=WALL;
          }
        }else{
          maze[currY+1][currX]=NO_WALL;
        }
        
        if(right>THRESH_2){
          if(right>THRESH_1){
            maze[currY-1][currX]=WALL;
          }else{
            maze[currY-1][currX]=NO_WALL;
            //maze[currY-3][currX]=WALL;
          }
        }else{
          maze[currY-1][currX]=NO_WALL;
        }
        
        if(front>THRESH_2){
          if(front>THRESH_1){
            maze[currY][currX-1]=WALL;
          }else{
            maze[currY][currX-1]=NO_WALL;
            //maze[currY][currX-3]=WALL;
          }
        }else{
          maze[currY][currX-1]=NO_WALL;
        }
        break;
      default:
        break;
    } 
      
    left_wall = (left>THRESH_2) ? ((left>THRESH_1) ? 1 : 2) : 0;
    right_wall = (right>THRESH_2) ? ((right>THRESH_1) ? 1 : 2) : 0;
    front_wall = (front>THRESH_2) ? ((front>THRESH_1) ? 1 : 2) : 0;
    
    Serial.println("front:");
    Serial.println(front_wall);
    Serial.println("left:");
    Serial.println(left_wall);
    Serial.println("right:");
    Serial.println(right_wall);
   // outPutMaze();
    
}



int getVisitedFront() {
	if (currDirection == 0)
		return maze[currY-2][currX];
	else if (currDirection == 1)
		return maze[currY][currX+2];
	else if (currDirection == 2)
		return maze[currY+2][currX];
	else
		return maze[currY][currX-2];
}

int getVisitedRight() {
	if (currDirection == 0)
		return maze[currY][currX+2];
	else if (currDirection == 1)
		return maze[currY+2][currX];
	else if (currDirection == 2)
		return maze[currY][currX-2];
	else
		return maze[currY-2][currX];
}

int getVisitedLeft() {
	if (currDirection == 0)
		return maze[currY][currX-2];
	else if (currDirection == 1)
		return maze[currY-2][currX];
	else if (currDirection == 2)
		return maze[currY][currX+2];
	else
		return maze[currY+2][currX];
}

int getVisitedBehind() {
	if (currDirection == 0)
		return maze[currY+2][currX];
	else if (currDirection == 1)
		return maze[currY][currX-2];
	else if (currDirection == 2)
		return maze[currY-2][currX];
	else
		return maze[currY][currX+2];
}



void goTowards(byte dir){
   byte diff = (4+dir-(byte)currDirection)%4;
   if (diff==1){
     turnRight(); 
   }
   else if(diff==2){
     turnRight();
     turnRight();
   }
   else if(diff==3){
     turnLeft();
   }
}

byte queue[30];
byte pointer = 0;

void push(byte value){
  queue[pointer]=value;
  pointer++;
}

byte pop(){
  pointer--;
  return queue[pointer];
}

void pushLeft(){
  push(((byte)currDirection+3)%4);
}

void pushFront(){
  push((byte)currDirection);
}


void pushRight(){
  byte x=((byte) currDirection+1)%4;
  push(x);
}

void pushBack(){
   push(((byte)currDirection+2)%4); 
}

char hasVisited(byte k){
  if (k==0){
    return maze[currY-2][currX];
  }
  else if(k==1){
    return maze[currY][currX+2];
  }
  else if(k==2){
    return maze[currY+2][currX];
  }
  else{
    return maze[currY][currX-2]; 
  }
}

void navigate(){
  if(getSensorLeft()==0){
    if(getVisitedLeft()==0){
      pushRight();
      pushLeft();
    }
  }
  if((getSensorRight()==0)){
    if(getVisitedRight()==0){
       pushLeft();
       pushRight();
    }
  }
  if(getSensorFront()==0){
    if(getVisitedFront()==0){
       pushBack();
       pushFront();
    }
  }
  if(pointer>0){
    byte next = pop();
    /*
    while (!hasVisited(next)==0){
      if(pointer>0){
        next = pop();
      }
      else{
        Serial.println("done????");
        while(1){
         outputMaze();
        }
        delay(5000); 
      }
    }
    */
    goTowards(next);
  }
 
  else{
    Serial.println("done!!!");
    while(1){
     outputMaze();
    }
    delay(5000);
  }
}

int getSensorFront(){
    if (front_wall) return 1; else return 0;  
}

int getSensorLeft(){
    if (left_wall) return 1; else return 0;  
}

int getSensorRight(){
    if (right_wall) return 1; else return 0;  
}
