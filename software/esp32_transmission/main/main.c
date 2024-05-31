#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_GPIO GPIO_NUM_10

void app_main(void)
{
	// 初始化LED GPIO为输出模式
	gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

	// ESP_LOGI("main", "SD初始化!!!");
	// sd_card_init();
	// ESP_LOGI("main", "SD测试");
	// sd_card_test();


	// 点灯
	for (;;)
	{
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		gpio_set_level(LED_GPIO, 1);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		gpio_set_level(LED_GPIO, 0);
		ESP_LOGI("main", "Hello World!");
	}
}
