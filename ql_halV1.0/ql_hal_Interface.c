/**
  ******************************************************************************
  * @file           : ql_hal_Interface.c
  * @brief          : Main program body
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
#include "ql_hal_interface.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{

     char buf[MAX_BUFFER_SIZE]; /* 串口接收原始数据缓存区 */
     unsigned short length;     /* 串口接收原始数据长度 */
     char flag;
     char lock; /* 缓存区锁锁 */
} recv_buf;
static recv_buf ql_recv_buf; //串口接收缓存

/* Private function prototypes -----------------------------------------------*/
static int split_string_by_wrap(char *string, char **values, int max_values);
static FA_RESULT_CODE diverter_input(PUART_INFO_CTX dev, char *data, unsigned short length, unsigned char line_num);
static FA_RESULT_CODE urc_match(PUART_INFO_CTX dev, char *data, unsigned short int length);

/**
  * @brief  串口接收一包完整的数据之后，需要调用此接口把数据放到AT指令调用的buf中
  * @param  buf
  * @param  length
  * @retval FA_RESULT_CODE
  *
  */
FA_RESULT_CODE recv_at_packet(unsigned char *buf, unsigned short length)
{

     if ((*buf == NULL) || (length == 0) || (length > MAX_BUFFER_SIZE))
     {
          ql_recv_buf.length = 0;
          ql_recv_buf.flag = 0;
          return ENUM_RES_ERROR;
     }

     if (buf[length - 2] == '\r' && buf[length - 1] == '\n') /*判断是不是收到了正确的结尾*/
     {
          /* 内部缓存区在占用暂时不能释放资源 */
          if (ql_recv_buf.lock == 1)
               return ENUM_RES_MEMORY_BUSY; /*返回忙*/

          memcpy(ql_recv_buf.buf, buf, length);
          ql_recv_buf.length = length;
          ql_recv_buf.flag = 1;
          ql_recv_buf.lock = 1;
          return ENUM_RES_OK;
     }
     else
     {
          memset(ql_recv_buf.buf, 0, MAX_BUFFER_SIZE);
          ql_recv_buf.length = 0;
          ql_recv_buf.flag = 0;
          return ENUM_RES_ERROR;
     }
}

/**
  * @brief  串口发送超时
  * @param  dev
  * @param  command
  * @param  timeout
  * @retval FA_RESULT_CODE
  *
  */
FA_RESULT_CODE send_at_command(PUART_INFO_CTX dev, char *command, unsigned int timeout, unsigned char line_num)
{
     unsigned int start_times = 0;
     int len;
     static unsigned char send_buf[SEND_BUF_LEN];

     memset(dev->at_res_data, 0, sizeof(dev->at_res_data));
     dev->at_res_status = ENUM_AT_RES_OK;
     dev->at_res_num = 0; //回复行数

     len = strlen(command);
     memcpy(send_buf, command, len);
     send_buf[len++] = '\r';
     send_buf[len++] = '\n';
     dev->write(send_buf, len);                    /*发送数据*/
     start_times = ql_tick_get();                  /*开始计时*/
     while (ql_tick_elaps(start_times) <= timeout) /*超时处理*/
     {
          if (rec_data_parser(dev, line_num) == ENUM_RES_OK) /*有数据解析成功*/
          {
               return ENUM_RES_OK;
          }
     }
     /*超时返回*/
     dev->at_res_status = ENUM_AT_RES_MEMORY_BUSY;
     return ENUM_RES_ERROR;
}

/**
  * @brief  
  * @param  dev
  * @retval FA_RESULT_CODE
  *
  */
FA_RESULT_CODE rec_data_parser(PUART_INFO_CTX dev, unsigned char line_num)
{
     if (ql_recv_buf.flag == 1) /* 成功的读到数据 */
     {
          ql_recv_buf.flag = 0;
          ql_recv_buf.buf[ql_recv_buf.length] = '\0';

          /* 判断是否有urc以及urc匹配和对应handler执行 */
          if (diverter_input(dev, ql_recv_buf.buf, ql_recv_buf.length, line_num) == ENUM_RES_OK)
          {
               ql_recv_buf.lock = 0;
               return ENUM_RES_OK;
          }
          ql_recv_buf.lock = 0;
     }
     return ENUM_RES_ERROR;
}

/**
  * @brief  
  * @param  dev
  * @param  data
  * @param  length
  * @retval FA_RESULT_CODE
  *
  */
static FA_RESULT_CODE diverter_input(PUART_INFO_CTX dev, char *data, unsigned short length, unsigned char line_num)
{
     unsigned int n_vals, val_index;
     char *value;
     char *values[255];
     // char line_num = 0;

     n_vals = split_string_by_wrap(data, values, ARRAY_LENGTH(values)); /* Split the sentence into values */
     if (0 == n_vals)
     {
          return ENUM_RES_ERROR;
     }
     for (val_index = 0; val_index < n_vals; val_index++) /*将URC和AT指令回复消息分离*/
     {
          value = values[val_index];
          if (-1 == judge_value(value))
          {
               continue;
          }

          if (urc_match(dev, value, strlen(value)) == ENUM_RES_OK) /* URC数据 */
          {

#if URC_CALLBACK_ENABLE == 1
               dev->urc_callback(value, strlen(value), 0); /* URC回调打印给客户 */
#endif
          }
          else /* AT指令数据 */
          {
#if AT_CALLBACK_ENABLE == 1
               /*AT回调*/
               dev->urc_callback(value, strlen(value), 1);
#endif
               memcpy(dev->at_res_data[dev->at_res_num], value, strlen(value));
               dev->at_res_num++;
               /*这里加上AT指令回ERROR或者OK判断*/
               if (memcmp(value, RES_OK, strlen(RES_OK)) == 0)
               {
                    /* 根据响应结果获取结束数据，返回响应状态ENUM_AT_RES_OK。 */
                    dev->at_res_status = ENUM_AT_RES_OK;
               }
               else if (strstr(value, RES_ERROR) || (memcmp(value, RES_FAIL, strlen(RES_FAIL)) == 0))
               {
                    dev->at_res_status = ENUM_AT_RES_PARAMETER_ERROR;
               }
               else if (dev->at_res_num == line_num)
               {
                    /* 通过响应行获取结束数据，返回响应状态END_OK。*/
                    dev->at_res_status = ENUM_AT_RES_OK;
               }
          }
     }
     return ENUM_RES_OK;
}

/**
  * @brief  URC数据匹配处理
  * @param  dev
  * @param  data
  * @param  length
  * @retval FA_RESULT_CODE
  *
  */
FA_RESULT_CODE urc_match(PUART_INFO_CTX dev, char *data, unsigned short int length)
{

     if ((URC_NUMBER_MATCHES == NULL) || (dev->urc_num == NULL))
     {
          return ENUM_RES_ERROR;
     }

     for (char i = 0; i < dev->urc_num; i++)
     {
          /*判断包头包尾是否包含在数据中*/
          if ((strncmp(dev->urc_table[i].prefix, data, strlen(dev->urc_table[i].prefix)) && strncmp(dev->urc_table[i].suffix, data, strlen(dev->urc_table[i].suffix))) == ENUM_RES_OK)
          {
               return ENUM_RES_OK;
          }
     }
     return ENUM_RES_ERROR;
}

/**
  * @brief  解析字符串
  * @param  dev
  * @param  data
  * @param  length
  * @retval FA_RESULT_CODE
  *
  */
char *split_string_by_word(PUART_INFO_CTX dev, const char *word)
{
     char *resp_buf = NULL;

     for (char i = 0; i < dev->at_res_num; i++)
     {
          if (strstr(dev->at_res_data[i], word))
          {
               resp_buf = dev->at_res_data[i];

               return resp_buf;
          }
     }
     return NULL;
}

/**
  * @brief  Splits a string by comma or colon.
  *
  * @param  string：string is the string to split, will be manipulated. Needs to be null-terminated.
  * @param  values： values is a char pointer array that will be filled with pointers to the splitted 
                     values in the string.
  * @param  max_values：max_values is the maximum number of values to be parsed.
  * @retval FA_RESULT_CODE
  *
  */
int split_string_by_comma(char *string, char **values, int max_values)
{
     int i = 0;

     values[i++] = string;
     while (i < max_values && NULL != (string = strchr(string, ':')))
     {
          *string = '\0';
          values[i++] = ++string;                                        /*++的目的是去掉要查找的字符*/
          if (strlen(strchr(string, ':')) > strlen(strchr(string, ','))) //如果：的位置在，前面，继续查找：的位置。
          {
               continue;
          }
          while (NULL != (string = strchr(string, ',')))
          {
               *string = '\0';
               values[i++] = ++string;
               if (strlen(strchr(string, ':')) > strlen(strchr(string, ','))) //如果：的位置在，前面，继续查找：的位置
               {
                    break;
               }
          }
     }

     return i;
}

/**
  * @brief  
  * @param  string
  * @param  values
  * @param  max_values
  * @retval 
  *
  */
static int split_string_by_wrap(char *string, char **values, int max_values)
{
     int i = 0;
     values[i++] = string;
     while (i < max_values && NULL != (string = strchr(string, '\n')))
     {
          *string = '\0';
          values[i++] = ++string;
     }

     return i;
}

/**
 * @brief   Check if a value is not NULL and not empty.
 * 
 * @Returns 0 if set, otherwise -1.
 */
int judge_value(const char *value)
{
     if (NULL == value || '\0' == *value)
     {
          return -1;
     }

     return 0;
}

/**
 * @brief   字符串转数字
 * 
 * @Returns 0 if set, otherwise -1.
 */
int stringtoint(char *s)
{
     int flag = 1;
     int num = 0;
     int i = 0;
     while (s[i] != '\0')
     {
          if (s[i] >= '0' && s[i] <= '9')
               num = (num * 10) + (s[i] - '0');
          i++;
     }
     if (s[0] == '-')
          flag = -1;
     return flag * num;
}

/************************ (C) COPYRIGHT HEFEI FAE *****END OF FILE****/
