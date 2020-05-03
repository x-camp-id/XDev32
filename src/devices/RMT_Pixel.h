/*  RMT Pixel
    Copyright (C) 2020  Sebastian Tilders

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>
*/
#ifndef _RMT_PIXEL_H
#define _RMT_PIXEL_H
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "Arduino.h"


// Currently only RGB  and GRB order is allowed
enum RMT_ColorOrder {
	RMT_PIXEL_RGB = 0,
	RMT_PIXEL_GRB
};

#define ERR_RMT_PIXEL_OK 0x0	
#define ERR_RMT_PIXEL_NOMEMORY 0x1
#define ERR_RMT_PIXEL_INIT_FAIL 0x2
#define ERR_RMT_PIXEL_NO_INIT 0x3
#define ERR_RMT_PIXEL_ALREADY_INIT 0x4
#define ERR_RMT_PIXEL_OUT_OF_RANGE 0x5
#define ERR_RMT_PIXEL_SET_FAILED 0x6
#define ERR_RMT_PIXEL_UNKOWN_ORDER 0x7



class RMTPixelType {
public:
	RMTPixelType();
	int Begin(int channel,int num, RMT_ColorOrder order);
	int Write(void);
	int SetLED(int addr, char r, char g, char b);
	int FillLED(int start, int number, char r, char g, char b);
	void End();
	~RMTPixelType();
private:
	char (*led_rgb_values)[3];			// RGB-Formatted RGB data
	int size = 0;						// Bit size of RGB fields
	bool init = false;
	int num = 0;						// Number of WS2812 LEDs
	int channel = 0;					// GPIO-PIN
	RMT_ColorOrder order;				// Color Order of Strip (RGB or GRB)
	rmt_obj_t* rmt_handle =  0;			// RM Tranceiver handle
	rmt_data_t* led_rmt_data = 0;		// WS2812 RMT duty cycle format.
	static inline rmt_data_t gen_ws2812_rmt_data(int bit);
};

extern RMTPixelType RMTPixel;
#endif