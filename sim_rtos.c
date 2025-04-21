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
  printf("Start TIM setup\n");

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

  /* Set the initual output compare value for OC1. */
  timer_set_oc_value(TIM2, TIM_OC1, frequency_sequence[frequency_sel++]);

  /* Counter enable. */
  timer_enable_counter(TIM2);

  /* Enable Channel 1 compare interrupt to recalculate compare values */
  timer_enable_irq(TIM2, TIM_DIER_CC1IE);

  printf("TIM is set up.\n");
}

void tim2_isr(void) {
  if (timer_get_flag(TIM2, TIM_SR_CC1IF)) {
    /* Clear compare interrupt flag. */
    timer_clear_flag(TIM2, TIM_SR_CC1IF);

    /*
     * Get current timer value to calculate next
     * compare register value.
     */
    uint16_t compare_time = timer_get_counter(TIM2);

    /* Calculate and set the next compare value. */
    uint16_t frequency = frequency_sequence[frequency_sel++];
    uint16_t new_time = compare_time + frequency;

    timer_set_oc_value(TIM2, TIM_OC1, new_time);
    if (frequency_sel == ARRAY_LEN(frequency_sequence)) {
      frequency_sel = 0;
    }

    printf("LED toggled!\n");
    /* Toggle LED to indicate compare event. */
    gpio_toggle(LED1_PORT, LED1_PIN);
  }
}

/* Forward declarations */
int _write(int file, char* ptr, int len);

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
  /* Setup GPIO pin GPIO12 on GPIO port D for LED. */
  gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);

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

static void button_setup(void) {
  /* Enable GPIOA clock. */
  rcc_periph_clock_enable(RCC_GPIOA);

  /* Set GPIOA0 to 'input floating'. */
  gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
}

int _write(int file, char* ptr, int len) {
  int i;

  if (file == STDOUT_FILENO || file == STDERR_FILENO) {
    for (i = 0; i < len; i++) {
      if (ptr[i] == '\n') {
        usart_send_blocking(USART2, '\r');
      }
      usart_send_blocking(USART2, ptr[i]);
    }
    return i;
  }
  errno = EIO;
  return -1;
}

int main(void) {
  clock_setup();
  gpio_setup();
  usart_setup();
  button_setup();
  tim_setup();

  printf("hello world! -Zbee\n");

  // Initialize tasks
  // TaskControlBlock tcb_1 = {task_1, REGULAR_PRIORITY, 0};
  // TaskControlBlock tcb_2 = {task_2, REGULAR_PRIORITY, 0};
  // TaskControlBlock tcb_3 = {task_3, WARNING_PRIORITY, 0};

  // QueueNode node_3 = {&tcb_3, NULL};
  // QueueNode node_2 = {&tcb_2, &node_3};
  // QueueNode node_1 = {&tcb_1, &node_2};

  // Queue queue = {&node_1, &node_3, 3};

  while (1) {
    // printf("\n");
  }

  // run_scheduler(&queue);

  // bool button_is_pressed = false;

  // while (1) {
  //   if (!button_is_pressed && gpio_get(GPIOA, GPIO0)) {
  //     button_is_pressed = true;
  //   } else if (button_is_pressed && !gpio_get(GPIOA, GPIO0)) {
  //     printf("button pressed\n");
  //     button_is_pressed = false;
  //   }
  // }

  return 0;
}
