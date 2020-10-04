#ifndef WSM_6879A_H
#define WSM_6879A_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Arduino.h>

class WSM6879A: public Print {
public:

	/**
	* Constructor
	*
	* @param cs 	CS pin of display.
	* @param rd		RD pin of display.
	* @param clk	CLK pin of display.
	* @param data	Data pin of display.
	*/

	WSM6879A(uint8_t cs, uint8_t rd, uint8_t clk, uint8_t data	);

	/**
	 * Set the display in the correct begin state, must be called before anything else is done.
	 */

	void begin();

	/**
	 * Reset display to initial state.
	 */

	void reset();

	/**
	 * Show the character (if possible) on the LCD display in the position pos.
	 */

	bool printCharacter(uint8_t pos, uint8_t digit);

	/**
	 * Remove all the digits currently shown (need writeLcdBuffer to display).
	 */

	void clear();

	/**
	 * Show Antenna bars on the LCD  display (rssi in dBm) (need writeLcdBuffer to display).
	 */

	bool showAntenna(int rssi);

	/**
	 * Show Battery bars on the LCD  display (power in percentage) (need writeLcdBuffer to display).
	 */

	bool showBattery(int power);

	/**
	 * Show Decimal Point on the LCD display in position pos (need writeLcdBuffer to display)
	 */

	bool showDecimalPoint(uint8_t pos);
	bool showdoublePoint();

	/**
	 * Send LcdBuffer to the LCD display.
	 */

	void writeLcdBuffer();

	/**
	 * Send Buffer to the LcdBuffer.
	 */

	void writeBuffer ();

	virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t *, size_t);

private:
	uint8_t cs, rd, clk, data;
	uint8_t lcdBuffer[32];
	String Buffer;
	uint8_t mask [127];
	bool debug = false;

	bool isJustDigits(String str);
	void waitLcd();
	void writeCmd(uint8_t val);
	void writeByte(uint8_t val);
	void write4Bits(uint8_t val);
	void writeBit(bool val);
};

#endif
