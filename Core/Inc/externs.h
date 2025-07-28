#ifndef _EXTERNS_H_
#define _EXTERNS_H_

extern I2C_HandleTypeDef hi2c1;
extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;
extern RTC_TimeTypeDef sTime_backup;
extern RTC_DateTypeDef sDate_backup;
//main.c
extern System_Status_t g_system_status;
extern uint8_t g_temperature;
extern uint8_t g_humidity;
//cds.c
extern ADC_HandleTypeDef hadc1;

//wifi.c
extern UART_HandleTypeDef huart1;
extern Ring_Buffer_t g_wifi_buf;
extern Wifi_Info_t g_wifi_info[20];
extern Wifi_Info_t g_selected_wifi_info;
extern char g_wifi_password[20];
extern uint8_t g_wifi_password_idx;
extern uint8_t g_wifi_packet;
extern uint8_t g_wifi_response[MAX_BUFFER_SIZE];

//ESP01.c
extern const char* wifi_command_list[WIFI_CMD_COUNT];

//display.c
extern SPI_HandleTypeDef hspi1;
extern uint8_t g_disp_update_flag;

extern Time_t Time;

extern double g_radio_freq;
extern double g_radio_freq_backup;
extern Reserve_t g_reservation_info[6];
#endif /* INC_EXTERNS_H_ */
