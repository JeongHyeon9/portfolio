#ifndef _DEFINES_H_
#define _DEFINES_H_

#define TRUE 1
#define FALSE 0
#define HIGH 1
#define LOW 0
#define PLUS 1
#define MINUS 0
#define LINE 1
#define COL 0

//wifi
#define MAX_BUFFER_SIZE 5120
#define WIFI_CMD_MAX_LEN 256
#define MAX_SSID_LEN 20
#define WIFI_RSSI_THRESHOLD 85
#define MAX_WIFI_NUM 20
#define MAX_WIFI_PASSWORD_LEN 20
#define NTP_TIMESTAMP_DELTA 2208988800UL

//display
#define OLED_WIDTH    128
#define OLED_HEIGHT   64
#define PAGE_COUNT    (OLED_HEIGHT / 8)
#define START_CURSER_LINE 2
#define BLACK 1
#define WHITE 0

#define OK_X_POS 110
#define OK_Y_POS 7
#define TEMP_START_COL 70
#define TIME_X_POS 22
#define TIME_Y_POS 0
#define RADIO_FREQ_X_POS 12
#define RADIO_FREQ_Y_POS 4
//SSD1303
#define FONT_WIDTH 5
#define FONT_HEIGHT 7

//keypad
#define KEY_DELAY 100 //ms
#define KEY_DELAY_FREQ_UP_DONW 30
#define KEYPAD_NUMBER_LINE 0
#define KEYPAD_END_LINE 4


#define SET_DUTY(val)   (TIM2->CCR1 = (val))
#define BEEP_DELAY 100

#define TEA5767_ADDR  (0x60 << 1)  // HAL 라이브러리는 8-bit 주소 사용
#endif /* _DEFINES_H_ */
