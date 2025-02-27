//Luke Milburn - 2015
#include "Fuzzy.h"
#include <Windows.h>
FuzzyLogicClass::FuzzyLogicClass()
{
	//Constructors
	defaultLeft();
	defaultCentre();
	defaultRight();
	carPosition = new CarPositionClass();
	carMovement = new CarMovementClass();
}

FuzzyLogicClass::~FuzzyLogicClass()
{
	delete carPosition;
	delete carMovement;
}


void FuzzyLogicClass::initializeFuzzySystem()
{
	//Initialise membership functions
	carPosition->initialiseTraps();
	carMovement->initialiseTraps();
}

void FuzzyLogicClass::applyFuzzyLogic(float &xPos, float &movement)
{
	//Set the input state for the fuzzy associative map variable in the classes
	//Also construct the apporpriate trapezia for the membership functions of each input
	carPosition->updateTraps(xPos);
	carMovement->updateTraps(movement);

	//Store the input states from each of the instances of the CarPositionClass and CarMovementClass
	presentPosition = carPosition->getState();
	presentVelocity = carMovement->getState();

	//Calculate the output state of the system and select appropriate trapezia
	setOutput();

	//Check which of the trapezia has maximum membership for moving left, remaining stationary and moving right and select
	chooseMaxMembership();

}

void FuzzyLogicClass::calcMinInput(posTrapezium &select1, speedTrapezium &select2)
{
	//If the degree of membership of the position membership function is the minimum
	//Construct the temp trapezium out of position membership function trapezium
	if(select1.yCoord3 < select2.yCoord3)
	{
		temp.xCoord1 = select1.xCoord1;
		temp.xCoord2 = select1.xCoord2;
		temp.xCoord3 = select1.xCoord3;
		temp.xCoord4 = select1.xCoord4;
		temp.yCoord1 = select1.yCoord1;
		temp.yCoord2 = select1.yCoord2;
		temp.yCoord3 = select1.yCoord3;
		temp.yCoord4 = select1.yCoord4;
	}
	//Or if the degree of membership of the speed membership function is the minimum
	//Construct the temp trapezium out of speed membership function trapezium
	else if(select2.yCoord3 < select1.yCoord3)
	{
		temp.xCoord1 = select2.xCoord1;
		temp.xCoord2 = select2.xCoord2;
		temp.xCoord3 = select2.xCoord3;
		temp.xCoord4 = select2.xCoord4;
		temp.yCoord1 = select2.yCoord1;
		temp.yCoord2 = select2.yCoord2;
		temp.yCoord3 = select2.yCoord3;
		temp.yCoord4 = select2.yCoord4;
	}
	//Or if the degree of membership of both the speed and position membership functions are the same
	//Construct the temp trapezium out of the position membership function
	else if(select1.yCoord3 == select2.yCoord3)
	{
		temp.xCoord1 = select1.xCoord1;
		temp.xCoord2 = select1.xCoord2;
		temp.xCoord3 = select1.xCoord3;
		temp.xCoord4 = select1.xCoord4;
		temp.yCoord1 = select1.yCoord1;
		temp.yCoord2 = select1.yCoord2;
		temp.yCoord3 = select1.yCoord3;
		temp.yCoord4 = select1.yCoord4;
	}

	//Add temp trapezium to the appropriate output vector dependant on output state
	switch(action)
	{
		case moveLeft:
			leftTraps.push_back(temp);
			break;
		case motionless:
			midTraps.push_back(temp);
			break;
		case moveRight:
			rightTraps.push_back(temp);
			break;
	}
}
void FuzzyLogicClass::setOutput()
{
	switch(presentPosition)
	{
		case leftM:
			switch(presentVelocity)
			{
				case moveL:
					//Only branch - left position and left movement
					action = moveRight;
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidL());
					break;
				case moveLSlight:
					//Where there is an overlap in one of the input variables - there are two possible output states
					//Calculate the membership of left movement membership function and centre movement membership function
					//Calculate the minimum degree of membership of the left and centre functions as this will be the degree of membership of this particular output.
					//Calculate the maximum degree of membership of branches of output that are the same - this will be the final output shape for a particular membership function
					//........Here there is an overlap in the velocity membership function between the left membership function and central membership function
					//First deal with the moving left membership function....
					action = moveRight;
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidL());
					//Second deal with the stationary membership function
					action = moveRight;
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidC());
					break;
				case stationary:
					//Only branch - left position and stationary
					action = moveRight;
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidC());
					break;
				case moveRSlight:
					action = moveRight;
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidC());
					action = motionless;
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidR());
					break;
				case moveR:
					//Use the minimum membership function of left movement membership function and right movement membership function as the output function
					//make sure to set output functions correctly - in this case solely stationary output
					action = motionless;
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidR());
					break;
			}
			break;
		case leftAndCentreM:
			switch(presentVelocity)
			{
				case moveL:
					//First branch - left position and left movement
					action = moveRight;
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidL());
					//Second branch - centre position and left movement (Optimised)
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidL());
					break;
				case moveLSlight:
					//Where there is an overlap in both input variables - there are four output states
					//First branch - left position and left movement
					action = moveRight;
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidL());
					//Second branch - left position and motionless
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidC());
					//Third branch - centre position and left movement
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidL());
					//4th branch - centre position and motionless
					action = motionless;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidC());
					break;
				case stationary:
					//1st branch - left position and motionless
					action = moveRight;
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidC());
					//2nd branch - centre position and motionless
					action = motionless;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidC());
					break;
				case moveRSlight:
					//First branch - left position and right movement
					action = motionless;
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidR());
					//2nd branch - centre position and motionless (Optimised)
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidC());
					//3rd branch - left position and motionless
					action = moveRight;
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidC());
					//4th branch - centre position and right movement
					action = moveLeft;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidR());
					break;
				case moveR:
					//First branch - left position and right movement
					action = motionless;
					calcMinInput(carPosition->getTrapezoidL(), carMovement->getTrapezoidR());
					//Second branch - centre position and right movement
					action = moveLeft;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidR());
					break;
			}
			break;
		case centreM:
			switch(presentVelocity)
			{
				case moveL:
					//Only branch - centre position and left movement
					action = moveRight;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidL());
					break;
				case moveLSlight:
					//1st branch - centre position and left movement
					action = moveRight;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidL());
					//2nd branch - centre position and motionless
					action = motionless;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidC());
					break;
				case stationary:
					//Only branch - centre position and motionless
					action = motionless;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidC());
					break;
				case moveRSlight:
					//1st branch - centre position and right movement
					action = moveLeft;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidR());
					//2nd branch - centre position and motionless
					action = motionless;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidC());
					break;
				case moveR:
					//Only branch - centre position and right movement
					action = moveLeft;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidR());
					break;
			}
			break;
		case centreAndRightM:
			switch(presentVelocity)
			{
				case moveL:
					//First branch - centre position and left movement
					action = moveRight;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidL());
					//Second branch - right position and left movement 
					action = motionless;
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidL());
					break;
				case moveLSlight:
					//1st branch - centre position and left movement
					action = moveRight;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidL());
					//2nd branch - centre position and motionless
					action = motionless;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidC());
					//3rd branch - right position and left movement (Optimised)
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidL());
					//4th branch - right position and motionless
					action = moveLeft;
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidC());
					break;
				case stationary:
					//1st branch - centre position and motionless
					action = motionless;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidC());
					//2nd branch - right position and motionless
					action = moveLeft;
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidC());
					break;
				case moveRSlight:
					//1st branch - centre position and motionless
					action = motionless;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidC());
					//2nd branch - centre position and right movement
					action = moveLeft;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidR());
					//3rd branch - right position and motionless (Optimised)
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidC());
					//4th branch - right position and right movement
					action = moveLeft;
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidR());
					break;
				case moveR:
					//Use the minimum membership function of left movement membership function and right movement membership function as the output function
					//make sure to set output functions correctly - in this case solely stationary output
					//First branch - centre position and right movement
					action = moveLeft;
					calcMinInput(carPosition->getTrapezoidC(), carMovement->getTrapezoidR());
					//Second branch - right position and right movement (Optimised)
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidR());
					break;
			}
			break;
		case rightM:
			switch(presentVelocity)
			{
				case moveL:
					//Only branch - right position and left movement
					action = motionless;
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidL());
					break;
				case moveLSlight:
					//Calculate the membership of left movement membership function and centre movement membership function
					//apply membership to output
					//construct output shape
					//1st branch - right position and left movement
					action = motionless;
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidL());
					//2nd branch - right position and motionless
					action = moveLeft;
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidC());
					break;
				case stationary:
					//Only branch - right position and motionless
					action = moveLeft;
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidC());
					break;
				case moveRSlight:
					//1st branch - right position and motionless
					action = moveLeft;
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidC());
					//2nd branch - right position and right movement (Optimised)
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidR());
					break;
				case moveR:
					//4th branch - right position and right movement
					action = moveLeft;
					calcMinInput(carPosition->getTrapezoidR(), carMovement->getTrapezoidR());
					break;
			}
			break;
	}
}

void FuzzyLogicClass::defaultLeft()
{
	chosenLeft.xCoord1 = 0.0;
	chosenLeft.xCoord2 = 0.0;
	chosenLeft.xCoord3 = 0.0;
	chosenLeft.xCoord4 = 0.0;
	chosenLeft.yCoord1 = 0.0;
	chosenLeft.yCoord2 = 0.0;
	chosenLeft.yCoord3 = 0.0;
	chosenLeft.yCoord4 = 0.0;
}

void FuzzyLogicClass::defaultCentre()
{
	chosenCentre.xCoord1 = 0.0;
	chosenCentre.xCoord2 = 0.0;
	chosenCentre.xCoord3 = 0.0;
	chosenCentre.xCoord4 = 0.0;
	chosenCentre.yCoord1 = 0.0;
	chosenCentre.yCoord2 = 0.0;
	chosenCentre.yCoord3 = 0.0;
	chosenCentre.yCoord4 = 0.0;	
}

void FuzzyLogicClass::defaultRight()
{
	chosenRight.xCoord1 = 0.0;
	chosenRight.xCoord2 = 0.0;
	chosenRight.xCoord3 = 0.0;
	chosenRight.xCoord4 = 0.0;
	chosenRight.yCoord1 = 0.0;
	chosenRight.yCoord2 = 0.0;
	chosenRight.yCoord3 = 0.0;
	chosenRight.yCoord4 = 0.0;
}

void FuzzyLogicClass::chooseMaxMembership()
{
	float maxMemL, maxMemC, maxMemR;
	//Obtain the trapezia with the maximum membership values for each vector containing the trapezia
	//Representing the output membership functions - 1 trapezium from each, if a vector is empty set
	//the corresponding chosen trapezium to NULL
	if(leftTraps.size()>0)
	{
		//Set the max membership value to initially be the membership value of the first trapezium in the vector 
		maxMemL = leftTraps[0].yCoord3;
		chosenLeft = leftTraps[0];
		for(int i = 0; i<leftTraps.size(); i++)
		{
			if(leftTraps[i].yCoord3>=maxMemL)
			{
				//Chooze trapezium with maximum membership
				maxMemL = leftTraps[i].yCoord3;
				chosenLeft = leftTraps[i];
			}
		}
	}
	//Otherwise set left output to 0
	else
		defaultLeft();
	//Repeat with vector containing chosen middle trapezium
	if(midTraps.size()>0)
	{
		//Set the max membership value to initially be the membership value of the first trapezium in the vector 
		maxMemC = midTraps[0].yCoord3;
		chosenCentre = midTraps[0];
		for(int i = 0; i<midTraps.size(); i++)
		{
			if(midTraps[i].yCoord3>=maxMemC)
			{
				//Chooze trapezium with maximum membership
				maxMemC = midTraps[i].yCoord3;
				chosenCentre = midTraps[i];
			}
		}
	}
	//Otherwise set centre output trapezium to 0
	else
		defaultCentre();
	//Repeat with vector containing chosen right trapezium
	if(rightTraps.size()>0)
	{
		//Set the max membership value to initially be the membership value of the first trapezium in the vector 
		maxMemR = rightTraps[0].yCoord3;
		chosenRight = rightTraps[0];
		for(int i = 0; i<rightTraps.size(); i++)
		{
			if(rightTraps[i].yCoord3>=maxMemR)
			{
				//Chooze trapezium with maximum membership
				maxMemR = rightTraps[i].yCoord3;
				chosenRight = rightTraps[i];
			}
		}
	}
	//Otherwise set right output trapezium to 0
	else
		defaultRight();

	//Empty all three output membership function vectors as we have no more use for them
	leftTraps.erase(leftTraps.begin(), leftTraps.end());
	midTraps.erase(midTraps.begin(), midTraps.end());
	rightTraps.erase(rightTraps.begin(), rightTraps.end());
	
}

//void FuzzyLogicClass::applyMaxVelo(speedTrapezium trap1, speedTrapezium trap2)
//{
//
//}

void FuzzyLogicClass::constructFuzzyIFS()
{

}

void FuzzyLogicClass::forceOutput()
{
	if(chosenLeft.yCoord3>0 && chosenLeft.yCoord3<0.01)
		chosenLeft.yCoord3 = 0.01;
	if(chosenCentre.yCoord3>0 && chosenCentre.yCoord3<0.01)
		chosenCentre.yCoord3 = 0.01;
	if(chosenRight.yCoord3>0 && chosenRight.yCoord3<0.01)
		chosenRight.yCoord3 = 0.01;
}

float FuzzyLogicClass::deffuzifyWAM()
{
	//If output is purely left
	if(chosenLeft.yCoord3 > 0 && chosenCentre.yCoord3==0 && chosenRight.yCoord3 == 0)
	{
		fuzzyOutput = ((chosenLeft.yCoord3*-2.5)/(chosenLeft.yCoord3-0.1));
	}
	//Left and centre
	else if(chosenLeft.yCoord3 > 0 && chosenCentre.yCoord3 > 0 && chosenRight.yCoord3 == 0)
	{
		fuzzyOutput = ((chosenLeft.yCoord3*-2.5 + chosenCentre.yCoord3*0.001)/(chosenLeft.yCoord3 + chosenCentre.yCoord3));
	}
	//Centre
	else if(chosenLeft.yCoord3 == 0 && chosenCentre.yCoord3 > 0 && chosenRight.yCoord3 == 0)
	{
		fuzzyOutput = ((chosenCentre.yCoord3*0.001)/(chosenCentre.yCoord3));
	}
	//Centre and right
	else if(chosenLeft.yCoord3 == 0 && chosenCentre.yCoord3 > 0 && chosenRight.yCoord3 > 0)
	{
		fuzzyOutput = ((chosenCentre.yCoord3*0.001 + chosenRight.yCoord3*2.5)/(chosenCentre.yCoord3 + chosenRight.yCoord3));
	}
	//Right
	else if(chosenLeft.yCoord3 == 0 && chosenCentre.yCoord3 == 0 && chosenRight.yCoord3 > 0)
	{
		fuzzyOutput = ((chosenRight.yCoord3*2.5)/(chosenRight.yCoord3-0.1));
	}
	//Left, centre and right
	else if(chosenLeft.yCoord3 > 0 && chosenCentre.yCoord3 > 0 && chosenRight.yCoord3 > 0)
	{
		fuzzyOutput = ((chosenLeft.yCoord3*-2.5 + chosenCentre.yCoord3*0.001 + chosenRight.yCoord3*2.5)/(chosenLeft.yCoord3 + chosenCentre.yCoord3 + chosenRight.yCoord3));
	}

	return fuzzyOutput;

}

float FuzzyLogicClass::deffuzifyCentroid()
{
	return 1;
}
