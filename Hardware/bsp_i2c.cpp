#include "bsp_i2c.h"

/**
 * @brief I2C初始化函数
 * @note  此函数将HAL库的初始化流程整合到了一个函数中，使用标准库函数实现。
 * - 使能GPIO和I2C时钟
 * - 配置SCL和SDA引脚为复用开漏模式
 * - 配置I2C外设参数（速率、模式等）
 * - 使能I2C
 */
void I2C_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    // 1. 使能I2C和GPIO的时钟 (对应 HAL_I2C_MspInit)
    RCC_APB1PeriphClockCmd(OLED_I2C_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(OLED_I2C_GPIO_CLK, ENABLE);

    // 2. 配置GPIO引脚 (SCL 和 SDA) (对应 HAL_I2C_MspInit)
    GPIO_InitStructure.GPIO_Pin = OLED_I2C_SCL_PIN | OLED_I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 对应 HAL_SPEED_FREQ_HIGH
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;   // 复用开漏输出
    GPIO_Init(OLED_I2C_GPIO_PORT, &GPIO_InitStructure);

    // 3. 配置I2C外设 (对应 MX_I2C1_Init)
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00; // 作为主机，地址任意，设为0x00与HAL库保持一致
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000; // 400KHz 快速模式
    
    I2C_Init(OLED_I2C, &I2C_InitStructure);

    // 4. 使能I2C (对应 HAL_I2C_Init 函数的最后部分)
    I2C_Cmd(OLED_I2C, ENABLE);
}
