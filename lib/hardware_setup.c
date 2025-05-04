#include "hardware_setup.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>

/* Define the short task period to be 3 ticks = 0.6 ms*/
const int SHORT_TASK_PERIOD = 15;

void tim2_setup(void) {
  /* Enable TIM2 clock. */
  rcc_periph_clock_enable(RCC_TIM2);

  /* Enable TIM2 interrupt. */
  nvic_enable_irq(NVIC_TIM2_IRQ);

  /* Reset TIM2 peripheral to defaults. */
  rcc_periph_reset_pulse(RST_TIM2);

  /* Timer global mode:
   * - No divider
   * - Alignment edge
   * - Direction up
   * (These are actually default values after reset above, so this call
   * is strictly unnecessary, but demos the api for alternative settings)
   */
  timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

  /*
   * Please take note that the clock source for STM32 timers
   * might not be the raw APB1/APB2 clocks.  In various conditions they
   * are doubled.  See the Reference Manual for full details!
   * In our case, TIM2 on APB1 is running at double frequency, so this
   * sets the prescaler to have the timer run at 5kHz
   */
  timer_set_prescaler(TIM2, ((rcc_apb1_frequency * 2) / 5000));

  /* Disable preload. */
  timer_disable_preload(TIM2);
  timer_continuous_mode(TIM2);

  /* count full range, as we'll update compare value continuously */
  timer_set_period(TIM2, 65535);

  /* Counter enable. (start timer)*/
  timer_enable_counter(TIM2);
}

void tim3_setup(void) {
  /* Enable TIM3 clock. */
  rcc_periph_clock_enable(RCC_TIM3);

  /* Enable TIM3 interrupt. */
  nvic_enable_irq(NVIC_TIM3_IRQ);

  /* Reset TIM3 peripheral to defaults. */
  rcc_periph_reset_pulse(RST_TIM3);

  /* Set timer mode */
  timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

  /* Set prescaler to 5 mHz */
  timer_set_prescaler(TIM3, ((rcc_apb1_frequency * 2) / 5000));

  /* Set period to SHORT_TASK_PERIOD=15 ticks => 3ms */
  timer_set_period(TIM3, SHORT_TASK_PERIOD);  // 15 ticks * 0.2ms = 3ms

  /* Enable interrupt when timer overflows */
  timer_enable_irq(TIM3, TIM_DIER_UIE);

  /* Start the counter */
  timer_enable_counter(TIM3);
}

void clock_setup(void) {
  /* Enable GPIOD clock for USARTs. */
  rcc_periph_clock_enable(RCC_GPIOA);

  /* Enable clocks for USART2. */
  rcc_periph_clock_enable(RCC_USART2);

  // Clock setup for timers
  rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
}

void usart_setup(void) {
  /* Setup USART2 parameters. */
  usart_set_baudrate(USART2, 115200);
  usart_set_databits(USART2, 8);
  usart_set_stopbits(USART2, USART_STOPBITS_1);
  usart_set_mode(USART2, USART_MODE_TX);
  usart_set_parity(USART2, USART_PARITY_NONE);
  usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

  /* Finally enable the USART. */
  usart_enable(USART2);
}

void gpio_setup(void) {
  /* Setup GPIO pins for USART2 transmit. */
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

  /* Setup USART2 TX pin as alternate function. */
  gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
}
