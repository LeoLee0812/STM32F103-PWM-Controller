#include <stm32f10x.h>
#include <stdlib.h>


#ifndef __PWM
	#define __PWM
	
	// 舵机的占空比最大最小值
	#define STEER_MIN 0.025
	#define STEER_MAX 0.125
	
	// 无刷电机的占空比最大最小值
	#define ENGINE_MIN 0.05
	#define ENGINE_MAX 0.09

	// 宏函数, 用于计算指定占空比对应的定时器反转值
	#define DEFAULT_INITIAL_PWM_PULSE(PERIOD, DUTY) ((uint16_t)(PERIOD * DUTY))
	
	typedef struct
	{
		// 总共8个PWM通道
		float init_duty[8];
	}PWM_CONFIG;
#endif

extern PWM_CONFIG pwm_config_structure_for_user;
// 总初始化函数
extern void PWM_Init(const uint8_t channels);
// 填入-1或任意小于0的数表示不对当前通道设置初始值
extern void PWM_SetInitDuty(float channel1, float channel2, float channel3, float channel4, float channel5, float channel6, float channel7, float channel8);
// 非必要的话建议使用直接操作寄存器以获得更高的效率
extern void PWM_SetNewDuty(uint8_t channel, float new_duty);
