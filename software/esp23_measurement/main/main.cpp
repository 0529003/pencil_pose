#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/message_buffer.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "parameter.h"
#include "nvs_flash.h"
#include "espnow.h"
#include "esp_now.h"
static const char *TAG = "MAIN";

QueueHandle_t xQueueTrans;

extern "C" {
	void app_main(void);
}

// MUP6050任务
void mpu6050(void *pvParameters);

void start_i2c(void) {
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = (gpio_num_t)CONFIG_GPIO_SDA;
	conf.scl_io_num = (gpio_num_t)CONFIG_GPIO_SCL;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = 400000;
	conf.clk_flags = 0;
	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));
}

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

	// Initialize i2c
	start_i2c();

	// Create Queue
	xQueueTrans = xQueueCreate(50, sizeof(POSE_t));
	configASSERT( xQueueTrans );

	// Start imu task
	xTaskCreate(&mpu6050, "IMU", 1024*8, NULL, 5, NULL);

	// Start espnow task
	xTaskCreate(&espnow_tasks, "ESPNOW", 1024*8, NULL, 5, NULL);

	// 读取队列
	POSE_t pose;
	while(1) {
		vTaskDelay(1);
		if(xQueueReceive(xQueueTrans, &pose, portMAX_DELAY)) {
			ESP_LOGD(TAG, "pose=%f %f %f", pose.roll, pose.pitch, pose.yaw);
		} else {
			ESP_LOGE(TAG, "xQueueReceive fail");
			break;
		}
	}
}
