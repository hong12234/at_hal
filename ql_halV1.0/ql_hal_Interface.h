/**
  ******************************************************************************
  * @file           : ql_hal_Interface.h
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

#ifndef QL_HAL_INTERFACE_H
#define QL_HAL_INTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ql_hal.h"

/* Private defines -----------------------------------------------------------*/
#define xassert(x) ((void)0)

#define ARRAY_LENGTH(a) (sizeof a / sizeof(a[0]))

#define RES_OK "OK"
#define RES_ERROR "ERROR"
#define RES_FAIL "FAIL"

#ifndef MAX_DIVERTER_SIZE
#define MAX_DIVERTER_SIZE 512 /*Buffer size for invalid areas */
#endif

#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 512 /*Buffer size for invalid areas */
#endif

#ifndef URC_NUMBER_MATCHES
#define URC_NUMBER_MATCHES 4 /*URC对照表最大长度 */
#endif

#ifndef SEND_BUF_LEN
#define SEND_BUF_LEN 50 /*串口发送命令的最大长度 */
#endif

	/* Exported types ------------------------------------------------------------*/
	typedef enum /* Results of Disk Functions */
	{
		ENUM_RES_OK = 0,	 /* 0: 成功 */
		ENUM_RES_ERROR,		 /* 1: 参数错误 */
		ENUM_RES_MEMORY_BUSY /* 2：内存忙 */
	} FA_RESULT_CODE;

	typedef enum /* Results of Disk Functions */
	{
		ENUM_AT_RES_OK = 0,			 /* 0: at返回成功 */
		ENUM_AT_RES_PARAMETER_ERROR, /* 1: 错误返回 */
		ENUM_AT_RES_MEMORY_BUSY		 /* 2：超时返回 */
	} AT_RESULT_CODE;

	typedef struct ST_URC_FIX_TABLE_REQ /*urc 匹配表*/
	{
		const char *prefix; /* 匹配字符串开头 */
		const char *suffix; /* 匹配字符串结尾 */
	} URC_FIX_TABLE_REQ, *PURC_FIX_TABLE_REQ;

	typedef void (*_urc_recevicedata_cb)(const char *data, unsigned short size, unsigned char type);
	typedef signed int (*uart_write)(unsigned char *data, unsigned short length);
	typedef struct ST_UART_INFO_CTX /*uart 信息体*/
	{
		uart_write write;				   /* 串口发送 */
		_urc_recevicedata_cb urc_callback; /* urc数据回调 */
		PURC_FIX_TABLE_REQ urc_table;	   /* urc匹配表 */
		unsigned char urc_num;			   /* urc数量 */
		char at_res_data[5][50];		   /* at响应字符串 */
		unsigned char at_res_num;		   /* at响应字符串数量 */
		AT_RESULT_CODE at_res_status;	   /*at指令返回状态 0成功返回 1错误返回 2超时返回*/
		void *handle;					   /* 预留 */
	} UART_INFO_CTX, *PUART_INFO_CTX;

	/* Exported functions prototypes ---------------------------------------------*/
	FA_RESULT_CODE send_at_command(PUART_INFO_CTX dev, char *command, unsigned int timeout, unsigned char line_num);
	unsigned char recv_at_packet(unsigned char *buf, unsigned short length);
	FA_RESULT_CODE rec_data_parser(PUART_INFO_CTX dev, unsigned char line_num);
	void usart_drv_init(void);
	int split_string_by_comma(char *string, char **values, int max_values);
	char *split_string_by_word(PUART_INFO_CTX dev, const char *word);
	int judge_value(const char *value);
	int stringtoint(char *s);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
