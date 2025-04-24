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

#ifndef ARRAY_LEN
#define ARRAY_LEN(array) (sizeof((array)) / sizeof((array)[0]))
#endif

#define LED1_PORT GPIOD
#define LED1_PIN GPIO12

/* Morse standard timings */
#define ELEMENT_TIME 500
#define DIT (1 * ELEMENT_TIME)
#define DAH (3 * ELEMENT_TIME)
#define INTRA (1 * ELEMENT_TIME)
#define INTER (3 * ELEMENT_TIME)
#define WORD (7 * ELEMENT_TIME)

uint16_t frequency_sequence[] = {
    DIT,   INTRA, DIT,   INTRA, DIT,   INTER, DAH,   INTRA, DAH,
    INTRA, DAH,   INTER, DIT,   INTRA, DIT,   INTRA, DIT,   WORD,
};

int frequency_sel = 0;

static void tim_setup(void) {
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

  // Timer tutorial GPIO setup
  /* Enable GPIO clock for leds. */
  rcc_periph_clock_enable(RCC_GPIOD);
  /* Enable led as output */
  gpio_mode_setup(LED1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED1_PIN);
  gpio_set(LED1_PORT, LED1_PIN);
}

int main(void) {
  clock_setup();
  gpio_setup();
  usart_setup();
  tim_setup();

  printf("hello world! -Zbee\n");

  // Initialize tasks
  TaskControlBlock tcb_1 = {long_task, REGULAR_PRIORITY, 0, 0};
  TaskControlBlock tcb_2 = {short_task, REGULAR_PRIORITY, 0, 0};
  // TaskControlBlock tcb_3 = {task_3, WARNING_PRIORITY, 0};

  // QueueNode node_3 = {&tcb_3, NULL};
  QueueNode node_2 = {&tcb_2, NULL};
  QueueNode node_1 = {&tcb_1, &node_2};

  Queue queue = {&node_1, &node_2, 3};

  // Scheduler runs forever
  run_scheduler(&queue);

  return 0;
}
