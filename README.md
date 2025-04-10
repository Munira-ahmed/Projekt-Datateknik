# Mini Flappy Bird Game

## Description
This project is a simplified version of Flappy Bird, developed by my project partner and me. The game displays random obstacles that the player must avoid, replicating the core gameplay mechanics of the original Flappy Bird.

The goal is to create an engaging, minimal version of the game that runs on embedded hardware using the ChipKIT platform.

## Equipment
To run this game, you will need the following hardware:

- ChipKIT Uno32 microcontroller board  
- ChipKIT Basic I/O Shield with a built-in display

The game graphics are shown on the display, and the game logic is executed on the Uno32 board.

## How to Run

### 1. Open the Terminal
Open the msys2 mingw64 terminal.

### 2. Navigate to the Project Directory

### 3. Set up the Build Environment
Activate the build tools environment by running:
. /opt/mcb32tools/environment

### 4. Compile the Code
Build the project with:
make

### 5. Upload the Code to the board
Install the compiled program on your Uno32 board. Replace /dev/ttyS2 with the appropriate serial port (it may vary between ttyS0 to ttyS5 on Windows):
make install TTYDEV=/dev/ttyS2

##Notes
1. Make sure the board is connected properly to your PC via USB before running make install.

2. If you're unsure which serial port to use, check your system's device manager (on Windows) or use dmesg | grep tty on Linux to identify the correct one.

##Authors
Munira Ahmed
Fariba Mohammedi 

*This project was developed as part of a course in Computer Hardware Engineering (IS1200) at KTH



