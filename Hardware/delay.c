#include "Delay.h"
#include <stdlib.h>

static uint64_t cur_count = 0;
volatile uint64_t systick_count = 0;

void Delay_ms(uint32_t ms)
{
    cur_count = systick_count;
    while(!(systick_count >= cur_count + ms));
}

void Delay_s(uint32_t s)
{
    cur_count = systick_count;
    while(!(systick_count >= cur_count + s*1000));
} 

void Delay_SystickStart(void)
{
    SysTick->LOAD = SYSTICK_RELOAD;			// 设置定时器重装值
    SysTick->VAL = 0x00;					// 清空当前计数值
    SysTick->CTRL = 0x00000003;				// 设置时钟源为HCLK/8:9MHz，启动定时器
}

void SysTick_Handler(void)  // 每1ms中断一次，使用该函数需要将stm32f10x_it.c文件中的该函数注释掉
{
    systick_count++;
}
