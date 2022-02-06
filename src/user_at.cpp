/**
 * @file user_at.cpp
 * @author Johan Sebastian Macias (johan.macias@rakwireless.com)
 * @brief Handle user defined AT commands
 * 		  
 * @version 0.1
 * @date 2022-02-06
 * 
 * @copyright Copyright (c) 2022
 * 
 * Based on: 
 * https://github.com/beegee-tokyo/WisBlock-API/blob/main/examples/LoRa-P2P/user_at.cpp
 * By: Bernd Giesecke (bernd.giesecke@rakwireless.com)
 * Date: 2021-11-15
 */

#include "app.h"

static int at_exec_msg(char *str)
{
	// Get MsgNum
	char *param;

	param = strtok(str, ":");
	uint8_t MsgNum = strtol(param, NULL, 0);
	if ((MsgNum == 0) || (MsgNum > 4))
	{
		return AT_ERRNO_PARA_VAL;
	}

	// Get message to set
	param = strtok(NULL, ":");
	int data_size = strlen(param);
	if (data_size > 80)
	{
		return AT_ERRNO_PARA_VAL;
	}

	switch (MsgNum)
	{
		case 1:
			memcpy(g_user_flash_data.epd_msg_1, param, 80);
			break;
		case 2:
			memcpy(g_user_flash_data.epd_msg_2, param, 80);
			break;
		case 3:
			memcpy(g_user_flash_data.epd_msg_3, param, 80);
			break;
		case 4:
			memcpy(g_user_flash_data.epd_msg_4, param, 80);
			break;
		default:
			break;
	}
	
	// Save message to User Flash Data
	save_user_flash_data();
	// Show the message
	gMsgNum = MsgNum;
	switch_epd_message();
	return 0;
}

/**
 * @brief List of all available commands with short help and pointer to functions
 * 
 */
atcmd_t g_user_at_cmd_list[] = {
	/*|    CMD    |     AT+CMD?      |    AT+CMD=?    |  AT+CMD=value |  AT+CMD  |*/
	// GNSS commands
	{"+SETMSG", "Set message", NULL, at_exec_msg, NULL},
};

/** Number of user defined AT commands */
uint8_t g_user_at_cmd_num = sizeof(g_user_at_cmd_list) / sizeof(atcmd_t);