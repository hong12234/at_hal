
/**
  ******************************************************************************
  * @file           : lv_hal.h
  * @brief          : 
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

#ifndef LV_HAL_H
#define LV_HAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
//#include "main.h"
#include "ql_hal_interface.h"
#include "ql_hal_tick.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

  /* Private defines -----------------------------------------------------------*/
#define MAX_DIVERTER_SIZE 512 /* Buffer size for invalid areas */

#define MAX_BUFFER_SIZE 512

#define URC_NUMBER_MATCHES 4 /*URC对照表最大长度 */

#define PARAM_NUM_MAX 12 /*AT指令解析最大长度 */

#define SEND_BUF_LEN 50 /*串口发送命令的最大长度 */

#define AT_RECEIVE_LEN 5 /*at响应字符串最大长度 */

#define URC_CALLBACK_ENABLE 1 /* URC指令回调数据打印 */
#define AT_CALLBACK_ENABLE 0  /* AT指令回调数据打印 */

#define INTERNAL_DEBUG

#ifdef INTERNAL_DEBUG
#define internal_log(fmt, ...) printf("[%s(%04d)]" fmt, __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define internal_log(fmt, ...)
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
