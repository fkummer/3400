/*This code is for the IR sensor*/

byte maze [11] [9];
byte currDirection;
byte currX;
byte currY;
#define front_sensor A4
#define left_sensor A3
#define right_sensor A5
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
  switch (currDirection) {
    case 0:
      if(left>400){
        if(left>820){
          maze[currX][currY]=2;
        } else{
          maze[currX][currY]=1;
          maze[currX][currY]=2;
        }
      }else{
        maze[currX][currY]=2;
      }
      
      if(right>400){
        if(right>820){
          maze[currX][currY]=2;
        }else{
          maze[currX][currY]=1;
          maze[currX][currY]=2;
        }
      }else{
        maze[currX][currY]=2;
      }
      
      if(front>400){
        if(front>820){
          maze[currX][currY]=2;
        }else{
          maze[currX][currY]=1;
          maze[currX][currY]=2;
        }
      }else{
        maze[currX][currY]=2;
      }
      break;
      
      
    case 1:
    if(left>400){
        if(left>820){
          maze[currX][currY]=2;
        } else{
          maze[currX][currY]=1;
          maze[currX][currY]=2;
        }
      }else{
        maze[currX][currY]=2;
      }
      
      if(right>400){
        if(right>820){
          maze[currX][currY]=2;
        }else{
          maze[currX][currY]=1;
          maze[currX][currY]=2;
        }
      }else{
        maze[currX][currY]=2;
      }
      
      if(front>400){
        if(front>820){
          maze[currX][currY]=2;
        }else{
          maze[currX][currY]=1;
          maze[currX][currY]=2;
        }
      }else{
        maze[currX][currY]=2;
      }
      break;
      
      
    case 2:
    if(left>400){
        if(left>820){
          maze[currX][currY]=2;
        }else{
          maze[currX][currY]=1;
          maze[currX][currY]=2;
        }
      }else{
        maze[currX][currY]=2;
      }
      
      if(right>400){
        if(right>820){
          maze[currX][currY]=2;
        }else{
          maze[currX][currY]=1;
          maze[currX][currY]=2;
        }
      }else{
        maze[currX][currY]=2;
      }
      
      if(front>400){
        if(front>820){
          maze[currX][currY]=2;
        }else{
          maze[currX][currY]=1;
          maze[currX][currY]=2;
        }
      }else{
        maze[currX][currY]=2;
      }
      break;
    case 3:
    
    
    if(left>400){
        if(left>820){
          maze[currX][currY]=2;
        }else{
          maze[currX][currY]=1;
          maze[currX][currY]=2;
        }
      }else{
        maze[currX][currY]=2;
      }
      
      if(right>400){
        if(right>820){
          maze[currX][currY]=2;
        }else{
          maze[currX][currY]=1;
          maze[currX][currY]=2;
        }
      }else{
        maze[currX][currY]=2;
      }
      
      if(front>400){
        if(front>820){
          maze[currX][currY]=2;
        }else{
          maze[currX][currY]=1;
          maze[currX][currY]=2;
        }
      }else{
        maze[currX][currY]=2;
      }
      break;
    default:
      break;
  } 
    
  left_wall = (left>400) ? ((left>820) ? 1 : 2) : 0;
  right_wall = (right>400) ? ((right>820) ? 1 : 2) : 0;
  front_wall = (front>400) ? ((front>820) ? 1 : 2) : 0;
  
  Serial.println("front:");
  Serial.println(front_wall);
  delay(500);
}
