#include "stm32f10x.h"

// 配置TIM4用于20ms的单次消抖延时
void TIM4_Debounce_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 使能TIM4时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    // 2. 配置TIM4
    // 定时器时钟为72MHz, 72,000,000 / (7199 + 1) = 10,000 Hz
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;
    // 计数10000次/秒，计200次就是20ms
    TIM_TimeBaseStructure.TIM_Period = 199; // 0-199
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // 3. 配置为单次脉冲模式，触发一次后自动停止
    TIM_SelectOnePulseMode(TIM4, TIM_OPMode_Single);

    // 4. 使能更新中断
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    
    // 5. 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 优先级可以设高一些
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 6. 清除一次标志位，防止刚初始化就进入中断
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}

// 启动定时器（只在EXTI中断中调用）
void TIM4_Start(void)
{
    TIM_SetCounter(TIM4, 0); // 每次启动前都复位计数器
    TIM_Cmd(TIM4, ENABLE);
}