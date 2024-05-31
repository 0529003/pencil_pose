#ifndef _ESPNOW_H_
#define _ESPNOW_H_

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "nvs_flash.h"
#include "esp_random.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_now.h"
#include "esp_crc.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief 初始化WIFI函数
 * 启动WIFI
 */
void wifi_init(void);

/**
 * @brief 初始化espnow函数
 * 启动espnow
 */
esp_err_t espnow_init(void);

// ESPNOW发送任务
void espnow_tasks(void *pvParameter);

#ifdef __cplusplus
}
#endif


#endif /* _ESPNOW_H_ */
