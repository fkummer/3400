#include <iostream>
#include "simulate.h"

extern int location[2] = {7,9};
int direction = 0;

//only edit maze.
//to change the maze, change any of the inner numbers.
//the maze is 9x11 to represent the physical 4'x5' maze.
//if you have trouble interpreting this, please ask a staff member.
int maze[99] = { 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 0, 0, 0, 1, 0, 0, 0, 1,
				1, 0, 1, 0, 1, 0, 1, 0, 1,
				1, 0, 1, 0, 1, 0, 1, 0, 1,
				1, 0, 1, 0, 1, 0, 1, 0, 1,
				1, 0, 1, 0, 1, 0, 1, 0, 1,
				1, 0, 1, 0, 1, 0, 1, 0, 1,
				1, 0, 1, 0, 1, 0, 1, 0, 1,
				1, 0, 1, 0, 1, 0, 1, 0, 1,
				1, 0, 1, 0, 0, 0, 1, 0, 1,
				1, 1, 1, 1, 1, 1, 1, 1, 1 };

bool forward() {
	using namespace std;
	int location1D = location[1]*9 + location[0];
	bool hitWall = false;
	if (direction == 0) {
		if (maze[location1D-9] == 1)
			hitWall = true;
		location[1]-= 2;
	}
	else if (direction == 1) {
		if (maze[location1D+9] == 1)
			hitWall = true;
		location[1] += 2;
	}
	else if (direction == 2) {
		if (maze[location1D+1] == 1)
			hitWall = true;
		location[0] += 2;
	}
	else {
		if (maze[location1D-1] == 1)
			hitWall = true;
		location[0] -= 2;
	}
	if (hitWall) {
		cout << "HIT WALL!" << endl;
		return false;
	}
	if (location[0] > 8 || location[0] < 0 || location[1] > 10 || location[1] < 0)
		return false;
	return true;
}

bool turnRight() {
	direction = (direction == 0 ? 2 : (
					direction == 1 ? 3 : (
					direction == 2 ? 1 : 0)));
	return true;
}

bool turnLeft() {
	direction = (direction == 0 ? 3 : (
					direction == 1 ? 2 : (
					direction == 2 ? 0 : 1)));
	return true;
}

int getSensorFront() {
	int location1D = location[1]*9 + location[0];
	if (direction == 0)
		return maze[location1D-9];
	else if (direction == 1)
		return maze[location1D+9];
	else if (direction == 2)
		return maze[location1D+1];
	else
		return maze[location1D-1];
}

int getSensorRight() {
	int location1D = location[1]*9 + location[0];
	if (direction == 0)
		return maze[location1D+1];
	else if (direction == 1)
		return maze[location1D-1];
	else if (direction == 2)
		return maze[location1D+9];
	else
		return maze[location1D-9];
}

int getSensorLeft() {
	int location1D = location[1]*9 + location[0];
	if (direction == 0)
		return maze[location1D-1];
	else if (direction == 1)
		return maze[location1D+1];
	else if (direction == 2)
		return maze[location1D-9];
	else
		return maze[location1D+9];}

int getSensorBehind() {
	int location1D = location[1]*9 + location[0];
	if (direction == 0)
		return maze[location1D+9];
	else if (direction == 1)
		return maze[location1D-9];
	else if (direction == 2)
		return maze[location1D-1];
	else
		return maze[location1D+1];
}


void printLocation() { //for debugging only
	cout << "x = " << location[0] << " y = " << location[1] << endl;
	return;
}

void printDirection() { //for debugging only
	cout << "facing ";
	if (direction == 0)
		cout << "north";
	else if (direction == 1)
		cout << "south";
	else if (direction == 2)
		cout << "east";
	else
		cout << "west";
	cout << endl;
	return;
}