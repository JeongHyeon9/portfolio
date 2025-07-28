#include "headers.h"
#include <stdio.h>

const char* wifi_command_list[WIFI_CMD_COUNT] = {
    [WIFI_RESET]         = "AT+RST",
	[WIFI_MODULE_CHECK]  = "AT",
    [WIFI_STATION_MODE]  = "AT+CWMODE=1",
	[WIFI_AP_SCAN]       = "AT+CWLAP",
    [WIFI_CONNECT]       = "AT+CWJAP",
	[WIFI_CONNECT_TIME_SERVER]= "AT+CIPSTART=\"UDP\",\"time.bora.net\",123\r\n",
};

void send_wifi_command (Wifi_Command_e cmd)
{
	char tx_buffer[WIFI_CMD_MAX_LEN];

	switch(cmd)
	{
		case WIFI_CONNECT:
			sprintf((char*)tx_buffer, (const char*)"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", g_selected_wifi_info.ssid, g_wifi_password);
			HAL_UART_Transmit_IT(&huart1, (const uint8_t*) tx_buffer, strlen(tx_buffer));
			break;

		case WIFI_CONNECT_TIME_SERVER:
			sprintf((char*)tx_buffer, (const char*)wifi_command_list[WIFI_CONNECT_TIME_SERVER]);
			HAL_UART_Transmit_IT(&huart1, (const uint8_t*) tx_buffer, strlen(tx_buffer));
			Time.wifi_connect_check_delay = 1000;
			break;

		default:
			snprintf((char*)tx_buffer, sizeof(tx_buffer), (const char*) "%s\r\n", wifi_command_list[cmd]);
			HAL_UART_Transmit_IT(&huart1, (const uint8_t*) tx_buffer, strlen(wifi_command_list[cmd])+2);
			break;
	}

	return;
}

void send_wifi_packet (uint8_t* data, uint16_t data_len)
{
	char tx_buffer[WIFI_CMD_MAX_LEN] = {0x00,};

	snprintf(tx_buffer, sizeof(tx_buffer), "AT+CIPSEND=%d\r\n", data_len);
	HAL_UART_Transmit_IT(&huart1, (const uint8_t*) tx_buffer, strlen(tx_buffer));

	HAL_Delay(1000);

	HAL_UART_Transmit_IT(&huart1, (uint8_t *)data, data_len);
}

void init_ESP01(void)
{
    //send_wifi_command (WIFI_MODULE_CHECK);
    HAL_Delay(100);
    send_wifi_command (WIFI_RESET);
    HAL_Delay(4000);
    send_wifi_command (WIFI_STATION_MODE);
    HAL_Delay(100);
    send_wifi_command (WIFI_AP_SCAN);
	char tx_buffer[WIFI_CMD_MAX_LEN];
	HAL_Delay(1000);
    sprintf((char*)tx_buffer, "AT+CWHOSTNAME=\"jhji\"\r\n");
    HAL_UART_Transmit_IT(&huart1, (uint8_t*)tx_buffer, strlen(tx_buffer));


}
