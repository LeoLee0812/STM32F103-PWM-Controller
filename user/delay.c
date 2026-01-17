// 导入头文件
#include "delay.h"


// 计数范围: 0~233015 us
void Delay_us(uint32_t num)
{
	SysTick->LOAD = 72 * num;							// 设置重装值
	SysTick->VAL = 0x00;								// 清除当前计数
	SysTick->CTRL = 0x00000005;							// 设置时钟源为 HCLK 并 启动 SysTick 定时器
	while(!(SysTick->CTRL & 0x00010000));				// 等待计时到0
	SysTick->CTRL = 0x00000004;							// 关闭 SysTick 定时器
}


// range: 0~4294967295
void Delay_ms(uint32_t num)
{
	while(num--)
	{
		Delay_us(1000);
	}
}


// range: 0~4294967295
void Delay_s(uint32_t num)
{
	while(num--)
	{
		Delay_ms(1000);
	}
} 
