/* THIS CODE DESIGNED FOR ECE 3400 STUDENTS ONLY.
   IT MAY NOT BE REPRODUCED OR DISTRIBUTED WITHOUT
   PERMISSION OF STAFF. */
   
#include "simulate.h"
#include <iostream>

//to compile this, use the command "g++ testSim.cpp simulate.cpp -o simulate" in the
//terminal, then run the program with the command "simulate"
//make sure you're in the directory that contains both cpp files and the h file.
int visited[99];
int i;
int adjacent_vis[3];
for(i = 0; i<99; i ++){
	visited[i] = 0;
}

int main() {
	using namespace std;
	
	//edit the code below with your own navigation algorithm.
	//default is right-wall following.
	//to change the maze, enter simulate.cpp and change the 1s and 0s.
	//Do not use numbers other than 1 and 0.
	int i = 30;
	while (i > 0) {
		int location1D = location[1]*9 + location[0];
		visited[location1D]++;
		/*if (getSensorRight() == 0) {		
			turnRight();
			if (!forward()) { //this still calls "forward", just checks if it worked as well 
				cout << "broke through wall" << endl;
				return 0;
			}
		}
		else if (getSensorFront() == 0) {
			if (!forward()) {
				cout << "broke through wall" << endl;
				return 0;
			}
		}
		else if (getSensorLeft() == 0) {
			turnLeft();
			if (!forward()) {
				cout << "broke through wall" << endl;
				return 0;
			}
		}
		else if (getSensorBehind() == 0) {
			turnLeft();
			turnLeft();
			if (!forward()) {
				cout << "broke through wall" << endl;
				return 0;
			}
		}
		else {
			cout << "trapped. exiting." << endl;
			return 0;
		}*/
   if(((getVisitedFront()>0 || getSensorFront()==1) && (getVisitedRight()>0 || getSensorRight()==1) && (getVisitedLeft()>0 || getSensorLeft() == 1) && location[0] == 7 && location[1] == 9))
       return 0;
		
		adjacent_vis[3] = {0,0,0};
		if(getSensorFront() == 1){
			adjacent_vis[0] = 200;
		}
		else{
			adjacent_vis[0] = getVisitedFront();
		}
		if(getSensorRight() == 1){
			adjacent_vis[1] = 200;
		}
		else{
			adjacent_vis[1] = getVisitedRight();
		}
		if(getSensorLeft() == 1){
			adjacent_vis[1] = 200;
		}
		else{
			adjacent_vis[1] = getVisitedLeft();
		}
		
		if(adjacent_vis[0] == 200 && adjacent_vis[1] == 200 && adjacent_vis[2] == 200){
			turnLeft();
			turnLeft();
			if (!forward()) {
				cout << "broke through wall" << endl;
				return 0;
			}
		}
		else{
			int minIdx = 4;
			int minValue = 200;
			for(int i = 0; i<3; i++){
				if(adjacent_vis[i] < minValue){
					minValue = adjacent_vis[i];
					minIdx = i;
				}
			}
			if(minIdx == 1){
				turnRight();
			}
			else if(minIdx == 2){
				turnLeft();
			}
			if (!forward()) {
				cout << "broke through wall" << endl;
				return 0;
			}			
		}
		printLocation();
		printDirection();
		i--;
	}
	return 0;	
}

int getVisitedFront() {
	int location1D = location[1]*9 + location[0];
	if (direction == 0)
		return visited[location1D-9];
	else if (direction == 1)
		return visited[location1D+9];
	else if (direction == 2)
		return visited[location1D+1];
	else
		return visited[location1D-1];
}

int getVisitedRight() {
	int location1D = location[1]*9 + location[0];
	if (direction == 0)
		return visited[location1D+1];
	else if (direction == 1)
		return visited[location1D-1];
	else if (direction == 2)
		return visited[location1D+9];
	else
		return visited[location1D-9];
}

int getVisitedLeft() {
	int location1D = location[1]*9 + location[0];
	if (direction == 0)
		return visited[location1D-1];
	else if (direction == 1)
		return visited[location1D+1];
	else if (direction == 2)
		return visited[location1D-9];
	else
		return visited[location1D+9];}

int getVisitedBehind() {
	int location1D = location[1]*9 + location[0];
	if (direction == 0)
		return visited[location1D+9];
	else if (direction == 1)
		return visited[location1D-9];
	else if (direction == 2)
		return visited[location1D-1];
	else
		return visited[location1D+1];
}