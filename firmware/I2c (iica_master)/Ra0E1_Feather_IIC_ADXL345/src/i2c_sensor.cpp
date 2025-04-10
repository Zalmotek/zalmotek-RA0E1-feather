/***********************************************************************************************************************
 * File Name    : i2c_sensor.c
 * Description  : Contains data structures and functions used in i2c_sensor.c.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

#include "i2c_sensor.h"


/*******************************************************************************************************************//**
 * @addtogroup r_IICA_master_ep
 * @{
 **********************************************************************************************************************/

/*
 * Global Variables
 */
/* Reading I2C call back event through i2c_Master callback */
static volatile i2c_master_event_t i2c_event = I2C_MASTER_EVENT_ABORTED;


/*
 * private function declarations
 */
static fsp_err_t get_device_id(uint8_t *dev_id);
static fsp_err_t validate_i2c_event(void);

/*******************************************************************************************************************//**
 *  @brief       initialize IICA master module and set up PMOD ACL sensor
 *  @param[IN]   None
 *  @retval      FSP_SUCCESS                  Upon successful open and start of timer
 *  @retval      Any Other Error code apart from FSP_SUCCESS is  Unsuccessful open or start
 **********************************************************************************************************************/
void init_sensor(void)
{
    uint8_t device_id = 0;
    fsp_err_t err     = FSP_SUCCESS;
    uint8_t measure_enable_payload[MEASURE_PAYLOAD_SIZE] = { POWER_CTL_REG, ENABLE_BIT };

    /* opening IICA master module */
    //err = R_IICA_MASTER_Close(&g_i2c_master_ctrl);
    err = R_IICA_MASTER_Open(&g_i2c_master0_ctrl, &g_i2c_master0_cfg);
    /* handle error */
    if (FSP_SUCCESS != err)
    {
        Serial1.print((uint8_t *)"** R_IICA_MASTER_Open API failed ** \r\n");
        i2cok = false;
        //APP_ERR_TRAP(err);
    }

    /* Get device ID as a start of communication */
    err =  get_device_id(&device_id);

    /* Verify I2C transaction is successful with PMOD ACL and correct device ID is received */
    if ( (FSP_SUCCESS == err) && (DEVICE_SIGNATURE == device_id) )
    {
        /* Access to POWER_CTL Register (0x2D) and enable measurement  */
        err = R_IICA_MASTER_Write(&g_i2c_master0_ctrl, measure_enable_payload, TWO_BYTE, false);
        if (FSP_SUCCESS == err)
        {
            validate_i2c_event();
            /* handle error */
            if(FSP_ERR_TRANSFER_ABORTED == err)
            {
                Serial1.print((uint8_t *)"** POWER_CTL reg I2C write failed!!! ** \r\n");
                i2cok = false;
                //APP_ERR_TRAP(err);
            }
        }
        /* handle error */
        else
        {
            /* Write API returns itself is not successful */
            Serial1.print((uint8_t *)"** R_IICA_MASTER_Write API failed ** \r\n");
            i2cok = false;
            //APP_ERR_TRAP(err);
        }
    }
    /* handle error */
    else
    {
        /* Failed to get Device ID */
        Serial1.print((uint8_t *)"Error in reading Device ID ** \r\n");
        i2cok = false;
        //APP_ERR_TRAP(err);
    }
}

/*******************************************************************************************************************//**
 *  @brief       Read The ACL Sensor data for X,Y and Z axis. And prints the raw data on the RTT console
 *  @param[IN]   Array pointer to get the sensor data
 *  @retval      FSP_SUCCESS                  Upon successful open and start of timer
 *  @retval      Any Other Error code apart from FSP_SUCCES  Unsuccessful open or start
 **********************************************************************************************************************/
void read_sensor_data(uint8_t *xyz_data)
{
    fsp_err_t err           = FSP_SUCCESS;
    uint8_t adxl_data[7]    = { 0 };
    uint8_t internal_reg    = AXIS_DATA;

    i2cok = true;

    if(NULL == xyz_data)
    {
        err = FSP_ERR_INVALID_POINTER;
        Serial1.print((uint8_t *)"** NULL Pointer check fail ** \r\n");
        i2cok = false;
        //APP_ERR_TRAP(err);
        return;
    }
    /* Write Accelerometer Data register   */
    err = R_IICA_MASTER_Write(&g_i2c_master0_ctrl, &internal_reg, ONE_BYTE, true);
    /* handle error */
    if (FSP_SUCCESS != err)
    {
        Serial1.print((uint8_t *)"** R_IICA_MASTER_Write API failed ** \r\n");
        i2cok = false;
        //APP_ERR_TRAP(err);
        return;
    }
    else
    {
        validate_i2c_event();

        /* Performs read operation only when write event is successful, failure part is handled at next stage*/
        if (FSP_SUCCESS == err)
        {
            err = R_IICA_MASTER_Read(&g_i2c_master0_ctrl, &adxl_data[0], DATA_REGISTERS, false);
            /* handle error */
            if (FSP_SUCCESS != err)
            {
                Serial1.print((uint8_t*)"** R_IICA_MASTER_Read API failed ** \r\n");
                i2cok = false;
                //APP_ERR_TRAP(err);
                return;
            }
            else
            {
                validate_i2c_event();
                if (FSP_SUCCESS != err)
                {
                    Serial1.print((uint8_t*)"** Data register, I2C read failed ** \r\n");
                    i2cok = false;
                    //APP_ERR_TRAP(err);
                    return;
                }
            }
        }
        else
        {
            Serial1.print((uint8_t*)"** Accel Data register, I2C write failed ** \r\n");
            i2cok = false;
            //APP_ERR_TRAP(err);
            return;
        }

        /*  Read is successful update the x,y,z values and return success */
        for (uint8_t itr = 0; itr < SENSOR_DATA_SIZE; itr++)
        {
            *(xyz_data+itr) = adxl_data[itr];
        }
    }
}

/*******************************************************************************************************************//**
 *  @brief     Read Device signature/ID from sensor
 *
 *  @param[in] dev_id                       retrieve PMOD ACL device ID
 *
 *  @retval    FSP_SUCCESS                  on successful I2C transaction
 *             FSP_ERR_INVALID_POINTER      if invalid parameters passed
 *             err                          whichever occurs on either API or on I2C event failure
 **********************************************************************************************************************/
static fsp_err_t get_device_id(uint8_t *dev_id)
{
    fsp_err_t err         = FSP_SUCCESS;
    uint8_t reg_address   = DEVICE_ID_REG;

    /* Parameter checking */
    if (NULL == dev_id)
    {
        err = FSP_ERR_INVALID_POINTER;
        Serial1.print((uint8_t*)"** NULL Pointer check fail ** \r\n");
        //APP_ERR_TRAP(err);
        i2cok = false;
        return err;
    }

    /* Send register address to sensor */
    err = R_IICA_MASTER_Write(&g_i2c_master0_ctrl, &reg_address, ONE_BYTE, true);
    /* handle error */
    if (FSP_SUCCESS != err)
    {
        Serial1.print((uint8_t*)"** R_IICA_MASTER_Write API failed ** \r\n");
        i2cok = false;
        //APP_ERR_TRAP(err);
        return err;
    }
    else
    {
        validate_i2c_event();
        /* handle error */
        if(FSP_ERR_TRANSFER_ABORTED == err)
        {
            Serial1.print((uint8_t*)"** Device ID reg, I2C write failed ** \r\n");
            //APP_ERR_TRAP(err);
            i2cok = false;
            return err;
        }
        else
        {
            /* Read only when I2C write and its callback event is successful */
            err  = R_IICA_MASTER_Read(&g_i2c_master0_ctrl, dev_id, ONE_BYTE, false);
            /* handle error */
            if (FSP_SUCCESS != err)
            {
                Serial1.print((uint8_t*)"** R_IICA_MASTER_Read API failed ** \r\n");
                //APP_ERR_TRAP(err);
                i2cok = false;
                return err;
                //  Do nothing, the error is returned in the end
            }
            else
            {
                validate_i2c_event();
                /* handle error */
                if(FSP_ERR_TRANSFER_ABORTED == err)
                {
                    Serial1.print((uint8_t*)"** Device ID read,  I2C read failed ** \r\n");
                    //APP_ERR_TRAP(err);
                    i2cok = false;
                    return err;
                }
            }
        }
    }
    /* On successful I2C transaction with PMOD-ACL return from here */
    return err;
}

/*******************************************************************************************************************//**
 *  @brief      User callback function
 *  @param[in]  p_args
 *  @retval None
 **********************************************************************************************************************/
void i2c_master_callback(i2c_master_callback_args_t *p_args)
{
    if (NULL != p_args)
    {
        /* capture callback event for validating the i2c transfer event*/
        i2c_event = p_args->event;
    }
}

/*******************************************************************************************************************//**
 *  @brief       Validate i2c receive/transmit  based on required write read operation
 *
 *  @param[in]   None
 *
 *  @retval      FSP_SUCCESS                       successful event receiving returns FSP_SUCCESS
 *               FSP_ERR_TRANSFER_ABORTED          Either on timeout elapsed or received callback event is
 *                                                 I2C_MASTER_EVENT_ABORTED
 **********************************************************************************************************************/
static fsp_err_t validate_i2c_event(void)
{
    uint16_t local_time_out = UINT16_MAX;

    /* resetting call back event capture variable */
    i2c_event = (i2c_master_event_t)0;

    do
    {
        /* This is to avoid infinite loop */
        --local_time_out;

        if(0 == local_time_out)
        {
            return FSP_ERR_TRANSFER_ABORTED;
        }

    }while(i2c_event == 0);

    if(i2c_event != I2C_MASTER_EVENT_ABORTED)
    {
        i2c_event = (i2c_master_event_t)0;  // Make sure this is always Reset before return
        return FSP_SUCCESS;
    }

    i2c_event = (i2c_master_event_t)0; // Make sure this is always Reset before return
    return FSP_ERR_TRANSFER_ABORTED;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup r_IICA_master_ep)
 **********************************************************************************************************************/
