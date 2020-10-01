/*
 * controller.c
 *
 *  Created on: Sep 28, 2020
 *      Author: Henry Larson
 */

#include "stm32l476xx.h"
#include "UART.h"
#include "LED.h"
#include "controller.h"

#include <string.h>
#include <stdio.h>

void parse() {
	char *prompt = "\rEnter a command: ";	// Input prompt
	char input_buffer[BufferSize];	// Input buffer
	char *current = input_buffer;	// Current input buffer index
	char input;	// Current input

	int count = 0;	// Character input count

	int flash_flag;	// Determines which LED flashes
	int flash_count = 0;

	// Initialize input buffer to be empty
	memset(input_buffer, 0, sizeof input_buffer);

	// Prompt user for input
	USART_Write(USART2, (uint8_t *)prompt, strlen(prompt));

	// Main loop
	while(1) {
		input = USART_Read_NB(USART2);

		if (input != 0) {
			if (input == '\r') {	// If input is 'enter'
				// Handle command
				flash_flag = handle(input_buffer);

				// Reset input buffer
				memset(input_buffer, 0, sizeof input_buffer);
				current = input_buffer;

				// Prompt user for input
				USART_Write(USART2, (uint8_t *)'\n', 1);
				USART_Write(USART2, (uint8_t *)prompt, strlen(prompt));
			} else if (input == '\x08' || input == '\x7F') {	// If input is 'backspace'
				if (count > 0) {
					// Decrement input buffer pointer and replace value
					*--current = 0;

					// Decrement input counter
					count--;

					print_buffer(input_buffer, prompt);
				} else if (count == 0) {
					// Do not decrement input buffer and replace initial value
					*current = 0;

					print_buffer(input_buffer, prompt);
				} else {
					// Backspace pressed but there is no value left in the buffer
					continue;
				}
			} else {
				// Add input to input buffer
				*current = input;

				// Write input back to console
				USART_Write(USART2, (uint8_t *)current, 1);

				// Increment input buffer pointer
				current++;

				// Increment input count
				count++;
			}
		}

		// Check flash_flag
		flash_count++;
		if (flash_count > 900) {
			flash_count = 0;
			if (flash_flag == 1) Red_LED_Toggle();
			if (flash_flag == 2) Green_LED_Toggle();
		}

		USART_Delay(500);
	}
}

int handle(char *buffer) {
	USART_Write(USART2, (uint8_t *)"\n\r", 2);

	if (strcmp(buffer, "RON") == 0) Red_LED_On();	// Handle RON
	else if (strcmp(buffer, "ROFF") == 0) Red_LED_Off();	// Handle ROFF
	else if (strcmp(buffer, "GON") == 0) Green_LED_On();	// Handle GON
	else if (strcmp(buffer, "GOFF") == 0) Green_LED_Off();	// Handle GOFF
	else if (strcmp(buffer, "RFLASH") == 0) return 1;	// Handle RFLASH, return red LED flag
	else if (strcmp(buffer, "GFLASH") == 0) return 2;	// Handle GFLASH, return green LED flag
	else if (strcmp(buffer, "FLASHOFF") == 0) disable_leds();	// Handle FLASHOFF
	else USART_Write(USART2, (uint8_t *)"\rInvalid command\n\r", 19);	// Handle invalid command

	return 0;	// Return null LED flag
}

void disable_leds() {
	Red_LED_Off();
	Green_LED_Off();
}

void print_buffer(char *buffer, char *prompt) {
	// Clear line in console
	USART_Write(USART2, (uint8_t *)"\33[2K", 4);

	// Write prompt to console
	USART_Write(USART2, (uint8_t *)prompt, strlen(prompt));

	// Write input buffer to console
	USART_Write(USART2, (uint8_t *)buffer, strlen(buffer));
}
