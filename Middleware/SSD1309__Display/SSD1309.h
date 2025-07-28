#ifndef _SSD1306_H_
#define _SSD1306_H_

void init_ssd1309(void);
void ssd1309_fill_color(uint8_t color);
void ssd1309_print(uint8_t page, uint8_t col, const char* str, uint8_t invert);
void ssd1309_send_data(uint8_t* data, uint16_t size);
void ssd1309_print_10x14(uint8_t page, uint8_t col, const char* str);
void ssd1309_fill_color_one_line(uint8_t color, uint8_t line);
void ssd1309_draw_signal_strength(uint8_t page, uint8_t col, char c, uint8_t invert);
void ssd1309_draw_char(uint8_t page, uint8_t col, char c, uint8_t invert);
void ssd1309_goto(uint8_t page, uint8_t column);
void ssd1309_print_24(uint8_t page, uint8_t col, const char* str, uint8_t invert);
void ssd1309_send_command(uint8_t cmd);
#endif /* SSD1306__DISPLAY_SSD1306_H_ */
