#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h" // 必须包含STM32的头文件来识别 GPIO_TypeDef 等

class LED {
public:
    // 构造函数：创建一个LED对象时，需要告诉它在哪个GPIO端口和哪个引脚
    LED(GPIO_TypeDef* port, uint16_t pin);

    // 公共成员函数：控制LED的行为
    void on();       // 点亮
    void off();      // 关闭
    void toggle();   // 翻转状态
    void blink(uint32_t delay_ms); // 闪烁

private:
    // 私有成员变量：存储每个LED对象自身的信息
    GPIO_TypeDef* gpio_port; // 存储端口号 (例如 GPIOB)
    uint16_t      gpio_pin;  // 存储引脚号 (例如 GPIO_Pin_0)
};

#endif // LED_H
