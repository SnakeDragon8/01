#ifndef __MATRIX_KEYBOARD_H
#define __MATRIX_KEYBOARD_H

#include "stm32f10x.h"

// 定义矩阵键盘引脚 - 修改为用户指定的新引脚配置
// 行引脚
#define MATRIX_ROW1_PORT     GPIOA
#define MATRIX_ROW1_PIN      GPIO_Pin_7  // 行1
#define MATRIX_ROW2_PORT     GPIOA
#define MATRIX_ROW2_PIN      GPIO_Pin_6  // 行2
#define MATRIX_ROW3_PORT     GPIOA
#define MATRIX_ROW3_PIN      GPIO_Pin_5  // 行3
#define MATRIX_ROW4_PORT     GPIOA
#define MATRIX_ROW4_PIN      GPIO_Pin_4  // 行4

// 列引脚
#define MATRIX_COL1_PORT     GPIOA
#define MATRIX_COL1_PIN      GPIO_Pin_3  // 列1
#define MATRIX_COL2_PORT     GPIOA
#define MATRIX_COL2_PIN      GPIO_Pin_2  // 列2
#define MATRIX_COL3_PORT     GPIOA
#define MATRIX_COL3_PIN      GPIO_Pin_1  // 列3
#define MATRIX_COL4_PORT     GPIOA
#define MATRIX_COL4_PIN      GPIO_Pin_0  // 列4

// 键盘按键值定义
#define KEY_1           0x01
#define KEY_2           0x02
#define KEY_3           0x03
#define KEY_A           0x0A
#define KEY_4           0x04
#define KEY_5           0x05
#define KEY_6           0x06
#define KEY_B           0x0B
#define KEY_7           0x07
#define KEY_8           0x08
#define KEY_9           0x09
#define KEY_C           0x0C
#define KEY_STAR        0x0E  // *
#define KEY_0           0x00
#define KEY_HASH        0x0F  // #
#define KEY_D           0x0D

// 函数声明
void MatrixKeyboard_Init_Interrupt(void);
void MatrixKeyboard_Init_Scan(void);
uint8_t Perform_Single_Scan(void);
void Disable_All_Row_Interrupts(void);
void Enable_All_Row_Interrupts(void);
void Switch_To_Scan_Mode(void);
void Switch_To_Interrupt_Mode(void);

// 声明全局变量，让其他文件可以使用它
extern volatile uint8_t g_KeyCode;

#endif
