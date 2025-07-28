#ifndef _ESP01_H_
#define _ESP01_H_

void send_wifi_command (Wifi_Command_e cmd);
void init_ESP01(void);
void send_wifi_packet (uint8_t* data, uint16_t data_len);

#endif /* ESP01__WIFI_ESP01_H_ */
