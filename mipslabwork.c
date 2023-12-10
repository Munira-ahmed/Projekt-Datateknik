/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */
/*
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"
#include <math.h>
#include <stdio.h>

#define MAX_TUBES 5
#define TUBE_SPAWN_CHANCE 4
#define TUBE_WIDTH 5
#define TUBE_HEIGHT 60

int tubeX[MAX_TUBES] = {0};
int tubeY[MAX_TUBES] = {0};
int tubeExists[MAX_TUBES] = {0};
int tubesPassed = 0;

int x = 15;
int y = 5;

// Function to create a new tube
void createTube() {
    int i;
    for (i = 0; i < MAX_TUBES; i++) {
        if (!tubeExists[i]) {
            // Set tube position and mark it as existing
            tubeX[i] = 128;               // Set initial X position to the right side
            tubeY[i] = getRandomInRange(30, 20);     // Use your logic to set Y position
            tubeExists[i] = 1;
            break;
        }
    }
}

/// Function to update tube position
void updateTubePositions() {
    int i;
    for (i = 0; i < MAX_TUBES; i++) {
        if (tubeExists[i]) {
           if (tubeX[i] + TUBE_WIDTH < x) {
                tubesPassed++;
                tubeExists[i] = 0;  // Mark the tube as passed
            }
            // Clear the previous tube position
            drawObjectTube(tubeX[i], tubeY[i], 1); // 1 to clear pixels

            // Update tube position
            tubeX[i]--;

            // Check if tube is off the screen
            if (tubeX[i] + TUBE_WIDTH < 0) {
                // Mark the tube as not existing
                tubeExists[i] = 0;
            } else {
                // Draw the tube at the updated position
                drawObjectTube(tubeX[i], tubeY[i], 0); // 0 to set pixels
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
  InteruptFlag40ms = 1;
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

void labwork(void) {
    int i;

    if (gameState == 1) {
        // Game over logic
        display_string_clear();
        display_string(0, "GAME OVER");

        // Convert the score (tubesPassed) to a string
        char tubesPassedStr[10];
        intToStr(tubesPassed, tubesPassedStr, sizeof(tubesPassedStr));

        // Display the score as part of the string
        display_string(2, "score: ");
        display_string(3, tubesPassedStr);

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
            display_image(0, icon);
            display_string_clear();
            display_update();
            gameState = 2;
        }
    }

    if (gameState == 2) {
        // Main Menu/Start Screen logic
        display_string_clear();
        delay(10);
        display_string(0, "  Flappy Bird");
        display_update();

        if (getbtns() == 4) {
            MainMenuFade();
            StartCountDown();
        }
    }

   if (gameState == 0) {
        // Game logic when in the playing state
        clearScreenMemory();
        y += 1; // Move taco down (Gravitation)
        drawBird(x, y);

        // Randomly create a new tube with a chance of TUBE_SPAWN_CHANCE
        if (getRandomInRange(1, 100) <= TUBE_SPAWN_CHANCE) {
            createTube();
        }

        // Update and draw the tubes
        updateTubePositions();
        int GAP_WIDTH = 35;
        // Draw all tubes
        for (i = 0; i < MAX_TUBES; i++) {
            if (tubeExists[i]) {
                drawObjectTube(tubeX[i], tubeY[i]);
                
                // Check for collision with the bird
                if (detectCollision(x, y, tubeX[i], tubeY[i], TUBE_WIDTH, TUBE_HEIGHT, GAP_WIDTH)) {
                    gameState = 1;  // Collision detected, set game over state
                }
            }
        }

        // Display updated image
        display_image(0, icon);

        // Handle button press
        if (getbtns()) {
            y -= 3; // Taco moves up when the button is pressed
        }
    }
}
