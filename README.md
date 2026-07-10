<div align="center">

# ⚡ STM32F103 PWM Controller

**基于 STM32F103C8 的八通道独立 PWM 输出控制器，适用于舵机 / 电调 / 机器人多轴控制**

*An 8-channel PWM controller on STM32F103C8 — servo & ESC ready, 50 Hz, per-channel duty control.*

![MCU](https://img.shields.io/badge/MCU-STM32F103C8-03234B?style=flat-square&logo=stmicroelectronics&logoColor=white)
![Core](https://img.shields.io/badge/Core-ARM%20Cortex--M3%20%40%2072MHz-0091BD?style=flat-square&logo=arm&logoColor=white)
![Language](https://img.shields.io/badge/Language-C-A8B9CC?style=flat-square&logo=c&logoColor=white)
![Channels](https://img.shields.io/badge/PWM-8%20Channels%20%40%2050Hz-success?style=flat-square)
![IDE](https://img.shields.io/badge/IDE-Keil%20MDK--ARM-orange?style=flat-square)

</div>

---

## ✨ 简介

这是一个基于 STM32F103C8 微控制器的多通道 PWM 输出控制系统，利用 TIM3 + TIM4 各 4 个比较通道，提供 **8 路独立的 50 Hz PWM 输出**，支持运行时逐通道动态调整占空比。适用于舵机控制、无刷电机（电调）驱动、多轴无人机、机器人等场景。

**核心特性：**

- ✅ 8 个独立 PWM 输出通道（TIM3 CH1-4 + TIM4 CH1-4）
- ✅ 50 Hz 标准频率（20 ms 周期），占空比分辨率 20000 步
- ✅ 位掩码按需使能任意通道组合，占空比可实时修改
- ✅ 内置舵机 / 无刷电调占空比上下限宏定义
- ✅ 基于 STM32 标准外设库，Keil MDK 开发环境，易于扩展

## 🔌 引脚配置

### 引脚映射表

| 通道 | GPIO 引脚 | 定时器通道 | 功能说明 |
|------|-----------|-----------|---------|
| CH1 | PA6 | TIM3_CH1 | PWM 输出通道 1 |
| CH2 | PA7 | TIM3_CH2 | PWM 输出通道 2 |
| CH3 | PB0 | TIM3_CH3 | PWM 输出通道 3 |
| CH4 | PB1 | TIM3_CH4 | PWM 输出通道 4 |
| CH5 | PB6 | TIM4_CH1 | PWM 输出通道 5 |
| CH6 | PB7 | TIM4_CH2 | PWM 输出通道 6 |
| CH7 | PB8 | TIM4_CH3 | PWM 输出通道 7 |
| CH8 | PB9 | TIM4_CH4 | PWM 输出通道 8 |

> 原始引脚设计记录见仓库根目录的 `八通道输出引脚.txt`。

### GPIO 与时序参数

- **输出模式：** 复用推挽输出（AF_PP），50 MHz 速度等级
- **PWM 时序：**

```
频率：50Hz    周期：20ms
分频系数：72   计数值：20000

定时器频率 = 72MHz / 72 = 1MHz
周期 = 20000 / 1MHz = 20ms  →  频率 = 50Hz
```

### 占空比范围（pwm_config.h 内置宏）

| 应用 | 宏定义 | 值 | 对应脉宽 |
|------|--------|-----|---------|
| 舵机最小位置 | `STEER_MIN` | 2.5% | 0.5 ms |
| 舵机最大位置 | `STEER_MAX` | 12.5% | 2.5 ms |
| 无刷电调最小 | `ENGINE_MIN` | 5% | 1.0 ms |
| 无刷电调最大 | `ENGINE_MAX` | 9% | 1.8 ms |

## ⚡ 快速开始

### 1. 编译烧录

```
用 Keil MDK 打开 user/project.uvprojx
Project → Rebuild all target files（或 Ctrl+F7）
通过 ST-Link / 串口 ISP 烧录
```

### 2. 核心 API（pwm_config.h）

```c
// 1. 先初始化 TIM3 / TIM4 时基：72 分频、20000 计数 → 20ms 周期
TIMInitManager(TIM3, 72-1, 20000-1, 0, TIM_CounterMode_Up, TIM_CKD_DIV1, 0, 1, 0);
TIMInitManager(TIM4, 72-1, 20000-1, 0, TIM_CounterMode_Up, TIM_CKD_DIV1, 0, 1, 0);

// 2. 设置 8 个通道的初始占空比（传 -1 等负值表示该通道略过）
PWM_SetInitDuty(0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075, 0.075);

// 3. 按位掩码使能通道：0xFF 全开；0x15 (0001 0101) 只开通道 1/3/5
PWM_Init(0xFF);

// 4. 运行时动态修改某一通道的占空比
PWM_SetNewDuty(0x01, STEER_MAX);  // 通道选择也是位掩码：0x01=CH1, 0x02=CH2, 0x04=CH3 ...
```

`user/main.c` 提供了一个完整 DEMO：八路舵机同步从最小角度扫到最大角度循环转动。

### 3. 硬件接线（以舵机为例）

```
STM32F103C8 开发板
├─ PA6/PA7/PB0/PB1/PB6/PB7/PB8/PB9 → 舵机 1~8 的 PWM 信号线（黄）
├─ GND → 舵机 GND（黑）  [共地]
└─ 5V  → 舵机 VCC（红）  [经独立稳压模块供电]
```

## 📁 项目结构

```
STM32F103-PWM-Controller/
├── user/                    # 用户应用代码（主要开发区）
│   ├── main.c               # 程序入口：八通道舵机扫动 DEMO
│   ├── pwm_config.c/.h      # ⭐ 核心：PWM 通道初始化 + 占空比控制
│   ├── gpio_init.c          # GPIO 复用推挽输出初始化
│   ├── tim_init.c           # TIM3/TIM4 时基与 PWM 模式配置
│   ├── delay.c              # 延时模块
│   ├── inc/                 # 用户头文件
│   └── project.uvprojx      # Keil MDK 工程文件
├── driver/                  # STM32F10x 标准外设库（inc/ + src/）
├── core/ & device/          # CMSIS 内核与设备启动配置
├── startup/                 # 启动汇编文件
└── 八通道输出引脚.txt        # 原始引脚配置设计记录
```

## 📊 技术规格

| 参数 | 值 |
|------|-----|
| 微控制器 | STM32F103C8（ARM Cortex-M3） |
| 系统时钟 | 72 MHz |
| PWM 频率 / 周期 | 50 Hz / 20 ms |
| 输出通道数 | 8 |
| 占空比分辨率 | 20000 步（1 µs 步进） |
| 输出电平 | 3.3V TTL（可经电平转换到 5V） |
| 占用引脚 | PA6, PA7, PB0, PB1, PB6, PB7, PB8, PB9 |
| 定时器 | TIM3（通道 1-4）、TIM4（通道 5-8） |

## ⚠️ 注意事项

1. **电源管理**：多个舵机 / 电机共享 5V 电源时，请使用独立稳压电源，避免用单片机直接驱动高功率负载。
2. **引脚冲突**：上表 8 个引脚已专用于 PWM 输出，请勿复用。
3. **修改引脚分配**时需同步更新 `pwm_config.h`、`gpio_init.c`、`tim_init.c` 三处。
4. **调试技巧**：用示波器观察各 PWM 引脚波形验证脉宽。

## 🛠️ 技术栈

- **C 语言** + STM32F10x 标准外设库（SPL）
- **Keil µVision 5**（MDK-ARM）
- **应用场景**：舵机控制、多轴无人机、机器人控制等

## ⭐ Star History

[![Star History Chart](https://api.star-history.com/svg?repos=LeoLee0812/STM32F103-PWM-Controller&type=Date)](https://www.star-history.com/#LeoLee0812/STM32F103-PWM-Controller&Date)
