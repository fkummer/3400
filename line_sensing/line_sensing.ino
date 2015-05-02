/* 
 * This is the function responsible for the general line following logic.
 * It first reads the sensor values, determines the error based on our target
 * sensor reading by using a linear mapping function that maps the difference
 * between sensor readings to a range from -4 to 4. Additional logic at the end
 * of the function resposible for handling intersections has been omitted
 */
#include <Servo.h>

Servo myservo1;  // left wheel
Servo myservo2; // right wheel

char right_direction;
char left_direction;

  // Variables for reading the line sensor values:
char qti_left;
char qti_right;
int left_side;
int right_side;

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

void setup(){
  Serial.begin(9600);
  myservo1.attach(9);  // left wheel
  myservo2.attach(10); // right wheel
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);// A4 and A5 are for the front line sensors
  pinMode(A5, INPUT);
  
  pinMode(4,OUTPUT); //the following 3 pins work for the mux
  pinMode(3,OUTPUT);
  pinMode(2,OUTPUT);
  
  qti_left = analogRead(A4); // left front sensor
  qti_right = analogRead(A5); //right fron sensor
  left_side = analogRead(A3); //left sensor
  right_side = analogRead(A2); //right sensor
  myservo1.write(100);   //left sensor start speed
  myservo2.write(80);    //right sensor start speed 
}

void loop(){
    //mux();
   // First we read the line sensor values:
    //update_qtis(&qti_left, &qti_right);
    qti_left = analogRead(A4);
    qti_right = analogRead(A5);
    
    digitalWrite(4, HIGH); // assigns input digits to the mux
    digitalWrite(3, LOW);
    digitalWrite(2, HIGH);
    left_side = analogRead(A3);
    right_side = analogRead(A2);
  
    error = qti_left - qti_right; 
   
    //Serial.println(left_side);
    //Serial.println(right_side);
   /* if (left_side && right_side) { 
      delay(170);
      myservo1.write(90);
      myservo2.write(90);
      delay(200);
      myservo1.write(97);
      myservo2.write(97);
      delay(640);
      myservo1.write(90);
      myservo2.write(90);
      if (qti_left < 400){
        myservo1.write(97);
        myservo2.write(97);
      }
      left_direction = 90;
      right_direction = 90;
    }*/
      //delay(500); */
   if (left_side > 750 && right_side > 750){
      delay(160);
      myservo1.write(90);
      myservo2.write(90);
      delay(200);
      myservo1.write(83);
      myservo2.write(83);
      delay(590);
      myservo1.write(90);
      myservo2.write(90);
      if (qti_right < 400){
        myservo1.write(83);
        myservo2.write(83);
      }
      left_direction = 90;
      right_direction = 90;
      //delay(500); 
      
    }  
    else{
      //Serial.println(error);
      if (error < 100 && error > -100) { 
        error = 0; 
        right_direction = (char)(80 + 0.08*error);
        left_direction = (char)(100 + 0.08*error);
      }
      else{
        right_direction = (char)(83 + 0.08*error + 0.01*err_diff);
        left_direction = (char)(97 + 0.08*error + 0.01*err_diff);
      }
    }
    //right_direction = 90;  //for tuning the servos
    //left_direction = 90;
    
    //Serial.println(error);
    myservo1.write(left_direction);
    myservo2.write(right_direction); 
    
    err_diff = error - prev_err;
    prev_err = error;  // Save this "P" term to determine the next "D" term
    //}
    /*myservo1.write(83);
    myservo2.write(83);
    delay(500);
    myservo1.write(90);
    myservo2.write(90);
    delay(3000); */
    //myservo1.write(90);
    //myservo2.write(90); 
    
   //Serial.print(left_side);
    //Serial.print("        ");
    //Serial.println(right_side);
    
    
    
    //myservo1.write(left_direction);
    //myservo2.write(right_direction); 
    /*
    // Calculate our "I" and "D" terms
    err_sum += error;
    err_diff = error - prev_err;
    prev_err = error;  // Save this "P" term to determine the next "D" term
    */
}

/*void mux(){
    if (counter == 3){
      counter = 0;
    }   
    
    if (counter == 0){
      digitalWrite(0,LOW);
      digitalWrite(1,LOW);
      digitalWrite(2,LOW);
    }
    
    else if (counter == 1){
      digitalWrite(0,LOW);
      digitalWrite(1,HIGH);
      digitalWrite(2,LOW);
    }
    
    else if (counter == 2){
      digitalWrite(0,LOW);
      digitalWrite(1,HIGH);
      digitalWrite(2,LOW);
    }
    
    counter++;
    
}
*/

