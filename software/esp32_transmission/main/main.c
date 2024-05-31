#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "espnow.h"
#include "esp_now.h"
#include "nvs_flash.h"
#define LED_GPIO GPIO_NUM_10

void app_main(void)
{
	// Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	// Initialize WiFi
	wifi_init();

	// Initialize ESPNOW
	espnow_init();

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
