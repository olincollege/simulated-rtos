#include "syscalls.h"

#include <errno.h>
#include <libopencm3/stm32/usart.h>  // USART2
#include <unistd.h>

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

int _close(void) {
  errno = EBADF;
  return -1;
}

int _fstat_r(void) {
  errno = EBADF;
  return -1;
}

int _isatty_r(void) {
  errno = EBADF;
  return -1;
}

int _lseek_r(void) {
  errno = EBADF;
  return -1;
}

int _read_r(void) {
  errno = EBADF;
  return -1;
}
