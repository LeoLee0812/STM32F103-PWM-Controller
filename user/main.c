/* 分别导入 PWM调制模块 | 延时模块 | 定时器配置模块*/
#include "pwm_config.h"
#include "delay.h"
#include "tim_init.h"

//  **************************************************
//  DEMO
//  演示让八个通道驱动舵机错位同时转动
//  **************************************************


//  **************************************************
// 	入口函数
//  **************************************************
int main(void)
{
	// 准备
	uint8_t i;
	// 每一个舵机的旋转角度错位值
	float interval;
	// 每个循环旋转的步长
	float resolution = 1e-5;
	// 占空比数组
	float current_dudy[8] = {STEER_MIN + interval * 1, STEER_MIN + interval * 2, STEER_MIN + interval * 3, STEER_MIN + interval * 4, STEER_MIN + interval * 5, STEER_MIN + interval * 6, STEER_MIN + interval * 7, STEER_MIN + interval * 8};
	
	// 八个通道等距分成八份
	interval = (STEER_MAX - STEER_MIN) / 8;
		
	// 启动 TIM3 TIM4 定时器
	TIMInitManager(TIM3, 72-1, 20000-1, 0, TIM_CounterMode_Up, TIM_CKD_DIV1, 0, 1, 0);  // 20 ms
	TIMInitManager(TIM4, 72-1, 20000-1, 0, TIM_CounterMode_Up, TIM_CKD_DIV1, 0, 1, 0);  // 20 ms
		
	// 设置初始占空比
	// 每个参数依次对应每个通道，共8个
	PWM_SetInitDuty(STEER_MIN + interval * 1, STEER_MIN + interval * 2, STEER_MIN + interval * 3, STEER_MIN + interval * 4, STEER_MIN + interval * 5, STEER_MIN + interval * 6, STEER_MIN + interval * 7, STEER_MIN + interval * 8);
	// 输出初始化, 0xFF代表全部使能, 0x00代表全部关闭, 0x15: (最高位通道)0001 0101(最低位通道) 代表开启第0,2,4个通道
	PWM_Init(0xFF);
	
	// 主循环
	while(1)
	{
		// 遍历更新8个通道
		for (i = 0; i < 8; i++)
		{
			// 第一个参数是通道选择0x01(0000 0001)代表通道1, 0x02(0000 0010)代表通道2, 0x04(0000 0100)代表通道3......
			// 以此类推:
			// (0000 0001)
			// (0000 0010)
			// (0000 0100)
			// (0000 1000)
			// (0001 0000)
			// (0010 0000)
			// (0100 0000)
			// (1000 0000)
			if (current_dudy[i] >= STEER_MAX)
			{
				current_dudy[i] = STEER_MIN;
			}
			else
			{
				// 按步长更新
				current_dudy[i] += resolution;
			}
			PWM_SetNewDuty((0x01 << i), current_dudy[i]);
		}
		Delay_us(200);
	}
}
