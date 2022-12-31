#include "who_lcd.h"
#include "esp_camera.h"
#include <string.h>
#include <memory.h>

static const char *TAG = "who_lcd";

static scr_driver_t g_lcd;
static scr_info_t g_lcd_info;

static QueueHandle_t xQueueFrameI = NULL;
static QueueHandle_t xQueueFrameO = NULL;
static bool gReturnFB = true;
static int sTopMargin = 3;
static int sStepback = 5;
static int sMinL = 20;
static void task_process_handler(void *arg)
{
    camera_fb_t *frame = NULL;
    static uint16_t PureBOrW[57600];
    static uint16_t * OFrame;
    int x, y, i, j, dx, k, dy;
    bool found, inz;
    typedef struct qr_codes{
        bool o;
        bool f;
        int x0;
        int y0;
        int x1;
        int y1;
        int x2;
        int y2;
        int x3;
        int y3
    } qr_code;
    qr_code l1[5];
    //int countt = 0;
    //long stop;
    //long start = xTaskGetTickCount();
    while (true)
    {
        if (xQueueReceive(xQueueFrameI, &frame, portMAX_DELAY))
        {
            for(int nr = 0; nr < 5; nr++)
            {
                l1[nr].o = false;
            }
            OFrame = frame->buf;
            y = 0;
            while (y < 240)
            {
                x = 0;
                while (x < 240)
                {
                    if((((OFrame[y*240+x]>>11)&(0b11111)) > 29) & (((OFrame[y*240+x]>>5)&(0b111111)) > 61) & (((OFrame[y*240+x])&(0b11111)) > 29))
                    {
                        PureBOrW[y*240+x] = 65535;
                    }
                    else
                    {
                        PureBOrW[y*240+x] = 0;
                    }
                    x += 1;
                }
                y += 1;
            }
            g_lcd.draw_bitmap(0, 0, frame->width, frame->height, &PureBOrW);
            y = 1;
            
            
            found = false;
            k = 0;
            while (y < 240)
            {
                x = 0;
                while (x < 240)
                {
                    inz = false;
                    for(int n = 0; n < 5; n++)
                    {
                        if(l1[n].o)
                        {
                            if((l1[n].x0 <= x) & (x <= l1[n].x1) & (l1[n].y0 <= y) & (y <= l1[n].y3))
                            {
                                inz = true;
                            }
                        }
                    }
                    if(!inz)
                    {
                        if((PureBOrW[y*240+x] != 0) & (PureBOrW[(y-sTopMargin)*240+x] == 0))
                        {
                            i = 1;
                            j = 0;
                            dx = 1;
                            found = false;
                            while(i < (240 - x))
                            {
                                if ((PureBOrW[(y + j)*240+x+i] != 0) & (PureBOrW[(j+y-sTopMargin)*240+x+i] == 0))
                                {
                                    dx += 1;
                                    i += 1;
                                    if(dx > sMinL)
                                    {
                                        found = true;
                                    }
                                }
                                else if ((PureBOrW[(j + y + 1)*240+x+i] != 0) & (PureBOrW[(j + y + 1 - sTopMargin)*240+x+i] == 0))
                                {
                                    dx += 1;
                                    i += 1;
                                    j += 1;
                                    if(dx > sMinL)
                                    {
                                        found = true;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                            }
                            if(found)
                            {
                                i = 1;
                                dy = 1;
                                found = false;
                                while (i < (240 - y + j))
                                {
                                    if(PureBOrW[(y + j + i)* 240 + x + dx - sStepback] != 0)
                                    {
                                        dy += 1;
                                        i += 1;
                                        if(dy > sMinL)
                                        {
                                            found = true;
                                        }
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                if(found)
                                {
                                    if(k < 5)
                                    {
                                        l1[k].o = true;
                                        l1[k].f = false;
                                        l1[k].x0 = x;
                                        l1[k].y0 = y;
                                        l1[k].x1 = x + dx;
                                        l1[k].y1 = y + j;
                                        l1[k].x2 = x + dx;
                                        l1[k].y2 = y + j + dy;
                                        l1[k].x3 = x;
                                        l1[k].y3 = y + 2 * j + dy;
                                        k += 1;
                                    }
                                }  
                            }
                            x += i;
                        }
                    }
                    x += 1;
                }
                y += 1;
            }
            y = 1;
            while (y < 240)
            {
                x = 239;
                while (x >= 0)
                {
                    inz = false;
                    for(int n = 0; n < 5; n++)
                    {
                        if(l1[n].o)
                        {
                            if (l1[n].f)
                            {
                                if((l1[n].x0 >= x) & (x >= l1[n].x1) & (l1[n].y0 <= y) & (y <= l1[n].y3))
                                {
                                    inz = true;
                                }
                            }
                            else
                            {
                                if((l1[n].x0 <= x) & (x <= l1[n].x1) & (l1[n].y0 <= y) & (y <= l1[n].y3))
                                {
                                    inz = true;
                                }
                            }
                        }
                    }
                    if(!inz)
                    {
                        if((PureBOrW[y*240+x] != 0) & (PureBOrW[(y-sTopMargin)*240+x] == 0))
                        {
                            i = -1;
                            j = 0;
                            dx = -1;
                            found = false;
                            while(i > (0 - x))
                            {
                                if ((PureBOrW[(y + j)*240+x+i] != 0) & (PureBOrW[(j+y-sTopMargin)*240+x+i] == 0))
                                {
                                    dx -= 1;
                                    i -= 1;
                                    if(dx < -sMinL)
                                    {
                                        found = true;
                                    }
                                }
                                else if ((PureBOrW[(j +y + 1)*240+x+i] != 0) & (PureBOrW[(j +y + 1 - sTopMargin)*240+x+i] == 0))
                                {
                                    dx -= 1;
                                    i -= 1;
                                    j += 1;
                                    if(dx < -sMinL)
                                    {
                                        found = true;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                            }
                            if(found)
                            {
                                i = 1;
                                dy = 1;
                                found = false;
                                while (i < (240 - y + j))
                                {
                                    if(PureBOrW[(y + j + i)* 240 + x + dx + sStepback] != 0)
                                    {
                                        dy += 1;
                                        i += 1;
                                        if(dy > sMinL)
                                        {
                                            found = true;
                                        }
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                if(found)
                                {
                                    if(k < 5)
                                    {
                                    l1[k].o = true;
                                    l1[k].f = true;
                                    l1[k].x0 = x;
                                    l1[k].y0 = y;
                                    l1[k].x1 = x + dx;
                                    l1[k].y1 = y + j;
                                    l1[k].x2 = x + dx;
                                    l1[k].y2 = y + j + dy;
                                    l1[k].x3 = x;
                                    l1[k].y3 = y + 2 * j + dy;
                                    k += 1;
                                    }
                                }  
                            }
                            x += dx;
                        }
                    }
                    x -= 1;
                }
                y += 1;
            }
            for(int z = 0; z < 5; z++)
            {
                if(l1[z].o)
                {
                    ESP_LOGI(TAG, "#%d", (l1[z].x1 + l1[z].x0) / 2);
                    //ESP_LOGI(TAG, "%d: c1(%d, %d), c2(%d, %d), c3(%d, %d), c4(%d, %d)", z, l1[z].x0, l1[z].y0, l1[z].x1, l1[z].y1, l1[z].x2, l1[z].y2, l1[z].x3, l1[z].y3);
                }
                
            }
            if(!l1[0].o)
            {
                ESP_LOGI(TAG, "#%d", 500);
            }


            //Debug function to paint green squares at corners

            /*g_lcd.draw_bitmap(0, 0, 240, 240, &PureBOrW);
            for(int n = 0; n < 5; n++)
            {
                if(l1[n].o)
                {
                    for(int yp = 0; yp < 15; yp++)
                    {
                        for(int xp = 0; xp < 15; xp++)
                        {
                            g_lcd.draw_pixel(l1[n].x0 - 8 + xp, l1[n].y0 - 8 + yp, g);
                        }
                        for(int xp = 0; xp < 15; xp++)
                        {
                            g_lcd.draw_pixel(l1[n].x1 - 8 + xp, l1[n].y1 - 8 + yp, g);
                        }
                    }
                    for(int yp = 0; yp < 15; yp++)
                    {
                        for(int xp = 0; xp < 15; xp++)
                        {
                            g_lcd.draw_pixel(l1[n].x2 - 8 + xp, l1[n].y2 - 8 + yp, g);
                        }
                        for(int xp = 0; xp < 15; xp++)
                        {
                            g_lcd.draw_pixel(l1[n].x3 - 8 + xp, l1[n].y3 - 8 + yp, g);
                        }
                    }
                }
            }*/

            //Benchmark function to meassure time

            /*
            if(countt > 99)
            {
                stop = xTaskGetTickCount();
                int time = ((stop - start) / pdMS_TO_TICKS(100));
                ESP_LOGI(TAG, "time = %d", time);
                countt = 0;
                start = xTaskGetTickCount();
            }
            else
            {
                countt += 1;
            }*/

            /*
            Here should be the function which requests cropped and scaled versions of the RAW image for all of the QR codes and then links the values with the corners
            and sends them to the navigation system.
            */
            if (xQueueFrameO)
            {
                xQueueSend(xQueueFrameO, &frame, portMAX_DELAY);
            }
            else if (gReturnFB)
            {
                esp_camera_fb_return(frame);
            }
            else
            {
                free(frame);
            }
        }
    }
}
/// @brief Sets up screen and starts QR posing algorithm
/// @param frame_i Frame queue to read image from
/// @param frame_o Frame queue to send image to after
/// @param return_fb Return frame memory to camera
/// @param TopMargin Dist between the white pixel being checked and the black pixel above it
/// @param MinL Min lenght for the frame sides
/// @param Stepback Amount to go back when looking for the side
/// @return Returns error if screen setup fails
esp_err_t register_lcd(const QueueHandle_t frame_i, const QueueHandle_t frame_o, const bool return_fb, const int TopMargin, const int MinL, const int Stepback)
{
    spi_config_t bus_conf = {
        .miso_io_num = BOARD_LCD_MISO,
        .mosi_io_num = BOARD_LCD_MOSI,
        .sclk_io_num = BOARD_LCD_SCK,
        .max_transfer_sz = 2 * 240 * 240 + 10,
    };
    spi_bus_handle_t spi_bus = spi_bus_create(SPI2_HOST, &bus_conf);

    scr_interface_spi_config_t spi_lcd_cfg = {
        .spi_bus = spi_bus,
        .pin_num_cs = BOARD_LCD_CS,
        .pin_num_dc = BOARD_LCD_DC,
        .clk_freq = 40 * 1000000,
        .swap_data = 0,
    };

    scr_interface_driver_t *iface_drv;
    scr_interface_create(SCREEN_IFACE_SPI, &spi_lcd_cfg, &iface_drv);
    esp_err_t ret = scr_find_driver(SCREEN_CONTROLLER_ST7789, &g_lcd);
    if (ESP_OK != ret)
    {
        return ret;
        ESP_LOGE(TAG, "screen find failed");
    }

    scr_controller_config_t lcd_cfg = {
        .interface_drv = iface_drv,
        .pin_num_rst = BOARD_LCD_RST,
        .pin_num_bckl = BOARD_LCD_BL,
        .rst_active_level = 0,
        .bckl_active_level = 0,
        .offset_hor = 0,
        .offset_ver = 0,
        .width = 240,
        .height = 240,
        .rotate = 0,
    };
    ret = g_lcd.init(&lcd_cfg);
    if (ESP_OK != ret)
    {
        return ESP_FAIL;
        ESP_LOGE(TAG, "screen initialize failed");
    }

    g_lcd.get_info(&g_lcd_info);
    ESP_LOGI(TAG, "Screen name:%s | width:%d | height:%d", g_lcd_info.name, g_lcd_info.width, g_lcd_info.height);
    xQueueFrameI = frame_i;
    xQueueFrameO = frame_o;
    gReturnFB = return_fb;
    sTopMargin = TopMargin;
    sMinL = MinL;
    sStepback = Stepback;
    xTaskCreatePinnedToCore(task_process_handler, TAG, 4 * 1024, NULL, 5, NULL, 0); 
    return ESP_OK;
}
