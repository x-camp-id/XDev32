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
#include "RMT_Pixel.h"

RMTPixelType::RMTPixelType()  {
	this->size = 24;
}

int RMTPixelType::Begin(int channel,int num_led, RMT_ColorOrder order) {
	if (this->init)
		return ERR_RMT_PIXEL_ALREADY_INIT;
	this->channel = channel;
	this->num = num_led;
	this->order = order;
	this->led_rgb_values = new char[this->num][3];
	if (this->led_rgb_values == NULL)
		return ERR_RMT_PIXEL_NOMEMORY;
	this->led_rmt_data = new rmt_data_t[this->size * this->num];
	if (this->led_rmt_data == NULL) {
		 delete this->led_rgb_values;
		return ERR_RMT_PIXEL_NOMEMORY;
	}
	for (int i=0; i < this->num; i++) {
		this->led_rgb_values[i][0] = 0;
		this->led_rgb_values[i][1] = 0;
		this->led_rgb_values[i][2] = 0;
	}
	this->rmt_handle = rmtInit(this->channel,true,RMT_MEM_64);
	if (this->rmt_handle == NULL)
		return ERR_RMT_PIXEL_INIT_FAIL;
	float tick = rmtSetTick(this->rmt_handle, 100);
	if(tick == 0)
		return ERR_RMT_PIXEL_INIT_FAIL;
	this->Write();
	this->init = true;
	return ERR_RMT_PIXEL_OK;
}

int RMTPixelType::SetLED(int addr,char r, char g, char b) {
	if (!this->init)
		return ERR_RMT_PIXEL_NO_INIT;
	if (addr >= this->num) {
		return ERR_RMT_PIXEL_OUT_OF_RANGE;
	}
	switch(this->order) {
		case RMT_PIXEL_RGB:
			this->led_rgb_values[addr][0] = r;
			this->led_rgb_values[addr][1] = g;
			this->led_rgb_values[addr][2] = b;
		break;
		case RMT_PIXEL_GRB:
			this->led_rgb_values[addr][0] = g;
			this->led_rgb_values[addr][1] = r;
			this->led_rgb_values[addr][2] = b;
		break;
		default:
			return ERR_RMT_PIXEL_UNKOWN_ORDER;
	}
	return ERR_RMT_PIXEL_OK;
}

int RMTPixelType::FillLED(int start, int number, char r, char g, char b) {
	int err = 0;
	if ((start+number) > this->num) {
		return ERR_RMT_PIXEL_OUT_OF_RANGE;
	}
	
	for(int i = start; i < (number+start); i++) {
		if (err = this->SetLED(i,r,g,b)) {
			return ERR_RMT_PIXEL_SET_FAILED;
		}
	}
	return ERR_RMT_PIXEL_OK;
}

int RMTPixelType::Write() {
	const int num_color_ch = (this->size/8);
	int i = 0;
	if (!this->init)
		return ERR_RMT_PIXEL_NO_INIT;
	for (int led=0; led<this->num; led++) {
		for(int color=0;color < num_color_ch; color++) {
			for (int bit=0; bit<8; bit++) {
				this->led_rmt_data[i] = RMTPixelType::gen_ws2812_rmt_data(this->led_rgb_values[led][color] & (1 << (7-bit)));
				i++;
			}
		}
	}
	rmtWrite(this->rmt_handle,this->led_rmt_data,this->size * this->num);
	return ERR_RMT_PIXEL_OK;
}

inline rmt_data_t RMTPixelType::gen_ws2812_rmt_data(int bit) {
	rmt_data_t val = {0,0,0,0};
	if (bit) {
		val.level0 = 1;
        val.duration0 = 8;
        val.level1 = 0;
        val.duration1 = 4;
    } else {
    	val.level0 = 1;
    	val.duration0 = 4;
    	val.level1 = 0;
    	val.duration1 = 8;
    }
    return val;
}

RMTPixelType::~RMTPixelType() {
	// rmtDeinit(this->rmt_handle); // Causes heap corruption TODO: file issue
	delete this->led_rgb_values;
	delete this->led_rmt_data;
}

RMTPixelType RMTPixel;