/**
 * @file user_flash_data.cpp
 * @author Johan Sebastian Macias (johan.macias@rakwireless.com)
 * @brief Initialize, read and write parameters from/to internal flash memory.
 * 		  The file used is a new one with the name user_flash_data_name. That 
 *        means there are two files in the nRF52 internal file system. The 
 *        first one is for the LoRaWAN/WisBlock-API settings and this one for
 *        user data specfic to the app.
 * 		  
 * @version 0.1
 * @date 2022-02-06
 * 
 * @copyright Copyright (c) 2022
 * 
 * Based on: 
 * https://github.com/beegee-tokyo/WisBlock-API/blob/main/src/flash-nrf52.cpp
 * By: Bernd Giesecke (bernd.giesecke@rakwireless.com)
 * Date: 2021-01-10
 */

#include "app.h"

#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
using namespace Adafruit_LittleFS_Namespace;

static const char user_flash_data_name[] = "USER_FLASH_DATA";

File user_flash_data_file(InternalFS);

s_user_flash_data g_user_flash_data;

s_user_flash_data g_user_flash_data_flash_content;

/**
 * @brief Initialize access to the file USER_FLASH_DATA in 
 * 		  the nRF52 internal file system
 */
void init_user_flash_data(void) {

    // Check if the USER_FLASH_DATA file exists
	user_flash_data_file.open(user_flash_data_name, FILE_O_READ);
	if (!user_flash_data_file)
	{
		MYLOG("USER_FLASH_DATA", "The User Flash Data File doesn't exist, it's being created now");
		delay(100);
        if (user_flash_data_file.open(user_flash_data_name, FILE_O_WRITE))
        {
            s_user_flash_data user_flash_data_file_default;
            user_flash_data_file.write((uint8_t *)&user_flash_data_file_default, sizeof(s_user_flash_data));
            user_flash_data_file.flush();
            user_flash_data_file.close();
        }
		user_flash_data_file.open(user_flash_data_name, FILE_O_READ);
	}

	uint8_t markers[2] = {0};
	user_flash_data_file.read(markers, 2);
	if ((markers[0] == 0xBA) && (markers[1] == MY_APP_DATA_MARKER))
	{
		MYLOG("USER_FLASH_DATA", "Data markers for User Flash Data found");
        user_flash_data_file.close();

        user_flash_data_file.open(user_flash_data_name, FILE_O_READ);
		user_flash_data_file.read((uint8_t *)&g_user_flash_data, sizeof(s_user_flash_data));
		user_flash_data_file.close();
		
        // Check if it is User Flash Data
		if ((g_user_flash_data.valid_mark_1 != 0xBA) || (g_user_flash_data.valid_mark_2 != MY_APP_DATA_MARKER))
		{
			// User Flash Data is not valid
			MYLOG("USER_FLASH_DATA", "The User Flash Data file needs review, it was not loaded correctly");
		} 
        else 
        {
            MYLOG("USER_FLASH_DATA", "The User Flash Data file is OK, the data was loaded correctly");
        }
	} else {
        MYLOG("USER_FLASH_DATA", "Markers for User Flash Data not found");
    }
}

/**
 * @brief Save changed User Flash Data if required
 * 
 * @return boolean 
 * 			result of saving
 */
boolean save_user_flash_data(void)
{
	bool result = true;
	// Read saved content
	user_flash_data_file.open(user_flash_data_name, FILE_O_READ);
	if (!user_flash_data_file)
	{
		API_LOG("FLASH", "File doesn't exist, force format");
		delay(100);
		// End the funtion excecution with performing flash_reset() 
		// to keep the info from the LPWAN settings file
		result = false;
		return result;
	}
	user_flash_data_file.read((uint8_t *)&g_user_flash_data_flash_content, sizeof(s_user_flash_data));
	user_flash_data_file.close();
	if (memcmp((void *)&g_user_flash_data_flash_content, (void *)&g_user_flash_data, sizeof(s_user_flash_data)) != 0)
	{
		API_LOG("FLASH", "Flash content changed, writing new data");
		delay(100);

		InternalFS.remove(user_flash_data_name);

		if (user_flash_data_file.open(user_flash_data_name, FILE_O_WRITE))
		{
			user_flash_data_file.write((uint8_t *)&g_user_flash_data, sizeof(s_user_flash_data));
			user_flash_data_file.flush();
		}
		else
		{
			result = false;
		}
		user_flash_data_file.close();
	}
	log_user_flash_data();
	return result;
}

/**
 * @brief Printout of all User Flash Data
 * 
 */
void log_user_flash_data(void)
{
	MYLOG("USER_FLASH_DATA", "Saved User Flash Data:");
	MYLOG("USER_FLASH_DATA", "000 Marks: %02X %02X", g_user_flash_data.valid_mark_1, g_user_flash_data.valid_mark_2);
	MYLOG("USER_FLASH_DATA", "002 Message 1: %.80s", g_user_flash_data.epd_msg_1);
	MYLOG("USER_FLASH_DATA", "082 Message 2: %.80s", g_user_flash_data.epd_msg_2);
	MYLOG("USER_FLASH_DATA", "162 Message 3: %.80s", g_user_flash_data.epd_msg_3);
	MYLOG("USER_FLASH_DATA", "242 Message 4: %.80s", g_user_flash_data.epd_msg_4);
}