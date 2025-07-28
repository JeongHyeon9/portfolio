#ifndef _DISPLAY_H_
#define _DISPLAY_H_

void display_process(void);
void disp_update_wifi(void);
void brightness_change(uint8_t contrast);
void ssd1309_draw_line(uint8_t page);
void disp_update_main(void);
void disp_update_reservation(void);

#define BRIGHTNESS_LOW 0x0F
#define BRIGHTNESS_MID 0xBF
#define BRIGHTNESS_HIGH 0xFF

#endif /* INC_DISPLAY_H_ */
