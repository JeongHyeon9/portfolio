#include "headers.h"
#include <time.h>

void time_process(void)
{
	if(g_system_status.screen_status != WIFI_SCREEN)
	{
		HAL_RTC_GetTime(&hrtc,&sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc,&sDate, RTC_FORMAT_BIN);

		if(sTime_backup.Minutes != sTime.Minutes)
		{
			g_disp_update_flag = TRUE;
			memset(Time.current_time_str, 0, sizeof(Time.current_time_str));
			snprintf(Time.current_time_str, sizeof(Time.current_time_str), "%02u:%02u", sTime.Hours, sTime.Minutes);
		}

		memcpy(&sTime_backup, &sTime, sizeof(RTC_TimeTypeDef));
	}
}

void set_time_from_NTP_response(uint16_t idx)
{
	uint32_t ntp_time = 0;

	g_system_status.time_set_flag = FALSE;
	g_disp_update_flag = TRUE;

	ntp_time = (g_wifi_response[idx+48]<<24) | (g_wifi_response[idx+49]<<16) | (g_wifi_response[idx+50]<<8) | (g_wifi_response[idx+51]);

	time_t raw_time = (time_t)ntp_time;
	struct tm *t = gmtime(&raw_time);

	sTime.Hours   = (t->tm_hour + 9) % 24;;
	sTime.Minutes = t->tm_min;
	sTime.Seconds = t->tm_sec;

	sDate.Year  = t->tm_year - 100;  // tm_year = 2025 - 1900 → 125 → 25
	sDate.Month = t->tm_mon + 1;     // tm_mon = 0~11 → RTC는 1~12
	sDate.Date  = t->tm_mday;

	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	snprintf(Time.current_time_str, sizeof(Time.current_time_str), "%02u:%02u", sTime.Hours, sTime.Minutes);
}

void get_time_from_NTP(void)
{
	uint8_t ntp_packet[48] = {0};
	ntp_packet[0] = 0b11100011;  // LI = 0, VN = 3, Mode = 3 (client)

	send_wifi_packet(ntp_packet, 48);
	g_system_status.time_set_flag = TRUE;
}
