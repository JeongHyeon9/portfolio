#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

typedef struct
{
	volatile uint16_t head;
	volatile uint16_t tail;
	uint8_t rx_data;
	uint8_t buf[MAX_BUFFER_SIZE];
}Ring_Buffer_t;

typedef struct
{
	uint8_t enc;
	int16_t rssi;
	char ssid[MAX_SSID_LEN];
}Wifi_Info_t;

typedef struct
{
	uint8_t screen_status;
	uint8_t previous_screen_status;
	uint8_t work_type;
	uint8_t previous_work_type;
	uint8_t cursor_line;
	uint8_t cursor_col;
	uint8_t selected;
	uint8_t key_event_flag;
	uint8_t time_set_flag;
	uint8_t radio_freq_set_flag;
	uint8_t radio_mute_flag;
	uint32_t serial_number;
	uint8_t num_of_reservations;
	uint8_t flash_write_flag;
}System_Status_t;

typedef struct
{
	uint32_t keypad_time;
	uint32_t disp_blink_time;
	uint32_t wifi_connect_check_delay;
	uint16_t key_input_delay;
	uint32_t beep_delay;
	uint32_t disp_brightness_check_delay;
	uint32_t temp_humid_check_delay;
	char current_time_str[32];
}Time_t;

typedef struct
{
	uint8_t hour;
	uint8_t min;
	double freq;
	uint8_t is_valid;
}Reserve_t;
#endif /* INC_STRUCTURES_H_ */
