
/**
  ******************************************************************************
  * @file           : at.c
  * @brief          : General instructions
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
#include "at.h"

/**
  * @defgroup  standard_at_cmd_at_get
  * @brief  AT指令测试
  * @param  dev			命令句柄
  * @param  val			返回信息机构体
  * @param  timeout		等待时间(单位:mS)
  * @retval	发送状态(0:Successful  1: Error code)
  *
  */
AT_DRESULT standard_at_cmd_at_get(PUART_INFO_CTX dev, standard_at_rep_t *val, unsigned int timeout)
{
	if (timeout == 0)
	{
		return AT_RES_ERROR; /* 错误 */
	}
	internal_log("SendData: %s\r\n", AT);
	if (send_at_command(dev, AT, timeout, 2) != ENUM_RES_OK) /* 发送指令并判断返回 */
	{
		val->status = QL_TIMEOUT; /* 超时 */
	}
	else /* 有数据返回 */
	{
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR) /* 返回错误 */
		{
			val->status = QL_ERROR; /* 错误 */
		}
		else if (dev->at_res_status == ENUM_AT_RES_OK) /* 返回正确可以解析数据 */
		{
			val->status = QL_OK; /* 成功 */
		}
	}
	internal_log("SendState: %d  [0:OK  1:ERROR  2:TIMEOUT]\r\n\r\n", val->status);
	return AT_RES_OK;
}

/**
  * @defgroup  standard_at_cmd_csq_get
  * @brief  信号强度,误码率查询
  * @param  dev			命令句柄
  * @param  val			返回信息机构体
  * @param  timeout		等待时间(单位:mS)
  * @retval	发送状态(0:Successful  1: Error code)
  *
  */
AT_DRESULT standard_at_cmd_csq_get(PUART_INFO_CTX dev, standard_csq_rep_t *val, unsigned int timeout)
{
	unsigned int at_n_vals, at_val_index;
	char *atvalues[20]; /* 比对数据 */
	char *atvalue = NULL;
	char *tmp_str = NULL;

	if (timeout == 0)
	{
		return AT_RES_ERROR; /* 错误 */
	}
	internal_log("SendData: %s\r\n", CSQ);
	if (send_at_command(dev, CSQ, timeout, 3) != ENUM_RES_OK) /* 发送指令并判断返回 */
	{
		val->status = QL_TIMEOUT; /* 超时 */
	}
	else /* 有数据返回 */
	{
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR) /* 返回错误 */
		{
			val->status = QL_ERROR; /* 错误 */
		}
		else if (dev->at_res_status == ENUM_AT_RES_OK) /* 返回正确可以解析数据 */
		{
			val->status = QL_OK;														  /* 成功 */
			tmp_str = split_string_by_word(dev, "+CSQ:");								  /* 数据比对,返回数据存在指针 */
			at_n_vals = split_string_by_comma(tmp_str, atvalues, ARRAY_LENGTH(atvalues)); /* 以':或'','为间隔断帧 */
			if (at_n_vals == 0)															  /* 断帧后没有数据 */
			{
				return AT_RES_ERROR; /* 错误 */
			}
			for (at_val_index = 0; at_val_index < at_n_vals; at_val_index++) /* 循环断帧后数据数 */
			{
				atvalue = atvalues[at_val_index]; /* 数据暂存 */
				if (-1 == judge_value(atvalue))	  /* 判断数据是否为空 */
				{
					continue;
				}
			}
			val->rssi = stringtoint(atvalues[1]); /* 读取信息 */
			internal_log("val->rssi: %d\r\n", val->rssi);
			val->ber = stringtoint(atvalues[2]); /* 读取信息 */
			internal_log("val->ber: %d\r\n", val->ber);
		}
	}
	internal_log("SendState: %d  [0:OK  1:ERROR  2:TIMEOUT]\r\n\r\n", val->status);
	return AT_RES_OK;
}

/**
  * @defgroup  standard_at_cmd_ate_get
  * @brief  关闭回显
  * @param  dev			命令句柄
  * @param  val			返回信息机构体
  * @param  timeout		等待时间(单位:mS)
  * @retval	发送状态(0:Successful  1: Error code)
  *
  */
AT_DRESULT standard_at_cmd_ate_get(PUART_INFO_CTX dev, standard_ate_rep_t *val, unsigned int timeout)
{
	if (timeout == 0)
	{
		return AT_RES_ERROR; /* 错误 */
	}
	internal_log("SendData: %s\r\n", ATE);
	if (send_at_command(dev, ATE, timeout, 2) != ENUM_RES_OK) /* 发送指令并判断返回 */
	{

		val->status = QL_TIMEOUT; /* 超时 */
	}
	else /* 有数据返回 */
	{
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR) /* 返回错误 */
		{
			val->status = QL_ERROR; /* 错误 */
		}
		else if (dev->at_res_status == ENUM_AT_RES_OK) /* 返回正确可以解析数据 */
		{
			val->status = QL_OK; /* 成功 */
		}
	}
	internal_log("SendState: %d  [0:OK  1:ERROR  2:TIMEOUT]\r\n\r\n", val->status);
	return AT_RES_OK;
}

/**
  * @defgroup  standard_at_cmd_ati_get
  * @brief  固件版本查询
  * @param  dev			命令句柄
  * @param  val			返回信息机构体
  * @param  timeout		等待时间(单位:mS)
  * @retval	发送状态(0:Successful  1: Error code)
  *
  */
AT_DRESULT standard_at_cmd_ati_get(PUART_INFO_CTX dev, standard_ati_rep_t *val, unsigned int timeout)
{
	unsigned int at_n_vals, at_val_index;
	char *atvalues[20]; /* 比对数据 */
	char *atvalue = NULL;
	char *tmp_str = NULL;

	if (timeout == 0)
	{
		return AT_RES_ERROR; /* 错误 */
	}
	internal_log("SendData: %s\r\n", ATI);
	if (send_at_command(dev, ATI, timeout, 5) != ENUM_RES_OK) /* 发送指令并判断返回 */
	{
		val->status = QL_TIMEOUT; /* 超时 */
	}
	else /* 有数据返回 */
	{
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR) /* 返回错误 */
		{
			val->status = QL_ERROR; /* 错误 */
		}
		else if (dev->at_res_status == ENUM_AT_RES_OK) /* 返回正确可以解析数据 */
		{
			val->status = QL_OK;														  /* 成功 */
			tmp_str = split_string_by_word(dev, "Revision:");							  /* 数据比对,返回数据存在指针 */
			at_n_vals = split_string_by_comma(tmp_str, atvalues, ARRAY_LENGTH(atvalues)); /* 以':或'','为间隔断帧 */
			if (at_n_vals == 0)															  /* 断帧后没有数据 */
			{
				return AT_RES_ERROR; /* 错误 */
			}
			for (at_val_index = 0; at_val_index < at_n_vals; at_val_index++) /* 循环断帧后数据数 */
			{
				atvalue = atvalues[at_val_index]; /* 数据暂存 */
				if (-1 == judge_value(atvalue))	  /* 判断数据是否为空 */
				{
					continue;
				}
			}
			val->Firmware = atvalues[1]; /* 读取信息 */
			internal_log("val->Firmware: %s\r\n", val->Firmware);
		}
	}
	internal_log("SendState: %d  [0:OK  1:ERROR  2:TIMEOUT]\r\n\r\n", val->status);
	return AT_RES_OK;
}

/**
  * @defgroup  standard_at_cmd_cesq_get
  * @brief  信号详细信息查询
  * @param  dev			命令句柄
  * @param  val			返回信息机构体
  * @param  timeout		等待时间(单位:mS)
  * @retval	发送状态(0:Successful  1: Error code)
  *
  */
AT_DRESULT standard_at_cmd_cesq_get(PUART_INFO_CTX dev, standard_cesq_rep_t *val, unsigned int timeout)
{
	unsigned int at_n_vals, at_val_index;
	char *atvalues[20]; /* 比对数据 */
	char *atvalue = NULL;
	char *tmp_str = NULL;

	if (timeout == 0)
	{
		return AT_RES_ERROR; /* 错误 */
	}
	internal_log("SendData: %s\r\n", CESQ);
	if (send_at_command(dev, CESQ, timeout, 3) != ENUM_RES_OK) /* 发送指令并判断返回 */
	{
		val->status = QL_TIMEOUT; /* 超时 */
	}
	else /* 有数据返回 */
	{
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR) /* 返回错误 */
		{
			val->status = QL_ERROR; /* 错误 */
		}
		else if (dev->at_res_status == ENUM_AT_RES_OK) /* 返回正确可以解析数据 */
		{
			val->status = QL_OK;														  /* 成功 */
			tmp_str = split_string_by_word(dev, "+CESQ:");								  /* 数据比对,返回数据存在指针 */
			at_n_vals = split_string_by_comma(tmp_str, atvalues, ARRAY_LENGTH(atvalues)); /* 以':或'','为间隔断帧 */
			if (at_n_vals == 0)															  /* 断帧后没有数据 */
			{
				return AT_RES_ERROR; /* 错误 */
			}
			for (at_val_index = 0; at_val_index < at_n_vals; at_val_index++) /* 循环断帧后数据数 */
			{
				atvalue = atvalues[at_val_index]; /* 数据暂存 */
				if (-1 == judge_value(atvalue))	  /* 判断数据是否为空 */
				{
					continue;
				}
			}
			val->rxlev = stringtoint(atvalues[1]); /* 读取信息 */
			internal_log("val->rxlev: %d\r\n", val->rxlev);
			val->ber = stringtoint(atvalues[2]); /* 读取信息 */
			internal_log("val->ber: %d\r\n", val->ber);
			val->rscp = stringtoint(atvalues[3]); /* 读取信息 */
			internal_log("val->rscp: %d\r\n", val->rscp);
			val->ecno = stringtoint(atvalues[4]); /* 读取信息 */
			internal_log("val->ecno: %d\r\n", val->ecno);
			val->rsrq = stringtoint(atvalues[5]); /* 读取信息 */
			internal_log("val->rsrq: %d\r\n", val->rsrq);
			val->rsrp = stringtoint(atvalues[6]); /* 读取信息 */
			internal_log("val->rsrp: %d\r\n", val->rsrp);
		}
	}
	internal_log("SendState: %d  [0:OK  1:ERROR  2:TIMEOUT]\r\n\r\n", val->status);
	return AT_RES_OK;
}

/**
  * @defgroup  standard_at_cmd_qsclk_get
  * @brief  关闭PSM睡眠模式
  * @param  dev			命令句柄
  * @param  val			返回信息机构体
  * @param  timeout		等待时间(单位:mS)
  * @retval	发送状态(0:Successful  1: Error code)
  *
  */
AT_DRESULT standard_at_cmd_qsclk_get(PUART_INFO_CTX dev, standard_qsclk_rep_t *val, unsigned int timeout)
{
	if (timeout == 0)
	{
		return AT_RES_ERROR; /* 错误 */
	}
	internal_log("SendData: %s\r\n", QSCLK);
	if (send_at_command(dev, QSCLK, timeout, 2) != ENUM_RES_OK) /* 发送指令并判断返回 */
	{
		val->status = QL_TIMEOUT; /* 超时 */
	}
	else /* 有数据返回 */
	{
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR) /* 返回错误 */
		{
			val->status = QL_ERROR; /* 错误 */
		}
		else if (dev->at_res_status == ENUM_AT_RES_OK) /* 返回正确可以解析数据 */
		{
			val->status = QL_OK; /* 成功 */
		}
	}
	internal_log("SendState: %d  [0:OK  1:ERROR  2:TIMEOUT]\r\n\r\n", val->status);
	return AT_RES_OK;
}

/**
  * @defgroup  standard_at_cmd_qsclk_get
  * @brief  开启网络指示灯
  * @param  dev			命令句柄
  * @param  val			返回信息机构体
  * @param  timeout		等待时间(单位:mS)
  * @retval	发送状态(0:Successful  1: Error code)
  *
  */
AT_DRESULT standard_at_cmd_qledmode_get(PUART_INFO_CTX dev, standard_qledmode_rep_t *val, unsigned int timeout)
{
	if (timeout == 0)
	{
		return AT_RES_ERROR; /* 错误 */
	}
	internal_log("SendData: %s\r\n", QLEDMODE);
	if (send_at_command(dev, QLEDMODE, timeout, 2) != ENUM_RES_OK) /* 发送指令并判断返回 */
	{
		val->status = QL_TIMEOUT; /* 超时 */
	}
	else /* 有数据返回 */
	{
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR) /* 返回错误 */
		{
			val->status = QL_ERROR; /* 错误 */
		}
		else if (dev->at_res_status == ENUM_AT_RES_OK) /* 返回正确可以解析数据 */
		{
			val->status = QL_OK; /* 成功 */
		}
	}
	internal_log("SendState: %d  [0:OK  1:ERROR  2:TIMEOUT]\r\n\r\n", val->status);
	return AT_RES_OK;
}

/**
  * @defgroup  standard_at_cmd_qsclk_get
  * @brief  SIM卡状态查询
  * @param  dev			命令句柄
  * @param  val			返回信息机构体
  * @param  timeout		等待时间(单位:mS)
  * @retval	发送状态(0:Successful  1: Error code)
  *
  */
AT_DRESULT standard_at_cmd_cpin_get(PUART_INFO_CTX dev, standard_cpin_rep_t *val, unsigned int timeout)
{
	unsigned int at_n_vals, at_val_index;
	char *atvalues[20]; /* 比对数据 */
	char *atvalue = NULL;
	char *tmp_str = NULL;

	if (timeout == 0)
	{
		return AT_RES_ERROR; /* 错误 */
	}
	internal_log("SendData: %s\r\n", CPIN);
	if (send_at_command(dev, CPIN, timeout, 3) != ENUM_RES_OK) /* 发送指令并判断返回 */
	{
		val->status = QL_TIMEOUT; /* 超时 */
	}
	else /* 有数据返回 */
	{
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR) /* 返回错误 */
		{
			val->status = QL_ERROR; /* 错误 */
		}
		else if (dev->at_res_status == ENUM_AT_RES_OK) /* 返回正确可以解析数据 */
		{
			val->status = QL_OK;														  /* 成功 */
			tmp_str = split_string_by_word(dev, "+CPIN:");								  /* 数据比对,返回数据存在指针 */
			at_n_vals = split_string_by_comma(tmp_str, atvalues, ARRAY_LENGTH(atvalues)); /* 以':或'','为间隔断帧 */
			if (at_n_vals == 0)															  /* 断帧后没有数据 */
			{
				return AT_RES_ERROR; /* 错误 */
			}
			for (at_val_index = 0; at_val_index < at_n_vals; at_val_index++) /* 循环断帧后数据数 */
			{
				atvalue = atvalues[at_val_index]; /* 数据暂存 */
				if (-1 == judge_value(atvalue))	  /* 判断数据是否为空 */
				{
					continue;
				}
			}
			val->SIMStatus = atvalues[1]; /* 读取信息 */
			internal_log("val->SIMStatus: %s\r\n", val->SIMStatus);
		}
	}
	internal_log("SendState: %d  [0:OK  1:ERROR  2:TIMEOUT]\r\n\r\n", val->status);
	return AT_RES_OK;
}

/**
  * @defgroup  standard_at_cmd_qsclk_get
	* @brief  IMEI号查询
  * @param  dev			命令句柄
  * @param  val			返回信息机构体
  * @param  timeout		等待时间(单位:mS)
  * @retval	发送状态(0:Successful  1: Error code)
  *
  */
AT_DRESULT standard_at_cmd_cgsn_get(PUART_INFO_CTX dev, standard_cgsn_rep_t *val, unsigned int timeout)
{
	unsigned int at_n_vals, at_val_index;
	char *atvalues[20]; /* 比对数据 */
	char *atvalue = NULL;
	char *tmp_str = NULL;

	if (timeout == 0)
	{
		return AT_RES_ERROR; /* 错误 */
	}
	internal_log("SendData: %s\r\n", CGSN);
	if (send_at_command(dev, CGSN, timeout, 3) != ENUM_RES_OK) /* 发送指令并判断返回 */
	{
		val->status = QL_TIMEOUT; /* 超时 */
	}
	else /* 有数据返回 */
	{
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR) /* 返回错误 */
		{
			val->status = QL_ERROR; /* 错误 */
		}
		else if (dev->at_res_status == ENUM_AT_RES_OK) /* 返回正确可以解析数据 */
		{
			val->status = QL_OK;														  /* 成功 */
			tmp_str = split_string_by_word(dev, "+CGSN:");								  /* 数据比对,返回数据存在指针 */
			at_n_vals = split_string_by_comma(tmp_str, atvalues, ARRAY_LENGTH(atvalues)); /* 以':或'','为间隔断帧 */
			if (at_n_vals == 0)															  /* 断帧后没有数据 */
			{
				return AT_RES_ERROR; /* 错误 */
			}
			for (at_val_index = 0; at_val_index < at_n_vals; at_val_index++) /* 循环断帧后数据数 */
			{
				atvalue = atvalues[at_val_index]; /* 数据暂存 */
				if (-1 == judge_value(atvalue))	  /* 判断数据是否为空 */
				{
					continue;
				}
			}
			val->IMSIMark = atvalues[1]; /* 读取信息 */
			internal_log("val->IMSIMark: %s\r\n", val->IMSIMark);
		}
	}
	internal_log("SendState: %d  [0:OK  1:ERROR  2:TIMEOUT]\r\n\r\n", val->status);
	return AT_RES_OK;
}

/**
  * @defgroup  standard_at_cmd_qsclk_get
	* @brief  CIMI号查询
  * @param  dev			命令句柄
  * @param  val			返回信息机构体
  * @param  timeout		等待时间(单位:mS)
  * @retval	发送状态(0:Successful  1: Error code)
  *
  */
AT_DRESULT standard_at_cmd_cimi_get(PUART_INFO_CTX dev, standard_cimi_rep_t *val, unsigned int timeout)
{
	unsigned int at_n_vals, at_val_index;
	char *atvalues[20]; /* 比对数据 */
	char *atvalue = NULL;
	char *tmp_str = NULL;

	if (timeout == 0)
	{
		return AT_RES_ERROR; /* 错误 */
	}
	internal_log("SendData: %s\r\n", CIMI);
	if (send_at_command(dev, CIMI, timeout, 3) != ENUM_RES_OK) /* 发送指令并判断返回 */
	{
		val->status = QL_TIMEOUT; /* 超时 */
	}
	else /* 有数据返回 */
	{
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR) /* 返回错误 */
		{
			val->status = QL_ERROR; /* 错误 */
		}
		else if (dev->at_res_status == ENUM_AT_RES_OK) /* 返回正确可以解析数据 */
		{
			val->status = QL_OK;														  /* 成功 */
			tmp_str = split_string_by_word(dev, "46");									  /* 数据比对,返回数据存在指针 */
			at_n_vals = split_string_by_comma(tmp_str, atvalues, ARRAY_LENGTH(atvalues)); /* 以':或'','为间隔断帧 */
			if (at_n_vals == 0)															  /* 断帧后没有数据 */
			{
				return AT_RES_ERROR; /* 错误 */
			}
			for (at_val_index = 0; at_val_index < at_n_vals; at_val_index++) /* 循环断帧后数据数 */
			{
				atvalue = atvalues[at_val_index]; /* 数据暂存 */
				if (-1 == judge_value(atvalue))	  /* 判断数据是否为空 */
				{
					continue;
				}
			}
			val->CIMIMark = atvalues[0]; /* 读取信息 */
			internal_log("val->CIMIMark: %s\r\n", val->CIMIMark);
		}
	}
	internal_log("SendState: %d  [0:OK  1:ERROR  2:TIMEOUT]\r\n\r\n", val->status);
	return AT_RES_OK;
}

/**
  * @defgroup  standard_at_cmd_qsclk_get
	* @brief  QCCID号查询
  * @param  dev			命令句柄
  * @param  val			返回信息机构体
  * @param  timeout		等待时间(单位:mS)
  * @retval	发送状态(0:Successful  1: Error code)
  *
  */
AT_DRESULT standard_at_cmd_qccid_get(PUART_INFO_CTX dev, standard_qccid_rep_t *val, unsigned int timeout)
{
	unsigned int at_n_vals, at_val_index;
	char *atvalues[20]; /* 比对数据 */
	char *atvalue = NULL;
	char *tmp_str = NULL;

	if (timeout == 0)
	{
		return AT_RES_ERROR; /* 错误 */
	}
	internal_log("SendData: %s\r\n", QCCID);
	if (send_at_command(dev, QCCID, timeout, 3) != ENUM_RES_OK) /* 发送指令并判断返回 */
	{
		val->status = QL_TIMEOUT; /* 超时 */
	}
	else /* 有数据返回 */
	{
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR) /* 返回错误 */
		{
			val->status = QL_ERROR; /* 错误 */
		}
		else if (dev->at_res_status == ENUM_AT_RES_OK) /* 返回正确可以解析数据 */
		{
			val->status = QL_OK;														  /* 成功 */
			tmp_str = split_string_by_word(dev, "+QCCID:");								  /* 数据比对,返回数据存在指针 */
			at_n_vals = split_string_by_comma(tmp_str, atvalues, ARRAY_LENGTH(atvalues)); /* 以':或'','为间隔断帧 */
			if (at_n_vals == 0)															  /* 断帧后没有数据 */
			{
				return AT_RES_ERROR; /* 错误 */
			}
			for (at_val_index = 0; at_val_index < at_n_vals; at_val_index++) /* 循环断帧后数据数 */
			{
				atvalue = atvalues[at_val_index]; /* 数据暂存 */
				if (-1 == judge_value(atvalue))	  /* 判断数据是否为空 */
				{
					continue;
				}
			}
			val->QCCIDMark = atvalues[1]; /* 读取信息 */
			internal_log("val->QCCIDMark: %s\r\n", val->QCCIDMark);
		}
	}
	internal_log("SendState: %d  [0:OK  1:ERROR  2:TIMEOUT]\r\n\r\n", val->status);
	return AT_RES_OK;
}

/**
  * @defgroup  standard_at_cmd_qsclk_get
	* @brief  入网状态查询
  * @param  dev			命令句柄
  * @param  val			返回信息机构体
  * @param  timeout		等待时间(单位:mS)
  * @retval	发送状态(0:Successful  1: Error code)
  *
  */
AT_DRESULT standard_at_cmd_cereg_get(PUART_INFO_CTX dev, standard_cereg_rep_t *val, unsigned int timeout)
{
	unsigned int at_n_vals, at_val_index;
	char *atvalues[20]; /* 比对数据 */
	char *atvalue = NULL;
	char *tmp_str = NULL;

	if (timeout == 0)
	{
		return AT_RES_ERROR; /* 错误 */
	}
	internal_log("SendData: %s\r\n", CEREG);
	if (send_at_command(dev, CEREG, timeout, 3) != ENUM_RES_OK) /* 发送指令并判断返回 */
	{
		val->status = QL_TIMEOUT; /* 超时 */
	}
	else /* 有数据返回 */
	{
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR) /* 返回错误 */
		{
			val->status = QL_ERROR; /* 错误 */
		}
		else if (dev->at_res_status == ENUM_AT_RES_OK) /* 返回正确可以解析数据 */
		{
			val->status = QL_OK;							/* 成功 */
			tmp_str = split_string_by_word(dev, "+CEREG:"); /* 数据比对,返回数据存在指针 */
			/*分析CSQ内容*/
			at_n_vals = split_string_by_comma(tmp_str, atvalues, ARRAY_LENGTH(atvalues)); /* 以':或'','为间隔断帧 */
			if (at_n_vals == 0)															  /* 断帧后没有数据 */
			{
				return AT_RES_ERROR; /* 错误 */
			}
			for (at_val_index = 0; at_val_index < at_n_vals; at_val_index++) /* 循环断帧后数据数 */
			{
				atvalue = atvalues[at_val_index]; /* 数据暂存 */
				if (-1 == judge_value(atvalue))	  /* 判断数据是否为空 */
				{
					continue;
				}
			}
			val->URCStatus = stringtoint(atvalues[1]); /* 读取信息 */
			internal_log("val->URCStatus: %d\r\n", val->URCStatus);
			val->NetworkStatus = stringtoint(atvalues[2]); /* 读取信息 */
			internal_log("val->NetworkStatus: %d\r\n", val->NetworkStatus);
		}
	}
	internal_log("SendState: %d  [0:OK  1:ERROR  2:TIMEOUT]\r\n\r\n", val->status);
	return AT_RES_OK;
}

/**
  * @defgroup  standard_at_cmd_qsclk_get
	* @brief  入网IP地址查询
  * @param  dev			命令句柄
  * @param  val			返回信息机构体
  * @param  timeout		等待时间(单位:mS)
  * @retval	发送状态(0:Successful  1: Error code)
  *
  */
AT_DRESULT standard_at_cmd_cgpaddr_get(PUART_INFO_CTX dev, standard_cgpaddr_rep_t *val, unsigned int timeout)
{
	unsigned int at_n_vals, at_val_index;
	char *atvalues[20]; /* 比对数据 */
	char *atvalue = NULL;
	char *tmp_str = NULL;

	if (timeout == 0)
	{
		return AT_RES_ERROR; /* 错误 */
	}
	internal_log("SendData: %s\r\n", CGPADDR);
	if (send_at_command(dev, CGPADDR, timeout, 3) != ENUM_RES_OK) /* 发送指令并判断返回 */
	{
		val->status = QL_TIMEOUT; /* 超时 */
	}
	else /* 有数据返回 */
	{
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR) /* 返回错误 */
		{
			val->status = QL_ERROR; /* 错误 */
		}
		else if (dev->at_res_status == ENUM_AT_RES_OK) /* 返回正确可以解析数据 */
		{
			val->status = QL_OK;														  /* 成功 */
			tmp_str = split_string_by_word(dev, "+CGPADDR:");							  /* 数据比对,返回数据存在指针 */
			at_n_vals = split_string_by_comma(tmp_str, atvalues, ARRAY_LENGTH(atvalues)); /* 以':或'','为间隔断帧 */
			if (at_n_vals == 0)															  /* 断帧后没有数据 */
			{
				return AT_RES_ERROR; /* 错误 */
			}
			for (at_val_index = 0; at_val_index < at_n_vals; at_val_index++) /* 循环断帧后数据数 */
			{
				atvalue = atvalues[at_val_index]; /* 数据暂存 */
				if (-1 == judge_value(atvalue))	  /* 判断数据是否为空 */
				{
					continue;
				}
			}
			val->IPAddr = atvalues[2]; /* 读取信息 */
			internal_log("val->IPAddr: %s\r\n", val->IPAddr);
		}
	}
	internal_log("SendState: %d  [0:OK  1:ERROR  2:TIMEOUT]\r\n\r\n", val->status);
	return AT_RES_OK;
}

/************************ (C) COPYRIGHT HEFEI FAE *****END OF FILE****/
