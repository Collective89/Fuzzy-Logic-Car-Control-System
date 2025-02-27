//Luke Milburn - 2015
#ifndef _CARMOVEMENTCLASS_H_
#define _CARMOVEMENTCLASS_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <vector>

//Trapeziums constructed from disected triangle of member function
// XCoord1-4 contain x coordinate of each vertex of trapezium
//yCoord1-4 store y coordinate of each vertex of trapezium
struct speedTrapezium
{
	//xCoord1 (left), xCoord2(right) - base xcoordinates, xCoord3 (left), XCoord4 (right) - membership x coordinates 
	float xCoord1, xCoord2, xCoord3, xCoord4; 
	
	//yCoord1 (left), yCoord2(right) - base y coordinates, yCoord3 (left), yCoord4 (right) - membership y coordinates
	float yCoord1, yCoord2, yCoord3, yCoord4;
};

//Use an enumerator for the state of the velocity - can be used
//in the case of an overlap to control which of the membership functions
//shall be used to determine the output - those which have maximal membership.

enum carVelState{moveL, moveLSlight, stationary, moveRSlight, moveR};

class CarMovementClass
{

public:

	//Functions
	CarMovementClass();        //Constructor
	~CarMovementClass();       //Destructor

	//Input functions relating to the position of the car on screen (functions triangle shape)
	//Functions will construct appropriate trapezium for each input function (disected triangle)
	//Also act as getters for the membership value
	void leftOfC(float inputVel);																			//Calculates the degree of membership of left membership function and returns
	void centre(float inputVel);																			//Calculates the degree of membership of the centre membership function and returns
	void rightOfC(float inputVel);																			//Calculates the degree of membership of the right membership function and returns

	//Functions which will set the state of the present input using the carPosState enumerator and
	//allow the fuzzy logic class to access the state of the inputs
	void setInputState(float carVel);																		//Sets posInputState depending on the position of car on screen
	carVelState getState();																					//Return posInput (Which is a carPosState) variable
	speedTrapezium getTrapezoidL();																			//Return shapes constructed from membership
	speedTrapezium getTrapezoidC();
	speedTrapezium getTrapezoidR();

	//Functions to initialise the trapezoids and then update them from
	//the shape of the triangles and membership function values as the car moves position
	void initialiseTraps();
	void checkMembership(float inputCarVel);					//Checks the value of each of the membership functions is not 0 in which case a trapezium can be contructed defaults to 0
	void updateTraps(float inputCarVel);						//Sets the shape of the central trapezium and two edge triangles based on the input x-coordinate and output from the membership functions

private:

	float membershipL, membershipC, membershipR;

	//A trapezium struct for each membership function
	speedTrapezium left, centreT, right;

	//Floats for bases of shapes
	float leftXBase1, leftXBase2, centreXBase1, centreXBase2, rightXBase1, rightXBase2, allYBase;

	//The input state of the car velocity
	carVelState velInput;

	
};

#endif