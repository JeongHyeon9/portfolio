#include "headers.h"
#define TEA5767_ADDRESS_WRITE   0xC0
#define TEA5767_ADDRESS_READ    0xC1

void TEA5767_SetFrequency(double frequency_hz)
{
    uint8_t tx_buffer[5]={0x00,};
    uint32_t freq_word=0;

    g_radio_freq_backup = frequency_hz;

    freq_word = (uint16_t)((4 * (frequency_hz * 1000000 + 225000)) / 32768);

    tx_buffer[0] = (uint8_t)((freq_word >> 8));
    tx_buffer[1] = (uint8_t)(freq_word & 0xFF);
    tx_buffer[2] = 0xB0;
    tx_buffer[3] = 0x10;
    tx_buffer[4] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, TEA5767_ADDRESS_WRITE, tx_buffer, 5, HAL_MAX_DELAY);


}

void TEA5767_SetMute(double frequency_hz)
{
    uint8_t tx_buffer[5]={0x00,};
    uint32_t freq_word=0;

    freq_word = (uint16_t)((4 * (frequency_hz * 1000000 + 225000)) / 32768);

    tx_buffer[0] = (uint8_t)((freq_word >> 8) | 0x80);
    tx_buffer[1] = (uint8_t)(freq_word & 0xFF);
    tx_buffer[2] = 0xB0;
    tx_buffer[3] = 0x10;
    tx_buffer[4] = 0x00;
    HAL_I2C_Master_Transmit(&hi2c1, TEA5767_ADDRESS_WRITE, tx_buffer, 5, HAL_MAX_DELAY);


}
