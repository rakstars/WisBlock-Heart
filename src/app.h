/**
 * @file app.cpp
 * @author Johan Sebastian Macias (johan.macias@rakwireless.com)
 * @brief For application specific includes and definitions
 * @version 0.1
 * @date 2022-01-25
 * 
 * @copyright Copyright (c) 2022
 * 
 * Based on: 
 * https://github.com/beegee-tokyo/RAK4631-RAK12014-Waterlevel/blob/bd9a98d527c5c85f5e32fdaa68603049948af7f1/src/app.h
 * By: Bernd Giesecke (bernd.giesecke@rakwireless.com)
 * Date: 2021-04-23
 */

#ifndef APP_H
#define APP_H

#include <Arduino.h>
/** Add you required includes after Arduino.h */

#include <Wire.h>
/** Include the WisBlock-API */
#include <WisBlock-API.h>

// Debug output set to 0 to disable app debug output
#ifndef MY_DEBUG
#define MY_DEBUG 0
#endif

#if MY_DEBUG > 0
#define MYLOG(tag, ...)                     \
	do                                      \
	{                                       \
		if (tag)                            \
			PRINTF("[%s] ", tag);           \
		PRINTF(__VA_ARGS__);                \
		PRINTF("\n");                       \
		if (g_ble_uart_is_connected)        \
		{                                   \
			g_ble_uart.printf(__VA_ARGS__); \
		}                                   \
	} while (0)
#else
#define MYLOG(...)
#endif

/** Application function definitions */
void setup_app(void);
bool init_app(void);
void app_event_handler(void);
void ble_data_handler(void) __attribute__((weak));
void lora_data_handler(void);

/** Application events */
#define ACC_TRIGGER 0b1000000000000000
#define N_ACC_TRIGGER 0b0111111111111111

/** Application stuff */
extern BaseType_t g_higher_priority_task_woken;

/** Accelerometer stuff */
#include <SparkFunLIS3DH.h>
#define INT1_PIN WB_IO3
bool init_acc(void);
void clear_acc_int(void);
void read_acc(void);

/** EPD stuff */
bool init_epd(void);
void switch_epd_message(void);
extern uint8_t gMsgNum;

/** User flash data stuff */
#define MY_APP_DATA_MARKER 0x65
struct s_user_flash_data
{
	uint8_t valid_mark_1 = 0xBA;			   // Just a marker for the Flash
	uint8_t valid_mark_2 = MY_APP_DATA_MARKER; // Just a marker for the Flash

	uint8_t epd_msg_1[80] = 
	{
		' ', ' ', 'H', 'i', '!', ' ', 'I', ' ', 'a', 'm', ' ', 't', 'h', 'i', 'n', 'k', 'i', 'n', 'g', ' ',
		'a', 'b', 'o', 'u', 't', ' ', 'y', 'o', 'u', '.', ' ', 'L', 'o', 'v', 'e', ' ', 'y', 'o', 'u', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	};
	uint8_t epd_msg_2[80] = 
	{
		'I', 'f', ' ', 'I', ' ', 'k', 'n', 'o', 'w', ' ', 'w', 'h', 'a', 't', ' ', 'l', 'o', 'v', 'e', ' ',
		'i', 's', ',', ' ', 'i', 't', ' ', 'i', 's', ' ', 'b', 'e', 'c', 'a', 'u', 's', 'e', ' ', 'o', 'f',
		'y', 'o', 'u', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	};
	uint8_t epd_msg_3[80] = 
	{
		' ', ' ', ' ', 'W', 'h', 'i', 'l', 'e', ' ', '(', 't', 'r', 'u', 'e', ')', ' ', '{', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', 'I', ' ', 'l', 'o', 'v', 'e', ' ', 'y', 'o', 'u', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', '}', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	};
	uint8_t epd_msg_4[80] = 
	{
		'E', 'v', 'e', 'n', ' ', 'w', 'h', 'e', 'n', ' ', 'I', ' ', 'a', 'm', ' ', 'n', 'o', 't', ' ', ' ',
		'w', 'i', 't', 'h', ' ', 'y', 'o', 'u', ',', ' ', 'a', 'l', 'l', ' ', 'I', ' ', 'c', 'a', 'n', ' ',
		't', 'h', 'i', 'n', 'k', ' ', 'o', 'f', ' ', 'i', 's', ' ', 'y', 'o', 'u', 'r', ' ', ' ', ' ', ' ',
		's', 'm', 'i', 'l', 'e', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	};
};
extern s_user_flash_data g_user_flash_data;
void init_user_flash_data(void);
void log_user_flash_data(void);
boolean save_user_flash_data(void);

#endif