#pragma once

/* Set how often the TIM3 timer adds a short task to the queue.*/
extern const int SHORT_TASK_PERIOD;

/**
 * @brief Set up the TIM2 timer.
 *
 * Set TIM2 to run at 5 kHz with a period of 65535 (full range) and start the
 * timer.
 */
void tim2_setup(void);

/**
 * @brief Set up the TIM3 timer.
 *
 * Set TIM3 to run at 5 kHz with a period of 3 ticks. Set Update Event interrupt
 * so every time the timer overflows (which will be every three ticks), it
 * triggers a TIM3 interrupt.
 */
void tim3_setup(void);

/**
 * @brief Set up system and peripheral clocks.
 *
 * Enable clocks for GPIOA (for USART2 pins), and USART2.
 * Configure the system clock to run at 168 MHz using the external 8 MHz crystal
 * oscillator and PLL via libopencm3's predefined clock setup.
 */
void clock_setup(void);

/**
 * @brief Set up USART2 for serial transmission.
 *
 * Configure USART2 with 115200 baud, 8 data bits, 1 stop bit, no parity,
 * no flow control, and transmit-only mode. Finally, enable the USART2
 * peripheral.
 */
void usart_setup(void);

/**
 * @brief Configure GPIO pin for USART2 transmit.
 *
 * Set PA2 (GPIO2 on port A) to alternate function mode with no pull-up/down,
 * and assign it to alternate function 7 for USART2 TX operation.
 */
void gpio_setup(void);
