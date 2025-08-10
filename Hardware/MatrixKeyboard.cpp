#include "MatrixKeyboard.h"
#include "Delay.h"  

// 全局变量，用于在中断和主循环之间传递键值
volatile uint8_t g_KeyCode = 0xFF; // volatile 关键字防止编译器过度优化

// 一个只进行一次物理扫描的函数，不包含状态机和消抖逻辑
// 这个函数将在TIM4中断中使用
uint8_t Perform_Single_Scan(void)
{
    uint8_t keyValue = 0xFF; // 默认无按键

    // 将所有行先置高
    GPIO_SetBits(MATRIX_ROW1_PORT, MATRIX_ROW1_PIN);
    GPIO_SetBits(MATRIX_ROW2_PORT, MATRIX_ROW2_PIN);
    GPIO_SetBits(MATRIX_ROW3_PORT, MATRIX_ROW3_PIN);
    GPIO_SetBits(MATRIX_ROW4_PORT, MATRIX_ROW4_PIN);

    // --- 扫描第1行 ---
    GPIO_ResetBits(MATRIX_ROW1_PORT, MATRIX_ROW1_PIN);
    if (GPIO_ReadInputDataBit(MATRIX_COL1_PORT, MATRIX_COL1_PIN) == 0) keyValue = KEY_1;
    else if (GPIO_ReadInputDataBit(MATRIX_COL2_PORT, MATRIX_COL2_PIN) == 0) keyValue = KEY_2;
    else if (GPIO_ReadInputDataBit(MATRIX_COL3_PORT, MATRIX_COL3_PIN) == 0) keyValue = KEY_3;
    else if (GPIO_ReadInputDataBit(MATRIX_COL4_PORT, MATRIX_COL4_PIN) == 0) keyValue = KEY_A;
    GPIO_SetBits(MATRIX_ROW1_PORT, MATRIX_ROW1_PIN); // 恢复第1行为高

    // --- 扫描第2行 ---
    if (keyValue == 0xFF) {
        GPIO_ResetBits(MATRIX_ROW2_PORT, MATRIX_ROW2_PIN);
        if (GPIO_ReadInputDataBit(MATRIX_COL1_PORT, MATRIX_COL1_PIN) == 0) keyValue = KEY_4;
        else if (GPIO_ReadInputDataBit(MATRIX_COL2_PORT, MATRIX_COL2_PIN) == 0) keyValue = KEY_5;
        else if (GPIO_ReadInputDataBit(MATRIX_COL3_PORT, MATRIX_COL3_PIN) == 0) keyValue = KEY_6;
        else if (GPIO_ReadInputDataBit(MATRIX_COL4_PORT, MATRIX_COL4_PIN) == 0) keyValue = KEY_B;
        GPIO_SetBits(MATRIX_ROW2_PORT, MATRIX_ROW2_PIN);
    }

    // --- 扫描第3行 ---
    if (keyValue == 0xFF) {
        GPIO_ResetBits(MATRIX_ROW3_PORT, MATRIX_ROW3_PIN);
        if (GPIO_ReadInputDataBit(MATRIX_COL1_PORT, MATRIX_COL1_PIN) == 0) keyValue = KEY_7;
        else if (GPIO_ReadInputDataBit(MATRIX_COL2_PORT, MATRIX_COL2_PIN) == 0) keyValue = KEY_8;
        else if (GPIO_ReadInputDataBit(MATRIX_COL3_PORT, MATRIX_COL3_PIN) == 0) keyValue = KEY_9;
        else if (GPIO_ReadInputDataBit(MATRIX_COL4_PORT, MATRIX_COL4_PIN) == 0) keyValue = KEY_C;
        GPIO_SetBits(MATRIX_ROW3_PORT, MATRIX_ROW3_PIN);
    }

    // --- 扫描第4行 ---
    if (keyValue == 0xFF) {
        GPIO_ResetBits(MATRIX_ROW4_PORT, MATRIX_ROW4_PIN);
        if (GPIO_ReadInputDataBit(MATRIX_COL1_PORT, MATRIX_COL1_PIN) == 0) keyValue = KEY_STAR;
        else if (GPIO_ReadInputDataBit(MATRIX_COL2_PORT, MATRIX_COL2_PIN) == 0) keyValue = KEY_0;
        else if (GPIO_ReadInputDataBit(MATRIX_COL3_PORT, MATRIX_COL3_PIN) == 0) keyValue = KEY_HASH;
        else if (GPIO_ReadInputDataBit(MATRIX_COL4_PORT, MATRIX_COL4_PIN) == 0) keyValue = KEY_D;
        GPIO_SetBits(MATRIX_ROW4_PORT, MATRIX_ROW4_PIN);
    }
    
    return keyValue;
}

// 中断模式
void MatrixKeyboard_Init_Interrupt(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. GPIO时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 2. 配置列线为推挽输出，并全部输出低电平
    GPIO_InitStructure.GPIO_Pin = MATRIX_COL1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MATRIX_COL1_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(MATRIX_COL1_PORT, MATRIX_COL1_PIN);

    GPIO_InitStructure.GPIO_Pin = MATRIX_COL2_PIN;
    GPIO_Init(MATRIX_COL2_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(MATRIX_COL2_PORT, MATRIX_COL2_PIN);

    GPIO_InitStructure.GPIO_Pin = MATRIX_COL3_PIN;
    GPIO_Init(MATRIX_COL3_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(MATRIX_COL3_PORT, MATRIX_COL3_PIN);

    GPIO_InitStructure.GPIO_Pin = MATRIX_COL4_PIN;
    GPIO_Init(MATRIX_COL4_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(MATRIX_COL4_PORT, MATRIX_COL4_PIN);

    // 3. 配置行线为上拉输入
    GPIO_InitStructure.GPIO_Pin = MATRIX_ROW1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(MATRIX_ROW1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MATRIX_ROW2_PIN;
    GPIO_Init(MATRIX_ROW2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MATRIX_ROW3_PIN;
    GPIO_Init(MATRIX_ROW3_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MATRIX_ROW4_PIN;
    GPIO_Init(MATRIX_ROW4_PORT, &GPIO_InitStructure);
   
    // 4. 配置行线的外部中断（EXTI）
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_Init(&EXTI_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
    EXTI_Init(&EXTI_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_Init(&EXTI_InitStructure);

    // 5. 配置NVIC（中断控制器）从PA7到PA4
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

// 扫描模式
void MatrixKeyboard_Init_Scan(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能所需GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置行引脚为推挽输出，默认输出高电平
    // 配置行1
    GPIO_InitStructure.GPIO_Pin = MATRIX_ROW1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MATRIX_ROW1_PORT, &GPIO_InitStructure);
    GPIO_SetBits(MATRIX_ROW1_PORT, MATRIX_ROW1_PIN); // 确保默认输出高电平
    
    // 配置行2
    GPIO_InitStructure.GPIO_Pin = MATRIX_ROW2_PIN;
    GPIO_Init(MATRIX_ROW2_PORT, &GPIO_InitStructure);
    GPIO_SetBits(MATRIX_ROW2_PORT, MATRIX_ROW2_PIN); // 确保默认输出高电平
    
    // 配置行3
    GPIO_InitStructure.GPIO_Pin = MATRIX_ROW3_PIN;
    GPIO_Init(MATRIX_ROW3_PORT, &GPIO_InitStructure);
    GPIO_SetBits(MATRIX_ROW3_PORT, MATRIX_ROW3_PIN); // 确保默认输出高电平
    
    // 配置行4
    GPIO_InitStructure.GPIO_Pin = MATRIX_ROW4_PIN;
    GPIO_Init(MATRIX_ROW4_PORT, &GPIO_InitStructure);
    GPIO_SetBits(MATRIX_ROW4_PORT, MATRIX_ROW4_PIN); // 确保默认输出高电平
    
    // 配置列引脚为上拉输入
    // 配置列1
    GPIO_InitStructure.GPIO_Pin = MATRIX_COL1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(MATRIX_COL1_PORT, &GPIO_InitStructure);
    
    // 配置列2
    GPIO_InitStructure.GPIO_Pin = MATRIX_COL2_PIN;
    GPIO_Init(MATRIX_COL2_PORT, &GPIO_InitStructure);
    
    // 配置列3
    GPIO_InitStructure.GPIO_Pin = MATRIX_COL3_PIN;
    GPIO_Init(MATRIX_COL3_PORT, &GPIO_InitStructure);
    
    // 配置列4
    GPIO_InitStructure.GPIO_Pin = MATRIX_COL4_PIN;
    GPIO_Init(MATRIX_COL4_PORT, &GPIO_InitStructure);
    
    // 行默认输出高电平
    GPIO_SetBits(MATRIX_ROW1_PORT, MATRIX_ROW1_PIN);
    GPIO_SetBits(MATRIX_ROW2_PORT, MATRIX_ROW2_PIN);
    GPIO_SetBits(MATRIX_ROW3_PORT, MATRIX_ROW3_PIN);
    GPIO_SetBits(MATRIX_ROW4_PORT, MATRIX_ROW4_PIN);
 
}

// 辅助函数，用于关闭所有行中断
void Disable_All_Row_Interrupts(void)
{
    EXTI->IMR &= ~(EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7);
}

// 辅助函数，用于开启所有行中断
void Enable_All_Row_Interrupts(void)
{
    EXTI->IMR |= (EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7);
}

// 切换到扫描模式
void Switch_To_Scan_Mode(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // 1. 行线配置为推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = MATRIX_ROW1_PIN | MATRIX_ROW2_PIN | MATRIX_ROW3_PIN | MATRIX_ROW4_PIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 2. 列线配置为上拉输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = MATRIX_COL1_PIN | MATRIX_COL2_PIN | MATRIX_COL3_PIN | MATRIX_COL4_PIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// 切换到中断模式
void Switch_To_Interrupt_Mode(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // 1. 行线配置为上拉输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = MATRIX_ROW1_PIN | MATRIX_ROW2_PIN | MATRIX_ROW3_PIN | MATRIX_ROW4_PIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 2. 列线配置为推挽输出，并输出低电平
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = MATRIX_COL1_PIN | MATRIX_COL2_PIN | MATRIX_COL3_PIN | MATRIX_COL4_PIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOA, MATRIX_COL1_PIN | MATRIX_COL2_PIN | MATRIX_COL3_PIN | MATRIX_COL4_PIN);
}


