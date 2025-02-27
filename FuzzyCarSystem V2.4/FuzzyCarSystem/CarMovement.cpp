//Luke Milburn - 2015
#include "CarMovement.h"
#include <Windows.h>
CarMovementClass::CarMovementClass()
{
	membershipL = 0.0;
	membershipC = 0.0;
	membershipR = 0.0;
	leftXBase1 = -4.0;
	leftXBase2 = -1.0;
	centreXBase1 = -3.2;
	centreXBase2 = 3.2;
	rightXBase1 = 1.0;
	rightXBase2 = 4.0;
	allYBase = 0.0;
}

CarMovementClass::~CarMovementClass()
{
}

void CarMovementClass::leftOfC(float inputVel)
{
	//Between -4.0 and -1.0
	//Construct the membership function for the left of centre (triangle shape)
	//Left half of membership function triangle
	if ((inputVel<=-1.0) && (inputVel>=-4.0))
	{
		//Degree of membership
		membershipL = ((inputVel + 1.0)/-3.0);					//3.0 being the difference between the highest velocity (-1.0) and lowest velocity (-4.0) in the moving left fuzzy set
		//alter the shape of the trapezoids of the member functions
		left.yCoord3 = membershipL;
		left.yCoord4 = membershipL;
	}
}

void CarMovementClass::centre(float inputVel)
{
	//Construct the membership function for the centre (triangle shape)
	//Left half of membership function triangle
	if ((inputVel <= 0) && (inputVel >= -3.2))
	{
		//Degree of membership
		membershipC = ((inputVel + 3.2)/3.2);
		//Calulate the xcoordinate of the 4th vertex of the trapezoid of the function
		//As it is the same distance from 0 as the xcoordinate of the 3rd vertex (which is the input value)
		centreT.xCoord3 = inputVel;
		centreT.xCoord4 = 0 - inputVel;						//Top right corner of the trapezium must have a positive x value - negate the negative input coord
	}
	//Right half of membership function triangle 
	else if ((inputVel <= 3.2) && (inputVel >= 0))
	{
		//degree of membership
		membershipC = ((3.2 - inputVel)/3.2);
		//Calulate the xcoordinate of the 3rd vertex of the trapezoid of the function
		//As it is the same distance from 0 as the xcoordinate of the 4th vertex (which is the input value)
		centreT.xCoord3 = -inputVel;
		centreT.xCoord4 = inputVel;
	}
	centreT.yCoord3 = membershipC;
	centreT.yCoord4 = membershipC;
}

void CarMovementClass::rightOfC(float inputVel)
{
	//Between 1.0 and 4.0
	//Construct the membership function for the right of centre (triangle shape)
	//Left half of membership function triangle
	if ((inputVel <= 4.0) && (inputVel >= 1.0))
	{
		//Degree of membership (y coordinate)
		membershipR = ((inputVel - 1.0)/3.0);
	}
	right.yCoord3 = membershipR;
	right.yCoord4 = membershipR;
}

void CarMovementClass::setInputState(float carVel)
{
	if(carVel>=-4.0 && carVel<-3.2)
		velInput = moveL;
	else if(carVel>=-3.2 && carVel<=-1.0)
		velInput = moveLSlight;
	else if(carVel>=-1.0 && carVel<=1.0)
		velInput = stationary;
	else if(carVel>=1.0 && carVel<=3.2)
		velInput = moveRSlight;
	else if(carVel>=3.2 && carVel<=4.0)
		velInput = moveR;
}

carVelState CarMovementClass::getState()
{
	//Return the enumerator describing the input state with regards to the car position
	return velInput;
}

speedTrapezium CarMovementClass::getTrapezoidL()
{
	return left;
}

speedTrapezium CarMovementClass::getTrapezoidC()
{
	return centreT;
}

speedTrapezium CarMovementClass::getTrapezoidR()
{
	return right;
}

void CarMovementClass::initialiseTraps()
{
	//Initialise every trapezoid to a flat line
	//Covering the range of their domain in x
	//Left membership function (right angled triangle)
	left.xCoord1 = leftXBase1;
	left.xCoord2 = leftXBase2;
	left.xCoord3 = leftXBase1;				//The third vertex x-coordinate of left trapezium is vertically above base x
	left.xCoord4 = leftXBase2;
	left.yCoord1 = allYBase;
	left.yCoord2 = allYBase;
	left.yCoord3 = allYBase;
	left.yCoord4 = allYBase;

	//Centre membership function (isoceles triangle)
	centreT.xCoord1 = centreXBase1;
	centreT.xCoord2 = centreXBase2;
	centreT.xCoord3 = centreXBase1;
	centreT.xCoord4 = centreXBase2;
	centreT.yCoord1 = allYBase;
	centreT.yCoord2 = allYBase;
	centreT.yCoord3 = allYBase;
	centreT.yCoord4 = allYBase;

	//Right membership function (right angled triangle)
	right.xCoord1 = rightXBase1;
	right.xCoord2 = rightXBase2;
	right.xCoord3 = rightXBase1;
	right.xCoord4 = rightXBase2;
	right.yCoord1 = allYBase;
	right.yCoord2 = allYBase;
	right.yCoord3 = allYBase;
	right.yCoord4 = allYBase;
}

void CarMovementClass::checkMembership(float inputCarVel)
{
	//If input position is within the bounds of the left membership function, call it
	if (inputCarVel >= -4.0 && inputCarVel <= -1.0)
	{
		leftOfC(inputCarVel);
		left.xCoord4 = inputCarVel;								//Set the upper right vertex x-coordinate of the left of centre member function trapezium to the value of inputCarVel (ADD THIS TO MEMBERSHIP functions)
	}
	//If not flatten left membership function triangle/trapezoid
	else
	{
		membershipL = 0.0;
		left.yCoord3 = membershipL;							//Must minimise the trapezoid for the left membership function to 0
		left.yCoord4 = membershipL;
	}
	//If input position is within the bounds of the central membership function, call it
	if (inputCarVel >= -3.2 && inputCarVel <= 3.2)
	{
		//If input value is a member of the domain of the central membership function
		//Calulate the degree of membership
		centre(inputCarVel);
	}
	//if not flatten central membership function triangle/trapezoid
	else
	{
		membershipC = 0.0;
		centreT.xCoord3 = centreXBase1;
		centreT.xCoord4 = centreXBase2;
		//Flatten middle trapezoid
		centreT.yCoord3 = membershipC;
		centreT.yCoord4 = membershipC;
	}
	//If input position is within the bounds of the right membership function, call it
	if (inputCarVel >= 1.0 && inputCarVel <= 4.0)
	{
		rightOfC(inputCarVel);
		right.xCoord3 = inputCarVel;							//Set the upper right vertex x-coordinate of the right of centre member function trapezium to the value of inputCarVel
	}
	//If not, flatten right membership function triangle/trapezoid
	else
	{
		membershipR = 0.0;
		right.xCoord3 = rightXBase1;
		right.xCoord4 = rightXBase2;
		right.yCoord3 = membershipR;
		right.yCoord4 = membershipR;
	}
}

void CarMovementClass::updateTraps(float inputCarVel)
{
	setInputState(inputCarVel);
	checkMembership(inputCarVel);

}

