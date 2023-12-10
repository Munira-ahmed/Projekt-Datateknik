#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <math.h>
#include <stdlib.h> 

int i;
int j;




/*Game starting functions*/


void MainMenuFade (void) {

	display_string(0, "   Flappy Bird");
	display_update();
	delay(200);
  display_string_clear();
	display_update();
	delay(50);
	display_string(1, "   Flappy Bird");
	display_update();
	delay(200);
  display_string_clear();
	display_update();
	delay(50);
	display_string(2, "   Flappy Bird");
	display_update();
	delay(200);
  display_string_clear();
	display_update();
	delay(50);
	display_string(3, "   Flappy Bird");
	display_update();
	delay(200);
  display_string_clear();
	display_update();
	delay(50);
	display_string(4, "   Flappy Bird");
	display_update();
	delay(200);
   display_string_clear();
	display_update();
	delay(50);
	display_string(5, "   Flappy Bird");
	display_update();
	delay(200);
   display_string_clear();
	display_update();
	delay(50);
	display_string(6, "   Flappy Bird");
	display_update();
	delay(200);


	delay(500);

}

void StartCountDown (void) {

	        display_string(2, "     Ready?");
	        display_update();
	        delay( 800 );

	        display_string(2, "      SET");
	        display_update();
	        delay( 800 );

	        display_string(2, "      GO!");
	        display_update();
	        delay( 800);

	        display_update();

         gameState = 0;   
}

/*Add displaying methods*/


void setPixels(int x, int y) {
    // Boundary check to ensure x and y are within display limits.
    if (x < 0 || x > 127 || y < 0 || y > 31) {
        return; // Exit if coordinates are out of bounds.
    }

    // Calculate the index in the 'icon' array based on y coordinate.
    // The display is divided into 4 segments, each 128 pixels wide.
    int index = x + (y / 8) * 128;

    // Calculate the bit position within a byte for the y coordinate.
    int bitPosition = y % 8;

    // Set the pixel by manipulating the specific bit.
    icon[index] &= ~(1 << bitPosition);
}
void modifyPixel(int x, int y, int setPixel) {
    // Boundary check to ensure x and y are within display limits.
    if (x < 0 || x > 127 || y < 0 || y > 31) {
        return; // Exit if coordinates are out of bounds.
    }

    // Calculate the index in the 'icon' array based on y coordinate.
    // The display is divided into 4 segments, each 128 pixels wide.
    int index = x + (y / 8) * 128;

    // Calculate the bit position within a byte for the y coordinate.
    int bitPosition = y % 8;

    // Check whether to set or clear the pixel.
    if (setPixel) {
        // Set the pixel by manipulating the specific bit.
        icon[index] &= ~(1 << bitPosition);
    } else {
        // Clear the pixel by setting the specific bit.
        icon[index] |= (1 << bitPosition);
    }
}

void pixleBird(int x, int y) {
      // Check if coordinates are within the display bounds.
    if (x < 0 || x > 128 || y < 0 || y > 32) {
		gameState = 1;
        return; // Exit if coordinates are out of bounds.
    }

    // Calculate the index in the 'icon' array and bit position.
    int index = x + (y / 8) * 128;
    int bitPosition = y % 8;

    // Collision detection: Check if the pixel is already set.
    if ((icon[index] & (1 << bitPosition)) == 0) {
        gameState = 1; // Collision detected, set game over state.
        return;
    }
	
	

    // Set the pixel for the taco.
    icon[index] &= ~(1 << bitPosition);
}

unsigned int customRandSeed = 42; // Seed value, choose any non-zero value

// Custom random number generator based on Linear Congruential Generator (LCG)
unsigned int customRand() {
    customRandSeed = (customRandSeed * 1664525 + 1013904223) & 0xFFFFFFFF;
    return customRandSeed;
}

// Function to get a random number within a specific range [min, max]
int getRandomInRange(int min, int max) {
    // Ensure the range is valid
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }

    // Calculate the range and return a random number within it
    int range = max - min + 1;
    return min + (customRand() % range);
}

void drawObjectTube(int x, int y, int clear) {
    int tubeWidth = 5;
    int tubeHeight = 60;
    int gapWidth = 30; // Adjust the gap width as needed

    if (clear) {
        // Clear the tube pixels
        for (i = 0; i < tubeHeight; i++) {
            for (j = 0; j < tubeWidth; j++) {
                modifyPixel(x + j, y + i, 1); // 1 to clear pixels
            }
        }
    } else {
        // Draw the tube
        // Initialize all pixels to white
        for (i = 0; i < tubeHeight; i++) {
            for (j = 0; j < tubeWidth; j++) {
                setPixels(x + j, y + i);
            }
        }

        // Clear a vertical gap in the white tube
        int gapStart = getRandomInRange(0, tubeHeight - gapWidth);
        int gapEnd = gapStart + gapWidth;

        for (i = gapStart; i < gapEnd; i++) {
            for (j = 0; j < tubeWidth; j++) {
                modifyPixel(x + j, y + i, 1); // Set the pixels to white in the gap
            }
        }
    }
}


void drawBird(int x, int y){
    // Drawing a small circle
    pixleBird(x, y); // center
    pixleBird(x, y - 1); // above
    pixleBird(x, y + 1); // below
    pixleBird(x - 1, y); // left
    pixleBird(x + 1, y); // right
/*Handling display*/

}
void clearScreenMemory (void){
	int i = 0;
	for(i = 0; i<512;i++){
		icon[i] = 255;
	}
	}

int detectCollision(int birdX, int birdY, int tubeX, int tubeY, int tubeWidth, int tubeHeight, int gapWidth) {
    int birdRight = birdX + 2;  // Adjust these values based on the bird size
    int birdBottom = birdY + 2; // Adjust these values based on the bird size

    int tubeRight = tubeX + tubeWidth;
    int tubeBottom = tubeY + tubeHeight;

    // Check for non-overlapping conditions
    if (birdX >= tubeRight || birdRight <= tubeX || birdY >= tubeBottom || birdBottom <= tubeY) {
        return 0;  // No collision
    }

    // Overlapping region detected, check for the white tubes
    int overlapXStart = (birdX > tubeX) ? birdX : tubeX;
    int overlapXEnd = (birdRight < tubeRight) ? birdRight : tubeRight;

    int overlapYStart = (birdY > tubeY) ? birdY : tubeY;
    int overlapYEnd = (birdBottom < tubeBottom) ? birdBottom : tubeBottom;

    // Check pixels in the overlapping region
    int i, j;
    for (i = overlapXStart; i <= overlapXEnd; i++) {
        for (j = overlapYStart; j <= overlapYEnd; j++) {
            int pixelIndex = i + (j / 8) * 128;
            int bitPosition = j % 8;

            // Check if the pixel is set (collision detected)
            if ((icon[pixelIndex] & (1 << bitPosition)) == 0) {
                return 1; // Collision detected
            }
        }
    }

    return 0; // No collision
}


void intToStr(int value, char *buffer, int bufferSize) {
    // Handle special case for zero
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    // Start filling the buffer from the end
    int index = bufferSize - 1;
    buffer[index] = '\0';

    // Fill the buffer with digits
    while (value > 0 && index > 0) {
        buffer[--index] = '0' + value % 10;
        value /= 10;
    }

    // Copy the result to the beginning of the buffer
    int i = 0;
    while (buffer[index] != '\0' && i < bufferSize - 1) {
        buffer[i++] = buffer[index++];
    }

    // Null-terminate the string
    buffer[i] = '\0';
}



