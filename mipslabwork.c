/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file was changed and updated 2023-12-10 by Munira Ahmed & Fariba Mohammedi

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */
/*
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"
#include <math.h>
#include <stdio.h>

#define MAX_TUBES 8
#define TUBE_SPAWN_CHANCE 4
#define TUBE_WIDTH 5
#define TUBE_HEIGHT 60

int tubeX[MAX_TUBES] = {0};
int tubeY[MAX_TUBES] = {0};
int tubeExists[MAX_TUBES] = {0};
int tubesPassed = 0;

int StageInTheGame = 2;
int x = 15;
int y = 5;

// Function to create a new tube
void createTube() {  // by Munira
    int i;
    for (i = 0; i < MAX_TUBES; i++) {
        if (!tubeExists[i]) {
            // Set tube position and mark  as existing
            tubeX[i] = 128;               // Set initial X position to the right side
            tubeY[i] = getRandomInRange(16, 25);     //  set Y position, controls the height of the obstacles (different heights).
            tubeExists[i] = 1;
            break;
        }
    }
}

/// Function to update tube position
void updateTubePositions() {  // by Munira
    int i;
    for (i = 0; i < MAX_TUBES; i++) {
        if (tubeExists[i]) {
           if (tubeX[i] + TUBE_WIDTH < x) {
                tubesPassed++;
                tubeExists[i] = 0;  // Mark the tube as passed
            }
            // Clear the previous tube position
            drawObjectTube(tubeX[i], tubeY[i]); // 1 to clear pixels

            // Update tube position
            tubeX[i]--;

            // Check if tube is off the screen
            if (tubeX[i] + TUBE_WIDTH < 0) {
                // Mark the tube as not existing
                tubeExists[i] = 0;
            } else {
                // Draw the tube at the updated position
                drawObjectTube(tubeX[i], tubeY[i]); // 0 to set pixels
            }
        }
    }
}

// Initieras globalt
volatile int *initPORTE = (volatile int *)0xbf886110;

// make install TTYDEV=/dev/cu.usbserial-AJV9JY30

/* Interrupt Service Routine */
void user_isr(void)
{
  IFSCLR(0) = 0x100;
  InteruptFlag = 1;
  return;
}

/* Lab-specific initialization goes here */
void labinit(void)
{

  // Initiera TRISE så att det är output.
  volatile int *initTRISE = (volatile int *)0xbf886100;
  *initTRISE = ~0xff;
  // PORTE sätts till 0 vid initiering.
  *initPORTE = 0;
  // Initiera TRISE så att det är input.
  TRISD = 0xfe0;

  T2CON = 0x70;                // 0111 0000 Vi sätter bit 6:4 till 111 prescale 1:256
                               // set prescaler at 1:1, internal clock source
  TMR2 = 0x0;                  // Clear timer register
  PR2 = (80000000 / 256) / 25; // = 31250 < 65 536 (=2^16)   // Load period register
  T2CONSET = 0x8000;           // Start timer


// Initialize pointers with appropriate casting
    int *IEC = (int *)0xbf881060;
    int *IPC2 = (int *)0xbf8810b0;

    *IEC = 0x100;
    *IPC2 = *IPC2 | 0x10;

    enable_interrupt();

    return;
}

//The code here was written by both Munira and Fariba
void gameoverStage(void) {
    int i;

    // Game over logic
    display_string_clear();
    display_string(0, "GAME OVER");

    // Convert the score (tubesPassed) to a string
    char* strScore = itoaconv(tubesPassed);

    // Display the score as part of the string
    display_string(2, "score: ");
    display_string(3, strScore);

    delay(100);
    display_update();

    // Clear all tubes
    for (i = 0; i < MAX_TUBES; i++) {
        tubeExists[i] = 0;
    }

    // Reset game state and objects
    x = 15;
    y = 5;

    if (getbtns()) {
        tubesPassed = 0;
        clearScreenMemory();
        display_image(0, background);
        display_string_clear();
        display_update();
        countDown();
        StageInTheGame = 0;
    }
}

void startScreenStage(void) {
    // Main Menu/Start Screen logic
    display_string_clear();
    delay(10);
    display_string(0, "  Flappy Bird");
    display_update();

    if (getbtns() == 4) {
        countDown();
    }
}

void playingStage(void) {
    int i;

    // Game logic when in the playing state
    clearScreenMemory();
    y += 1; // move the bird down constantly
    drawBird(x, y);

    // Randomly create a new tube with a chance of TUBE_SPAWN_CHANCE
    if (getRandomInRange(1, 100) <= TUBE_SPAWN_CHANCE) {
        createTube();
    }

    // Update and draw the tubes
    updateTubePositions();

    // Draw all tubes
    for (i = 0; i < MAX_TUBES; i++) {
        if (tubeExists[i]) {
            drawObjectTube(tubeX[i], tubeY[i]);

            // Check for collision with the bird
            if (detectCollision(x, y, tubeX[i], tubeY[i], TUBE_WIDTH, TUBE_HEIGHT)) {
                StageInTheGame = 1; // Collision detected, go to game over stage
            }
        }
    }

    // Display updated image
    display_image(0, background);

    // Handle button press
    if (getbtns()) {
        y -= 3; // The bird moves up when the buttons 4,3 and 2 are pressed. Button 1 has no function.
    }
}

void labwork(void) {
    if (StageInTheGame == 1) {
        gameoverStage();
    } else if (StageInTheGame == 2) {
        startScreenStage();
    } else {
        playingStage();
    }
}
