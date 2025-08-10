#include "led.h"
#include "Delay.h"

// 构造函数的实现
LED::LED(GPIO_TypeDef* port, uint16_t pin) {
    // 1. 将传入的端口和引脚信息保存到类的私有成员变量中
    gpio_port = port;
    gpio_pin = pin;

    // 2. 开启对应的GPIO端口时钟 (这是C++ RAII思想的体现)
    //    在对象创建时，自动完成其所需的时钟初始化
    if (port == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    } else if (port == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    } else if (port == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    } else if (port == GPIOD) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    } else if (port == GPIOE) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    } else if (port == GPIOF) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    } else if (port == GPIOG) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    }

    // 3. 配置GPIO引脚为推挽输出模式
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = gpio_pin; // 使用我们保存的引脚号
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(gpio_port, &GPIO_InitStructure); // 使用我们保存的端口号

    // 4. 初始化后，默认将LED关闭
    off();
}

// "on" 函数的实现
void LED::on() {
    // 高电平点亮，所以调用 SetBits
    GPIO_SetBits(gpio_port, gpio_pin);
}

// "off" 函数的实现
void LED::off() {
    GPIO_ResetBits(gpio_port, gpio_pin);
}

// "toggle" 函数的实现
void LED::toggle() {
    // 通过读取输出数据寄存器(ODR)的当前状态来判断
    if (GPIO_ReadOutputDataBit(gpio_port, gpio_pin) == Bit_SET) { // 如果当前引脚是高电平
        off(); // 就把它关闭
    } else { // 否则
        on();  // 就把它点亮
    }
}

// "blink" 函数的实现
void LED::blink(uint32_t delay_time) {
    delay_ms(delay_time);
    toggle();
}

