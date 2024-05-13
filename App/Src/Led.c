/*
 * Led.c
 *
 *  Created on: May 10, 2024
 *      Author: matio
 */

#include "main.h"
#include "led.h"

#define NUM_OF_PIXELS (4)
#define PAYLOAD_BYTES (40)
#define DISP_WIDTH (2)
#define DISP_HEIGHT (2)
#define COLOR_SIZE (8)
#define SPI_WORD_LEN (8)

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_tx;

uint8_t matrix_buffor[PAYLOAD_BYTES + NUM_OF_PIXELS * 3 * SPI_WORD_LEN];
uint8_t input[NUM_OF_PIXELS * 3];
const uint8_t zero = ~0b11100000;
const uint8_t one = ~0b11111000;
const uint8_t reset = ~0b00000000;


typedef struct{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}Pixel;

Pixel px[NUM_OF_PIXELS];
Pixel matrix[DISP_HEIGHT][DISP_WIDTH];
Pixel tempMatrix[DISP_HEIGHT][2*DISP_WIDTH];
Pixel blank = {0,0,0};
uint8_t control = 0;
uint8_t animation_number = 0;

void create_px_stream(Pixel *px, uint8_t *outputbuf) {
	for (size_t i = 0; i < COLOR_SIZE; i++) {
		if (px->green & (1 << (7 - i)))
			outputbuf[i] = one;
		else
			outputbuf[i] = zero;
	}
	outputbuf = &outputbuf[COLOR_SIZE];

	for (size_t i = 0; i < COLOR_SIZE; i++) {
		if (px->red & (1 << (7 - i)))
			outputbuf[i] = one;
		else
			outputbuf[i] = zero;
	}
	outputbuf = &outputbuf[COLOR_SIZE];

	for (size_t i = 0; i < COLOR_SIZE; i++) {
		if (px->blue & (1 << (7 - i)))
			outputbuf[i] = one;
		else
			outputbuf[i] = zero;
	}
}


void create_preambule_stream(uint8_t *outputbuf){
	for(size_t i = 0; i < PAYLOAD_BYTES; i++){
		outputbuf[i] = reset;
	}
}

void create_matrix_stream(Pixel* px, uint8_t *outputbuf, uint8_t pixelcount){
	create_preambule_stream(outputbuf);
	uint8_t* payload = &outputbuf[PAYLOAD_BYTES];
	for(size_t i = 0; i < pixelcount; i++){
		create_px_stream(&px[i], &(payload[i*sizeof(Pixel)*SPI_WORD_LEN]));
	}
}

void send_data_to_stoplight(){
	create_matrix_stream(px, matrix_buffor, NUM_OF_PIXELS);
	HAL_SPI_Transmit_DMA(&hspi1, matrix_buffor, sizeof(matrix_buffor));
//	HAL_SPI_Transmit_DMA(&hspi1, tab, sizeof(tab));
//	status =  HAL_SPI_Transmit(&hspi1, tab, sizeof(tab), 1000);
}

void update_matrix(){
	for (size_t row = 0; row < DISP_HEIGHT; row++){
		for (size_t col = 0; col < DISP_WIDTH; col++){
			matrix[row][col] = px[col + row*DISP_WIDTH];
		}
	}
}

void sliding_right_anmation(){
	update_matrix();
	for (size_t row = 0; row < DISP_HEIGHT; row++){
		for (size_t col = 0; col < DISP_WIDTH; col++){
			if (col == 0)
				px[col + row*DISP_WIDTH] = matrix[row][DISP_WIDTH - 1];
			else
				px[col + row * DISP_WIDTH] = matrix[row][col-1];
		}
	}
	send_data_to_stoplight();
}

void sliding_left_animation() {
    update_matrix();
    for (size_t row = 0; row < DISP_HEIGHT; row++) {
        for (size_t col = 0; col < DISP_WIDTH; col++) {
            if (col == DISP_WIDTH - 1)
                px[col + row * DISP_WIDTH] = matrix[row][0];
            else
                px[col + row * DISP_WIDTH] = matrix[row][col + 1];
        }
    }
	send_data_to_stoplight();
}
void sliding_once() {
	update_matrix();
	if (control > DISP_WIDTH){
		for (size_t row = 0; row < DISP_HEIGHT; row++){
			for (size_t col = 0; col < DISP_WIDTH; col++){ 				// od środka do zniknięcia z prawej strony
				if (col == 0){
					px[col + row*DISP_WIDTH] = blank;
				}
				else
					px[col + row * DISP_WIDTH] = matrix[row][col-1];
			}
		}
	}
	send_data_to_stoplight();
}


void display_init(){
	for(size_t i = 0; i < NUM_OF_PIXELS; i++){
		px[i].red = 162;
		px[i].blue = 255;
		px[i].green= 100;
	}
}

void LED_Update(){
	send_data_to_stoplight();
//	HAL_Delay(1000);

}
