#ifndef _SPI_MAX31723_H_
#define _SPI_MAX31723_H_

#include "hal_data.h"
#include "SerialCompatibility.h"

/* Slave Select pin for all boards to enable data transfer */
#define CS_PIN  (BSP_IO_PORT_04_PIN_07)

/* Function macros to assert and de-assert Slave Select pins */
#define CS_ASSERT(x)     (R_IOPORT_PinWrite(&g_ioport_ctrl, (x), BSP_IO_LEVEL_HIGH))
#define CS_DE_ASSERT(x)  (R_IOPORT_PinWrite(&g_ioport_ctrl, (x), BSP_IO_LEVEL_LOW))

#define CONVERSION_TIME     (200U)  //Conversion time for 12-bit resolution
#define PRINT_DELAY         (2U)    //Delay for RTT viewer prints

/* Function to check occurrence of event after data transfer */
static void sau_spi_event_check(void);
/* Cleanup function for opened module */
static void sau_spi_deinit(void);

void max31723_init();
float get_temperature();

void spi_callback(spi_callback_args_t *p_args);

static bool spiok = true;


#endif //_SPI_MAX31723_H_
