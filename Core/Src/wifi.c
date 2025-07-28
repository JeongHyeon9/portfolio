#include "headers.h"

char g_wifi_password[20]={0,};
uint8_t g_wifi_password_idx = 0;
uint8_t g_wifi_packet = 0;
uint8_t g_wifi_response[MAX_BUFFER_SIZE] = {0,};

void init_wifi(void)
{
	memset(&g_wifi_buf, 0, sizeof(g_wifi_buf));
	memset(g_wifi_info, 0, sizeof(g_wifi_info));
	init_ESP01();

	return;
}

static void get_wifi_response(void)
{
	uint16_t index = 0;

	while(g_wifi_buf.head != g_wifi_buf.tail)
	{
		g_wifi_response[index++] = g_wifi_buf.buf[g_wifi_buf.tail++];

		if(g_wifi_buf.tail == MAX_BUFFER_SIZE)
		{
			g_wifi_buf.tail = 0;
		}

		if(g_wifi_response[index-4] == 'O'  &&
		   g_wifi_response[index-3] == 'K'  &&
		   g_wifi_response[index-2] == '\r' &&
		   g_wifi_response[index-1] == '\n')
		{
			g_wifi_packet--;
			break;
		}
	}
}

int compare_wifi_rssi(const void* a, const void* b)
{
    const Wifi_Info_t* wa = (const Wifi_Info_t*)a;
    const Wifi_Info_t* wb = (const Wifi_Info_t*)b;

    return wb->rssi - wa->rssi;
}

static void parse_wifi_response(void)
{
	char sended_command[20] = {0,};
	uint8_t counter=0;
	uint16_t wifi_enc_pos = 0;
	uint16_t ssid_start_pos = 0;
	uint16_t i = 0;
	uint16_t j = 0;

	while(g_wifi_response[i] != 0)
	{
		if(strncmp(&g_wifi_response[i], "AT", 2) == 0)
		{
			for(uint16_t index = 0 ; index < 20 ; index++)
			{
				if(g_wifi_response[i+index] == '\r')
				{
					break;
				}
				sended_command[index] = g_wifi_response[i+index];
			}

			if(strncmp(sended_command, wifi_command_list[WIFI_AP_SCAN], strlen(wifi_command_list[WIFI_AP_SCAN])) == 0)
			{
				g_system_status.work_type = SEARCH_RESULT;
			}
		}
		else if(strncmp(&g_wifi_response[i], "+IPD,", 5) == 0)
		{
			g_wifi_packet--;
			if(g_system_status.time_set_flag)
			{
				set_time_from_NTP_response(i);
			}
		}
		i++;
	}

	if(g_system_status.work_type == SEARCH_RESULT)
	{
		memset(g_wifi_info, 0, sizeof(g_wifi_info));

		for(i=7 ; i<MAX_BUFFER_SIZE-7 ; i++)
		{
			if(strncmp(&g_wifi_response[i-7], "+CWLAP:(", 8) == 0)
			{
				wifi_enc_pos = i+1;
				ssid_start_pos = i+4;

				for(j=ssid_start_pos ; j<(ssid_start_pos+128) && counter < MAX_WIFI_NUM ; j++)
				{
					if(g_wifi_response[j] == '"')
					{
						set_wifi_info (&i, &j, &counter, ssid_start_pos);
						break;
					}
				}
				g_disp_update_flag = TRUE;
			}
			qsort(g_wifi_info, counter, sizeof(Wifi_Info_t), compare_wifi_rssi);
		}
	}
	else if(g_system_status.work_type == CONNETCING)
	{
		for(uint16_t i=5 ; i<MAX_BUFFER_SIZE-7 ; i++)
		{
			if(strncmp(&g_wifi_response[i-5], "GOT IP", 6) == 0)
			{
				g_system_status.work_type++;
				g_disp_update_flag = TRUE;
			}
		}
	}
}
uint8_t wifi_sig_lv_select(void)
{
	uint8_t sig_lv=0;

	if(g_selected_wifi_info.rssi > -50)
	{
		sig_lv = 2;
	}
	else if(g_selected_wifi_info.rssi > -80)
	{
		sig_lv = 1;
	}
	else
	{
		sig_lv = 0;
	}

	return sig_lv;
}

void wifi_process(void)
{
	if(g_wifi_packet)
	{
		get_wifi_response();
		parse_wifi_response();
		memset(g_wifi_response, 0, sizeof(g_wifi_response));
	}

	if(g_system_status.screen_status == WIFI_SCREEN)
	{
		g_system_status.previous_screen_status = WIFI_SCREEN;

		switch(g_system_status.work_type)
		{
			case SEARCH_RESULT:
				if(g_system_status.selected == TRUE)
				{
					g_system_status.selected = FALSE;
					g_system_status.work_type++;
					memset(g_wifi_password, 0, sizeof(g_wifi_password));
					g_wifi_password_idx = 0;
					memcpy( g_selected_wifi_info.ssid, g_wifi_info[g_system_status.cursor_line].ssid, MAX_SSID_LEN);
					memcpy( &g_selected_wifi_info.rssi, &g_wifi_info[g_system_status.cursor_line].rssi, sizeof(g_selected_wifi_info.rssi));
				}
				break;

			case INPUT_PASSWORD:
				if(g_system_status.cursor_line > 4)
				{
					g_system_status.cursor_line = 4;
				}

				if(g_system_status.cursor_col == 13 && g_system_status.selected == TRUE)
				{

					g_system_status.work_type++;
					g_system_status.selected = FALSE;
					g_disp_update_flag = TRUE;
				}

				break;

			case CONNETCING:
				send_wifi_command(WIFI_CONNECT);
				HAL_Delay(2000);
				break;

			case RESULT:
				if(g_system_status.previous_work_type == RESULT)
				{
					g_system_status.work_type++;
					g_disp_update_flag = TRUE;
				}
				break;

			case RETURN_MAIN_SCREEN:
				g_disp_update_flag = TRUE;
				g_system_status.screen_status = MAIN_SCREEN;
				send_wifi_command(WIFI_CONNECT_TIME_SERVER);
				HAL_Delay(1000);
				get_time_from_NTP();
				HAL_Delay(1000);
				break;

			case NONE:
			default:
				break;
		}
	}
}

void set_wifi_info (uint16_t *i, uint16_t *j, uint8_t *counter, uint16_t ssid_start_pos)
{
	char rssi_str[3]= {0,};
	uint16_t ssid_len = 0;
	uint16_t wifi_enc_pos = 0;
	uint16_t wifi_rssi_pos1 = 0;
	uint16_t wifi_rssi_pos2 = 0;

	ssid_len = (*j) - ssid_start_pos;
	wifi_rssi_pos1 = (*j)+3;
	wifi_rssi_pos2 = (*j)+4;
	rssi_str[0] = g_wifi_response[wifi_rssi_pos1];
	rssi_str[1] = g_wifi_response[wifi_rssi_pos2];

	if(ssid_len == 0 || atoi(rssi_str) > WIFI_RSSI_THRESHOLD)
	{
		(*i) += ssid_len;
		return;
	}
	else if(ssid_len > MAX_SSID_LEN)
	{
		ssid_len = MAX_SSID_LEN;
	}

	g_wifi_info[*counter].enc = g_wifi_response[wifi_enc_pos] - '0';
	g_wifi_info[*counter].rssi = (-1)*atoi(rssi_str);
	strncpy((char*) g_wifi_info[*counter].ssid, (char*) &g_wifi_response[ssid_start_pos], ssid_len);
	(*counter)++;
	(*i)=(*j);
}
