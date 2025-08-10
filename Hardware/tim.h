#ifndef __TIM_H // 这是标准的头文件保护宏，防止同一个头文件在项目中被重复包含（include），从而导致重定义错误。
#define __TIM_H

// 必须包含STM32的官方头文件，这样才能识别像 TIM3、RCC 这些寄存器指针，以及 NVIC_InitTypeDef 这样的结构体。
#include "stm32f10x.h" 
#include <functional>

// 【核心】定义一个名为 TIM3_Callback_t 的新类型。
// 这个类型是 std::function<void()>，它是一个通用的函数包装器，可以持有任何“无参数，无返回值”的可调用对象。
// 相比C语言的函数指针，它的巨大优势是能够存储和调用带有捕获列表的Lambda表达式，使得回调机制极为灵活。
typedef std::function<void()> TIM3_Callback_t;

// --- 公共函数接口声明 ---
// 这些是在其他文件中可以被调用的函数。

// 初始化TIM3定时器，配置为每1秒触发一次更新中断。
void TIM3_Init_1s(void);

// 注册一个回调函数。当定时器中断发生时，这个被注册的函数将会被执行。
// 参数类型是我们上面定义的 TIM3_Callback_t，因此可以接收Lambda表达式。
void TIM3_Register_Callback(TIM3_Callback_t callback);


// 【C/C++混合编程的关键】
// #ifdef __cplusplus 和 extern "C" 确保在C++编译器下，被包裹的代码遵循C语言的链接规范。
#ifdef __cplusplus
extern "C" {
#endif

// 声明中断处理函数。
// 这个函数本体在 tim.cpp 中实现，但它的声明需要用 extern "C" 包裹。
// 这样做的目的是告诉C++编译器，不要对这个函数名进行“名字修饰（Name Mangling）”。
// 从而保证在纯C文件 stm32f10x_it.c 中通过函数名 TIM3_HandleInterrupt 能够正确地链接到它的实现。
void TIM3_HandleInterrupt(void);

// 与 #ifdef __cplusplus 成对出现，结束 extern "C" 块。
#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __TIM_H, 结束头文件保护宏。