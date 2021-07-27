#include "ch32v10x_usart.h"
#include "ch32v10x_rcc.h"
#include "core_riscv.h"
#include "board.h"
#include "zf_pit.h"
#include "CH32V10x_TIM.h"

#include "clock_config.h"
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>
#include "headfile.h"

//finsh组件接收串口数据，是通过在串口中断内发送邮件，finsh线程接收邮件进行获取的
rt_mailbox_t uart_mb;

static uint32_t systick_config(rt_uint32_t ticks)
{
    if ((ticks - 1) > 0xFFFFFFFF)
    {
        return 1;
    }
    ticks -= 1;

    SysTick->CNTL0 = 0;
    SysTick->CNTL1 = 0;
    SysTick->CNTL2 = 0;
    SysTick->CNTL3 = 0;

    SysTick->CNTH0 = 0;
    SysTick->CNTH1 = 0;
    SysTick->CNTH2 = 0;
    SysTick->CNTH3 = 0;

    SysTick->CMPLR0 = (ticks >>  0) & 0xff;
    SysTick->CMPLR1 = (ticks >>  8) & 0xff;
    SysTick->CMPLR2 = (ticks >> 16) & 0xff;
    SysTick->CMPLR3 = (ticks >> 24) & 0xff;

    SysTick->CMPHR0 = 0;
    SysTick->CMPHR1 = 0;
    SysTick->CMPHR2 = 0;
    SysTick->CMPHR3 = 0;

    NVIC_SetPriority(SysTicK_IRQn, 255);
    NVIC_EnableIRQ(SysTicK_IRQn);
    SysTick->CTLR = 1;
    return 0;
}

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 1624
static uint32_t rt_heap[RT_HEAP_SIZE];     // heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{
    /* System Clock Update */
    board_init();

    /* System Tick Configuration */
    systick_config(sys_clk / RT_TICK_PER_SECOND / 8);//systick频率等于HCLK/8  1ms 一次中断

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init(); //初始化
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());//动态内存
#endif

    uart_mb = rt_mb_create("uart_mb", 10, RT_IPC_FLAG_FIFO);//创建了一个邮箱

    NVIC_EnableIRQ(Software_IRQn); //软件中断
    NVIC_SetPriority(Software_IRQn, 255);//设置软件中断的优先级

    //NVIC_PriorityGroupConfig(0);
}


__attribute__((interrupt()))
void SysTick_Handler(void)
{
    //进入中断
    rt_interrupt_enter();

    //需要手动清除CNT的值
    SysTick->CNTL0 = 0;
    SysTick->CNTL1 = 0;
    SysTick->CNTL2 = 0;
    SysTick->CNTL3 = 0;
    SysTick->CNTH0 = 0;
    SysTick->CNTH1 = 0;
    SysTick->CNTH2 = 0;
    SysTick->CNTH3 = 0;

    rt_tick_increase();

    //退出中断
    rt_interrupt_leave();
}


void board_init(void)
{
    //获取系统主频
    sys_clk = 8000000 * (((RCC->CFGR0 >> 18)&0x0F) + 2);
    //初始化DEBUG串口
    uart_init(DEBUG_UART, DEBUG_UART_BAUD, DEBUG_UART_TX_PIN, DEBUG_UART_RX_PIN);
//    uart_init(DEBUG_UART, 115200, DEBUG_UART_TX_PIN, DEBUG_UART_RX_PIN);
    uart_rx_irq(DEBUG_UART, ENABLE);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      printf重定向
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:              重定向printf到DEBUG串口上
//-------------------------------------------------------------------------------------------------------------------
#if (1 == PRINTF_ENABLE)
int _write(int fd, char *buf, int size)
{
    int i;
    for(i=0; i<size; i++)
    {
        while (USART_GetFlagStatus((USART_TypeDef*)UARTN[DEBUG_UART], USART_FLAG_TC) == RESET);
        USART_SendData((USART_TypeDef*)UARTN[DEBUG_UART], *buf++);
    }
    return size;
}
#endif

void rt_hw_console_output(const char *str)
{
    while(RT_NULL != *str)
    {
        if('\n' == *str)
        {
            uart_putchar(DEBUG_UART, '\r');
        }
        uart_putchar(DEBUG_UART, *str++);
    }
}

char rt_hw_console_getchar(void) //串口接收
{
//    uint32 dat;
//    rt_mb_recv(uart_mb, &dat, RT_WAITING_FOREVER);
//    //uart_getchar(DEBUG_UART, &dat);
//    return (char)dat;
    return 0;             //8266 test
}


void USART1_IRQHandler(void) __attribute__((interrupt()));
void USART1_IRQHandler(void)
{
//    uint8 dat;
    uint8 data_temp;            //8266test
    rt_interrupt_enter();       //进入中断

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
//        uart_getchar(DEBUG_UART, &dat);
//        rt_mb_send(uart_mb, dat);           //发送邮件
/*********************************************************************/
        data_temp = (uint8)USART_ReceiveData(USART1);
        esp8266_buf[esp8266_cnt++] = data_temp;
        if (esp8266_cnt > 63) {
            esp8266_cnt = 0;
        }
        if (data_temp == 0x0A) { //接收到了换行符
            rt_sem_release(esp8266_sem); //释放信号量
        }
/**********************************************************************/
    }

    rt_interrupt_leave();       //退出中断
}
