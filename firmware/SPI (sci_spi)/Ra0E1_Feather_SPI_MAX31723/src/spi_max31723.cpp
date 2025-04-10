#include <spi_max31723.h>

static volatile spi_event_t g_master_event_flag;    // Master Transfer Event completion flag
/* Wait counter for wait operation monitoring */
static volatile int32_t g_wait_count = INT32_MAX;
/* Read register address of temperature sensor */
static uint8_t read_temperature_reg[3] = {0x01};
/* Variable to store read temperature values */
static uint8_t temperature_values[3] = {0};
uint8_t config_read[2] = {0x00};
static char dataBuff[30];

void max31723_init() {
    fsp_err_t err = FSP_SUCCESS;

    /* Value for configuration register write. Set resolution as 12-bits*/
    const uint8_t config_sensor[3] =
    {
     0x80, 0x06
    };

    /* Initialize SPI_B channel as master */
    err = R_SAU_SPI_Open(&g_spi0_ctrl, &g_spi0_cfg);

    /* Handle Error */
    if (FSP_SUCCESS != err)
    {
        Serial.print((uint8_t*)"\r\n SPI_B open failed");
        //APP_ERR_TRAP(err);
        spiok = false;
        return;
    }

    /* Assert Slave select pin to start data transfer */
    CS_ASSERT(CS_PIN);

    /* Configure temperature sensor */
    err = R_SAU_SPI_Write(&g_spi0_ctrl, &config_sensor[0], sizeof(config_sensor), SPI_BIT_WIDTH_8_BITS);

    /* Handle Error */
    if (FSP_SUCCESS != err)
    {
        sau_spi_deinit();
        Serial.print((uint8_t*)"\r\nSPI Write failed\r\n");
        //APP_ERR_TRAP(err);
        spiok = false;
        return;
    }

    /* Check for transfer complete event and handle error in the case of event failure */
    sau_spi_event_check();

    /* De-assert Slave select pin to stop data transfer */
    CS_DE_ASSERT(CS_PIN);

    /* Resetting SPI Master event flag */
    g_master_event_flag = (spi_event_t)0;

    /* Assert Slave select pin to start data transfer */
    CS_ASSERT(CS_PIN);

    /* Read configured temperature sensor */
    err = R_SAU_SPI_Read(&g_spi0_ctrl, &config_read[0], sizeof(config_read), SPI_BIT_WIDTH_8_BITS);

    /* Handle Error */
    if (FSP_SUCCESS != err)
    {
        sau_spi_deinit();
        Serial.print((uint8_t*)"\r\nSPI Write failed\r\n");
        //APP_ERR_TRAP(err);
        spiok = false;
        return;
    }

    /* Check for transfer complete event and handle error in the case of event failure */
    sau_spi_event_check();

    /* De-assert Slave select pin to stop data transfer */
    CS_DE_ASSERT(CS_PIN);

    /* Check if sensor is configured as expected */
    if (config_sensor[1] != config_read[1])
    {
        /* Incorrect configuration of temperature sensor */
        err = FSP_ERR_INVALID_HW_CONDITION;
        //APP_ERR_TRAP(err);
        spiok = false;
        return;
    }

    /* Adding conversion time as 200ms for the configured 12-bit resolution,
     *  before asserting CS_PIN for reading the temperature values in a loop */
    R_BSP_SoftwareDelay(CONVERSION_TIME, BSP_DELAY_UNITS_MILLISECONDS);

}

float get_temperature() {
    spiok = true;
    fsp_err_t err = FSP_SUCCESS;
    float temperature = 0;

    /* Resetting SPI Master event flag */
    g_master_event_flag = (spi_event_t)0;

    memset(dataBuff, 0, sizeof(dataBuff));

    /* Assert Slave select pin to start data transfer */
    CS_ASSERT(CS_PIN);
    /* Read the temperature */
    err = R_SAU_SPI_WriteRead(&g_spi0_ctrl, read_temperature_reg, temperature_values, sizeof(temperature_values), SPI_BIT_WIDTH_8_BITS);

    /* Handle Error */
    if (FSP_SUCCESS != err)
    {
        sau_spi_deinit();
        Serial.print((uint8_t*)"\r\nSPI ReadWrite failed\r\n");
        //APP_ERR_TRAP(err);
        spiok = false;
        return 0;
    }

    /* Check for transfer complete event and handle error in the case of event failure */
    sau_spi_event_check();

    /* De-assert Slave select pin to stop data transfer */
    CS_DE_ASSERT(CS_PIN);

    /* Manipulating the read temperature values to print for users */
    temperature = (float) ((int32_t)( ((uint32_t) temperature_values[2] << 4) | ((uint32_t) temperature_values[1] >> 4) )) / 16.0f;

    if ((int)temperature == 255) {
        spiok = false;
    }

    return temperature;
}

/*******************************************************************************************************************//**
 * @brief This function checks occurrence of data transfer complete event until timeout occurs and handles errors.
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
static void sau_spi_event_check(void)
{
    while(SPI_EVENT_TRANSFER_COMPLETE != g_master_event_flag)
    {
        //APP_PRINT("Wait count: %d\n", g_wait_count);
        g_wait_count--;
        if (0 >= g_wait_count)
        {
            sau_spi_deinit();
            Serial.print((uint8_t*)"\r\nSPI module blocked in Write/Read operation.\r\n");
            /* Return time out error if SPI operation fails to complete */
            //APP_ERR_TRAP(FSP_ERR_TIMEOUT);
            spiok = false;
        }
        /* All other error events are handled here */
        else if (SPI_EVENT_TRANSFER_ABORTED == g_master_event_flag)
        {
            sau_spi_deinit();
            Serial.print((uint8_t*)"\r\nSPI module transfer aborted.\r\n");
            //APP_ERR_TRAP(FSP_ERR_TRANSFER_ABORTED);
            spiok = false;
        }
        else
        {
            /* Do nothing */
        }
    }
}

/*******************************************************************************************************************//**
 * @brief This function closes opened SPI_B module before the project ends up in an Error Trap.
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
static void sau_spi_deinit(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Close SPI_B module */
    err = R_SAU_SPI_Close(&g_spi0_ctrl);

    /* handle error */
    if (FSP_SUCCESS != err)
    {
        /* SPI Close failure message */
        Serial.print((uint8_t*)"** R_SPI_B_Close API for master failed **  \r\n");
        spiok = false;
    }
}

/*******************************************************************************************************************//**
 * @brief Master SPI_B callback function.
 * @param[in]  p_args
 * @retval     None
 **********************************************************************************************************************/
void spi_callback(spi_callback_args_t *p_args)
{
    if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event)
    {
        g_master_event_flag = SPI_EVENT_TRANSFER_COMPLETE;
    }
    else
    {
        /* Updating the flag here to capture and handle all other error events */
        g_master_event_flag = SPI_EVENT_TRANSFER_ABORTED;
    }
}

