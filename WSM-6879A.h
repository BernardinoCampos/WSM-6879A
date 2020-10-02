#ifndef WSM-6879A_H
#define WSM-6879A_H

#include <inttypes.h>

class WSM-6879A {
public:

	/**
	* Constructor
	*
	* @param cs 	CS pin of display.
	* @param clk	CLK pin of display.
	* @param data	Data pin of display.
	*/

	void WSM-6879A(uint8_t cs, uint8_t clk, uint8_t data	);

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
	 * Remove all the digits currently shown.
	 */

	void clear();

	/**
	 * Show Antenna bars in display.
	 */

	void setAntenna(int rssi);

private:
	uint8_t cs, clk, data;
	uint8_t lcdBuffer[32];
	uint8_t mask [] = {
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
	void waitLcd();
	void writeCmd(uint8_t data);
	void writeByte(uint8_t data);
	void write4Bits(uint8_t data);
	void writeBit(bool data);
};

#endif
