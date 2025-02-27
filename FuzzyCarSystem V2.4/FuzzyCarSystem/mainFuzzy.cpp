
#include <SFML/Graphics.hpp>
#include <SFML\Config.hpp>
#include <SFML\Main.hpp>
#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>
#include <SFML\System.hpp>
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <string>
#include <sstream>
#include "Fuzzy.h"


//Struct to represent a moving car
struct MovingSprite
{
	sf::Sprite mSprite;
	float vX;															//Velocity in the x axis
};

//Function Prototypes
void prepSprites();																	//Loads textures from file and applies them to sprites to be rendered
void renderSprites();																//Render all the sprites
void prepText();																	//Initialise text
void dispCrisp(float output);														//Display crisp output
void moveBackground(sf::Sprite & background);										//Scroll Background
void checkBackground(sf::Sprite & background, sf::Sprite & background2);			//Make sure background is continous
void useAI(MovingSprite & player);													//Changed from the original move player function to one that utilises the AI Fuzzy Inference System
void moveCar(MovingSprite & player);												//Move car based on output from AI algorithm

//Global Variables
//The Window
sf::RenderWindow window(sf::VideoMode(700, 700), "Fuzzy Car System");
//Texture variables
sf::Texture playerText;
sf::Texture backText1;
sf::Texture backText2;
sf::Text crispOutput;													//Variable that stores text to be displayed on screen
sf::Font theFont;


//Sprite variables
MovingSprite theCar;
sf::Sprite Player;														//The car
sf::Sprite Background1;													//Two backgrounds neccessary for scrolling
sf::Sprite Background2;
FuzzyLogicClass* fuzzySystem;											//Instance of fuzzy logic class


int main()
{
	sf::Clock timer;														//Timer to control execution of fuzzy logic algorithm
	prepSprites();															//Prepare sprites for rendering
	prepText();
	//Initialise fuzzy logic class
	fuzzySystem = new FuzzyLogicClass;
	fuzzySystem->initializeFuzzySystem();
	
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		//Control AI execution
		if(timer.getElapsedTime()>sf::milliseconds(160))
		{
			useAI(theCar);
			timer.restart();													// Reset timer
		}
		moveCar(theCar);
		checkBackground(Background1, Background2);
        window.clear();
		renderSprites();														//Draw Background road and car
        //window.draw(shape);
        window.display();
    }


	//Delete pointers and instance of fuzzy logic class
	fuzzySystem->~FuzzyLogicClass();
	delete fuzzySystem;
    return 0;
}

void prepSprites()
{
	//Set the car image to have transparency in the white border sections
	sf::Image carImage;
	carImage.loadFromFile("Car.png");
	carImage.createMaskFromColor(sf::Color(255, 255, 255), 0);							//Make borders transparent
	carImage.saveToFile("Car.png");														//Save changes back to original car file
	
	//Load bitmaps from file to be used as textures
	playerText.loadFromFile("Car.png");
	backText1.loadFromFile("Road.bmp");
	backText2.loadFromFile("Road.bmp");

	//Apply textures to sprites and set Sprite variables
	//The car
	Player.setTexture(playerText);
	Player.setTextureRect(sf::IntRect(0, 0, 32, 64));
	Player.setColor(sf::Color(255, 255, 255, 255));
	Player.setPosition(350.0f, 328.0f);
	//Load theCar Moving Sprite struct with Player
	theCar.mSprite = Player;
	theCar.vX = -1.0f;													//Set initial speed to 0

	//First Road
	Background1.setTexture(backText1);
	Background1.setTextureRect(sf::IntRect(0, 0, 700, 700));
	Background1.setColor(sf::Color(255, 255, 255, 255));
	Background1.setPosition(0.0f, 0.0f);

	//Second Road
	Background2.setTexture(backText2);
	Background2.setTextureRect(sf::IntRect(0, 0, 700, 700));
	Background2.setColor(sf::Color(255, 255, 255, 255));
	Background2.setPosition(0, 700);

}

void renderSprites()
{
	window.draw(Background1);
	window.draw(Background2);
	window.draw(theCar.mSprite);
	window.draw(crispOutput);
}

void prepText()
{
	theFont.loadFromFile("Xcelsion.ttf");
	crispOutput.setFont(theFont);
	crispOutput.setColor(sf::Color::White);					//Set display colour to white
	crispOutput.setPosition(0.0f, 0.0f);
	crispOutput.setCharacterSize(20U);
}

void dispCrisp(float output)
{
	std::ostringstream txtStream;
	txtStream<<"The crisp output is:  "<<output;
	crispOutput.setString(txtStream.str());
}

void moveBackground(sf::Sprite & background)
{
	background.move(0.0, 0.4);
}

void checkBackground(sf::Sprite & background, sf::Sprite & background2)
{
	sf::Vector2f back1Pos = background.getPosition();						//Vector containing two floats to hold the x and y position of both backgrounds
	sf::Vector2f back2Pos = background2.getPosition();
	
	if(back1Pos.y>=0 && (back1Pos.y+700)>700 )
	{
		moveBackground(background);
		background2.setPosition(back2Pos.x,back1Pos.y-700);
	}
	if(back2Pos.y>=0 && (back2Pos.y+700)>700 )
	{
		moveBackground(background2);
		background.setPosition(back1Pos.x,back2Pos.y-700);
	}
}

//function to check key pressed and then move the player in the correct direction
void useAI(MovingSprite & player)
{
	sf::Vector2f carPosDat = player.mSprite.getPosition();											//2D float Vector to hold the position data of the car
	float balancedPos;

	//Obtain a coordinate for player x position compatible with fuzzy Logic Class input (between -350 and 350)
	balancedPos = carPosDat.x - 334.0f;
	float xPos = balancedPos;
	float vX = player.vX;
	fuzzySystem->applyFuzzyLogic(xPos, vX);															//Apply fuzzy logic
	//fuzzySystem->forceOutput();																	//If any output membership is too small make it a minimum of 0.1
	vX =  (fuzzySystem->deffuzifyWAM()*0.7);
	player.vX = vX;																					//Player speed equal to output
}

void moveCar(MovingSprite & player)
{
	player.mSprite.move(player.vX, 0.0f);																	//Move the car using deffuzified values
	dispCrisp(player.vX);																					//Display output text
}

