#include "tim_init.h"
#include "gpio_init.h"
	
	
//  **************************************************
// Timer Init
//  **************************************************
void TIMInitManager(TIM_TypeDef* timer, uint16_t prescaler, uint16_t period, uint8_t repetition_counter, uint16_t mode, uint16_t div, uint8_t enable_it, uint8_t preemption, uint8_t sub)
{
	NVIC_InitTypeDef priority_config;
	TIM_TimeBaseInitTypeDef tim_config;
	
	// TIM1
	if ((uint32_t)timer == TIM1_BASE)
	{		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		tim_config.TIM_RepetitionCounter = repetition_counter;  // 100ms
		
		priority_config.NVIC_IRQChannel = TIM1_UP_IRQn;
		priority_config.NVIC_IRQChannelPreemptionPriority = preemption;  // 0-3
		priority_config.NVIC_IRQChannelSubPriority = sub;  // 0-3
		priority_config.NVIC_IRQChannelCmd = ENABLE;
	}
	
	else if ((uint32_t)timer == TIM2_BASE)
	{		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

		priority_config.NVIC_IRQChannel = TIM2_IRQn;
		priority_config.NVIC_IRQChannelPreemptionPriority = preemption;  // 0-3
		priority_config.NVIC_IRQChannelSubPriority = sub;  // 0-3
		priority_config.NVIC_IRQChannelCmd = ENABLE;
	}
	
	else if ((uint32_t)timer == TIM3_BASE)
	{		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		
		priority_config.NVIC_IRQChannel = TIM3_IRQn;
		priority_config.NVIC_IRQChannelPreemptionPriority = preemption;  // 0-3
		priority_config.NVIC_IRQChannelSubPriority = sub;  // 0-3
		priority_config.NVIC_IRQChannelCmd = ENABLE;
	}
	
	else if ((uint32_t)timer == TIM4_BASE)
	{		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		priority_config.NVIC_IRQChannel = TIM4_IRQn;
		priority_config.NVIC_IRQChannelPreemptionPriority = preemption;  // 0-3
		priority_config.NVIC_IRQChannelSubPriority = sub;  // 0-3
		priority_config.NVIC_IRQChannelCmd = ENABLE;
	}
	NVIC_Init(&priority_config);
	
	// tim time base init
	tim_config.TIM_Prescaler = prescaler;  // 1us
	tim_config.TIM_Period = period;  // 1ms
	tim_config.TIM_CounterMode = mode;
	tim_config.TIM_ClockDivision = div;
	TIM_TimeBaseInit(timer, &tim_config);
	
	if (enable_it)
	{
		// Interrupt config
		TIM_ITConfig(timer, TIM_IT_Update, ENABLE);  // Enable IT flag: TIM_IT_Update = TF0 or TF1
	}
	
	TIM_Cmd(timer, ENABLE);
}
