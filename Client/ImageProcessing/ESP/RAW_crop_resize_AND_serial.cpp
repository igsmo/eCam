#include <stdio.h>
#include "who_camera.h"
#include <memory.h>

/*This program demonstrates grabbing a RAW image frame, using customized drivers for ESP-S3-EYE
using the OV2640 camera.
 The program also demonstrates the following:
1. sending the frame buffer over serial
2. Resizing the frame buffer into a 96x96 image
3. Cropping the frame buffer into a 300x400 image */

#define LED_PIN GPIO_NUM_3  //This is the pin for the front green LED
int valid = 0; //used to sync serial transmission of images with MATLAB

extern "C" void app_main()
{

    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT); //Define GPIO3 as output
    gpio_set_level(LED_PIN, 0); //turn off front LED. So that it does not interfere with the camera.

    register_ecam(PIXFORMAT_RAW, FRAMESIZE_UXGA); //sets up the camera. The customized camera drivers are needed for RAW!!

//Set initial camera values. This should happen after all drivers have set these values.
sensor_t * s = esp_camera_sensor_get();
    s->set_brightness(s, 0);     // -2 to 2.  This is comparable to digital gain. Should not be used.
    s->set_contrast(s, 2);       // -2 to 2. Contrast increases the difference between blacks and whites. This should be beneficial.
    s->set_saturation(s, 0);     // -2 to 2. Since there is no color data. We dont touch this. It might have unwanted effects.
    s->set_special_effect(s, 0); // Silly effects we don't need
    s->set_whitebal(s, 0);       // Auto-whitebalance. Is not used in the eCam. Color balance is set manually.
   // s->set_awb_gain(s, 1);       // This enables the Color mixer.
    s->set_wb_mode(s, 10);        // this sets the RGB mixer to some preset value. 11 is a custom mode balanced for white LED light.
                                // These modes can be edited in: ov2640_settings.h -> wb_modes_regs
    s->set_exposure_ctrl(s, 0);  // Auto-exposure is not used by the eCam. We use a fixed exposure time.
    s->set_aec2(s, 0);           // part of Auto-exposure
    s->set_ae_level(s, 0);       // -2 to 2 part of Auto-exposure
    s->set_aec_value(s, 87);    // 0 to 1200. Manual exposure value: Set to 87 for eCam moving robot.
    s->set_gain_ctrl(s, 1);      // Auto-Gain. This is the analog gain amplifier. eCam uses this to automatically set the brightness.
    s->set_agc_gain(s, 0);       // 0 to 30.  Manual Gain Setting. Not used if Auto-Gain is on.
    s->set_gainceiling(s, (gainceiling_t)5);  // 0 to 6  This sets a maximum Gain for the Auto-Gain. Smart to set to something sensible.
    s->set_bpc(s, 1);            // black-pixel cancelling. Detects pixels that are stuck at zero and puts them to medium-grey.
    s->set_wpc(s, 1);            // white-pixel cancelling. Detects pixels that are stuck at 255 and puts them to medium-grey.
    s->set_raw_gma(s, 1);        // Gamma correction to compensate for non-linearity of pixel sensitivity.
    s->set_lenc(s, 1);           // Lens compensation: compensates for pixels getting different levels of light according to their position on the sensor.
    s->set_hmirror(s, 1);        // reverses order of pixels horizontally
    s->set_vflip(s, 1);          // reverses order of lines vertically
    s->set_dcw(s, 0);            // 0 = disable , 1 = enable
    s->set_colorbar(s, 0);       // replaces the image with a test pattern used for calibration

    

    camera_fb_t * frame = esp_camera_fb_get(); //Get a frame at full resolution RAW format
    size_t len = frame->len;                 /*! Get length of the buffer in bytes/pixels */



// Here begins the CROP function

    uint16_t y_offs = 450;  //This sets how many lines to skip from the top. Or the y-position of the window.
    uint16_t x_offs = 600;  //This sets how many pixels to skip from the left. Or the x-position of the window.
    printf("Defined params!");
    size_t x_out = 400; //width of the window
    size_t y_out = 300; //height of the window
    //size_t crop_len = x_out*y_out;  //length of the output buffer in number of pixels
printf("Defined params!");
    uint8_t* fb_crop = (uint8_t * )malloc(120000*sizeof(uint8_t));    //allocates memory for the cropped image
printf("Mem alloc");
    for (uint16_t y = 0; y < y_out; y++){   //copy the cropped frame pixels to fb_crop array in the correct order
        for (uint16_t x = 0; x < x_out; x++){
                uint32_t k = (y*x_out)+x;
                uint32_t i = ((y+y_offs)*1600)+x+x_offs;
                fb_crop[k] = frame->buf[i];
        }
    }
    printf("Crop done!");
    
//Here ends the CROP function

//Here begins the RESIZE function
  
    size_t x_out_r = 96; //width of the resized image
    size_t y_out_r = 96; //height of the resized image
    size_t rez_len = x_out_r*y_out_r;  //length of the resized image buffer in number of pixels
    uint8_t* fb_rez = (uint8_t * )malloc(rez_len*sizeof(uint8_t));    //allocates memory for the resized image
    uint32_t pixsum = 0;   //This is a placeholder to sum up the values of pixels in each window
    size_t x_wind = 16; //x and y size of the window used for avaraging
    size_t y_wind = 8;     //8x16 is chosen because it equals 128. Thus bit shifting can be used for fast division.
    size_t win_len = x_wind*y_wind;     //number of pixels in each window (128 in our case)
    uint16_t y_rem = 432;  //the number of lines left out when windowing. This value is used to offset the windowing grid
                            //so that lines are skipped from the top of the image, rather than at the bottom. As the floor is more important.
                            // the top 1/3 of the image is skipped for faster avaraging by use of bit-shifting
    uint32_t k = 0;
    for (uint16_t yw = 0; yw < y_out_r; yw++)  { //move window y-axis along windowing grid
        for (uint16_t xw = 0; xw < x_out_r; xw++) {  //move window x-axis along windowing grid
            pixsum = 0; //reset accumulator for each new window coordinate
            for (size_t y = 0; y < y_wind; y++){ //select line from within window
                for (size_t x = 0; x < x_wind; x++) {//collect pixel from within line of window
                    uint32_t i = ((yw*y_wind)+y_rem+y)*1600+(xw*x_wind)+x; //index from original image frame buffer
                    pixsum += frame->buf[i];
                }
            }
            //avarage out all the pixels stored in the window and write to output image buffer
            fb_rez[k++] = uint8_t(pixsum >> 7);
        }
    } 
    printf("Resize done!");
//Here ends the RESIZE function

//Here begins the serial transmission of the frame buffer to MATLAB. For testing purposes.
    while (valid != 1) {    //halt the program until a 'valid' signal is sent from MATLAB to ESP over serial
        scanf("%d",&valid);
        vTaskDelay( 200 );
    }
    //when valid signal is recieved from MATLAB the program responds by sending a start sequence 
    // Real image data would never be this sharp. So this type of sequence will never occur in the image data.
    putchar(0);
    putchar(255);
    putchar(0);
    putchar(255);
    //this is done to sync the serial buffer and the image reader. As there might be data in the buffer before the image is transmitted.
    /*
    //Transmit the resize buffer as stored in memory to serial
   for (uint8_t i = 0; i < rez_len; i++) {
           printf("%s",fb_rez++);
    }
*/
    //Transmit the original frame buffer as stored in memory to serial
   for (uint8_t i = 0; i < len; i++) {
           printf("%s",frame->buf++);
    }
//Program stops. Reset to take another photo.

}
