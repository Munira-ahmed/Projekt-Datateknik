/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
int prime = 1234567;
int mytime = 0x5957;
int timeoutcounter;
int count;
char textstring[] = "text, more text, and even more text!";

void user_isr(void)
{

  if (IFS(0) & 0x100)
  {
    timeoutcounter++; // öka altid countern
    if (timeoutcounter == 10)
    { // updatera var tionde time-out

      time2string(textstring, mytime);
      display_string(3, textstring);
      display_update();
      tick(&mytime);

      timeoutcounter = 0;
    }  
    IFSCLR(0) = 0x100; // clr flaggan - infinite loop
  }
  if (IFS(0) & 0x80000)
  {
    count++;
    PORTE = count;
    IFSCLR(0) = 0x80000;
  }

}

/* Lab-specific initialization goes here */
void labinit(void)
{
  count = 0;
  TMR2 = 0x0;
  PR2 = 31250; // period
  // enable timer 2 so that it can interrupt the processor
  IPCSET(2) = 4;        //  timer2 has lowest priority - T2IP (bits 4-2)
  IECSET(0) = 0x100;    // T2IE in register IEC(0) enables the timer2 interrupter
  T2CONSET = 0x8070; // enable timer;preset equals = 1:256
  // sätt porte to output
  TRISESET = 0x00;
  // enable interrupt from sw4
  IECSET(0) = 0x80000;
  IPCSET(4) = 0x4000000;

  enable_interrupt();
}

/* This function is called repetitively from the main program */
void labwork(void)
{ /*
   nextprime: Return the first prime number larger than the integer given as a parameter.
   The integer must be positive.
  */
  prime = nextprime(prime);
  display_string(0, itoaconv(prime)); // itoa: convert binary to decimal -  Returns pointer to (static) char array
  display_update();
}
