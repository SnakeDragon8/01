#include "main.h"

int main(void) {
    LED led1(GPIOB, GPIO_Pin_0);
    LED led2(GPIOB, GPIO_Pin_1);

    // 调用我们封装好的函数，初始化TIM3定时器，使其每秒触发一次中断。
    TIM3_Init_1s();

    TIM4_Debounce_Init();
    MatrixKeyboard_Init_Interrupt();

    LightSensor_Init();

    // 【C++特性的核心应用】
    // 调用回调注册函数，将一个“Lambda表达式”作为参数传递进去。
    // [ &led1 ]  -> 这是Lambda的“捕获列表”，表示通过“引用”方式捕获外部的led1对象。
    //             这样，在Lambda函数体内就可以直接访问和操作main函数里的led1了。
    // ()         -> 这是Lambda的参数列表，这里为空，表示它是一个无参函数。
    // { ... }    -> 这是Lambda的函数体，定义了具体要执行的操作。
    TIM3_Register_Callback([&led1]() {
        // 当定时器中断发生时，此处的代码将被执行。
        // 调用led1对象的toggle方法，实现LED状态的翻转（亮变灭，灭变亮）。
        led1.toggle();
    });

    I2C_Configuration();
    OLED_Init();
    Encoder_Init();
    // OLED_NewFrame();
    // OLED_ShowFrame();
    OLED_PrintString(0, 0, (char *)"Hello World!", &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();

    

    int16_t count = 0;


    while (1) {
        if(g_KeyCode != 0xFF)
        {
            uint8_t currentKey = g_KeyCode; // 读取键值
            g_KeyCode = 0xFF;       // 【重要】立即复位全局变量，表示已处理
            if(currentKey == KEY_0)
            {
                led2.on();
                delay_ms(500);
            }
            else if(currentKey == KEY_1)
            {
                led2.off();
                delay_ms(500);
            }
            else if(currentKey == KEY_2)
            {
                led2.toggle();
                delay_ms(500);
            }


        }
        uint8_t lightState = LightSensor_GetState();
        if(lightState == 0)
        {
            led2.on();
            OLED_PrintString(1, 16, (char *)"Light: ON ", &font16x16, OLED_COLOR_NORMAL);
            OLED_ShowFrame();
        }
        else
        {
            led2.off();
            OLED_PrintString(1, 16, (char *)"Light: OFF", &font16x16, OLED_COLOR_NORMAL);
            OLED_ShowFrame();
        }

        count += Encoder_GetCount();
        char count_str[20];
        sprintf(count_str, "Count: %5d", count);
        OLED_PrintString(0, 33, count_str, &font16x16, OLED_COLOR_NORMAL);
        OLED_ShowFrame();


    }
}