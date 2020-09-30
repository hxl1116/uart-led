/*
 * controller.h
 *
 *  Created on: Sep 28, 2020
 *      Author: Henry Larson
 */

#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

void parse();
int handle(char *buffer);
void disable_leds();
void print_buffer(char *buffer, char *prompt);

#endif /* INC_CONTROLLER_H_ */
