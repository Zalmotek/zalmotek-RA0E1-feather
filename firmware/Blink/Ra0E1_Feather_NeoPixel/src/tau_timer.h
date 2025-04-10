/***********************************************************************************************************************
 * File Name    : tau_timer.c
 * Description  : Contains function definition.
 **********************************************************************************************************************/

#ifndef TAU_TIMER_H_
#define TAU_TIMER_H_

#include "common_utils.h"

/* Macros definitions */
#define BUF_SIZE                 (16U)           /* Size of buffer for RTT input data */
#define INITIAL_VALUE            ('\0')
#define TIMER_UNITS_MICROSECONDS (1U)            /* Timer unit in microsecond */
#define CLOCK_TYPE_SPECIFIER      (1ULL)         /* Type specifier */
#define TAU_MAX_PERIOD_COUNT     (0xFFFF)        /* Max Period Count for 16-bit Timer */

/* Function declaration */
fsp_err_t init_tau_timer(timer_ctrl_t * const p_timer_ctl, timer_cfg_t const * const p_timer_cfg);
fsp_err_t start_tau_timer (timer_ctrl_t * const p_timer_ctl);
void deinit_tau_timer(timer_ctrl_t * const p_timer_ctl);

volatile uint32_t tick = 0;

uint32_t micros();
uint32_t micros() {
    timer_status_t status;
    (void) R_TAU_StatusGet(&g_timer0_ctrl, &status);
    tick = status.counter;
    return (uint32_t)(tick);
}

void beginTimer();
void beginTimer() {
    fsp_err_t err = FSP_SUCCESS;

    /* Initialize TAU Timer */
    err = init_tau_timer(&g_timer0_ctrl, &g_timer0_cfg);
    if(FSP_SUCCESS != err) {
        APP_ERR_PRINT("** TAU TIMER INIT FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }
    /* Start TAU Timer */
    err = start_tau_timer(&g_timer0_ctrl);
    if(FSP_SUCCESS != err) {
        APP_ERR_PRINT("** TAU TIMER START FAILED ** \r\n");
        /* Close TAU Timer instance */
        deinit_tau_timer(&g_timer0_ctrl);
        APP_ERR_TRAP(err);
    }
}

/*****************************************************************************************************************
 * @brief       Initialize TAU timer.
 * @param[in]   p_timer_ctl     Timer instance control structure
 * @param[in]   p_timer_cfg     Timer instance Configuration structure
 * @retval      FSP_SUCCESS     Upon successful open of timer.
 * @retval      Any Other Error code apart from FSP_SUCCESS on Unsuccessful open.
 ****************************************************************************************************************/
fsp_err_t init_tau_timer(timer_ctrl_t * const p_timer_ctl, timer_cfg_t const * const p_timer_cfg)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Initialize TAU Timer */
    err = R_TAU_Open(p_timer_ctl, p_timer_cfg);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT ("\r\n ** R_TAU_Open FAILED ** \r\n");
        return err;
    }
    return err;
}

/*****************************************************************************************************************
 * @brief       Start TAU timers in periodic, one-shot, PWM mode.
 * @param[in]   p_timer_ctl     Timer instance control structure
 * @retval      FSP_SUCCESS     Upon successful start of timer.
 * @retval      Any Other Error code apart from FSP_SUCCESS on Unsuccessful start.
 ****************************************************************************************************************/
fsp_err_t start_tau_timer (timer_ctrl_t * const p_timer_ctl)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Starts TAU timer */
    err = R_TAU_Start(p_timer_ctl);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT ("\r\n ** R_TAU_Start API failed ** \r\n");
    }
    return err;
}

/*****************************************************************************************************************
 * @brief      Close the TAU HAL driver.
 * @param[in]  p_timer_ctl     Timer instance control structure
 * @retval     None
 ****************************************************************************************************************/
void deinit_tau_timer(timer_ctrl_t * const p_timer_ctl)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Timer Close API call*/
    err = R_TAU_Close(p_timer_ctl);
    if (FSP_SUCCESS != err) {
        APP_ERR_PRINT ("\r\n ** R_TAU_Close FAILED ** \r\n");
    }
}

#endif /* TAU_TIMER_H_ */
