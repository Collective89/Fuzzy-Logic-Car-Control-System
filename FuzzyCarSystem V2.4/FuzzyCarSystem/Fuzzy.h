//Luke Milburn - 2015
#ifndef _FUZZYLOGICCLASS_H_
#define _FUZZYLOGICCLASS_H_

//Std library includes
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <vector>
#include <memory>

//Framework includes
#include "CarPosition.h"
#include "CarMovement.h"

//Trapeziums constructed from disected triangle of member function
// XCoord1-4 contain x coordinate of each vertex of trapezium
//yCoord1-4 store y coordinate of each vertex of trapezium
struct trapezium
{
	//xCoord1 (left), xCoord2(right) - base xcoordinates, xCoord3 (left), XCoord4 (right) - membership x coordinates 
	float xCoord1, xCoord2, xCoord3, xCoord4; 
	
	//yCoord1 (left), yCoord2(right) - base y coordinates, yCoord3 (left), yCoord4 (right) - membership y coordinates
	float yCoord1, yCoord2, yCoord3, yCoord4;
};

class FuzzyLogicClass
{

public:

	//Functions
	FuzzyLogicClass();        //Constructor
	~FuzzyLogicClass();       //Destructor

	//Functions to set up fuzzy system and ascertain the output state of the system
	void initializeFuzzySystem();															//Function to initialise the instances of the car movement and position classes
	void applyFuzzyLogic(float &xPos, float &movement);										//Checks the present state of the fuzzy associative map for the system 
	void calcMinInput(posTrapezium &select1, speedTrapezium &select2);						//Calculates which of the two input membership trapezia has the minimum mebership value and adds it to teh appropriate membership vector 
	void setOutput();																		//Function which uses the state of the action variable to set the output state (presentCSitu variable) 
	void defaultLeft();																		//Set all cordinates of chosen left trapezium to the value 0
	void defaultCentre();																	//Set all coordinates of chosen centre trapezium to the value 0
	void defaultRight();																	//Set all coordinates of chosen right trapezium to the value 0
	
	//Functions which are used to determine what membership functions have the maximum membership for current input for both velocity and position
	void chooseMaxMembership();																//In cases where there are multiple branches of FIS producing the same output - select the output which has maximum membership
	void constructFuzzyIFS();																//Constructs the shape from the two trapezoids of the input functions from which we shall calculate the centroid

	//Output functions
	void forceOutput();																		//If one of the membership functions is a ridiculously small number force it to be a minimum of 0.1
	float deffuzifyWAM();																	//Crisp output is weighted average of 
	float deffuzifyCentroid();																//Calulates the centroid of the combined shape (which is the output of the constructFuzzyIFS function) and ultimately crisp output 									
																							//Which is the new velocity of the car

private:

	//A trapezium struct for each membership function
	trapezium chosenLeft, chosenCentre, chosenRight;

	//Variables to store the input to the fuzzy associative map
	carVelState presentVelocity;
	carPosState presentPosition;

	//Enumerator for the output state
	enum outputState{moveLeft, motionless, moveRight};

	//The output state
	outputState action;

	//Pointer to instance of car position class
	CarPositionClass* carPosition;
	
	//Pointer to instance of car movement class
	CarMovementClass* carMovement;

	//Variables to contain the appropriate trapezia from setting up calculations
	//These trapezia will then be used to create the fuzzy output shape
	std::vector<posTrapezium> selectedPInputs;
	std::vector<speedTrapezium> selectedVInputs;

	//Vectors containing output trapezia from constructed inputs
	std::vector<trapezium> leftTraps;
	std::vector<trapezium> midTraps;
	std::vector<trapezium> rightTraps;

	//Temporary trapezium built from selected input which is to be stored in one of the three above vectors
	trapezium temp;

	//Float to contain final crisp value after deffuzification has occured (value handed back to main program
	//then multiplied by 4 to obtain the horizontal car movement velocity)
	float fuzzyOutput;
	
};

#endif