# STM32F103C8 8通道PWM输出控制项目

## 项目概述

这是一个基于STM32F103C8微控制器的多通道PWM输出控制系统，支持8个独立的PWM输出通道，适用于舵机控制、电机驱动、多轴无人机或机器人等应用场景。

**核心特性：**
- ✅ 8个独立PWM输出通道
- ✅ 50Hz标准频率（20ms周期）
- ✅ 动态占空比调整，支持实时脉宽修改
- ✅ 采用STM32标准库，易于扩展
- ✅ Keil MDK开发环境

---

## 🔌 引脚配置详解

### 引脚映射表

| 通道 | GPIO端口 | 引脚号 | 定时器 | 功能说明 |
|------|---------|--------|--------|---------|
| CH1 | PA | PIN6 | TIM3 | PWM输出通道1 |
| CH2 | PA | PIN7 | TIM3 | PWM输出通道2 |
| CH3 | PB | PIN0 | TIM3 | PWM输出通道3 |
| CH4 | PB | PIN1 | TIM3 | PWM输出通道4 |
| CH5 | PB | PIN6 | TIM4 | PWM输出通道5 |
| CH6 | PB | PIN7 | TIM4 | PWM输出通道6 |
| CH7 | PB | PIN8 | TIM4 | PWM输出通道7 |
| CH8 | PB | PIN9 | TIM4 | PWM输出通道8 |

### 定时器分配

**TIM3（定时器3）控制通道1-4：**
- PA6 → TIM3_CH1
- PA7 → TIM3_CH2
- PB0 → TIM3_CH3
- PB1 → TIM3_CH4

**TIM4（定时器4）控制通道5-8：**
- PB6 → TIM4_CH1
- PB7 → TIM4_CH2
- PB8 → TIM4_CH3
- PB9 → TIM4_CH4

### GPIO配置参数

所有PWM输出引脚配置如下：
- **输出模式：** 复用推挽输出（AF_PP - Alternate Function Push-Pull）
- **速度等级：** 50MHz（高速模式）
- **初始状态：** 低电平

### PWM时序参数

```
频率：50Hz
周期：20ms
计数值：20000（在72MHz系统时钟下）
分频系数：72

计算公式：
  定时器频率 = 系统时钟 / 分频系数 = 72MHz / 72 = 1MHz
  周期 = 计数值 / 定时器频率 = 20000 / 1MHz = 20ms
  频率 = 1 / 周期 = 50Hz
```

### 占空比范围

根据应用不同，占空比范围如下：

**舵机控制（SERVO）：**
```
最小位置 (STEER_MIN): 2.5%  (0.025 × 20ms = 0.5ms)
中间位置 (STEER_MID): 7.5%  (0.075 × 20ms = 1.5ms)
最大位置 (STEER_MAX): 12.5% (0.125 × 20ms = 2.5ms)
```

**电机控制（ENGINE）：**
```
停止 (ENGINE_MIN): 根据电调配置设定
全速 (ENGINE_MAX): 根据电调配置设定
```

---

## 📁 项目结构

```
project/main/
│
├── core/                           # ARM Cortex-M3核心库
│   ├── core_cm3.h                  # Cortex-M3核心头文件
│   └── core_cm3.c                  # Cortex-M3核心实现
│
├── device/                         # STM32F103设备配置
│   ├── stm32f10x.h                 # 设备主头文件（寄存器定义）
│   ├── stm32f10x_conf.h            # 驱动库配置文件
│   ├── stm32f10x_it.h/.c           # 中断处理程序
│   └── system_stm32f10x.h/.c       # 系统时钟初始化
│
├── driver/                         # STM32标准驱动库（CMSIS）
│   ├── inc/                        # 驱动头文件目录
│   │   ├── stm32f10x_gpio.h        # GPIO驱动接口
│   │   ├── stm32f10x_tim.h         # 定时器驱动接口
│   │   ├── stm32f10x_rcc.h         # 时钟控制驱动
│   │   └── [其他21个驱动模块]
│   └── src/                        # 驱动实现目录
│       ├── stm32f10x_gpio.c
│       ├── stm32f10x_tim.c
│       └── [对应的25个实现文件]
│
├── startup/                        # 启动文件
│
├── user/                           # 用户应用代码（主要开发区）
│   ├── main.c                      # 程序主入口
│   ├── gpio_init.c                 # GPIO初始化模块
│   ├── tim_init.c                  # 定时器初始化模块
│   ├── pwm_config.c                # PWM配置管理模块 ⭐ 关键文件
│   ├── delay.c                     # 延时函数模块
│   ├── inc/                        # 用户代码头文件
│   │   ├── gpio_init.h
│   │   ├── tim_init.h
│   │   ├── pwm_config.h            # ⭐ PWM配置头文件
│   │   └── delay.h
│   ├── project.uvprojx             # Keil MDK项目文件
│   └── project.uvoptx              # Keil MDK项目选项
│
├── 八通道输出引脚.txt             # 引脚配置文档（原始配置记录）
└── README.md                       # 本文件
```

---

## 🎯 关键文件说明

### 1. **pwm_config.c / pwm_config.h** ⭐ 核心引脚控制文件

这是项目中最重要的**引脚配置和PWM控制模块**，主要职责：

**功能：**
- 8个PWM通道的初始化配置
- 动态修改各通道占空比
- 通过位操作进行高效的多通道管理
- 支持通道独立配置

**关键函数/宏定义（预期）：**
```c
// 通道定义
#define PWM_CHANNEL1  0  // PA6 - TIM3_CH1
#define PWM_CHANNEL2  1  // PA7 - TIM3_CH2
#define PWM_CHANNEL3  2  // PB0 - TIM3_CH3
#define PWM_CHANNEL4  3  // PB1 - TIM3_CH4
#define PWM_CHANNEL5  4  // PB6 - TIM4_CH1
#define PWM_CHANNEL6  5  // PB7 - TIM4_CH2
#define PWM_CHANNEL7  6  // PB8 - TIM4_CH3
#define PWM_CHANNEL8  7  // PB9 - TIM4_CH4

// 核心函数
void pwm_init(void);                           // 初始化所有PWM通道
void pwm_set_duty(uint8_t channel, float duty); // 设置指定通道占空比 (0.0~1.0)
void pwm_set_all_duty(float duty);             // 同时设置所有通道占空比
```

**在main.c中的使用示例：**
```c
// 初始化
pwm_init();

// 设置单个通道：舵机控制（向右转45度）
pwm_set_duty(PWM_CHANNEL1, 0.100);  // 设置通道1为10%占空比

// 设置所有通道：批量操作
pwm_set_all_duty(0.075);  // 所有舵机回到中间位置

// 运行时动态调整
for(float duty = 0.025; duty <= 0.125; duty += 0.002) {
    pwm_set_all_duty(duty);  // 扫描所有通道从最小到最大
    delay_ms(50);
}
```

### 2. **gpio_init.c / gpio_init.h** - GPIO初始化模块

负责PA和PB端口的GPIO引脚初始化：
- 启用GPIOA和GPIOB时钟
- 配置8个引脚为复用推挽输出模式（AF_PP）
- 设置50MHz高速工作模式

### 3. **tim_init.c / tim_init.h** - 定时器初始化模块

负责TIM3和TIM4定时器的初始化：
- 配置72分频（1MHz计数频率）
- 设置20000计数值（20ms周期）
- 启用PWM输出模式

---

## 🚀 快速开始

### 1. 编译项目

```bash
# 使用Keil MDK打开项目
# 打开: user/project.uvprojx

# 编译快捷键: Ctrl+F7
# 或点击菜单: Project → Rebuild all target files
```

### 2. 连接硬件

按照以下方式连接外设（以舵机为例）：

```
STM32F103C8开发板
│
├─ PA6  → 舵机1的PWM输入 (黄线)
├─ PA7  → 舵机2的PWM输入
├─ PB0  → 舵机3的PWM输入
├─ PB1  → 舵机4的PWM输入
├─ PB6  → 舵机5的PWM输入
├─ PB7  → 舵机6的PWM输入
├─ PB8  → 舵机7的PWM输入
├─ PB9  → 舵机8的PWM输入
│
├─ GND  → 舵机GND (黑线)  [所有舵机共享]
└─ 5V   → 舵机VCC (红线)  [通过稳压模块供电]
```

---

## 📊 技术规格

| 参数 | 值 |
|------|-----|
| **微控制器** | STM32F103C8 (ARM Cortex-M3) |
| **系统时钟** | 72MHz |
| **PWM频率** | 50Hz |
| **PWM周期** | 20ms |
| **输出通道数** | 8个 |
| **占空比分辨率** | 20000步（1MHz/20000） |
| **输出电压** | 3.3V TTL (或通过电平转换到5V) |
| **GPIO口** | PA6, PA7, PB0, PB1, PB6, PB7, PB8, PB9 |
| **定时器** | TIM3 (通道1-4), TIM4 (通道5-8) |

---

## ⚠️ 注意事项

1. **电源管理**：
   - 多个舵机/电机共享5V电源时，需要使用独立的稳压电源
   - 避免通过单片机直接驱动高功率负载

2. **引脚冲突**：
   - PA6, PA7, PB0, PB1, PB6, PB7, PB8, PB9 仅用于PWM输出

3. **调试技巧**：
   - 使用示波器观察各PWM引脚的波形

---

## 📝 文件说明

**原始引脚配置文档：** `八通道输出引脚.txt`

该文件包含项目设计初期的引脚配置记录，是项目的设计基础。若需要修改硬件引脚分配，应同时更新以下文件：
- pwm_config.h (PWM操作功能模块)
- gpio_init.c (GPIO初始化模块)
- tim_init.c (定时器配置模块)

---

## 📧 项目信息

- **目标芯片**: STM32F103C8T6
- **开发环境**: Keil µVision 5
- **编程语言**: C
- **应用场景**: 舵机控制、多轴无人机、机器人控制等

---

**最后更新**: 2026年1月
**项目状态**: 仅包含基础模块
