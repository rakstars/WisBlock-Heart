/**
 * @file app.cpp
 * @author Johan Sebastian Macias (johan.macias@rakwireless.com)
 * @brief Application specific functions. Mandatory to have init_app(), 
 *        app_event_handler(), ble_data_handler(), lora_data_handler()
 *        and lora_tx_finished()
 * @version 0.1
 * @date 2022-01-25
 * 
 * @copyright Copyright (c) 2022
 * 
 * Based on: 
 * https://github.com/beegee-tokyo/RAK4631-RAK12014-Waterlevel/blob/main/src/app.cpp
 * By: Bernd Giesecke (bernd.giesecke@rakwireless.com)
 * Date: 2021-04-23
 */

#include "app.h"

/** Set the device name, max length is 10 characters */
char g_ble_dev_name[10] = "WB-Love";

/** Flag showing if TX cycle is ongoing */
bool lora_busy = false;

/** Required to give semaphore from ISR. Giving the semaphore wakes up the loop() */
BaseType_t g_higher_priority_task_woken = pdTRUE;

/** Send Fail counter **/
uint8_t send_fail = 0;

/**
 * @brief Application specific setup functions
 * 
 */
void setup_app(void)
{
	// Enable BLE
	g_enable_ble = true;

	// Set firmware version
	api_set_version(SW_VERSION_1, SW_VERSION_2, SW_VERSION_3);
}

/**
 * @brief Application specific initializations
 * 
 * @return true Initialization success
 * @return false Initialization failure
 */
bool init_app(void)
{
    bool init_result = true;

	MYLOG("APP", "Application initialization");

    // Initialize ACC sensor
	init_result |= init_acc();
	return init_result;
}

/**
 * @brief Application specific event handler
 *      Requires as minimum the handling of STATUS event
 *      Here you handle as well your application specific events
 */
void app_event_handler(void)
{
	// Timer triggered event
	if ((g_task_event_type & STATUS) == STATUS)
	{
		g_task_event_type &= N_STATUS;
		MYLOG("APP", "Timer wakeup");

		// If BLE is enabled, restart Advertising
		if (g_enable_ble)
		{
			restart_advertising(15);
		}

        // Dummy packet
        uint8_t dummy_packet[] = {0x10, 0x00, 0x00};

        lmh_error_status result = send_lora_packet(dummy_packet, 3);
        switch (result)
        {
            case LMH_SUCCESS:
                    MYLOG("APP", "Packet enqueued");
                    break;
            case LMH_BUSY:
                    MYLOG("APP", "LoRa transceiver is busy");
                    break;
            case LMH_ERROR:
                    MYLOG("APP", "Packet error, too big to send with current DR");
                    break;
		}
	}

    // ACC trigger event
	if ((g_task_event_type & ACC_TRIGGER) == ACC_TRIGGER &&  g_lpwan_has_joined)
	{
		g_task_event_type &= N_ACC_TRIGGER;
		MYLOG("APP", "ACC triggered");
		clear_acc_int();
        // Trigger a packet sending
        g_task_event_type |= STATUS;
	}

}

/**
 * @brief Handle BLE UART data
 * 
 */
void ble_data_handler(void)
{
	if (g_enable_ble)
	{
		// BLE UART data handling
		if ((g_task_event_type & BLE_DATA) == BLE_DATA)
		{
			MYLOG("AT", "RECEIVED BLE");
			// BLE UART data arrived
			// in this example we forward it to the AT command interpreter
			g_task_event_type &= N_BLE_DATA;

			while (g_ble_uart.available() > 0)
			{
				at_serial_input(uint8_t(g_ble_uart.read()));
				delay(5);
			}
			at_serial_input(uint8_t('\n'));
		}
	}
}

/**
 * @brief Handle received LoRa Data
 * 
 */
void lora_data_handler(void)
{
	// LoRa Join finished handling
	if ((g_task_event_type & LORA_JOIN_FIN) == LORA_JOIN_FIN)
	{
		g_task_event_type &= N_LORA_JOIN_FIN;
		if (g_join_result)
		{
			MYLOG("APP", "Successfully joined network");
		}
		else
		{
			MYLOG("APP", "Join network failed");
			/// \todo here join could be restarted.
			lmh_join();

			// If BLE is enabled, restart Advertising
			if (g_enable_ble)
			{
				restart_advertising(15);
			}
		}
	}

	// LoRa data handling
	if ((g_task_event_type & LORA_DATA) == LORA_DATA)
	{
		g_task_event_type &= N_LORA_DATA;
		MYLOG("APP", "Received package over LoRa");
		char log_buff[g_rx_data_len * 3] = {0};
		uint8_t log_idx = 0;
		for (int idx = 0; idx < g_rx_data_len; idx++)
		{
			sprintf(&log_buff[log_idx], "%02X ", g_rx_lora_data[idx]);
			log_idx += 3;
		}
		MYLOG("APP", "%s", log_buff);
	}

	// LoRa TX finished handling
	if ((g_task_event_type & LORA_TX_FIN) == LORA_TX_FIN)
	{
		g_task_event_type &= N_LORA_TX_FIN;

		MYLOG("APP", "LPWAN TX cycle %s", g_rx_fin_result ? "finished ACK" : "failed NAK");

		if (!g_rx_fin_result)
		{
			// Increase fail send counter
			send_fail++;

			if (send_fail == 10)
			{
				// Too many failed sendings, reset node and try to rejoin
				delay(100);
				sd_nvic_SystemReset();
			}
		}
	}
}