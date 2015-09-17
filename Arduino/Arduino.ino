/******************************************************************************
 * SequencerGUI                                                               *
 * Copyright (C) 2015                                                         *
 * Tomassino Ferrauto <t_ferrauto@yahoo.it>                                   *
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

// The baud rate to use for communication with computer
const long baudRate = 115200;

void setup()
{
	Serial.begin(baudRate);
}

void loop()
{
	while (Serial.available() > 0) {
		const int datum = Serial.read();
	}

	char msg[10];
	msg[0] = 'D';
	msg[1] = 5;
	msg[2] = 'C';
	msg[3] = 'i';
	msg[4] = 'a';
	msg[5] = 'o';
	msg[6] = '!';
	msg[7] = 0;

	Serial.write(msg);

	delay(500);
}
