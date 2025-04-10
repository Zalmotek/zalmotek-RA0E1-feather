#ifndef SerialCompatibility_h
#define SerialCompatibility_h

#include "hal_data.h"

#define MAX_BUFF_SIZE             512
#define TRANSFER_LENGTH           (512u)
#define UART_ERROR_EVENTS         (UART_EVENT_BREAK_DETECT | UART_EVENT_ERR_OVERFLOW | UART_EVENT_ERR_FRAMING | \
                                    UART_EVENT_ERR_PARITY)

#define ENABLE_SERIAL0
#define ENABLE_SERIAL1

#ifdef ENABLE_SERIAL0
/* Serial definition
 * channel 0
 * uart_callback
 * g_uart0
 *
 *
 *
 */


void uart_callback_0 (uart_callback_args_t * p_args);

class SerialCompatibilityLayer {
private:
    uint8_t buff[MAX_BUFF_SIZE], aux[MAX_BUFF_SIZE];
    uint16_t idx, fin;
    int16_t cnt;
    void fill_buff();

    fsp_err_t uart_read(uint8_t *rx, uint16_t *len);
    fsp_err_t uart_print(uint8_t *p_msg, uint16_t msg_len);
    void uart_init(uint32_t *baud);
    void uart_close(void);

public:
    SerialCompatibilityLayer();
    uint16_t available();
    uint8_t read();
    void write(uint8_t byte);
    void print(uint8_t *p_msg);
    void println(uint8_t *p_msg);
    void print(long long p_msg);
    void println(long long p_msg);
    uint8_t peek();
    void begin(uint32_t baud);
};

extern SerialCompatibilityLayer Serial;
#endif

#ifdef ENABLE_SERIAL1
/* Serial1 definition
 * channel 1
 * uart_callback_1
 * g_uart1
 *
 *
 *
 */

void uart_callback_1 (uart_callback_args_t * p_args);

class SerialCompatibilityLayer_1 {
private:
    uint8_t buff[MAX_BUFF_SIZE], aux[MAX_BUFF_SIZE];
    uint16_t idx, fin;
    int16_t cnt;
    void fill_buff();

    fsp_err_t uart_read(uint8_t *rx, uint16_t *len);
    fsp_err_t uart_print(uint8_t *p_msg, uint16_t msg_len);
    void uart_init(uint32_t *baud);
    void uart_close(void);

public:
    SerialCompatibilityLayer_1();
    uint16_t available();
    uint8_t read();
    void write(uint8_t byte);
    void print(uint8_t *p_msg);
    void println(uint8_t *p_msg);
    void print(long long p_msg);
    void println(long long p_msg);
    uint8_t peek();
    void begin(uint32_t baud);
};

extern SerialCompatibilityLayer_1 Serial1;
#endif

#endif //SerialCompatibility_h
