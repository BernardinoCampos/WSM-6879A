#ifndef WSM_6879A_H
#define WSM_6879A_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Arduino.h>

class WSM6879A {
public:

	/**
	* Constructor
	*
	* @param cs 	CS pin of display.
	* @param clk	CLK pin of display.
	* @param data	Data pin of display.
	*/

	WSM6879A(uint8_t cs, uint8_t clk, uint8_t data	);

	/**
	 * Set the display in the correct begin state, must be called before anything else is done.
	 */

	void begin();

	/**
	 * Reset display to initial state.
	 */

	void reset();

	/**
	 * Show the digit on the LCD display in the position pos.
	 */

	bool printDigit(uint8_t pos, uint8_t digit);

	/**
	 * Remove all the digits currently shown (need writeBuffer to display).
	 */

	void clear();

	/**
	 * Show Antenna bars on the LCD  display (rssi in dBm) (need writeBuffer to display).
	 */

	bool showAntenna(int rssi);

	/**
	 * Show Battery bars on the LCD  display (power in percentage) (need writeBuffer to display).
	 */

	bool showBattery(int power);

	/**
	 * Show Decimal Point on the LCD display in position pos (need writeBuffer to display)
	 */

	bool showDecimalPoint(uint8_t pos);

	void writeBuffer ();

	void setDebug(bool flag);
private:
	uint8_t cs, clk, data;
	uint8_t lcdBuffer[32];
	uint8_t mask [10] = {
			0x7d,   // 0
			0x60,   // 1
			0x3e,   // 2
			0x7a,   // 3
			0x63,   // 4
			0x5b,   // 5
			0x5f,   // 6
			0x70,   // 7
			0x7f,   // 8
			0x7b    // 9
	};
	bool debug = false;

	void waitLcd();
	void writeCmd(uint8_t val);
	void writeByte(uint8_t val);
	void write4Bits(uint8_t val);
	void writeBit(bool val);
};

#endif
