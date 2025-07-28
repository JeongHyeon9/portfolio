#include "headers.h"

void keypad_process(void)
{
	if(Time.key_input_delay)
	{
		return;
	}

	switch(g_system_status.screen_status)
	{
		case WIFI_SCREEN:
			keypad_process_wifi_screen();
			break;
		case MAIN_SCREEN:
			keypad_process_main_screen();
			break;
		case RESERVATION_SCREEN:
			keypad_process_reservation_screen();
			break;
		default:
			break;
	}
}
void keypad_process_wifi_screen(void)
{
	if(HAL_GPIO_ReadPin(SELECT_KEY_GPIO_Port, SELECT_KEY_Pin) == LOW)
	{
		if(g_system_status.key_event_flag == FALSE)
		{
			Time.keypad_time = 0;
			g_system_status.key_event_flag = TRUE;
		}
		else if(g_system_status.key_event_flag == TRUE &&  Time.keypad_time > 100)
		{
			g_system_status.key_event_flag = FALSE;
			g_system_status.selected = TRUE;
			g_disp_update_flag = TRUE;
			Time.key_input_delay = KEY_DELAY;
			Time.beep_delay = BEEP_DELAY;
			SET_DUTY(499);
		}
	}
	else if(HAL_GPIO_ReadPin(DOWN_KEY_GPIO_Port, DOWN_KEY_Pin) == LOW)
	{
		if(g_system_status.key_event_flag == FALSE)
		{
			Time.keypad_time = 0;
			g_system_status.key_event_flag = TRUE;
		}
		else if(g_system_status.key_event_flag == TRUE &&  Time.keypad_time > 100)
		{
			g_system_status.key_event_flag = FALSE;
			if(g_system_status.cursor_line < 5)
			{
				handle_key_input_feedback(LINE, PLUS);
			}
		}
	}
	else if(HAL_GPIO_ReadPin(UP_KEY_GPIO_Port, UP_KEY_Pin) == LOW)
	{
		if(g_system_status.key_event_flag == FALSE)
		{
			Time.keypad_time = 0;
			g_system_status.key_event_flag = TRUE;
		}
		else if(g_system_status.key_event_flag == TRUE &&  Time.keypad_time > 100)
		{
			g_system_status.key_event_flag = FALSE;
			if(g_system_status.cursor_line > 0)
			{
				handle_key_input_feedback(LINE, MINUS);
			}
		}
	}
	else if(HAL_GPIO_ReadPin(LEFT_KEY_GPIO_Port, LEFT_KEY_Pin) == LOW)
	{
		if(g_system_status.key_event_flag == FALSE)
		{
			Time.keypad_time = 0;
			g_system_status.key_event_flag = TRUE;
		}
		else if(g_system_status.key_event_flag == TRUE &&  Time.keypad_time > 100)
		{
			g_system_status.key_event_flag = FALSE;
			if(g_system_status.cursor_col > 0)
			{
				handle_key_input_feedback(COL, MINUS);
			}
		}
	}
	else if(HAL_GPIO_ReadPin(RIGHT_KEY_GPIO_Port, RIGHT_KEY_Pin) == LOW)
	{
		if(g_system_status.key_event_flag == FALSE)
		{
			Time.keypad_time = 0;
			g_system_status.key_event_flag = TRUE;
		}
		else if(g_system_status.key_event_flag == TRUE &&  Time.keypad_time > 100)
		{
			g_system_status.key_event_flag = FALSE;

			if(g_system_status.cursor_col < 12 && g_system_status.cursor_line != KEYPAD_NUMBER_LINE)
			{
				handle_key_input_feedback(COL, PLUS);
			}
			else if(g_system_status.cursor_col < 9 && g_system_status.cursor_line == KEYPAD_NUMBER_LINE)
			{
				handle_key_input_feedback(COL, PLUS);
			}
			else if(g_system_status.cursor_col < 13 && g_system_status.cursor_line == KEYPAD_END_LINE)
			{
				handle_key_input_feedback(COL, PLUS);
			}
		}
	}
	else if(g_system_status.key_event_flag == TRUE && Time.keypad_time > KEY_DELAY)
	{
		g_system_status.key_event_flag = FALSE;
	}
}

void keypad_process_main_screen(void)
{
	if(HAL_GPIO_ReadPin(SELECT_KEY_GPIO_Port, SELECT_KEY_Pin) == LOW)
	{
		if(g_system_status.key_event_flag == FALSE)
		{
			Time.keypad_time = 0;
			g_system_status.key_event_flag = TRUE;
		}
		else if(g_system_status.key_event_flag == TRUE &&  Time.keypad_time > 100)
		{
			g_system_status.key_event_flag = FALSE;
			g_system_status.selected = TRUE;
			g_system_status.radio_freq_set_flag = TRUE;
			if(g_radio_freq_backup == g_radio_freq)
			{
				g_system_status.radio_mute_flag = !g_system_status.radio_mute_flag;
			}
			else
			{
				g_system_status.radio_mute_flag = FALSE;
				g_radio_freq_backup = g_radio_freq;
			}

			g_disp_update_flag = TRUE;
			Time.key_input_delay = KEY_DELAY;
			Time.beep_delay = BEEP_DELAY;
			SET_DUTY(499);
		}
	}
	else if(HAL_GPIO_ReadPin(DOWN_KEY_GPIO_Port, DOWN_KEY_Pin) == LOW)
	{
		if(g_system_status.key_event_flag == FALSE)
		{
			Time.keypad_time = 0;
			g_system_status.key_event_flag = TRUE;
		}
		else if(g_system_status.key_event_flag == TRUE &&  Time.keypad_time > 50)
		{
			g_system_status.key_event_flag = FALSE;
			handle_key_input_feedback(LINE, MINUS);
		}
	}
	else if(HAL_GPIO_ReadPin(UP_KEY_GPIO_Port, UP_KEY_Pin) == LOW)
	{
		if(g_system_status.key_event_flag == FALSE)
		{
			Time.keypad_time = 0;
			g_system_status.key_event_flag = TRUE;
		}
		else if(g_system_status.key_event_flag == TRUE &&  Time.keypad_time > 50)
		{
			g_system_status.key_event_flag = FALSE;
			handle_key_input_feedback(LINE, PLUS);
		}
	}
	else if(HAL_GPIO_ReadPin(RIGHT_KEY_GPIO_Port, RIGHT_KEY_Pin) == LOW)
	{
		if(g_system_status.key_event_flag == FALSE)
		{
			Time.keypad_time = 0;
			g_system_status.key_event_flag = TRUE;
		}
		else if(g_system_status.key_event_flag == TRUE &&  Time.keypad_time > 100)
		{
			g_system_status.key_event_flag = FALSE;
			g_system_status.work_type = ENTER;
			g_disp_update_flag = TRUE;
			g_system_status.cursor_line = g_system_status.cursor_col = 0;
			g_system_status.screen_status = RESERVATION_SCREEN;
		}
	}
}

void keypad_process_reservation_screen(void)
{
	if(HAL_GPIO_ReadPin(SELECT_KEY_GPIO_Port, SELECT_KEY_Pin) == LOW)
	{
		if(g_system_status.key_event_flag == FALSE)
		{
			Time.keypad_time = 0;
			g_system_status.key_event_flag = TRUE;
		}
		else if(g_system_status.key_event_flag == TRUE &&  Time.keypad_time > 100)
		{
			g_system_status.key_event_flag = FALSE;
			g_system_status.selected = TRUE;
			g_disp_update_flag = TRUE;
			Time.key_input_delay = KEY_DELAY;
			Time.beep_delay = BEEP_DELAY;
			SET_DUTY(499);
		}
	}
	else if(HAL_GPIO_ReadPin(DOWN_KEY_GPIO_Port, DOWN_KEY_Pin) == LOW)
	{
		if(g_system_status.key_event_flag == FALSE)
		{
			Time.keypad_time = 0;
			g_system_status.key_event_flag = TRUE;
		}
		else if(g_system_status.key_event_flag == TRUE &&  Time.keypad_time > 50)
		{
			g_system_status.key_event_flag = FALSE;
			if(g_system_status.cursor_line < 5)
			{
				handle_key_input_feedback(LINE, PLUS);
			}
		}
	}
	else if(HAL_GPIO_ReadPin(UP_KEY_GPIO_Port, UP_KEY_Pin) == LOW)
	{
		if(g_system_status.key_event_flag == FALSE)
		{
			Time.keypad_time = 0;
			g_system_status.key_event_flag = TRUE;
		}
		else if(g_system_status.key_event_flag == TRUE &&  Time.keypad_time > 50)
		{
			g_system_status.key_event_flag = FALSE;
			if(g_system_status.cursor_line > 0)
			{
				handle_key_input_feedback(LINE, MINUS);
			}
		}
	}
	else if(HAL_GPIO_ReadPin(LEFT_KEY_GPIO_Port, LEFT_KEY_Pin) == LOW)
	{
		if(g_system_status.key_event_flag == FALSE)
		{
			Time.keypad_time = 0;
			g_system_status.key_event_flag = TRUE;
		}
		else if(g_system_status.key_event_flag == TRUE &&  Time.keypad_time > 100)
		{
			g_system_status.key_event_flag = FALSE;
			g_disp_update_flag = TRUE;
			g_system_status.screen_status = MAIN_SCREEN;
			g_system_status.work_type = RETURN_MAIN_SCREEN;
		}
	}
}

void handle_key_input_feedback(uint8_t line_or_col, uint8_t plus_or_minus)
{

	switch(g_system_status.screen_status)
	{
		case RESERVATION_SCREEN:
		case WIFI_SCREEN :
			Time.key_input_delay = KEY_DELAY;
			if(line_or_col == LINE)
			{
				plus_or_minus ? g_system_status.cursor_line++ : g_system_status.cursor_line--;
			}
			else if(line_or_col == COL)
			{
				plus_or_minus ? g_system_status.cursor_col++ : g_system_status.cursor_col--;
			}
			else
			{
				return;
			}
			Time.beep_delay = BEEP_DELAY;
			SET_DUTY(499);
			break;
		case MAIN_SCREEN:
			Time.key_input_delay = KEY_DELAY_FREQ_UP_DONW;
			if(plus_or_minus == PLUS)
			{
				g_radio_freq += 0.1;
			}
			else if(plus_or_minus == MINUS)
			{
				g_radio_freq -=0.1;
			}
			else
			{
				return;
			}
			break;
		default:
			break;
	}
	g_disp_update_flag = TRUE;
}
