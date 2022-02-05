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

#endif