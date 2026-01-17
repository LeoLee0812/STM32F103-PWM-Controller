#include "gpio_init.h"


//  **************************************************
//  GPIO初始化
//  **************************************************
void GPIOInitManager(GPIO_TypeDef* port, unsigned short int pin, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed)
{
	// 标准库初始化配置结构体
	GPIO_InitTypeDef gpio_config;
	
	
	// 开启对应时钟, 只能开启GPIOA, GPIOB, GPIOC
	if ((uint32_t)port == GPIOA_BASE)
	{
		// 开启GPIOA所在的APB2外设时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	}
	
	else if ((uint32_t)port == GPIOB_BASE)
	{
		// 开启GPIOB所在的APB2外设时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	}
	
	else if ((uint32_t)port == GPIOC_BASE)
	{
		// 开启GPIOC所在的APB2外设时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	}
	
	
	// 配置GPIO.PIN的工作模式
	gpio_config.GPIO_Pin   = pin;  // 理论上每一个GPIO有16个PIN: PIN0-PIN15, 实际取决于封装
	gpio_config.GPIO_Mode  = mode;
	gpio_config.GPIO_Speed = speed;
	
	GPIO_Init(port, &gpio_config);   // 输入配置
}
