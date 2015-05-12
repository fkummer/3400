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


//Initializes a two dimensional array to hold information about the maze
int maze[9][11];

//correct start is x=9; y=1; dir=3

byte currDirection = 3;
byte currX = 9;
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
     if(maze[y][x+1]==WALL || maze[y][x-1]==WALL ||
 maze[y+1][x]==WALL || maze[y-1][x]==WALL){
       maze[y][x]=WALL;
     }
     else if(maze[y][x+1]==NO_WALL && maze[y][x-1]==NO_WALL &&
 maze[y+1][x]==NO_WALL && maze[y-1][x]==NO_WALL){
        maze[y][x]=NO_WALL;
     }
    }
  } 
}

void updateEdges(){
  //maze[1][10]=WALL;
  for(byte r=0; r<9;r++){
     maze[r][0]=WALL;
     maze[r][10]=WALL;
  }
  for(byte c=0; c<11; c++){
     maze[0][c]=WALL;
     maze[8][c]=WALL;
  }
}

//Outputs the maze array to the serial monitor
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
    
    // Create array to be sent to base station 
    for(int m = 0; m <= 10; m++){
     for(int n = 0; n <= 7; n++){
        if (maze[n][m] == 0){          // 0 = unexplored
          bit_maze = bit_maze << 2;    // add 00 onto the end of the bit_maze and shift it so the next two bits can be added
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
     
     send_maze[m] = bit_maze;		   // Put the integer for the column onto the array to be sent
     bit_maze = 0;
     
    }
    
    
    
    
    //Transmit the array
    bool ok = radio.write(&send_maze, sizeof(send_maze));
  
    bit_maze = 0;					   // Reset bit_maze for next transmission
    radio.startListening(); 		   // Need to start and stop listening in between transmissions
  
}



void setup(){
  int iter;
  for(iter = 0; iter<99; iter ++){
	visited[iter] = 0;
  }
  
  
  Serial.begin(9600);
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
  

  myservo1.write(90);   //left sensor start speed, starts stoppped
  myservo2.write(90);    //right sensor start speed , starts stopped.
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
  updateEdges();
}

void loop(){
    //Do not start running maze-mapping code until the start button is pressed.
    if(started){
      //If this is the first iteration of the loop, begin driving forward at the default speed.
      if(startup == LOW){
        myservo1.write(100);
        myservo2.write(80);
        startup = HIGH;
      }
    
     // First we read the line sensor values:
      qti_left = analogRead(A4);
      qti_right = analogRead(A5);
      
      //Sets mux to read line sensors
      digitalWrite(4, HIGH); // assigns input digits to the mux
      digitalWrite(3, LOW);
      digitalWrite(2, HIGH);
      
      //Read left and right sensors to determine if we're on an intersection.
      left_side = analogRead(A3);
      right_side = analogRead(A2);
      
      //Determine current error for PID.
      error = qti_left - qti_right; 
      
      //If we are on an intersection, move onto it, stop, updapte our position and our map of the maze, and then turn appropriately.
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
        }  
      
      else{
        //If we are not on an intersection, but our error is significant, correct for it.  
        if (error < 100 && error > -100) { 
          error = 0; 
          right_direction = (81 + 0.08*error);
          left_direction = (100 + 0.08*error);
        }
        else if (error > 100) { //veering to the right
          right_direction = (81 + 0.05*error + 0.005*err_diff);
          left_direction = (101 + 0.05*error + 0.005*err_diff);
        }
        else { //veering to the right
          right_direction = (81 + 0.055*error + 0.0055*err_diff);
          left_direction = (101 + 0.055*error + 0.0055*err_diff);
        }
   

      myservo1.write(left_direction); 
      myservo2.write(right_direction); 
      
 
      err_diff = error - prev_err;
      prev_err = error;  // Save this "P" term to determine the next "D" term
      
   }else{
     //Read start button.
    started = digitalRead(button); 
    
   }
}


//Turns the robot left by 90 degrees
void turnLeft(){
        myservo1.write(83);
        myservo2.write(83);
        delay(300);
        while (analogRead(A5) < 750);//Turn until the right-front sensor is over the black. 
        myservo1.write(90);
        myservo2.write(90);
        currDirection = (currDirection+3)%4;
        prev_err=0;
}

//Turns the robot right by 90 degrees.
void turnRight() { 
        myservo1.write(97);
        myservo2.write(97);
        delay(300);
        while (analogRead(A4) < 750);//Turn until the left-front sensor is over the black.
        myservo1.write(90);
        myservo2.write(90);
        currDirection = (currDirection+1)%4;
        prev_err=0;
}

//Updates the robots current position in the maze at intersections based on the direction it last moved in.
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
          }
        }else{
          maze[currY][currX-1]=NO_WALL;
        }
        
        if(right>THRESH_2){
          if(right>THRESH_1){
            maze[currY][currX+1]=WALL;
          }else{
            maze[currY][currX+1]=NO_WALL;
          }
        }else{
          maze[currY][currX+1]=NO_WALL;
        }
        
        if(front>THRESH_2){
          if(front>THRESH_1){
            maze[currY-1][currX]=WALL;
          }else{
            maze[currY-1][currX]=NO_WALL;
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
          }
        }else{
          maze[currY][currX+1]=NO_WALL;
        }
        
        if(right>THRESH_2){
          if(right>THRESH_1){
            maze[currY][currX-1]=WALL;
          }else{
            maze[currY][currX-1]=NO_WALL;
          }
        }else{
          maze[currY][currX-1]=NO_WALL;
        }
        
        if(front>THRESH_2){
          if(front>THRESH_1){
            maze[currY+1][currX]=WALL;
          }else{
            maze[currY+1][currX]=NO_WALL;
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


byte DDist[20];
byte DPath[20];
byte DQ[20];

byte DQueue[2];
byte DPointer = 0;


byte DQMin(){
  byte minVal=123;
  byte minI=123;
  for(byte c=0; c<20; c++){
    if(DQ[c] && DDist[c]<minVal){
       minVal=DDist[c];
       minI=c;   
    }
  }
  return minI;
}


void Dijkstra(){
 byte currNode;
 byte newDist;
 byte x;
 byte y;
 
  for (byte k=0; k<20; k++){
    x=(k%5)*2+1;
    y=(k/5)*2+1;
    if (x==currX && y==currY){
      DDist[k]=0;
      DPath[k]=123;
    }
    else{
      DDist[k]=123;
      DPath[k]=123;
    }
    DQ[k]=1;
  }
  currNode=DQMin();
  while(currNode!=123){
    DQ[currNode]=0;
    if(DDist[currNode]!=123){
      newDist=DDist[currNode]+1;
      x=(currNode%5)*2+1;
      y=(currNode/5)*2+1;
      if((y>1) && (maze[y-1][x]!=2) && (newDist<DDist[currNode-5])){
        DDist[currNode-5]=newDist;
        DPath[currNode-5]=0;
      }
      if((x>1) && (maze[y][x-1]!=2) && (newDist<DDist[currNode-1])){
        DDist[currNode-1]=newDist;
        DPath[currNode-1]=3;
      }
      if(x<9) {
        if (maze[y][x+1]!=2){
          if(newDist<DDist[currNode+1]){
            DDist[currNode+1]=newDist;
            DPath[currNode+1]=1;
      }}}
      if((y<7) && (maze[y+1][x]!=2) && (newDist<DDist[currNode+5])){
        DDist[currNode+5]=newDist;
        DPath[currNode+5]=2;
      }
    }
    currNode=DQMin();
  }
}



void labelUnreachable(){
  byte x;
  byte y;
  for(byte k=0;k<20;k++){
    if(DDist[k]==123){
      x=(k%5)*2+1;
      y=(k/5)*2+1;
      maze[y][x]=UNREACHABLE;
    }
  }
}

byte minUnvisited(){
  byte minVal=123;
  byte minI=123;
  byte mx;
  byte my;
  for(byte c=0; c<20; c++){
    mx=(c%5)*2+1;
    my=(c/5)*2+1;
    if(DDist[c]<minVal && (maze[my][mx]==0)){
       minVal=DDist[c];
       minI=c;   
    }
  }
  return minI;
}


void pathToTake(){
  byte target = minUnvisited();
  if (target == 123) {
    fillUnreachable();
    while(1){
      transmitMaze();
      delay(500);
    }
  }
  byte dir = DPath[target];
  byte nextDir = dir;
  if (dir == 123){
    fillUnreachable();
     while (1){
      transmitMaze();
      delay(500);
     }
  }
  while (nextDir != 123){
    dir=nextDir;
    if(dir==0) {
      target+=5;
      nextDir = DPath[target];
    }
    else if(dir==1) {
      target-=1;
      nextDir = DPath[target];
    }
    else if(dir==2) {
      target-=5;
      nextDir = DPath[target];
    }
    else {
      target+=1;
      nextDir = DPath[target];
    }
  }
  Serial.println(dir);
  goTowards(dir);
}

void navigate(){
  findKnown();
  Dijkstra();
  //outputPaths();
  //outputDistances();
  updateMaze();
  labelUnreachable();  
  //outputMaze();
  pathToTake();
}

void outputPaths(){
  Serial.println("paths"); 
  for(byte row=0; row<4; row++){
  for(byte col=0; col<5; col++){
    Serial.print(DPath[row*5+col]);
    Serial.print("  ");
  } 
  Serial.println(" ");
 }
}


void outputDistances(){
  Serial.println("output"); 
  for(byte row=0; row<4; row++){
  for(byte col=0; col<5; col++){
    Serial.print(DDist[row*5+col]);
    Serial.print("  ");
  } 
  Serial.println(" ");
 }
}


void fillUnreachable(){
 for (int x=0; x<11;x++){
  for(int y=0; y<9;y++){
    if(maze[y][x]==0){
      maze[y][x]=UNREACHABLE;
    }
  }
 } 
}

void findKnown(){
  byte x;
  byte y;
  
  for(byte i=0;i<20;i++){
    x=(i%5)*2+1;
    y=(i/5)*2+1;
    if(maze[y][x]==0){
      if(maze[y-1][x]!=0 && maze[y][x+1]!=0 && maze[y+1][x]!=0 && maze[y][x-1]!=0){
        maze[y][x]=1;
      }
    }
  }
}


