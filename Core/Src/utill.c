#include "headers.h"
#include "stm32f1xx_hal.h"

void Flash_WriteWord(uint32_t address, uint32_t data)
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef eraseInit;
    uint32_t pageError = 0;
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.PageAddress = address;
    eraseInit.NbPages = 1;

    HAL_FLASHEx_Erase(&eraseInit, &pageError);

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data);

    HAL_FLASH_Lock();
}

void Flash_WriteReservationData(Reserve_t* data, uint32_t length)
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef erase;
    uint32_t page_error = 0;
    erase.TypeErase = FLASH_TYPEERASE_PAGES;
    erase.PageAddress = FLASH_RESERVATION_DATA_ADDRESS;
    erase.NbPages = 1;
    HAL_FLASHEx_Erase(&erase, &page_error);

    uint32_t address = FLASH_RESERVATION_DATA_ADDRESS;
    uint8_t* raw = (uint8_t*)data;
    uint32_t total_bytes = sizeof(Reserve_t) * length;

    for (uint32_t i = 0; i < total_bytes; i += 4)
    {
        uint32_t word = 0xFFFFFFFF;
        memcpy(&word, &raw[i], (total_bytes - i >= 4) ? 4 : (total_bytes - i));
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, word);
        address += 4;
    }

    HAL_FLASH_Lock();
}

void Flash_ReadReservationData(Reserve_t* dest, uint32_t length)
{
    memcpy(dest, (void*)FLASH_RESERVATION_DATA_ADDRESS, sizeof(Reserve_t) * length);
}

uint32_t Flash_ReadWord(uint32_t address)
{
    return *(uint32_t*)address;
}

void init_dwt(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (HAL_RCC_GetHCLKFreq() / 1000000);

    while ((DWT->CYCCNT - start) < ticks);
}

void init_system_status(void)
{
	  memset(&g_system_status, 0, sizeof(System_Status_t));
	  memset(&g_reservation_info, 0, sizeof(g_reservation_info));

	  g_system_status.screen_status = WIFI_SCREEN;
	 // g_system_status.radio_mute_flag = TRUE;
	  g_system_status.serial_number = Flash_ReadWord(FLASH_TARGET_ADDRESS);

	  Time.disp_brightness_check_delay = 1000;
	  Time.temp_humid_check_delay = 1000;

	  Flash_ReadReservationData(g_reservation_info, 6);

	  if(g_reservation_info[0].is_valid == 0xFF)
	  {
		  memset(&g_reservation_info, 0, sizeof(g_reservation_info));
		  Flash_WriteReservationData(g_reservation_info, 6);
	  }

	  DHT11_Read(&g_humidity, &g_temperature);
}
void reorder_reservations(void)
{
    Reserve_t temp[6];
    int idx = 0;

    // 1단계: is_valid == TRUE 인 항목을 앞에 복사
    for (int i = 0; i < 6; ++i)
    {
        if (g_reservation_info[i].is_valid)
        {
            temp[idx++] = g_reservation_info[i];
        }
    }

    // 2단계: 나머지 항목 (is_valid == FALSE)
    for (int i = 0; i < 6; ++i)
    {
        if (!g_reservation_info[i].is_valid)
        {
            temp[idx++] = g_reservation_info[i];
        }
    }

    // 3단계: 다시 원래 배열에 복사
    memcpy(g_reservation_info, temp, sizeof(g_reservation_info));
}
void reservation_process(void)
{
	if(g_system_status.flash_write_flag == TRUE)
	{
		 g_system_status.flash_write_flag = FALSE;
		 reorder_reservations();
		 Flash_WriteReservationData(g_reservation_info, 6);
		 g_disp_update_flag = TRUE;
	}

	for(uint8_t i=0 ; i<6 ; i++)
	{
		if(g_reservation_info[i].is_valid == TRUE &&
		   g_reservation_info[i].hour == sTime.Hours &&
		   g_reservation_info[i].min == sTime.Minutes)
		{
			g_radio_freq = g_reservation_info[i].freq;
		    g_system_status.radio_freq_set_flag = TRUE;
		    g_disp_update_flag = TRUE;
		}
	}

}
