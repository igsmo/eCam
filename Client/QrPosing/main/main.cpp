#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "who_camera.h"
#include "QR_Posing.h"
#include "esp_code_scanner.h"
#include <string.h>
//#include "logo_en_240x240_lcd.h"
const TickType_t xDelay = 500 / 200;

static QueueHandle_t xQueueLCDFrame = NULL;
static QueueHandle_t xQueueCAMFrame = NULL;



#define GPIO_BOOT GPIO_NUM_0

extern "C" void app_main()
{
    
    gpio_config_t gpio_conf;
    gpio_conf.mode = GPIO_MODE_OUTPUT_OD;
    gpio_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_conf.pin_bit_mask = 1LL << GPIO_NUM_3;
    gpio_config(&gpio_conf);
    xQueueCAMFrame = xQueueCreate(2, sizeof(camera_fb_t *));
    register_camera(PIXFORMAT_RGB565, FRAMESIZE_240X240, 2, xQueueCAMFrame);
    register_lcd(xQueueCAMFrame, NULL, true, 5, 20, 5);

    while (true)
    {
        vTaskDelay(xDelay);
    }
}