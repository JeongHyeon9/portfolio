#ifndef INC_UTILL_H_
#define INC_UTILL_H_

#define FLASH_TARGET_ADDRESS 0x0801F800
#define FLASH_RESERVATION_DATA_ADDRESS  0x0801FC00
void Flash_WriteWord(uint32_t address, uint32_t data);

uint32_t Flash_ReadWord(uint32_t address);
void Flash_ReadReservationData(Reserve_t* dest, uint32_t length);
void init_dwt(void);
void delay_us(uint32_t us);
void init_system_status(void);
void flash_process(void);

#endif /* INC_UTILL_H_ */
