#include "headers.h"

uint8_t g_disp_update_flag = FALSE;

void display_process(void)
{
	brightness_control();

	if(g_disp_update_flag)
	{
		g_disp_update_flag = FALSE;

		switch(g_system_status.screen_status)
		{
			case WIFI_SCREEN :
				disp_update_wifi();
				break;

			case MAIN_SCREEN :
				disp_update_main();
				break;

			case RESERVATION_SCREEN :
				disp_update_reservation();
				break;

			default:
				break;
		}
	}
	return;
}

void display_keypad()
{
	uint8_t invert = FALSE;
	uint8_t page = 3;
	uint8_t col = 0;
	const char* keypad = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (*keypad)
    {
    	if(*keypad == 'a' || *keypad == 'n' || *keypad == 'A' || *keypad == 'N')
    	{
    		page++;
    		col = 0;
    	}

        if(g_system_status.cursor_line == page-3 && g_system_status.cursor_col*6 == col)
        {
        	invert = TRUE;
        	if(g_system_status.selected == TRUE && g_wifi_password_idx < MAX_WIFI_PASSWORD_LEN)
			{
				g_system_status.selected = FALSE;
				g_wifi_password[g_wifi_password_idx++] = *keypad;
			}
        }
        else
        {
        	invert = FALSE;
        }

        ssd1309_draw_char(page, col, *keypad, invert);
        col += FONT_WIDTH + 1;
        keypad++;
    }
    if(g_system_status.cursor_col == 13)
    {
    	ssd1309_print(OK_Y_POS, OK_X_POS, "OK", BLACK);
    }
    else
    {
    	ssd1309_print(OK_Y_POS, OK_X_POS, "OK", WHITE);
    }
}

void disp_temp_humid(void)
{
	uint8_t buf[16] = {0,};

	sprintf((char*)buf,"%u", g_temperature);
	ssd1309_print(0, TEMP_START_COL, (char*)buf, WHITE);
	ssd1309_goto(0, TEMP_START_COL + 12);
	buf[0]=0x03;
	buf[1]=0x03;
	ssd1309_send_data(buf, 2);
	memset(buf,0,sizeof(buf));
	sprintf((char*)buf,"C / %u%%", g_humidity);
	ssd1309_print(0, TEMP_START_COL + 12+3,(char*)buf, WHITE);
}

void disp_time(void)
{
	ssd1309_print(TIME_Y_POS, TIME_X_POS, Time.current_time_str, 0);
}

void disp_radio_freq(void)
{
	char freq_info[16] = {0,};

	if(g_radio_freq > 99.98)
	{
		snprintf(freq_info, sizeof(freq_info), "%.1f MHz", g_radio_freq);
		ssd1309_print_24(RADIO_FREQ_Y_POS, 0, freq_info, 0);
	}
	else
	{
		snprintf(freq_info, sizeof(freq_info), "% .1f MHz", g_radio_freq);
		ssd1309_print_24(RADIO_FREQ_Y_POS, 0, freq_info, 0);
	}
}

void disp_reservations(void)
{
	char str[64] = {0,};
	for(uint8_t i=0 ; i<6 ; i++)
	{
		if(g_reservation_info[i].is_valid == TRUE)
		{
			sprintf(str, "%u:  %02u:%02u  %.1f MHz", i+1, g_reservation_info[i].hour, g_reservation_info[i].min, g_reservation_info[i].freq);
			ssd1309_print(i+2, 2, str, WHITE);
		}

	}
}
void disp_update_main(void)
{
	if(g_system_status.work_type == RETURN_MAIN_SCREEN)
	{
		if(g_system_status.previous_work_type != RETURN_MAIN_SCREEN)
		{
			ssd1309_fill_color(0x00);
			ssd1309_draw_signal_strength(0, 1, wifi_sig_lv_select(), WHITE);
			ssd1309_draw_line(1);
			g_system_status.previous_work_type = RETURN_MAIN_SCREEN;
			g_system_status.work_type = NONE;
			g_system_status.screen_status = MAIN_SCREEN;
			//ssd1309_print_24(RADIO_FREQ_Y_POS, 0, " 89.1 MHz", 0);
		}
	}
	else if(g_system_status.work_type == NONE)
	{
		g_system_status.previous_work_type = NONE;

	}
	disp_time();
	disp_temp_humid();
	disp_radio_freq();
}

void disp_update_reservation(void)
{
	if(g_system_status.work_type == ENTER || g_system_status.work_type == DELETE)
	{
		g_system_status.previous_work_type = ENTER;
		g_system_status.work_type = NONE;
		ssd1309_fill_color(0x00);
		ssd1309_print(0, 60, "Reservation", WHITE);
		ssd1309_draw_signal_strength(0, 1, wifi_sig_lv_select(), WHITE);
		ssd1309_draw_line(1);
		disp_time();
		disp_reservations();
	}
	else if(g_system_status.work_type == NONE)
	{
		g_system_status.previous_work_type = NONE;
		disp_time();
		disp_reservations();
	}

}

void disp_update_wifi(void)
{
	uint8_t sig_lv = 0;
	if(g_system_status.work_type == SEARCH_RESULT)
	{
		if(g_system_status.previous_work_type != SEARCH_RESULT)
		{
			ssd1309_fill_color(0x00);
			ssd1309_print(0, 0, "SEARCHED WIFI LIST",WHITE);
		}

		for(uint8_t i=2;i<8;i++)
		{
			ssd1309_fill_color_one_line(0x00, i);
		}

		ssd1309_fill_color_one_line(0xFF, g_system_status.cursor_line+START_CURSER_LINE);

		for(uint8_t i=START_CURSER_LINE;i<PAGE_COUNT;i++)
		{
			if(g_wifi_info[i-START_CURSER_LINE].ssid[0] == '\0')
			{
				break;
			}

			if(g_wifi_info[i-START_CURSER_LINE].rssi > -50)
			{
				sig_lv = 2;
			}
			else if(g_wifi_info[i-START_CURSER_LINE].rssi > -80)
			{
				sig_lv = 1;
			}
			else
			{
				sig_lv = 0;
			}

			if(i == g_system_status.cursor_line+START_CURSER_LINE)
			{
				ssd1309_draw_signal_strength(i, 1, sig_lv, BLACK);
				ssd1309_print(i, 8, g_wifi_info[i-START_CURSER_LINE].ssid, BLACK);
			}
			else
			{
				ssd1309_draw_signal_strength(i, 1, sig_lv, WHITE);
				ssd1309_print(i, 8, g_wifi_info[i-START_CURSER_LINE].ssid, WHITE);
			}
		}
		g_system_status.previous_work_type = SEARCH_RESULT;
	}
	else if(g_system_status.work_type == INPUT_PASSWORD)
	{
		if(g_system_status.previous_work_type != INPUT_PASSWORD)
		{
			ssd1309_fill_color(0x00);
			ssd1309_print(0, 0, "INPUT PASSWORD",WHITE);
			g_system_status.cursor_line = g_system_status.cursor_col = 0;
			ssd1309_print(1, 0,"____________________", WHITE);
		}

		display_keypad();

		if(g_wifi_password_idx > 0)
		{
			ssd1309_print(1, 0, g_wifi_password, WHITE);
		}

		g_system_status.previous_work_type = INPUT_PASSWORD;
	}
	else if(g_system_status.work_type == CONNETCING)
	{
		if(g_system_status.previous_work_type != CONNETCING)
		{
			ssd1309_fill_color(0x00);
			ssd1309_print(3, 25, "CONNECTING ...",WHITE);
			g_system_status.cursor_line = g_system_status.cursor_col = 0;
		}
		g_system_status.previous_work_type = CONNETCING;

	}
	else if(g_system_status.work_type == RESULT)
	{
		if(g_system_status.previous_work_type != RESULT)
		{
			ssd1309_fill_color(0x00);
			ssd1309_print(3, 38, "CONNECTED",WHITE);
			g_system_status.previous_work_type = RESULT;
		}
	}
	else if(g_system_status.work_type == RETURN_MAIN_SCREEN)
	{

	}
	return;
}

void brightness_change(uint8_t contrast)
{
    ssd1309_send_command(0x81);        // Contrast Control command
    ssd1309_send_command(contrast);
}
