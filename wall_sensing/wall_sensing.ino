/*This code is for the IR sensor*/

#define UNEXPLORED 0
#define WALL 1
#define NO_WALL 2
#define UNREACHABLE 3

#define THRESH_1 1500
#define THRESH_2 1500


byte maze [11] [9];
byte currDirection;
byte currX=0;
byte currY=0;
#define front_sensor A0
#define left_sensor A2
#define right_sensor A1
#define led  13
byte left_wall;
byte right_wall;
byte front_wall;


void setup(){
  Serial.begin(9600);
  pinMode(front_sensor, INPUT);
  pinMode(left_sensor,INPUT);
  pinMode(right_sensor,INPUT);
  pinMode(led, OUTPUT);
  delay(500);
  Serial.println("hello");
}

void loop(){
  int left = 0;
  int right = 0;
  int front = 0;
  for (int i=0;i<10;i++) {
  left += analogRead(left_sensor);
  right += analogRead(right_sensor);
  front += analogRead(front_sensor);
  //closest wall 82
  //further wall 40
  }
  
  Serial.println("why?");
  //return;
  /*
  switch (currDirection) {
    case 0: //robot facing up
      if(left>THRESH_2){
        if(left>THRESH_1){
          maze[currX-1][currY]=WALL;
        } else{
          maze[currX-1][currY]=NO_WALL;
          maze[currX-3][currY]=WALL;
        }
      }else{
        maze[currX-1][currY]=NO_WALL;
        maze[currX-3][currY]=NO_WALL;
      }
      
      if(right>THRESH_2){
        if(right>THRESH_1){
          maze[currX+1][currY]=WALL;
        }else{
          maze[currX+1][currY]=NO_WALL;
          maze[currX+3][currY]=WALL;
        }
      }else{
        maze[currX+1][currY]=NO_WALL;
        maze[currX+3][currY]=NO_WALL;
      }
      
      if(front>THRESH_2){
        if(front>THRESH_1){
          maze[currX][currY-1]=WALL;
        }else{
          maze[currX][currY-1]=NO_WALL;
          maze[currX][currY-3]=WALL;
        }
      }else{
        maze[currX][currY-1]=NO_WALL;
        maze[currX][currY-3]=NO_WALL;
      }
      break;
      
      
    case 1: //robot facing right
    if(left>THRESH_2){
        if(left>THRESH_1){
          maze[currX][currY-1]=WALL;
        } else{
          maze[currX][currY-1]=NO_WALL;
          maze[currX][currY-3]=WALL;
        }
      }else{
        maze[currX][currY-1]=NO_WALL;
        maze[currX][currY-3]=NO_WALL;
      }
      
      if(right>400){
        if(right>THRESH_1){
          maze[currX][currY+1]=WALL;
        }else{
          maze[currX][currY+1]=NO_WALL;
          maze[currX][currY+3]=WALL;
        }
      }else{
        maze[currX][currY+1]=NO_WALL;
        maze[currX][currY+3]=NO_WALL;
      }
      
      if(front>THRESH_2){
        if(front>THRESH_1){
          maze[currX+1][currY]=WALL;
        }else{
          maze[currX+1][currY]=NO_WALL;
          maze[currX+3][currY]=WALL;
        }
      }else{
        maze[currX+1][currY]=NO_WALL;
        maze[currX+3][currY]=NO_WALL;
      }
      break;
      
      
    case 2: //robot facing down
    if(left>THRESH_2){
        if(left>THRESH_1){
          maze[currX+1][currY]=WALL;
        }else{
          maze[currX+1][currY]=NO_WALL;
          maze[currX+3][currY]=WALL;
        }
      }else{
        maze[currX+1][currY]=NO_WALL;
        maze[currX+3][currY]=NO_WALL;
      }
      
      if(right>THRESH_2){
        if(right>THRESH_1){
          maze[currX-1][currY]=WALL;
        }else{
          maze[currX-1][currY]=NO_WALL;
          maze[currX-3][currY]=WALL;
        }
      }else{
        maze[currX-1][currY]=NO_WALL;
        maze[currX-3][currY]=NO_WALL;
      }
      
      if(front>THRESH_2){
        if(front>THRESH_1){
          maze[currX][currY+1]=WALL;
        }else{
          maze[currX][currY+1]=NO_WALL;
          maze[currX][currY+3]=WALL;
        }
      }else{
        maze[currX][currY+1]=NO_WALL;
        maze[currX][currY+3]=NO_WALL;
      }
      break;
    
    case 3: //robot facing left
    
    
    if(left>THRESH_2){
        if(left>THRESH_1){
          maze[currX][currY+1]=WALL;
        }else{
          maze[currX][currY+1]=NO_WALL;
          maze[currX][currY+3]=WALL;
        }
      }else{
        maze[currX][currY+1]=NO_WALL;
        maze[currX][currY+3]=NO_WALL;
      }
      
      if(right>THRESH_2){
        if(right>THRESH_1){
          maze[currX][currY-1]=WALL;
        }else{
          maze[currX][currY-1]=NO_WALL;
          maze[currX][currY-3]=WALL;
        }
      }else{
        maze[currX][currY-1]=NO_WALL;
        maze[currX][currY-3]=NO_WALL;
      }
      
      if(front>THRESH_2){
        if(front>THRESH_1){
          maze[currX-1][currY]=WALL;
        }else{
          maze[currX-1][currY]=NO_WALL;
          maze[currX-3][currY]=WALL;
        }
      }else{
        maze[currX-1][currY]=NO_WALL;
        maze[currX-3][currY]=NO_WALL;
      }
      break;
    default:
      break;
  }
 */ 
  
  Serial.println("ok then");
  int left_wall = (left>THRESH_2) ? ((left>THRESH_1) ? 1 : 2) : 0;
  int right_wall = (right>THRESH_2) ? ((right>THRESH_1) ? 1 : 2) : 0;
  int front_wall = (front>THRESH_2) ? ((front>THRESH_1) ? 1 : 2) : 0;
  Serial.println("how about it");

  Serial.println("front:");
  Serial.println(front);
  Serial.println(front_wall);
  Serial.println("left:");
  Serial.println(left);
  Serial.println(left_wall);
  Serial.println("right:");
  Serial.println(right);
  Serial.println(right_wall);
  delay(3000);
}





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


void outPutMaze(){
  for(int x=0; x<11; x++){
    Serial.print("[");
    for(int y=0; y<9; y++){
      Serial.print(maze[x][y]);
      Serial.print(", "); 
    }
    Serial.println("]");
  } 
}



