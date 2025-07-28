#ifndef _DHT11_H_
#define _DHT11_H_

void DHT11_SetPinInput(void);
void DHT11_SetPinOutput(void);
uint8_t DHT11_Start(void);
uint8_t DHT11_ReadBit(void);
uint8_t DHT11_ReadByte(void);
uint8_t DHT11_Read(uint8_t *humidity, uint8_t *temperature);
void temp_humid_process(void);

#endif /* DHT11__TEMPERATURE_HUMIDITY_DHT11_H_ */
