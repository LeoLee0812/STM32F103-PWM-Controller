#include "pwm_config.h"
#include "gpio_init.h"

// 私有函数, 只供模块内的接口函数调用
void TIM4_PWMInit(TIM_OCInitTypeDef* pwm_config, uint8_t channels);  // 高4通道由TIM4控制
void TIM3_PWMInit(TIM_OCInitTypeDef* pwm_config, uint8_t channels);  // 低4通道由TIM3控制

// 全局变量, PWM配置结构体
PWM_CONFIG pwm_config_structure_for_user;


void PWM_Init(const uint8_t channels)
{
	uint8_t i;
	uint8_t H, L;
	TIM_OCInitTypeDef pwm_config[8];
	
	H = channels >> 4;
	L = channels & 0x0F;
	
	// 循环遍历低4位通道的配置结构体数组
	for (i = 0; i < 4; i++)
	{
		pwm_config[i].TIM_OCMode = TIM_OCMode_PWM1;              // 当小于设定的反转阈值时输出值为有效值,即高电平
		pwm_config[i].TIM_OutputState = TIM_OutputState_Enable;  // 使能输出
		pwm_config[i].TIM_Pulse = DEFAULT_INITIAL_PWM_PULSE(TIM3->ARR, pwm_config_structure_for_user.init_duty[i]);
		pwm_config[i].TIM_OCPolarity = TIM_OCPolarity_High;  // 设置为高电平有效
	}
	
	// 循环遍历高4位通道的配置结构体数组
	for (i = 4; i < 8; i++)
	{
		pwm_config[i].TIM_OCMode = TIM_OCMode_PWM1;              // 当小于设定的反转阈值时输出值为有效值,即高电平
		pwm_config[i].TIM_OutputState = TIM_OutputState_Enable;  // 使能输出
		pwm_config[i].TIM_Pulse = DEFAULT_INITIAL_PWM_PULSE(TIM4->ARR, pwm_config_structure_for_user.init_duty[i]);
		pwm_config[i].TIM_OCPolarity = TIM_OCPolarity_High;  // 设置为高电平有效
	}
	
	TIM4_PWMInit(&pwm_config[0], H);
	TIM3_PWMInit(&pwm_config[0], L);
}


void TIM4_PWMInit(TIM_OCInitTypeDef* pwm_config, uint8_t channels)  // HIGH
{
	if (channels & 0x01)  // PWM5  1: ENABLE  0: DISABLE
	{
		TIM_OC1Init(TIM4, (pwm_config + 4));
		
		GPIOInitManager(GPIOB, GPIO_Pin_6, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	}
	channels >>= 1;
	if (channels & 0x01)  // PWM6  1: ENABLE  0: DISABLE
	{
		TIM_OC2Init(TIM4, (pwm_config + 5));
		
		GPIOInitManager(GPIOB, GPIO_Pin_7, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	}
	channels >>= 1;
	if (channels & 0x01)  // PWM7  1: ENABLE  0: DISABLE
	{
		TIM_OC3Init(TIM4, (pwm_config + 6));
		
		GPIOInitManager(GPIOB, GPIO_Pin_8, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	}
	channels >>= 1;
	if (channels & 0x01)  // PWM8  1: ENABLE  0: DISABLE
	{
		TIM_OC4Init(TIM4, (pwm_config + 7));
		
		GPIOInitManager(GPIOB, GPIO_Pin_9, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	}
}


void TIM3_PWMInit(TIM_OCInitTypeDef* pwm_config, uint8_t channels)  // LOW
{
	if (channels & 0x01)  // PWM1  1: ENABLE  0: DISABLE
	{
		TIM_OC1Init(TIM3, (pwm_config + 0));
		
		GPIOInitManager(GPIOA, GPIO_Pin_6, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	}
	channels >>= 1;
	if (channels & 0x01)  // PWM2  1: ENABLE  0: DISABLE
	{
		TIM_OC2Init(TIM3, (pwm_config + 1));
		
		GPIOInitManager(GPIOA, GPIO_Pin_7, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	}
	channels >>= 1;
	if (channels & 0x01)  // PWM3  1: ENABLE  0: DISABLE
	{
		TIM_OC3Init(TIM3, (pwm_config + 2));
		
		GPIOInitManager(GPIOB, GPIO_Pin_0, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	}
	channels >>= 1;
	if (channels & 0x01)  // PWM4  1: ENABLE  0: DISABLE
	{
		TIM_OC4Init(TIM3, (pwm_config + 3));
		
		GPIOInitManager(GPIOB, GPIO_Pin_1, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
	}
}


void PWM_SetNewDuty(uint8_t channel, float new_duty)
{
	// 方案1
	// 八个通道的CCR地址
	static volatile uint16_t* crr[9];
	// 定时器基地址选取
	static TIM_TypeDef* timer;
	// 高四位偏移
	static uint8_t offset;
	
	offset = ((channel & 0xF0) ? 4 : 0);
	
	timer = (TIM_TypeDef*)(APB1PERIPH_BASE + ((channel & 0xF0) ? 0x0800 : 0x0400));
		
	crr[1] = &((timer)->CCR1);
	crr[2] = &((timer)->CCR2);
	crr[4] = &((timer)->CCR3);
	crr[8] = &((timer)->CCR4);
	
	*(crr[channel>>offset]) = (uint16_t)((timer)->ARR * new_duty);

	// 方案2
	/*
	switch(channel)
	{
		case 0x01:
			TIM3->CCR1 = (uint16_t)(TIM3->ARR * new_duty);
			break;
		case 0x02:
			TIM3->CCR2 = (uint16_t)(TIM3->ARR * new_duty);
			break;
		case 0x04:
			TIM3->CCR3 = (uint16_t)(TIM3->ARR * new_duty);
			break;
		case 0x08:
			TIM3->CCR4 = (uint16_t)(TIM3->ARR * new_duty);
			break;
		case 0x10:
			TIM4->CCR1 = (uint16_t)(TIM4->ARR * new_duty);
			break;
		case 0x20:
			TIM4->CCR2 = (uint16_t)(TIM4->ARR * new_duty);
			break;
		case 0x40:
			TIM4->CCR3 = (uint16_t)(TIM4->ARR * new_duty);
			break;
		case 0x80:
			TIM4->CCR4 = (uint16_t)(TIM4->ARR * new_duty);
			break;
	}*/
}


void PWM_SetInitDuty(float channel1, float channel2, float channel3, float channel4, float channel5, float channel6, float channel7, float channel8)
{
	if (channel1 >= 0)
	{
		pwm_config_structure_for_user.init_duty[0] = channel1;
	}
	if (channel2 >= 0)
	{
		pwm_config_structure_for_user.init_duty[1] = channel2;
	}
	if (channel3 >= 0)
	{
		pwm_config_structure_for_user.init_duty[2] = channel3;
	}
	if (channel4 >= 0)
	{
		pwm_config_structure_for_user.init_duty[3] = channel4;
	}
	if (channel5 >= 0)
	{
		pwm_config_structure_for_user.init_duty[4] = channel5;
	}
	if (channel6 >= 0)
	{
		pwm_config_structure_for_user.init_duty[5] = channel6;
	}
	if (channel7 >= 0)
	{
		pwm_config_structure_for_user.init_duty[6] = channel7;
	}
	if (channel8 >= 0)
	{
		pwm_config_structure_for_user.init_duty[7] = channel8;
	}
}
