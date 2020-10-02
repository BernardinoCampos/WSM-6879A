#include "WSM-6879A.h"

void WSM-6879A::WSM-6879A(uint8_t cs, uint8_t clk, uint8_t data) {
	WSM-6879A::cs 	= cs;
	WSM-6879A::clk 	= clk;
	WSM-6879A::data = data;
}

void WSM-6879A::begin() {
	memset(lcdBuffer, 0, 32);
	lcdBuffer[28] = 0x08;
	lcdBuffer[31] = 0x08;

	pinMode(cs, OUTPUT);
	pinMode(clk, OUTPUT);
	pinMode(data, OUTPUT);
	digitalWrite(cs,HIGH);
	digitalWrite(clk,HIGH);
	reset();
}

void WSM-6879A::reset() {
	digitalWrite(cs,LOW);
	waitLcd();
	writeBit(1);
	writeBit(0);
	writeBit(0);
	writeCmd(0x00);
	writeCmd(0x02);
	writeCmd(0x18);
	writeCmd(0x01);
	writeCmd(0x03);
	writeCmd(0x29);
	digitalWrite(cs,HIGH);
	waitLcd();
}

bool WSM-6879A::printDigit(uint8_t pos, uint8_t digit) {
	if (pos>14 || digit>10)
		return false;

	lcdBuffer[pos*2]	&= (uint8_t) 0x08;
	lcdBuffer[pos*2]	|= (uint8_t) (mask[digit]>>4);
	lcdBuffer[pos*2+1]	=  (uint8_t) (mask[digit] & 0x0f);

	return true;
}

void WSM-6879A::setAntenna(int rssi) {
	if (rssi>-60)
		lcdBuffer[0x1e] |= 0x01;
	if (rssi>-70)
		lcdBuffer[0x1e] |= 0x02;
	if (rssi>-80)
		lcdBuffer[0x1e] |= 0x04;
	if (rssi>-90)
		lcdBuffer[0x1e] |= 0x08;
	if (rssi>-100)
		lcdBuffer[0x1f] |= 0x04;
}

void WSM-6879A::waitLcd() {
	delayMicroseconds(50);
}

void WSM-6879A::writeCmd(uint8_t data) {
	writeByte(data);
	writeBit(0);
}

void WSM-6879A::writeByte(uint8_t data) {
	for (int ii=7; ii>=0; ii--)
		writeBit(data>>ii&1);
}

void WSM-6879A::write4Bits(uint8_t data) {
	for (int ii=3; ii>=0; ii--)
		writeBit(data>>ii&1);
}

void WSM-6879A::writeBit(bool data) {
	digitalWrite(clk,LOW);
	digitalWrite(data,(data)?HIGH:LOW);
	waitLcd();
	digitalWrite(clk,HIGH);
	waitLcd();
}



