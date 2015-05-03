/* THIS CODE DESIGNED FOR ECE 3400 STUDENTS ONLY.
   IT MAY NOT BE REPRODUCED OR DISTRIBUTED WITHOUT
   PERMISSION OF STAFF. */
   
#ifndef SIMULATE
#define SIMULATE

bool forward();
bool turnRight();
bool turnLeft();

int getSensorFront();
int getSensorRight();
int getSensorLeft();
int getSensorBehind();
int getVisitedFront();
int getVisitedRight();
int getVisitedLeft();
int getSensorBehind();
void printLocation();
void printDirection();


#endif