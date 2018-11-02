/* ============================================
Arduino SDS011 - Sensor Library
      2018 F.Bisinger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "SDS011.h"

static const byte SLEEPCMD[19] = {
	0xAA,	// head
	0xB4,	// command id
	0x06,	// data byte 1
	0x01,	// data byte 2 (set mode)
	0x00,	// data byte 3 (sleep)
	0x00,	// data byte 4
	0x00,	// data byte 5
	0x00,	// data byte 6
	0x00,	// data byte 7
	0x00,	// data byte 8
	0x00,	// data byte 9
	0x00,	// data byte 10
	0x00,	// data byte 11
	0x00,	// data byte 12
	0x00,	// data byte 13
	0xFF,	// data byte 14 (device id byte 1)
	0xFF,	// data byte 15 (device id byte 2)
	0x05,	// checksum
	0xAB	// tail
};

SDS011::SDS011(void) {
}

int SDS011::read(float *p25, float *p10) {
	byte buffer;
	int value;
	int len = 0;
	int pm10_serial = 0;
	int pm25_serial = 0;
	int checksum_is;
	int checksum_ok = 0;
	int error = 1;
	while ((sds_data->available() > 0) && (sds_data->available() >= (10-len))) {
		buffer = sds_data->read();
		value = int(buffer);
		switch (len) {
			case (0): if (value != 170) { len = -1; }; break;
			case (1): if (value != 192) { len = -1; }; break;
			case (2): pm25_serial = value; checksum_is = value; break;
			case (3): pm25_serial += (value << 8); checksum_is += value; break;
			case (4): pm10_serial = value; checksum_is += value; break;
			case (5): pm10_serial += (value << 8); checksum_is += value; break;
			case (6): checksum_is += value; break;
			case (7): checksum_is += value; break;
			case (8): if (value == (checksum_is % 256)) { checksum_ok = 1; } else { len = -1; }; break;
			case (9): if (value != 171) { len = -1; }; break;
		}
		len++;
		if (len == 10 && checksum_ok == 1) {
			*p10 = (float)pm10_serial/10.0;
			*p25 = (float)pm25_serial/10.0;
			len = 0; checksum_ok = 0; pm10_serial = 0.0; pm25_serial = 0.0; checksum_is = 0;
			error = 0;
		}
		yield();
	}
	return error;
}

void SDS011::sleep() {
	for (uint8_t i = 0; i < 19; i++) {
		sds_data->write(SLEEPCMD[i]);
	}
	sds_data->flush();
	while (sds_data->available() > 0) {
		sds_data->read();
	}
}

void SDS011::wakeup() {
	sds_data->write(0x01);
	sds_data->flush();
}

void SDS011::begin(uint8_t pin_rx, uint8_t pin_tx) {
	_pin_rx = pin_rx;
	_pin_tx = pin_tx;

	SoftwareSerial *softSerial = new SoftwareSerial(_pin_rx, _pin_tx);
	softSerial->begin(9600);
	sds_data = softSerial;
}

