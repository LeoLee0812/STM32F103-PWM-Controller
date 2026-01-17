// 导入头文件
#include <stm32f10x.h>

#ifndef __DELAY
	#define __DELAY
#endif

// 函数定义
extern void Delay_s(uint32_t num);
extern void Delay_ms(uint32_t num);
extern void Delay_us(uint32_t num);
