#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "stm32f10x.h"

// 定义OLED所使用的I2C端口、引脚及相关时钟
#define OLED_I2C                I2C1
#define OLED_I2C_CLK            RCC_APB1Periph_I2C1
#define OLED_I2C_GPIO_CLK       RCC_APB2Periph_GPIOB
#define OLED_I2C_SCL_PIN        GPIO_Pin_6
#define OLED_I2C_SDA_PIN        GPIO_Pin_7
#define OLED_I2C_GPIO_PORT      GPIOB

/**
 * @brief I2C初始化函数声明
 */
void I2C_Configuration(void);

#endif
