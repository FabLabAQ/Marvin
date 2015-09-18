// Simple sketch used to update the face bitmap on startup using Adafruit's LED backpack with 8x8 matrix.

/******************************************************************************
 * Face.ino                                                                   *
 * Copyright (C) 2015                                                         *
 * Luca Anastasio <anastasio.lu@gmail.com>                                    *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation; either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA *
 ******************************************************************************/

#include <Wire.h>                                // import Wire library to use I²C
#include "Adafruit_LEDBackpack.h"                // import backpack library to use LED backpacks
#include "Adafruit_GFX.h"                        // import GFX library to draw bitmaps on LED backpacks

Adafruit_8x8matrix face = Adafruit_8x8matrix();  // create face object

static const uint8_t PROGMEM                     // declare a smile bitmap
  smile_bmp[] =
  { B00000000,
    B01100110,
    B00000000,
    B00100100,
    B00000000,
    B01000010,
    B00111100,
    B00000000 };
    
void initializeFace()
{
  face.begin(0x70);         // initialize LED backpack over I²C
  face.setRotation(3);      // set rotation to match the position on the robot
  face.setBrightness(7);    // set an appropriate brightness (0-15)
}

void smile()
{
  face.clear();                                        // clear whatever has left on the display
  face.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);      // send bitmap to cover the entire display area
  face.writeDisplay();                                 // show image
}

void setup()
{
  initializeFace();         // initialize Adafruit's LED backpack
  smile();                  // draw a smiling face
}

void loop()
{
  //nothing to do here
}
