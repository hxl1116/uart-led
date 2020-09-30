#include "stm32l476xx.h"
#include "SysClock.h"
#include "UART.h"
#include "LED.h"
#include "controller.h"

#include <string.h>
#include <stdio.h>

//char buffer[BufferSize];

int main(void) {
	SysClock_Init();
	UART2_Init();
	LED_Init();

	parse();
}
