#include <errno.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <unistd.h>

#include "queue.h"
#include "scheduler.h"
#include "task.h"

/*
Create global task queue and task tcbs.
Can tweak the frquency of how often the short task is enqueued
*/
Queue* global_queue = NULL;  // create glabal queue so any .c file can access it
TaskControlBlock short_task_tcbs[MAX_SHORT_TASK];  // initialize them so the
                                                   // interrupt can access
QueueNode short_task_nodes[MAX_SHORT_TASK];

static void tim2_setup(void) {
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

static void tim3_setup(void) {
  const int SHORT_TASK_PERIOD = 3;
  /* Enable TIM3 clock. */
  rcc_periph_clock_enable(RCC_TIM3);

  /* Enable TIM3 interrupt. */
  nvic_enable_irq(NVIC_TIM3_IRQ);

  /* Reset TIM3 peripheral to defaults. */
  rcc_periph_reset_pulse(RST_TIM3);

  /* Set timer mode */
  timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

  /* Set prescaler to slow down the clock */
  timer_set_prescaler(TIM3, ((rcc_apb1_frequency * 2) / 5000));
  /* Timer now ticks at 10kHz = 0.1ms per tick */

  /* Set period to 500 ticks => 50ms */
  timer_set_period(TIM3, SHORT_TASK_PERIOD);  // 100 ticks * 0.1ms = 50ms

  /* Enable Update Event Interrupt */
  timer_enable_irq(TIM3, TIM_DIER_UIE);

  /* Start the counter */
  timer_enable_counter(TIM3);
}

static void clock_setup(void) {
  /* Enable GPIOD clock for LED & USARTs. */
  rcc_periph_clock_enable(RCC_GPIOD);
  rcc_periph_clock_enable(RCC_GPIOA);

  /* Enable clocks for USART2. */
  rcc_periph_clock_enable(RCC_USART2);

  // Clock setup from timer example
  rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
}

static void usart_setup(void) {
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

static void gpio_setup(void) {
  /* Setup GPIO pins for USART2 transmit. */
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

  /* Setup USART2 TX pin as alternate function. */
  gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
}

int main(void) {
  clock_setup();
  gpio_setup();
  usart_setup();
  tim2_setup();
  tim3_setup();

  printf("hello world! -Zbee\n");

  // Initialize long task
  TaskControlBlock tcb_long = {long_task, REGULAR_PRIORITY, 0, 0, NULL};
  // initialize short tasks
  for (int i = 0; i < MAX_SHORT_TASK; i++) {
    short_task_tcbs[i].func = short_task;
    short_task_tcbs[i].priority = REGULAR_PRIORITY;
    short_task_tcbs[i].is_available = 1;  // free
  }

  // TaskControlBlock tcb_3 = {task_3, WARNING_PRIORITY, 0};

  // QueueNode node_3 = {&tcb_3, NULL};
  QueueNode node_long = {&tcb_long, NULL};
  for (int i = 0; i < MAX_SHORT_TASK; i++) {
    short_task_nodes[i].tcb = &short_task_tcbs[i];
    short_task_nodes[i].next = NULL;
    short_task_tcbs[i].my_node = &short_task_nodes[i];
  }

  Queue queue = {&node_long, &node_long, 1};
  global_queue = &queue;

  // Scheduler runs forever
  run_scheduler(&queue);

  while (1) {
  }

  return 0;
}
