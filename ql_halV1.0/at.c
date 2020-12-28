
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
  * @brief  
  * @param  dev
  * @param  val
  * @retval          
  *
  */
AT_DRESULT standard_at_cmd_at_get(PUART_INFO_CTX dev, standard_at_rep_t *val, unsigned int timeout)
{
	if (timeout == 0)
	{
		return AT_RES_ERROR;
	}
	/*3000ms*/
	if (send_at_command(dev, AT, timeout, 2) != ENUM_RES_OK)
	{
		/*超时*/
		val->status = QL_TIMEOUT;
	}
	else
	{
		/*返回错误*/
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR)
		{
			val->status = QL_ERROR;
		}
		/*返回正确可以解析数据*/
		else if (dev->at_res_status == ENUM_AT_RES_OK)
		{
			val->status = QL_OK;
		}
	}
	return AT_RES_OK;
}

/**
  * @brief  
  * @param  dev
  * @param  val
  * @retval          
  *
  */
AT_DRESULT standard_at_cmd_csq_get(PUART_INFO_CTX dev, standard_csq_rep_t *val, unsigned int timeout)
{
	unsigned int at_n_vals, at_val_index;
	char *atvalues[20];
	char *atvalue = NULL;
	char *tmp_str = NULL;

	if (timeout == 0)
	{
		return AT_RES_ERROR;
	}
	/*3000ms*/
	if (send_at_command(dev, AT_CSQ, timeout, 3) != ENUM_RES_OK)
	{
		/*超时*/
		val->status = QL_TIMEOUT;
	}
	else
	{
		/*返回错误*/
		if (dev->at_res_status == ENUM_AT_RES_PARAMETER_ERROR)
		{
			val->status = QL_ERROR;
		}
		/*返回正确可以解析数据*/
		else if (dev->at_res_status == ENUM_AT_RES_OK)
		{
			val->status = QL_OK;
			tmp_str = split_string_by_word(dev, "+CSQ:");
			/*分析CSQ内容*/
			at_n_vals = split_string_by_comma(tmp_str, atvalues, ARRAY_LENGTH(atvalues));
			if (at_n_vals == 0)
			{
				return AT_RES_ERROR;
			}
			for (at_val_index = 0; at_val_index < at_n_vals; at_val_index++)
			{
				atvalue = atvalues[at_val_index];
				if (-1 == judge_value(atvalue))
				{
					continue;
				}
				printf("atvalue[%d] = %s\r\n", at_val_index, atvalue);
			}
			val->rssi = stringtoint(atvalues[1]);
			val->ber = stringtoint(atvalues[2]);
		}
	}
	return AT_RES_OK;
}
/************************ (C) COPYRIGHT HEFEI FAE *****END OF FILE****/
