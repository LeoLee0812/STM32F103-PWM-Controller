#include <stm32f10x.h>
#include <stdlib.h>

#ifndef __GPIO_INIT
	#define __GPIO_INIT
#endif

extern void GPIOInitManager(GPIO_TypeDef* port, unsigned short int pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed);
