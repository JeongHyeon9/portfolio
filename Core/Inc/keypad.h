#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

void handle_key_input_feedback(uint8_t line_or_col, uint8_t plus_or_minus);
void keypad_process(void);
void keypad_process_wifi_screen(void);
void keypad_process_main_screen(void);
void keypad_process_reservation_screen(void);
#endif /* INC_KEYPAD_H_ */
