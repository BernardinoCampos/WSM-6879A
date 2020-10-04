#include "WSM-6879A.h"

WSM6879A::WSM6879A(uint8_t cs, uint8_t rd, uint8_t clk, uint8_t data) {
	WSM6879A::cs 	= cs;
	WSM6879A::rd 	= rd;
	WSM6879A::clk 	= clk;
	WSM6879A::data	= data;

	memset(mask, 0, 127);
	mask['0'] = 0x7d;
	mask['1'] = 0x60;
	mask['2'] = 0x3e;
	mask['3'] = 0x7a;
	mask['4'] = 0x63;
	mask['5'] = 0x5b;
	mask['6'] = 0x5f;
	mask['7'] = 0x70;
	mask['8'] = 0x7f;
	mask['9'] = 0x7b;
	mask['?'] = 0x36;
	mask['A'] = mask['a'] = 0x77;
	mask['B'] = mask['b'] = 0x4f;
	mask['C'] = mask['c'] = 0x1d;
	mask['D'] = mask['d'] = 0x6e;
	mask['E'] = mask['e'] = 0x1f;
	mask['F'] = mask['f'] = 0x17;
}

void WSM6879A::begin() {
	clear();

	pinMode(cs, OUTPUT);
	pinMode(clk, OUTPUT);
	pinMode(data, OUTPUT);
	pinMode(rd, OUTPUT);
	digitalWrite(rd,HIGH);
	digitalWrite(cs,HIGH);
	digitalWrite(clk,HIGH);

	reset();
}

void WSM6879A::reset() {
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

void WSM6879A::clear() {
	memset(lcdBuffer, 0, 32);
	lcdBuffer[28] = 0x08;
	lcdBuffer[31] = 0x08;
}

bool WSM6879A::printCharacter(uint8_t pos, uint8_t character) {
	if (pos>14 || character > 127)
		return false;

	lcdBuffer[(14-pos)*2]	&= (uint8_t) 0x08;
	lcdBuffer[(14-pos)*2]	|= (uint8_t) (mask[character]>>4);
	lcdBuffer[(14-pos)*2+1]	=  (uint8_t) (mask[character] & 0x0f);

	return true;
}

bool WSM6879A::showAntenna(int rssi) {
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

bool WSM6879A::showBattery(int power) {
	if (power>99)
		return false;

	if (power>89)
		lcdBuffer[0x00] |= 0x08;
	if (power>69)
		lcdBuffer[0x12] |= 0x08;
	if (power>49)
		lcdBuffer[0x14] |= 0x08;
	if (power>29)
		lcdBuffer[0x18] |= 0x08;
	if (power>9)
		lcdBuffer[0x1a] |= 0x08;

	return true;
}

bool WSM6879A::showdoublePoint() {
	lcdBuffer[0x1f] |= 0x02;
}

bool WSM6879A::showDecimalPoint(uint8_t pos) {
	if (pos>7)
		return false;

	switch(pos) {
		case (0):
			lcdBuffer[0x10] |= 0x08;
			break;
		case (1):
			lcdBuffer[0x0e] |= 0x08;
			break;
		case (2):
			lcdBuffer[0x0c] |= 0x08;
			break;
		case (3):
			lcdBuffer[0x0a] |= 0x08;
			break;
		case (4):
			lcdBuffer[0x08] |= 0x08;
			break;
		case (5):
			lcdBuffer[0x06] |= 0x08;
			break;
		case (6):
			lcdBuffer[0x04] |= 0x08;
			break;
		case (7):
			lcdBuffer[0x02] |= 0x08;
			break;
	}

	return true;
}

void WSM6879A::writeBuffer() {
	if (isJustDigits(Buffer)) {
		Buffer.trim();
		delay(500);
		int ptrBuffer = 0;
		int ptrLcd = (15-Buffer.length());
		for(;ptrLcd>0;ptrBuffer++) {
			char ch = Buffer.charAt(ptrBuffer);
			Serial.printf("ptrBuffer=%d ptrLcd=%d ch=%c\n",ptrBuffer,ptrLcd,ch);
			if ( ch != '.')
				printCharacter(ptrLcd--, ch);
			else
				showDecimalPoint(ptrLcd-6);
		}
	}
	else
		Serial.println("Nao Digito");

	writeLcdBuffer();
}

void WSM6879A::writeLcdBuffer() {
	int ii;

	digitalWrite(cs,LOW);
	waitLcd();
	writeBit(1);
	writeBit(0);
	writeBit(1);

	for (ii=0; ii<6; ii++)
		writeBit(0);

	for (ii=0; ii<32; ii++)
		write4Bits(lcdBuffer[ii] & 0x0f);

	digitalWrite(cs,HIGH);
	waitLcd();
}

size_t WSM6879A::write(uint8_t ch) {
	Buffer.concat(ch);

	writeBuffer();
}

size_t WSM6879A::write(const uint8_t *buffer, size_t size) {
	char tmp [size+1];
	strncpy(tmp,(const char *)buffer,size);
	tmp[size]=0;
	Buffer = tmp;

	writeBuffer();
}

/*
 * ************** Private Methods *****************
 * */

bool WSM6879A::isJustDigits(String str) {
	for (int ii=0; ii<str.length(); ii++) {
		char ch;
		ch = str.charAt(ii);
		if (ch!=' ' && ch!='.' && !isDigit(ch)) {
			return false;
		}
	}
	return true;
}

void WSM6879A::waitLcd() {
	delayMicroseconds(50);
}

void WSM6879A::writeCmd(uint8_t val) {
	writeByte(val);
	writeBit(0);
}

void WSM6879A::writeByte(uint8_t val) {
	for (int ii=7; ii>=0; ii--)
		writeBit(val>>ii&1);
}

void WSM6879A::write4Bits(uint8_t val) {
	for (int ii=3; ii>=0; ii--)
		writeBit(val>>ii&1);
}

void WSM6879A::writeBit(bool val) {
	digitalWrite(clk,LOW);
	digitalWrite(data,(val)?HIGH:LOW);
	waitLcd();
	digitalWrite(clk,HIGH);
	waitLcd();
}



