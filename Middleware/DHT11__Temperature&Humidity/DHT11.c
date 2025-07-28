#include "headers.h"

void DHT11_set_pin_output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

void DHT11_set_pin_input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

uint8_t DHT11_Start(void)
{
	DHT11_set_pin_output();
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);  // 18ms 이상
    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
    delay_us(30);

    DHT11_set_pin_input();
    delay_us(40);

    if (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_RESET)
    {
        delay_us(80);
        if (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET)
        {
            delay_us(80);
            return TRUE;
        }
    }
    return FALSE;
}

uint8_t DHT11_ReadBit(void)
{
    while(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_RESET);
    delay_us(30);

    if (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET)
    {
        while(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET);
        return TRUE;
    }
    return FALSE;
}

uint8_t DHT11_ReadByte(void)
{
    uint8_t i, byte = 0;
    for(i = 0; i < 8; i++)
    {
        byte <<= 1;
        byte |= DHT11_ReadBit();
    }
    return byte;
}

uint8_t DHT11_Read(uint8_t *humidity, uint8_t *temperature)
{
    uint8_t hum_int, hum_dec, temp_int, temp_dec, checksum;

    if (DHT11_Start())
    {
        hum_int = DHT11_ReadByte();
        hum_dec = DHT11_ReadByte();
        temp_int = DHT11_ReadByte();
        temp_dec = DHT11_ReadByte();
        checksum = DHT11_ReadByte();

        if (checksum == (hum_int + hum_dec + temp_int + temp_dec))
        {
            *humidity = hum_int;
            *temperature = temp_int;
            return 1;
        }
    }
    return 0;
}

void temp_humid_process(void)
{
	if(g_system_status.screen_status != WIFI_SCREEN && Time.temp_humid_check_delay == 0)
	{
		Time.temp_humid_check_delay = 1000;
		DHT11_Read(&g_humidity, &g_temperature);
		g_disp_update_flag = TRUE;
	}
}
