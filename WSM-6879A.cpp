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

bool WSM-6879A::showAntenna(int rssi) {
	if (rssi>0 || rssi<-120)
		return false;

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

	return true;
}

bool WSM-6879A::showBattery(int power) {
	if (power>99)
		return false;

	if (power>89)
		lcdBuffer[0x1a] |= 0x08;
	if (power>69)
		lcdBuffer[0x18] |= 0x08;
	if (power>49)
		lcdBuffer[0x14] |= 0x08;
	if (power>29)
		lcdBuffer[0x12] |= 0x08;
	if (power>9)
		lcdBuffer[0x00] |= 0x08;

	return true;
}

bool WSM-6879A::showDecimalPoint(uint8_t pos) {
	if (pos>9)
		return false;

	switch(pos) {
		case(9):
			lcdBuffer[0x1f] |= 0x02;
			break;
		case (8):
			lcdBuffer[0x10] |= 0x08;
			break;
		case (7):
			lcdBuffer[0x0e] |= 0x08;
			break;
		case (6):
			lcdBuffer[0x0c] |= 0x08;
			break;
		case (5):
			lcdBuffer[0x0a] |= 0x08;
			break;
		case (4):
			lcdBuffer[0x08] |= 0x08;
			break;
		case (3):
			lcdBuffer[0x06] |= 0x08;
			break;
		case (2):
			lcdBuffer[0x04] |= 0x08;
			break;
		case (1):
			lcdBuffer[0x02] |= 0x08;
			break;
	}

	return true;
}

void WSM-6879A::writeBuffer() {
	int ii;

	digitalWrite(CS,LOW);
	waitLcd();
	writeBit(1);
	writeBit(0);
	writeBit(1);

	for (ii=0; ii<6; ii++)
		writeBit(0);

	for (ii=0; ii<32; ii++)
		write4Bits(lcdBuffer[ii] & 0x0f);

	digitalWrite(CS,HIGH);
	waitLcd();
}

/*
 * ************** Private Methods *****************
 * */

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



