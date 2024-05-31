#include <stdio.h>
#include "serial_screen.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "string.h"

#define TX_GPIO_NUM 9
#define RX_GPIO_NUM 8

// 配置串口3
void uart_config(void) {
    const uart_port_t uart_num = UART_NUM_2;

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };

    // 配置UART参数
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    // 设置UART引脚
    ESP_ERROR_CHECK(uart_set_pin(uart_num, TX_GPIO_NUM, RX_GPIO_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // 安装UART驱动程序，不使用事件队列
    const int uart_buffer_size = (1024 * 2);
    ESP_ERROR_CHECK(uart_driver_install(uart_num, uart_buffer_size, 0, 0, NULL, 0));
}

// 串口发送测试
void uart_send_receive_demo(void) {
    const uart_port_t uart_num = UART_NUM_2;
    char test_str[] = "page0.t0.txt=\"down\"\xFF\xFF\xFF";

    // 发送数据
    uart_write_bytes(uart_num, test_str, strlen(test_str));
}



