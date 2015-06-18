/*
 * Copyright (C) 2014  Raphaël Poggi <poggi.raph@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <board.h>
#include <stdio.h>
#include <utils.h>
#include <interrupt.h>
#include <scheduler.h>
#include <armv7m/system.h>
#include <arch/nvic.h>
#include <time.h>
#include <pio.h>
#include <common.h>


void hardfault_handler(void)
{
	while (1)
		;
}

void memmanage_handler(void)
{
	while (1)
		;
}

void busfault_handler(void)
{
	while (1)
		;
}

void usagefault_handler(void)
{
	while (1)
		;
}

void systick_handler(void)
{
	unsigned int val = readl(SCB_ICSR);

	system_tick++;

	val |= SCB_ICSR_PENDSVSET;
	writel(SCB_ICSR, val);
}

void pendsv_handler(void)
{
	schedule_task(NULL);
}

void timer2_handler(void)
{
	nvic_clear_interrupt(TIM2_IRQn);
	debug_printk("timer2 trig\r\n");
//	pio_toggle_value(GPIOE_BASE, 6);
	decrease_task_delay();
}


void i2c1_event_handler(void)
{
}

void i2c1_error_handler(void)
{
}

void dma2_stream0_handler(void)
{
	if (DMA2->LISR & DMA_LISR_TCIF0) {
		debug_printk("transfert complete\r\n");
		DMA2->LIFCR = DMA_LIFCR_CTCIF0;
	}

	if (DMA2->LISR & DMA_LISR_HTIF0) {
		debug_printk("half transfer interrupt\r\n");
		DMA2->LIFCR = DMA_LIFCR_CHTIF0;
	}

	if (DMA2->LISR & DMA_LISR_TEIF0) {
		debug_printk("transfert error\r\n");
		DMA2->LIFCR = DMA_LIFCR_CTEIF0;

	}
	
	if (DMA2->LISR & DMA_LISR_DMEIF0) {
		debug_printk("direct mode error\r\n");
		DMA2->LIFCR = DMA_LIFCR_CDMEIF0;
	}

	if (DMA2->LISR & DMA_LISR_FEIF0) {
		debug_printk("fifo error\r\n");
		DMA2->LIFCR = DMA_LIFCR_CFEIF0;
	}
}

void exti0_handler(void)
{
	nvic_clear_interrupt(EXTI0_IRQn);
	EXTI->PR |= (1 << 0);

	printk("exti0_handler\r\n");
}

void exti1_handler(void)
{
	nvic_clear_interrupt(EXTI1_IRQn);
	EXTI->PR |= (1 << 1);

	printk("exti1_handler\r\n");
}

void exti2_handler(void)
{
	nvic_clear_interrupt(EXTI2_IRQn);
	EXTI->PR |= (1 << 2);

	printk("exti2_handler\r\n");
}
void exti3_handler(void)
{
	nvic_clear_interrupt(EXTI3_IRQn);
	EXTI->PR |= (1 << 3);

	printk("exti3_handler\r\n");
}
void exti4_handler(void)
{
	nvic_clear_interrupt(EXTI4_IRQn);
	EXTI->PR |= (1 << 4);

	printk("exti4_handler\r\n");
}

void exti9_5_handler(void)
{
	nvic_clear_interrupt(EXTI9_5_IRQn);
	EXTI->PR |= (0x3E);

	printk("exti9_5_handler\r\n");
}
void exti15_10_handler(void)
{
	nvic_clear_interrupt(EXTI15_10_IRQn);
	EXTI->PR |= (0xFC00);

	printk("exti15_10_handler\r\n");
}
