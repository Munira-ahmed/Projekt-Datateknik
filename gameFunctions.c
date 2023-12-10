/**
 * This file was written 2023 and updated 2023-12-10 by Munira Ahmed & Fariba Mohammedi
*/

#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define FADE_DELAY 200 // Delay in milliseconds

int i;
int j;

/*Game starting functions*/

void countDown(void)
{ // by Fariba

	const char *messages[] = {" Ready? ", " SET ", " GO! "};
	int i;

	for (i = 0; i < 3; i++)
	{
		display_string(2, messages[i]); // display message
		display_update();				// update the display
		delay(800);						// wait for 800 ms
	}

	StageInTheGame = 0; // game state = 0
}

/*Add displaying methods*/


/*The display is divided into 4 segments, each 128 pixels wide (128/4 = 8)*/

void modifyPixel(int x, int y)
{ // by Fariba

	// x from 0 to 127, y from 0 to 31, whole display is 128*32
	if (x < 0 || x > 127 || y < 0 || y > 31)
	{
		return; // if coordinates are out of bounds, exit!
	}

    //map the 2D coordinates to the appropriate position in the 1D array that represents the display in memory
	int index = x + (y / 8) * 128;

	// specific bit in a byte at the index corresponding to the y coordinate
	int bitPosition = y % 8;
	
		// Set the pixel by manipulating the specific bit.
	background[index] &= ~(1 << bitPosition);
	
}

void pixleBird(int x, int y)
{	// by Fariba

	// coordinates are within the display bounds x(0,128) y(0,32)
	if (x < 0 || x > 128 || y < 0 || y > 32)
	{
		StageInTheGame = 1;
		return; //if coordinates are out of bounds, exit!
	}

	//where the bird's pixel should be drawn in the background
	int index = x + (y / 8) * 128;

	//specific bit in a byte at the index corresponding to the y coordinate
	int bitPosition = y % 8;

	//sets a specific bit in the background array to draw the bird's pixel
	background[index] &= ~(1 << bitPosition);
}

//pseudo-random number between 0 and 2^32 - 1 (0xFFFFFFFF). Example of this found in stackoverflow. https://stackoverflow.com/questions/12897992/random-number-generator-that-doesnt-use-rand-srand-c-functions
unsigned int customRandSeed = 52; 
unsigned int customRand()
{
	customRandSeed = (customRandSeed * 1664525 + 1013904223) & 0xFFFFFFFF;
	return customRandSeed;
}

// Function to get a random number within a specific range [min, max]. A pseudo-random number generator inspired by stackoverflow (not copied) since we couldn't use rand() or srand().
int getRandomInRange(int min, int max){ // by Munira
	// Ensure the range is valid
	if (min > max)
	{
		int temp = min;
		min = max;
		max = temp;
	}

	// Calculate the range and return a random number within it
	int range = max - min + 1;
	return min + (customRand() % range);
}

//Draw or remove a tube from the display using modifyPixel()
void drawObjectTube(int x, int y){   // by Munira
	int tubeWidth = 5;
	int tubeHeight = 60;

		// Draw the tube, by setting all the pixels. Or clear the tube.
		for (i = 0; i < tubeHeight; i++)
		{
			for (j = 0; j < tubeWidth; j++)
			{
				modifyPixel(x + j, y + i); 
			}
		}		
}

void drawBird(int x, int y)
{ // by Fariba
	// Drawing a small bird ;)
	pixleBird(x, y);	 // center
	pixleBird(x, y - 1); // above
	pixleBird(x, y + 1); // below
	pixleBird(x - 1, y); // left
	pixleBird(x + 1, y); // right
	
}

void clearScreenMemory(void)
{ // by Fariba
	int i;

	//iterate through the entire array 
	//total size in bytes                 //the size of a single element in the array
	for (i = 0; i < sizeof(background) / sizeof(background[0]); i++)
	{
		//clear the screen by setting each pixel to its max value
		background[i] = 0xFF; // 0xFF is equivalent to 255 in dec and 1111 1111 in binary 
	}
}

//This function detects when the bird collides with an obstacle
int detectCollision(int birdX, int birdY, int tubeX, int tubeY, int tubeWidth, int tubeHeight){	// by Munira

	int birdRight = birdX + 3;	
	int birdBottom = birdY + 3; 

	int tubeRight = tubeX + tubeWidth;
	int tubeBottom = tubeY + tubeHeight;

	// Check if the bird is overlapping with a tube
	if (birdX >= tubeRight || birdRight <= tubeX || birdY >= tubeBottom || birdBottom <= tubeY)
	{
		return 0; // No crash
	}else
	{
		return 1; //There is a crash
	}
	
}
