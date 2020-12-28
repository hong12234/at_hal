/**
  ******************************************************************************
  * @file           : lv_hal_tick.h
  * @brief          : Provide access to the system tick with 1 millisecond resolution
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
#ifndef QL_HAL_TICK_H
#define QL_HAL_TICK_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ql_hal.h"

  void ql_tick_inc(unsigned int tick_period);
  unsigned int ql_tick_get(void);
  unsigned int ql_tick_elaps(unsigned int prev_tick);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_HAL_TICK_H*/

/************************ (C) COPYRIGHT HEFEI FAE *****END OF FILE****/
