/**
  ******************************************************************************
  * @file           : systick.c
  * @brief          : Provide access to the system tick with 1 millisecond
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 HEFEI FAE.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by HEFEI FAE under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ql_hal_tick.h"

/* Private variables ---------------------------------------------------------*/
static unsigned int sys_time = 0;
static volatile unsigned char tick_irq_flag;

/**
 * You have to call this function periodically
 * @param tick_period the call period of this function in milliseconds
 */
void ql_tick_inc(unsigned int tick_period)
{
    tick_irq_flag = 0;
    sys_time += tick_period;
}

/**
 * Get the elapsed milliseconds since start up
 * @return the elapsed milliseconds
 */
unsigned int ql_tick_get(void)
{
    /* If `lv_tick_inc` is called from an interrupt while `sys_time` is read
     * the result might be corrupted.
     * This loop detects if `lv_tick_inc` was called while reading `sys_time`.
     * If `tick_irq_flag` was cleared in `lv_tick_inc` try to read again
     * until `tick_irq_flag` remains `1`. */
    unsigned int result;
    do
    {
        tick_irq_flag = 1;
        result = sys_time;
    } while (!tick_irq_flag); /*Continue until see a non interrupted cycle */

    return result;
}

/**
 * Get the elapsed milliseconds since a previous time stamp
 * @param prev_tick a previous time stamp (return value of systick_get() )
 * @return the elapsed milliseconds since 'prev_tick'
 */
unsigned int ql_tick_elaps(unsigned int prev_tick)
{
    unsigned int act_time = ql_tick_get();

    /*If there is no overflow in sys_time simple subtract*/
    if (act_time >= prev_tick)
    {
        prev_tick = act_time - prev_tick;
    }
    else
    {
        prev_tick = UINT32_MAX - prev_tick + 1;
        prev_tick += act_time;
    }

    return prev_tick;
}

/************************ (C) COPYRIGHT HEFEI FAE *****END OF FILE****/
