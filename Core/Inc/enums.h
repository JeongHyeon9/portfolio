#ifndef INC_ENUMS_H_
#define INC_ENUMS_H_

//wifi
typedef enum
{
    WIFI_RESET,
	WIFI_MODULE_CHECK,
    WIFI_STATION_MODE,
	WIFI_AP_SCAN,
	WIFI_CONNECT,
	WIFI_CONNECT_TIME_SERVER,
    WIFI_CMD_COUNT
} Wifi_Command_e;

typedef enum
{
	NONE,
	SEARCH,
	SEARCH_RESULT,
	INPUT_PASSWORD,
	CONNETCING,
	RESULT,
	RETURN_MAIN_SCREEN
}Wifi_Work_Type_e;

//main
typedef enum
{
	MAIN_SCREEN,
	WIFI_SCREEN,
	RESERVATION_SCREEN,
} Screen_Type_e;

typedef enum
{
	ENTER = 1,
	DELETE,
}RESERVATION_Work_Type_e;

typedef enum
{
	RADIO_OFF,
	RADIO_ON,
} RADIO_SET_e;
#endif /* INC_ENUMS_H_ */
