//Luke Milburn - 2015
#include "CarPosition.h"
#include <Windows.h>
CarPositionClass::CarPositionClass()
{
	membershipL = 0.0;
	membershipC = 0.0;
	membershipR = 0.0;
	leftXBase1 = -350.0;
	leftXBase2 = -80.0;
	centreXBase1 = -125.0;
	centreXBase2 = 125.0;
	rightXBase1 = 80.0;
	rightXBase2 = 350.0;
	allYBase = 0.0;
}

CarPositionClass::~CarPositionClass()
{
}

void CarPositionClass::leftOfC(float inputPos)
{
	//Between -350 and -80
	//Construct the membership function for the left of centre (triangle shape)
	//Left half of membership function triangle
	if ((inputPos >= -350) && (inputPos <= -80))
	{
		//Degree of membership
		membershipL = ((inputPos + 80)/-270);					//270 being the difference between the highest x-value (-80) and lowest x-value (-350)
		//alter the shape of the trapezoids of the member functions
		left.yCoord3 = membershipL;
		left.yCoord4 = membershipL;
	}
}

void CarPositionClass::centre(float inputPos)
{
	//Construct the membership function for the centre (triangle shape)
	//Left half of membership function triangle
	if ((inputPos <= 0) && (inputPos >= -125))
	{
		//Degree of membership
		membershipC = ((inputPos + 125)/ 125.0);
		//Calulate the xcoordinate of the 4th vertex of the trapezoid of the function
		//As it is the same distance from 0 as the xcoordinate of the 3rd vertex (which is the input value)
		centreT.xCoord3 = inputPos;
		centreT.xCoord4 = 0 - inputPos;						//Top right corner of the trapezium must have a positive x value - negate the negative input coord
	}
	//Right half of membership function triangle 
	else if ((inputPos <= 125) && (inputPos >= 0))
	{
		//degree of membership
		membershipC = ((125 - inputPos)/125.0);
		//Calulate the xcoordinate of the 3rd vertex of the trapezoid of the function
		//As it is the same distance from 0 as the xcoordinate of the 4th vertex (which is the input value)
		centreT.xCoord3 = -inputPos;
		centreT.xCoord4 = inputPos;
	}
	centreT.yCoord3 = membershipC;
	centreT.yCoord4 = membershipC;
}

void CarPositionClass::rightOfC(float inputPos)
{
	//Between 80 and 350
	//Construct the membership function for the right of centre (triangle shape)
	//Left half of membership function triangle
	if ((inputPos <= 350) && (inputPos >= 80))
	{
		//Degree of membership (y coordinate)
		membershipR = ((inputPos - 80)/270.0);
	}
	right.yCoord3 = membershipR;
	right.yCoord4 = membershipR;
}

void CarPositionClass::setInputState(float carPos)
{
	//Definitively left
	if(carPos>=-350 && carPos<-125)
		posInput = leftM;
	//Ovelap between left and centre
	else if(carPos>=-125 && carPos<=-80)
		posInput = leftAndCentreM;
	else if(carPos>=-80 && carPos<=80)
		posInput = centreM;
	else if(carPos>=80 && carPos<=125)
		posInput = centreAndRightM;
	else if(carPos>=125 && carPos<=350)
		posInput = rightM;
}

carPosState CarPositionClass::getState()
{
	//Return the enumerator describing the input state with regards to the car position
	return posInput;
}

posTrapezium CarPositionClass::getTrapezoidL()
{
	return left;
}

posTrapezium CarPositionClass::getTrapezoidC()
{
	return centreT;
}

posTrapezium CarPositionClass::getTrapezoidR()
{
	return right;
}

void CarPositionClass::initialiseTraps()
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

void CarPositionClass::checkMembership(float inputX1)
{
	//If input position is within the bounds of the left membership function, call it
	if (inputX1 >= -350 && inputX1 <= -80)
	{
		leftOfC(inputX1);
		left.xCoord4 = inputX1;								//Set the upper right vertex x-coordinate of the left of centre member function trapezium to the value of inputX1 (ADD THIS TO MEMBERSHIP functions)
	}
	//If not flatten left membership function triangle/trapezoid
	else
	{
		membershipL = 0.0;
		left.yCoord3 = membershipL;							//Must minimise the trapezoid for the left membership function to 0
		left.yCoord4 = membershipL;
	}
	//If input position is within the bounds of the central membership function, call it
	if (inputX1 >= -125 && inputX1 <= 125)
	{
		//If input value is a member of the domain of the central membership function
		//Calulate the degree of membership
		centre(inputX1);
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
	if (inputX1 >= 80 && inputX1 <= 350)
	{
		rightOfC(inputX1);
		right.xCoord3 = inputX1;							//Set the upper right vertex x-coordinate of the right of centre member function trapezium to the value of inputX1
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

void CarPositionClass::updateTraps(float inputX1)
{
	setInputState(inputX1);
	checkMembership(inputX1);

}

