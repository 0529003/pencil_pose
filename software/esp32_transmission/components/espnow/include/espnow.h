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

/**
 * @brief 初始化WIFI函数
 * 启动WIFI
 */
void wifi_init(void);

/**
 * @brief 初始化espnow函数
 * 启动espnow
 */
void espnow_init(void);

/**
 * @brief 测试espnow函数
 * 测试espnow
 */
void espnow_rec(void);

/**
 * @brief 测试espnow_sen函数
 * 测试espnow_sen
 */
void espnow_sen(void);


#endif /* _ESPNOW_H_ */
