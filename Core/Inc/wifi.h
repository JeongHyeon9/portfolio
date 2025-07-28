#ifndef _WIFI_H_
#define _WIFI_H_


void init_wifi(void);
void wifi_process(void);
uint8_t wifi_sig_lv_select(void);
void set_wifi_info (uint16_t *i, uint16_t *j, uint8_t *counter, uint16_t ssid_start_pos);

#endif /* INC_WIFI_H_ */
