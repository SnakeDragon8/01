// 包含对应的头文件，获取函数声明和类型定义
#include "tim.h"

// 定义一个静态的、本文件私有的回调函数对象。
// "static" 关键字确保 s_TIM3_Callback 这个变量只能在当前文件(tim.cpp)内部被访问。
// "s_" 是一种常见的命名约定，用来表示静态变量(static)。
// 初始化为 nullptr 是一个好习惯，表示默认没有注册任何回调。
static TIM3_Callback_t s_TIM3_Callback = nullptr;

// TIM3定时器初始化函数
void TIM3_Init_1s(void) {
    // 1. 使能TIM3外设的时钟。TIM3挂载在APB1总线上。
    RCC->APB1ENR |= (1<<1);
    
    // 2. 配置定时器时基单元
    // 预分频器 (Prescaler)。时钟频率为 72MHz，72,000,000 / (7199 + 1) = 10,000 Hz。
    TIM3->PSC = 7199;
    // 自动重装载寄存器 (Auto-Reload Register)。计数器从0计数到9999，产生一次更新事件（中断）。
    // 这样中断频率就是 10,000 Hz / (9999 + 1) = 1 Hz，周期为1秒。
    TIM3->ARR = 9999;
    
    // 3. 使能定时器更新中断。
    // DIER (DMA/Interrupt Enable Register) 的 UIE 位 (Update Interrupt Enable) 置1。
    TIM3->DIER |= (1<<0);

    // 4. 配置NVIC（嵌套向量中断控制器）
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;                   // 选择TIM3的中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;         // 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;                // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                   // 使能这个中断通道
    NVIC_Init(&NVIC_InitStructure);                                   // 调用库函数执行配置

    // 5. 启动定时器。
    // CR1 (Control Register 1) 的 CEN 位 (Counter Enable) 置1。
    TIM3->CR1 |= (1<<0);
}

// 回调函数注册函数
void TIM3_Register_Callback(TIM3_Callback_t callback) {
    // 将用户传入的回调函数（比如main.cpp中的Lambda表达式）保存到静态变量中。
    s_TIM3_Callback = callback;
}

// 使用 extern "C" 来确保C/C++混合编译时链接正确
extern "C" {

// 这是中断事件的真正处理函数，它由 stm32f10x_it.c 中的中断入口函数 TIM3_IRQHandler 调用
void TIM3_HandleInterrupt(void) {
    // 【安全检查】判断是否真的是更新中断标志位 (Update Interrupt Flag, UIF) 被置位了。
    // 这是严谨的编程习惯，确保我们只处理我们关心的中断源。
    if((TIM3->SR & (1<<0)) != 0) {
        // 【安全检查】判断回调函数是否已经被注册（即不是nullptr）。
        if(s_TIM3_Callback != nullptr) {
            // 如果已经注册，就执行它。
            // 这里就是调用main.cpp中定义的Lambda表达式的地方。
            s_TIM3_Callback();
        }
    }
    // 【关键步骤】清除中断标志位。
    // 硬件检测到事件后会自动把这个标志位置1，但它不会自动清零。
    // 必须由软件在处理完中断后手动将其清零，否则中断控制器会认为中断事件仍在发生，
    // 导致程序退出中断后立刻又重新进入，形成死循环。
    TIM3->SR &= ~(1<<0);
} 
    
}