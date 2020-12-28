/**
  ******************************************************************************
  * @file           : at.h
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

#ifndef QL_HAL_GEBERAL_AT_H
#define QL_HAL_GEBERAL_AT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ql_hal.h"

#define AT "AT"
#define AT_CSQ "AT+CSQ"

  typedef enum
  {
    QL_OK = 0, /* 0: Successful */
    QL_ERROR,  /* 1: Error code */
    QL_TIMEOUT,
  } QL_DRESULT;

  /*at函数执行成功失败*/
  typedef enum
  {
    AT_RES_OK = 0, /* 0: Successful */
    AT_RES_ERROR,  /* 1: Error code */
  } AT_DRESULT;

  /* Exported functions prototypes ---------------------------------------------*/
  typedef struct
  {
    QL_DRESULT status; /*回复状态 */
  } standard_at_rep_t;
  AT_DRESULT standard_at_cmd_at_get(PUART_INFO_CTX dev, standard_at_rep_t *val, unsigned int timeout);

  typedef struct
  {
    int rssi;          /* 接收信号强度等级 */
    int ber;           /* 信道误码率（百分比） */
    QL_DRESULT status; /*回复状态 */

  } standard_csq_rep_t;

  AT_DRESULT standard_at_cmd_csq_get(PUART_INFO_CTX dev, standard_csq_rep_t *val, unsigned int timeout);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

/************************ (C) COPYRIGHT HEFEI FAE *****END OF FILE****/
