#include "headers.h"

uint16_t get_cds_value(void)
{
	uint16_t adc_value = 0;

    HAL_ADC_Start(&hadc1);

    HAL_ADC_PollForConversion(&hadc1, 100);

    adc_value = HAL_ADC_GetValue(&hadc1);

    HAL_ADC_Stop(&hadc1);

    return adc_value;
}

void brightness_control(void)
{
	static uint16_t cds_value = 0;

	if(Time.disp_brightness_check_delay == 0)
	{
		cds_value = get_cds_value();

		Time.disp_brightness_check_delay = 1000;

		if(cds_value > 3000)
		{
			brightness_change(BRIGHTNESS_LOW);
		}
		else if(cds_value > 2000)
		{
			brightness_change(BRIGHTNESS_MID);
		}
		else
		{
			brightness_change(BRIGHTNESS_HIGH);
		}
	}

	return;
}
