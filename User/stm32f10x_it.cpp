/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "main.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

void TIM3_IRQHandler(void) 
{
  TIM3_HandleInterrupt();
}

// EXTI中断服务函数 (行线 PA5, PA6, PA7)
void EXTI9_5_IRQHandler(void) 
{
  // 统一处理，只要有中断进来就关闭所有行中断并启动定时器
  Disable_All_Row_Interrupts();
  TIM4_Start();

  // // 判断并清除具体线路的标志位
  // if(EXTI_GetITStatus(EXTI_Line5) != RESET) EXTI_ClearITPendingBit(EXTI_Line5);
  // if(EXTI_GetITStatus(EXTI_Line6) != RESET) EXTI_ClearITPendingBit(EXTI_Line6);
  // if(EXTI_GetITStatus(EXTI_Line7) != RESET) EXTI_ClearITPendingBit(EXTI_Line7);
  EXTI_ClearITPendingBit(EXTI_Line5);
  EXTI_ClearITPendingBit(EXTI_Line6);
  EXTI_ClearITPendingBit(EXTI_Line7);
}

// EXTI中断服务函数 (行线 PA4)
void EXTI4_IRQHandler(void) 
{
  if(EXTI_GetITStatus(EXTI_Line4) != RESET)
  {
    Disable_All_Row_Interrupts(); // 关闭所有行中断防止抖动
    TIM4_Start();                 // 启动20ms消抖定时器
  }
  EXTI_ClearITPendingBit(EXTI_Line4); // 清除标志位
}

// TIM4中断服务函数（消抖和扫描）
void TIM4_IRQHandler(void) 
{
  if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {
    // 1.切换到扫描模式
    Switch_To_Scan_Mode();
    // 2.扫描一次，获取键值
    g_KeyCode = Perform_Single_Scan();
    // 3.切换回中断等待模式
    Switch_To_Interrupt_Mode();
    // 4.清除所有可能在切换期间产生的悬挂标志位
    EXTI_ClearITPendingBit(EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7);
    // 5. 重新使能所有行中断，准备下一次按键
    Enable_All_Row_Interrupts();
  }
  // 6. 清除TIM4更新中断标志位
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}

void EXTI15_10_IRQHandler(void)
{
    // 检查是不是Line12触发了中断
    if(EXTI_GetITStatus(EXTI_Line12) != RESET)
    {
        // 编码器解码逻辑
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
        {
            Encoder_Count--;
        }
        else
        {
            Encoder_Count++;
        }

        // 【关键】在处理完之后，立刻清除Line12的中断标志位！
        
    }
    EXTI_ClearITPendingBit(EXTI_Line12); 

    // 【安全保护】检查是不是Line13也可能触发中断
    // 即使我们只打算用Line12，这个检查也能保证程序的健壮性
    if(EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        // 如果Line13也意外触发了，我们不进行计数，但必须清除它的标志位
        // 否则程序会因为这个未清除的标志位而卡死在这里
        
    }
    EXTI_ClearITPendingBit(EXTI_Line13);
}
