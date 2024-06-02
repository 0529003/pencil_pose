#include <stdio.h>
#include "serial_screen.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "string.h"
#include "esp_log.h"
#include "math.h"
static const char *TAG = "TEST";
#define TX_GPIO_NUM 9
#define RX_GPIO_NUM 8
extern	int first;
extern	float second;
extern  int third; 
extern  int fourth;
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
void uart_send_receive_demo() {
    const uart_port_t uart_num = UART_NUM_2;
    // char test_str[] = "page0.t0.txt=\"down\"\xFF\xFF\xFF";
    char test_str[50];
    char test_str_2[50];
    char test_str_3[50];
    char test_str_4[50];
    char fourth_transt[50];
    char fourth_transt_1;
    char aa[50]="";
    sprintf(fourth_transt,"%d",fourth);
    fourth_transt_1=fourth_transt[0];
    // ESP_LOGI(TAG, "FOUTH: %d", fourth_transt_1);
    sprintf(test_str,"page0.t0.txt=\"%d\"\xFF\xFF\xFF",first);
    sprintf(test_str_2,"page0.t1.txt=\"%f\"\xFF\xFF\xFF",second);
    sprintf(test_str_3,"page1.t0.txt=\"%d\"\xFF\xFF\xFF",third);
    sprintf(test_str_4,"page2.t0.txt=\"%d\"\xFF\xFF\xFF",fourth);
    float a=rand()%3+9;
    sprintf(aa,"page3.t0.txt=\"%f\"\xFF\xFF\xFF",a);
    strcat(test_str, test_str_2);
    strcat(test_str, test_str_3);
    strcat(test_str, test_str_4);
    // 发送数据
    uart_write_bytes(uart_num, test_str, strlen(test_str));
    uart_write_bytes(uart_num, aa, strlen(aa));
}



