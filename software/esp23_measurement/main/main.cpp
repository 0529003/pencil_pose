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
#include "math.h"
#include "time.h"
#define LED_GPIO GPIO_NUM_2
static const char *TAG = "MAIN";
char recive[1000]="0";
char b[1000]="0";
char str[30]="0";
char number[30]="";
char three[30]=",22";
char four[30]=",0";
typedef enum {
    STATE_INIT,
    STATE_RISING,
    STATE_FALLING,
    STATE_COMPLETED
} state_t;
QueueHandle_t xQueueTrans;

extern "C" {
	void app_main(void);
}
void setup() {
  //设置LED管脚为输出模式
  // 初始化LED GPIO为输出模式
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
}
//竖判断
void process_variable_absy(int var_1,int var_2) {
    static state_t current_state = STATE_INIT;
    static bool process_flag = false;

    switch (current_state) {
        case STATE_INIT:
            if (var_1 < 200) {
                current_state = STATE_RISING;
                // printf("State changed to RISING\n");
            }
            break;

        case STATE_RISING:
            if (var_1 > 300&&var_2<200) {
                current_state = STATE_FALLING;
                // printf("State changed to FALLING\n");
            }
            break;

        case STATE_FALLING:
            if (var_1 <200) {
                current_state = STATE_COMPLETED;
                process_flag = true;
                // printf("State changed to COMPLETED\n");
            }
            break;

        case STATE_COMPLETED:
            // 过程已完成，可以根据需要进行处理
            if (process_flag) {
				strcat(number,"1");
				gpio_set_level(LED_GPIO, 1);
                printf("Process completed. Variable assigned to 1.\n");
                process_flag = false;
				vTaskDelay(10);
                current_state = STATE_INIT; // 重置状态
            }
            break;

        default:
            break;
    }
}
//横判断
void process_variable_absx(int var_1,int var_2) {
    static state_t current_state = STATE_INIT;
    static bool process_flag = false;

    switch (current_state) {
        case STATE_INIT:
            if (var_1 < 200) {
                current_state = STATE_RISING;
                // printf("State changed to RISING\n");
            }
            break;

        case STATE_RISING:
            if (var_1 > 300&&var_2<100) {
                current_state = STATE_FALLING;
                // printf("State changed to FALLING\n");
            }
            break;

        case STATE_FALLING:
            if (var_1 <200) {
                current_state = STATE_COMPLETED;
                process_flag = true;
                // printf("State changed to COMPLETED\n");
            }
            break;

        case STATE_COMPLETED:
            // 过程已完成，可以根据需要进行处理
            if (process_flag) {
				strcat(number,"0");
				 gpio_set_level(LED_GPIO, 1);
                printf("Process completed. Variable assigned to 1.\n");
                process_flag = false;
				vTaskDelay(10);
                current_state = STATE_INIT; // 重置状态
            }
            break;

        default:
            break;
    }
}

float dt = 0.01; // 假设采样周期为 10ms
float velocity[3]={0,0,0};
float displayment[3]={0,0,0};
void calculate_displacement()
{
	POSE_t pose;
		float accel[3];
        if (xQueueReceive(xQueueTrans, &pose, portMAX_DELAY))
		 {  

            float absRoll_1 = pose.roll;
            float absPitch_1 = pose.pitch;
            float absx_1 = pose.x;
            float absy_1 = pose.y;
			float absz_1 = pose.z;
			accel[0]=absx_1/16384.0;
			accel[1]=absy_1/16384.0;
			accel[2]=absz_1/16384.0;
			for (int i=0;i<3;i++)
			{
				velocity[i]+=accel[i]*dt;
			}
			for (int i=0;i<3;i++)
			{
				displayment[i]+=velocity[i]*dt;
			}
			
		 }
		 printf("x:%f\n y:%f\n",displayment[0],displayment[1]);
}
void processDataFromQueue() {
    POSE_t pose;
    while (1) {
		static float count = 0;
		char recive[1000] = ""; 
		vTaskDelay(1);
		 gpio_set_level(LED_GPIO, 0);
        if (xQueueReceive(xQueueTrans, &pose, portMAX_DELAY))
		 {  

            float absRoll = fabs(pose.roll);
            float absPitch = fabs(pose.pitch);
			float absyae = fabs(pose.yaw);
			// double Z_Z  = asin(pose.REAL_Z/);

            // float absx = fabs(pose.x)*cos(absRoll);
            // float absy = fabs(pose.y)*cos(absPitch);
			printf("absRoll=%f\n",90-absRoll);
			printf("absPitch=%f\n",90-absPitch);
			float absx = fabs(pose.xx);
			float absy = fabs(pose.yy);
			float absz = fabs(pose.zz);
			for (int i=0;i<10;i++)
			{
				absx+=absx;
				absy+=absy;
			}
			absy=absy/1638.0;
			absx=absx/1638.0;
			printf("absx=%f\n",absx);
			printf("absy=%f\n",absy);

			//第一问
			if ((absRoll>60&&absRoll<120)||(absPitch>60&&absPitch<120))
			{
				// printf("放倒\n");
				sprintf(recive,"%s","12");
			}
			else if ((absRoll>=0&&absRoll<30)||(absPitch>=0&&absPitch<30))
			{
				// printf("竖直\n");
				sprintf(recive,"%s","21");

			}


			//第二问
			
			
			if(fabs(90-absRoll)<=fabs(90-absPitch))
				{
					sprintf(str,",%f",fabs(90-absRoll));
					strcat(recive, str);
				}
			else if(fabs(90-absRoll)>=fabs(90-absPitch))
				{
					sprintf(str,",%f",fabs(90-absPitch));
					strcat(recive, str);
				}
			printf(recive);
			printf("倾斜角度：%f\n",90-absPitch);
			

			// // //第三问
			if (absx>300&&absy<100)
			{
				printf("X轴\n");
				// strcat(oder, ",0");
				sprintf(three,"%s",",0");
			}
			else if (absy>300&&absx<200)
			{
				printf("y轴\n");
				// strcat(oder, ",1");
				sprintf(three,"%s",",1");
			}
			strcat(recive,three);
			// sprintf(three,"%s","\0");


			strcpy(b,recive);
			// //第四问
		    
			process_variable_absy(absy,absx);
			process_variable_absx(absx,absy);
			if(absy>500&&absx>500)	
				{
					if(strcmp(number,"1")==0)//判断1
					{
						printf("写的数字是1\n");
						strcat(b, ",1");
						
						// strcat(recive, four);
						sprintf(four,"%s","\0");
					}
					if(strcmp(number,"01010")==0)//判断2
					{
						printf("写的数字是2\n");
						strcat(b, ",2");
						sprintf(four,"%s","\0");
						// sprintf(number,"%s","\0");
					}

					if(strcmp(number,"010010")==0)//判断3
					{
						printf("写的数字是3\n");
						strcat(b, ",3");
						sprintf(four,"%s","\0");
						// sprintf(number,"%s","\0");
					}
					
					if(strcmp(number,"101")==0)//判断4
					{
						printf("写的数字是4\n");
						strcat(b, ",4");
						sprintf(four,"%s","\0");

					}
					if(strcmp(number,"10100")==0||strcmp(number,"1010010")==0||strcmp(number,"101000")==0||strcmp(number,"101010")==0)//判断5
					{
						printf("写的数字是5\n");
						strcat(b, ",5");
						sprintf(four,"%s","\0");

					}
					if(strcmp(number,"011010")==0)//判断6
					{
						printf("写的数字是6\n");
						strcat(b, ",6");
						sprintf(four,"%s","\0");

					}
					if(strcmp(number,"01")==0)//判断7
					{
						printf("写的数字是7\n");
						strcat(b, ",7");
						sprintf(four,"%s","\0");

					}
					if(strcmp(number,"10101010")==0)//判断8
					{
						printf("写的数字是8\n");
						strcat(b, ",8");
						sprintf(four,"%s","\0");

					}
					if(strcmp(number,"010110")==0)//判断9
					{
						printf("写的数字是9\n");
						strcat(b, ",9");
						sprintf(four,"%s","\0");

					}
				}
			// strcat(recive, four);
			printf("number:%s\n",number);
			// printf("发送的信息：%s",recive);
			// if(absx>200)
			// {
			// 	vTaskDelay(10);
			// 	count+=0.1;
			// 	sprintf(four,"f",count);
			// 	strcat(b,four);
			// }
			// float a=rand()%3+9;
			// char aa=[10]="";
			// sprintf(aa,",%f",a);
			// strcat(b, aa);
			
		// sprintf(recive,"%s","\0");
		//加速度
			
		}
	}
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

	setup();

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
		gpio_set_level(LED_GPIO, 1);
		vTaskDelay(100);
		processDataFromQueue();
		if(xQueueReceive(xQueueTrans, &pose, portMAX_DELAY)) {
			ESP_LOGD(TAG, "pose=%f %f %f", pose.roll, pose.pitch, pose.yaw);
		} else {
			ESP_LOGE(TAG, "xQueueReceive fail");
			break;
		}
	}
}
