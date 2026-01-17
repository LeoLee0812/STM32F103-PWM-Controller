#include <stm32f10x.h>
#include <stdlib.h>


#ifndef __TIM
	#define __TIM
#endif

extern void TIMInitManager(TIM_TypeDef* timer, uint16_t prescaler, uint16_t period, uint8_t repetition_counter, uint16_t mode, uint16_t div, uint8_t enable_it, uint8_t preemption, uint8_t sub);
