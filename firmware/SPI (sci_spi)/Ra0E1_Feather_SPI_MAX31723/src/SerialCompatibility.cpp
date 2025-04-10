#include "SerialCompatibility.h"

#ifdef ENABLE_SERIAL0
/* Serial definition
 * channel 0
 * uart_callback
 * g_uart0
 *
 *
 *
 */

static volatile uint8_t g_uart_event_0 = (0x00);

uint8_t  g_dest_0[TRANSFER_LENGTH];
uint8_t  g_out_of_band_received_0[TRANSFER_LENGTH];
static volatile uint32_t g_transfer_complete_0 = 0;
static volatile uint32_t g_receive_complete_0  = 0;
static volatile uint16_t g_out_of_band_index_0 = 0;

void uart_callback_0 (uart_callback_args_t * p_args)
{
    g_uart_event_0 = (uint8_t)p_args->event;
    switch (p_args->event)
    {
        case UART_EVENT_RX_CHAR:
        {
            if (sizeof(g_out_of_band_received_0) > g_out_of_band_index_0)
            {
                if (UART_DATA_BITS_8 >= g_uart0_cfg.data_bits)
                {
                    g_out_of_band_received_0[g_out_of_band_index_0++] = (uint8_t) p_args->data;
                }
                else
                {
                    uint16_t * p_dest = (uint16_t *) &g_out_of_band_received_0[g_out_of_band_index_0];
                    *p_dest              = (uint16_t) p_args->data;
                    g_out_of_band_index_0 += 2;
                }
            }
            break;
        }
        /* Receive complete */
        case UART_EVENT_RX_COMPLETE:
        {
            g_receive_complete_0 = 1;
            break;
        }
        /* Transmit complete */
        case UART_EVENT_TX_COMPLETE:
        {
            g_transfer_complete_0 = 1;
            break;
        }
        default:
        {
        }
    }
}

void SerialCompatibilityLayer::begin(uint32_t baud) {
    this->uart_init(&baud);
}

void SerialCompatibilityLayer::write(uint8_t byte) {
    this->uart_print(&byte, 1);
}

SerialCompatibilityLayer::SerialCompatibilityLayer() {
    memset(this->buff, 0, sizeof(this->buff));
    this->idx = 0;
    this->fin = 0;
    this->cnt = 0;
}

uint16_t SerialCompatibilityLayer::available() {
    this->fill_buff();
    return this->cnt;
}

uint8_t SerialCompatibilityLayer::read() {
    this->fill_buff();
    uint8_t ret = this->buff[this->idx++];
    this->cnt--;
    if (this->idx == MAX_BUFF_SIZE) {
        this->idx = 0;
    }
    if (this->cnt < 0) {
        assert(false);
    }
    return ret;
}

void SerialCompatibilityLayer::fill_buff() {
    uint16_t len = 0;
    fsp_err_t err = this->uart_read(this->aux, &len);
    if (err == FSP_SUCCESS) {
        this->cnt += (int16_t)len;
        for (int i = 0; i < len; i++) {
            this->buff[this->fin++] = this->aux[i];
            if (this->fin == MAX_BUFF_SIZE) {
                this->fin = 0;
            }
            if (this->fin == this->idx) {
                assert(false);
            }
        }
    }
}

uint8_t SerialCompatibilityLayer::peek() {
    this->fill_buff();
    return this->buff[this->idx];
}

fsp_err_t SerialCompatibilityLayer::uart_read(uint8_t *rx, uint16_t *len) {
    fsp_err_t err = FSP_SUCCESS;

    if (g_out_of_band_index_0) {
        g_receive_complete_0 = false;
        if (rx != NULL) {
            memcpy(rx, g_out_of_band_received_0, g_out_of_band_index_0);
        }
        *len = g_out_of_band_index_0;
        g_out_of_band_index_0 = 0;
        return FSP_SUCCESS;
    }
    else {
        return FSP_ERR_IP_CHANNEL_NOT_PRESENT;
    }
    return err;
}

fsp_err_t SerialCompatibilityLayer::uart_print(uint8_t *p_msg, uint16_t msg_len) {
    fsp_err_t err = FSP_SUCCESS;

    /* Reset callback capture variable */
    g_uart_event_0 = (0x00);
    g_transfer_complete_0 = 0;

    /* Writing to terminal */
    err = R_UARTA_Write(&g_uart0_ctrl, p_msg, msg_len);
    assert(err == FSP_SUCCESS);

    while (g_transfer_complete_0 == 0) {

    }
    //R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);

    return err;
}

void SerialCompatibilityLayer::uart_init(uint32_t *baud) {
    fsp_err_t err = R_UARTA_Open(&g_uart0_ctrl, &g_uart0_cfg);
    assert(err == FSP_SUCCESS);
    uarta_baud_setting_t SCI_baud_setting;
    uint32_t baud_rate = *baud;
    //bool enable_bitrate_modulation = true;
    uint32_t error_rate_x_1000 = 5000;
    e_uarta_clock_source clock_source = UARTA_CLOCK_SOURCE_MOSC;
    err = R_UARTA_BaudCalculate(baud_rate, error_rate_x_1000, clock_source, &SCI_baud_setting);
    assert(FSP_SUCCESS == err);
    err = R_UARTA_BaudSet(&g_uart0_ctrl, (void *) &SCI_baud_setting);
}

void SerialCompatibilityLayer::uart_close(void) {
    fsp_err_t err = R_UARTA_Close (&g_uart0_ctrl);
    assert(err == FSP_SUCCESS);
}

void SerialCompatibilityLayer::print(uint8_t *p_msg) {
    if (p_msg == NULL) {
        this->print((long long)0);
    }
    uint16_t len = 0;
    while (p_msg[len]) {
        len++;
    }
    this->uart_print(p_msg, len);
}

void SerialCompatibilityLayer::println(uint8_t *p_msg) {
    uint16_t len = 0;
    while (p_msg[len]) {
        len++;
    }
    this->uart_print(p_msg, len);
    this->uart_print((uint8_t*)"\n", 1);
}

void SerialCompatibilityLayer::print(long long p_msg) {
    if (p_msg == 0) {
        this->uart_print((uint8_t*)"0", 1);
        return;
    }

    if (p_msg < 0) {
        this->uart_print((uint8_t*)"-", 1);
        p_msg *= -1;
    }

    uint16_t len = 0;
    uint8_t nr[20];
    while (p_msg) {
        nr[20 - len - 1] = (uint8_t)(p_msg % 10) + '0';
        len++;
        p_msg /= 10;
    }
    this->uart_print(nr + (20 - len), len);
}

void SerialCompatibilityLayer::println(long long p_msg) {
    this->print(p_msg);
    this->uart_print((uint8_t*)"\n", 1);
}

SerialCompatibilityLayer Serial;
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

static volatile uint8_t g_uart_event_1 = (0x00);

uint8_t  g_dest_1[TRANSFER_LENGTH];
uint8_t  g_out_of_band_received_1[TRANSFER_LENGTH];
static volatile uint32_t g_transfer_complete_1 = 0;
static volatile uint32_t g_receive_complete_1  = 0;
static volatile uint16_t g_out_of_band_index_1 = 0;

void uart_callback_1 (uart_callback_args_t * p_args)
{
    g_uart_event_1 = (uint8_t)p_args->event;
    switch (p_args->event)
    {
        case UART_EVENT_RX_CHAR:
        {
            if (sizeof(g_out_of_band_received_1) > g_out_of_band_index_1)
            {
                if (UART_DATA_BITS_8 >= g_uart0_cfg.data_bits)
                {
                    g_out_of_band_received_1[g_out_of_band_index_1++] = (uint8_t) p_args->data;
                }
                else
                {
                    uint16_t * p_dest = (uint16_t *) &g_out_of_band_received_1[g_out_of_band_index_1];
                    *p_dest              = (uint16_t) p_args->data;
                    g_out_of_band_index_1 += 2;
                }
            }
            break;
        }
        /* Receive complete */
        case UART_EVENT_RX_COMPLETE:
        {
            g_receive_complete_1 = 1;
            break;
        }
        /* Transmit complete */
        case UART_EVENT_TX_COMPLETE:
        {
            g_transfer_complete_1 = 1;
            break;
        }
        default:
        {
        }
    }
}

void SerialCompatibilityLayer_1::begin(uint32_t baud) {
    this->uart_init(&baud);
}

void SerialCompatibilityLayer_1::write(uint8_t byte) {
    this->uart_print(&byte, 1);
}

SerialCompatibilityLayer_1::SerialCompatibilityLayer_1() {
    memset(this->buff, 0, sizeof(this->buff));
    this->idx = 0;
    this->fin = 0;
    this->cnt = 0;
}

uint16_t SerialCompatibilityLayer_1::available() {
    this->fill_buff();
    return this->cnt;
}

uint8_t SerialCompatibilityLayer_1::read() {
    this->fill_buff();
    uint8_t ret = this->buff[this->idx++];
    this->cnt--;
    if (this->idx == MAX_BUFF_SIZE) {
        this->idx = 0;
    }
    if (this->cnt < 0) {
        assert(false);
    }
    return ret;
}

void SerialCompatibilityLayer_1::fill_buff() {
    uint16_t len = 0;
    fsp_err_t err = this->uart_read(this->aux, &len);
    if (err == FSP_SUCCESS) {
        this->cnt += (int16_t)len;
        for (int i = 0; i < len; i++) {
            this->buff[this->fin++] = this->aux[i];
            if (this->fin == MAX_BUFF_SIZE) {
                this->fin = 0;
            }
            if (this->fin == this->idx) {
                assert(false);
            }
        }
    }
}

uint8_t SerialCompatibilityLayer_1::peek() {
    this->fill_buff();
    return this->buff[this->idx];
}

fsp_err_t SerialCompatibilityLayer_1::uart_read(uint8_t *rx, uint16_t *len) {
    fsp_err_t err = FSP_SUCCESS;

    if (g_out_of_band_index_1) {
        g_receive_complete_1 = false;
        if (rx != NULL) {
            memcpy(rx, g_out_of_band_received_1, g_out_of_band_index_1);
        }
        *len = g_out_of_band_index_1;
        g_out_of_band_index_1 = 0;
        return FSP_SUCCESS;
    }
    else {
        return FSP_ERR_IP_CHANNEL_NOT_PRESENT;
    }
    return err;
}

fsp_err_t SerialCompatibilityLayer_1::uart_print(uint8_t *p_msg, uint16_t msg_len) {
    fsp_err_t err = FSP_SUCCESS;

    /* Reset callback capture variable */
    g_uart_event_1 = (0x00);
    g_transfer_complete_1 = 0;

    /* Writing to terminal */
    err = R_UARTA_Write(&g_uart0_ctrl, p_msg, msg_len);
    assert(err == FSP_SUCCESS);

    while (g_transfer_complete_1 == 0) {

    }
    //R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);

    return err;
}

void SerialCompatibilityLayer_1::uart_init(uint32_t *baud) {
    fsp_err_t err = R_UARTA_Open(&g_uart0_ctrl, &g_uart0_cfg);
    assert(err == FSP_SUCCESS);
    uarta_baud_setting_t SCI_baud_setting;
    uint32_t baud_rate = *baud;
    //bool enable_bitrate_modulation = true;
    uint32_t error_rate_x_1000 = 5000;
    e_uarta_clock_source clock_source = UARTA_CLOCK_SOURCE_MOSC;
    err = R_UARTA_BaudCalculate(baud_rate, error_rate_x_1000, clock_source, &SCI_baud_setting);
    assert(FSP_SUCCESS == err);
    err = R_UARTA_BaudSet(&g_uart0_ctrl, (void *) &SCI_baud_setting);
}

void SerialCompatibilityLayer_1::uart_close(void) {
    fsp_err_t err = R_UARTA_Close (&g_uart0_ctrl);
    assert(err == FSP_SUCCESS);
}

void SerialCompatibilityLayer_1::print(uint8_t *p_msg) {
    uint16_t len = 0;
    while (p_msg[len]) {
        len++;
    }
    this->uart_print(p_msg, len);
}

void SerialCompatibilityLayer_1::println(uint8_t *p_msg) {
    uint16_t len = 0;
    while (p_msg[len]) {
        len++;
    }
    this->uart_print(p_msg, len);
    this->uart_print((uint8_t*)"\n", 1);
}

void SerialCompatibilityLayer_1::print(long long p_msg) {
    if (p_msg == 0) {
        this->uart_print((uint8_t*)"0", 1);
    }

    if (p_msg < 0) {
        this->uart_print((uint8_t*)"-", 1);
        p_msg *= -1;
    }

    uint16_t len = 0;
    uint8_t nr[20];
    while (p_msg) {
        nr[20 - len - 1] = (uint8_t)(p_msg % 10) + '0';
        len++;
        p_msg /= 10;
    }
    this->uart_print(nr + (20 - len), len);
}

void SerialCompatibilityLayer_1::println(long long p_msg) {
    this->print(p_msg);
    this->uart_print((uint8_t*)"\n", 1);
}

SerialCompatibilityLayer_1 Serial1;
#endif
