#include "headers.h"

double g_radio_freq = 89.1; //default
double g_radio_freq_backup = 0;
void radio_process(void)
{
	if(g_system_status.screen_status == MAIN_SCREEN && g_system_status.radio_freq_set_flag == TRUE)
	{
		 if(g_system_status.radio_mute_flag == FALSE)
		 {
			 TEA5767_SetFrequency(g_radio_freq);
		 }
		 else
		 {
			 TEA5767_SetMute(g_radio_freq);
		 }
		 g_system_status.radio_freq_set_flag = FALSE;
	}
}
