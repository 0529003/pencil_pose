idf_component_register(SRCS "espnow.c"
                    INCLUDE_DIRS "include"
                    REQUIRES log freertos driver nvs_flash esp_event esp_netif esp_wifi
                    )
