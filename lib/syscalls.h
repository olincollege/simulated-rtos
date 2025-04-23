/*
Syscall function library. Most functions here are not needed for the project and
are implemented as dummy functions to supress compiler warnings.
*/

/**
 * @brief  Syscall write function to redirect stdout/stderr to USART2.
 *
 * This function is used by the standard library (e.g., printf) to write output.
 * It sends each character in the buffer to the USART2 peripheral. Newline
 * characters ('\n') are automatically preceded by a carriage return ('\r') to
 * ensure proper formatting on terminals that expect CRLF line endings.
 *
 * @param file File descriptor (usually STDOUT_FILENO or STDERR_FILENO).
 * @param ptr  Pointer to the data buffer to send.
 * @param len  Number of bytes to write from the buffer.
 *
 * @return Number of bytes written on success, -1 on failure (sets errno to
 * EIO).
 */
int _write(int file, char* ptr, int len);

int _close(void);

int _fstat_r(void);

int _isatty_r(void);

int _lseek_r(void);

int _read_r(void);
