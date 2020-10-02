#ifndef WSM-6879A_H
#define WSM-6879A_H

#endif

class WSM-6879A {
public:
	/**
	* Constructor
	*
	* @param cs 	CS pin of display.
	* @param clk	CLK pin of display.
	* @param data	Data pin of display.
	*/
	WSM-6879A(int cs, int clk, int data	);

private:
	int cs, clk, data;
	unsigned char lcdBuffer[32];
};