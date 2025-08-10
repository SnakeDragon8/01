#include "pwm_tim1.h"

/**
  * @brief  初始化TIM1的通道4(PA11)作为PWM输出
  * @param  arr: 自动重载值 (决定周期)
  * @param  psc: 预分频值 (决定计数频率)
  * @retval None
  */
 
void TIM1_CH4_PWM_Init(u16 arr, u16 psc)
{
    // 定义GPIO和定时器的初始化结构体
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 1. 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  // 使能TIM1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // 使能复用功能时钟

    // 2. 初始化GPIO: PA11 -> 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 初始化定时器基础设置
    TIM_TimeBaseStructure.TIM_Period = arr; // 设置自动重载寄存器的值 (ARR)
    TIM_TimeBaseStructure.TIM_Prescaler = psc; // 设置预分频器的值 (PSC)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; // 时钟分割，TDS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // TIM1/TIM8特有：重复计数器
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // 4. 初始化PWM通道4设置
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 选择PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出
    TIM_OCInitStructure.TIM_Pulse = 0; // 设置待装入捕获比较寄存器的脉冲值，初始占空比为0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 设置输出极性为高
    // 注意：高级定时器还有互补通道的配置，这里我们只用主通道，所以下面的配置维持默认即可
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure); // 注意是OC4Init，对应通道4

    // 5. 使能各部分模块
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能CH4的预装载
    TIM_ARRPreloadConfig(TIM1, ENABLE); // 使能ARR的预装载

    // 【关键步骤】使能TIM1的主输出，否则PWM不会有波形
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    // 最后，使能TIM1计数器
    TIM_Cmd(TIM1, ENABLE);
}