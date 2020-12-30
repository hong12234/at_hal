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

  typedef enum
  {
    QL_OK = 0,  /* 0: Successful */
    QL_ERROR,   /* 1: Error code */
    QL_TIMEOUT, /* 2: TimeOut 	 */
  } QL_DRESULT;

  /*at函数执行成功失败*/
  typedef enum
  {
    AT_RES_OK = 0, /* 0: Successful */
    AT_RES_ERROR,  /* 1: Error code */
  } AT_DRESULT;

/* Exported functions prototypes */
#define AT "AT"
  typedef struct
  {
    QL_DRESULT status; /* 回复状态 */
  } standard_at_rep_t;
  AT_DRESULT standard_at_cmd_at_get(PUART_INFO_CTX dev, standard_at_rep_t *val, unsigned int timeout);

#define CSQ "AT+CSQ"
  typedef struct
  {
    int rssi;          /* 接收信号强度等级 */
    int ber;           /* 信道误码率(百分比) */
    QL_DRESULT status; /* 回复状态 */
  } standard_csq_rep_t;
  AT_DRESULT standard_at_cmd_csq_get(PUART_INFO_CTX dev, standard_csq_rep_t *val, unsigned int timeout);

#define ATE "ATE0"
  typedef struct
  {
    QL_DRESULT status; /* 回复状态 */
  } standard_ate_rep_t;
  AT_DRESULT standard_at_cmd_ate_get(PUART_INFO_CTX dev, standard_ate_rep_t *val, unsigned int timeout);

#define ATI "ATI"
  typedef struct
  {
    char *Firmware;    /* 固件版本 */
    QL_DRESULT status; /* 回复状态 */
  } standard_ati_rep_t;
  AT_DRESULT standard_at_cmd_ati_get(PUART_INFO_CTX dev, standard_ati_rep_t *val, unsigned int timeout);

#define CESQ "AT+CESQ"
  typedef struct
  {
    int rxlev;         /* 接收信号强度等级 */
    int ber;           /* 信道误码率(百分比) */
    int rscp;          /* 接收信号码功率 */
    int ecno;          /* Ec/No */
    int rsrq;          /* 信号接收质量 */
    int rsrp;          /* 信号接收功率 */
    QL_DRESULT status; /* 回复状态 */
  } standard_cesq_rep_t;
  AT_DRESULT standard_at_cmd_cesq_get(PUART_INFO_CTX dev, standard_cesq_rep_t *val, unsigned int timeout);

#define QSCLK "AT+QSCLK=0"
  typedef struct
  {
    QL_DRESULT status; /* 回复状态 */
  } standard_qsclk_rep_t;
  AT_DRESULT standard_at_cmd_qsclk_get(PUART_INFO_CTX dev, standard_qsclk_rep_t *val, unsigned int timeout);

#define QLEDMODE "AT+QLEDMODE=1"
  typedef struct
  {
    QL_DRESULT status; /* 回复状态 */
  } standard_qledmode_rep_t;
  AT_DRESULT standard_at_cmd_qledmode_get(PUART_INFO_CTX dev, standard_qledmode_rep_t *val, unsigned int timeout);

#define CPIN "AT+CPIN?"
  typedef struct
  {
    char *SIMStatus;   /* SIM卡状态 */
    QL_DRESULT status; /* 回复状态 */
  } standard_cpin_rep_t;
  AT_DRESULT standard_at_cmd_cpin_get(PUART_INFO_CTX dev, standard_cpin_rep_t *val, unsigned int timeout);

#define CGSN "AT+CGSN=1"
  typedef struct
  {
    char *IMSIMark;    /* IMEI号 */
    QL_DRESULT status; /* 回复状态 */
  } standard_cgsn_rep_t;
  AT_DRESULT standard_at_cmd_cgsn_get(PUART_INFO_CTX dev, standard_cgsn_rep_t *val, unsigned int timeout);

#define CIMI "AT+CIMI"
  typedef struct
  {
    char *CIMIMark;    /* CIMI号 */
    QL_DRESULT status; /* 回复状态 */
  } standard_cimi_rep_t;
  AT_DRESULT standard_at_cmd_cimi_get(PUART_INFO_CTX dev, standard_cimi_rep_t *val, unsigned int timeout);

#define QCCID "AT+QCCID"
  typedef struct
  {
    char *QCCIDMark;   /* QCCID号 */
    QL_DRESULT status; /* 回复状态 */
  } standard_qccid_rep_t;
  AT_DRESULT standard_at_cmd_qccid_get(PUART_INFO_CTX dev, standard_qccid_rep_t *val, unsigned int timeout);

#define CEREG "AT+CEREG?"
  typedef struct
  {
    char URCStatus;     /* URC状态 */
    char NetworkStatus; /* 网络状态 */
    QL_DRESULT status;  /* 回复状态 */
  } standard_cereg_rep_t;
  AT_DRESULT standard_at_cmd_cereg_get(PUART_INFO_CTX dev, standard_cereg_rep_t *val, unsigned int timeout);

#define CGPADDR "AT+CGPADDR=1"
  typedef struct
  {
    char *IPAddr;      /* 模块IP地址 */
    QL_DRESULT status; /* 回复状态 */
  } standard_cgpaddr_rep_t;
  AT_DRESULT standard_at_cmd_cgpaddr_get(PUART_INFO_CTX dev, standard_cgpaddr_rep_t *val, unsigned int timeout);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

/************************ (C) COPYRIGHT HEFEI FAE *****END OF FILE****/
