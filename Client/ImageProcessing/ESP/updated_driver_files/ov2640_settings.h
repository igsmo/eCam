// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef _OV2640_SETTINGS_H_
#define _OV2640_SETTINGS_H_

#include <stdint.h>
#include <stdbool.h>
#include "esp_attr.h"
#include "ov2640_regs.h"

typedef enum {
    OV2640_MODE_UXGA, OV2640_MODE_SVGA, OV2640_MODE_CIF, OV2640_MODE_MAX
} ov2640_sensor_mode_t;

typedef struct {
        union {
                struct {
                        uint8_t pclk_div:7;
                        uint8_t pclk_auto:1;
                };
                uint8_t pclk;
        };
        union {
                struct {
                        uint8_t clk_div:6;
                        uint8_t reserved:1;
                        uint8_t clk_2x:1;
                };
                uint8_t clk;
        };
} ov2640_clk_t;

typedef struct {
        uint16_t offset_x;
        uint16_t offset_y;
        uint16_t max_x;
        uint16_t max_y;
} ov2640_ratio_settings_t;

static const DRAM_ATTR ov2640_ratio_settings_t ratio_table[] = {
    // ox,  oy,   mx,   my
    {   0,   0, 1600, 1200 }, //4x3
    {   8,  72, 1584, 1056 }, //3x2
    {   0, 100, 1600, 1000 }, //16x10
    {   0, 120, 1600,  960 }, //5x3
    {   0, 150, 1600,  900 }, //16x9
    {   2, 258, 1596,  684 }, //21x9
    {  50,   0, 1500, 1200 }, //5x4
    { 200,   0, 1200, 1200 }, //1x1
    { 462,   0,  676, 1200 }  //9x16
};

// 30fps@24MHz
const DRAM_ATTR uint8_t ov2640_settings_cif[][2] = {
    {BANK_SEL, BANK_DSP},   //Select bank 0
    {0x2c, 0xff},   //reserved
    {0x2e, 0xdf},   //reserved

    {BANK_SEL, BANK_SENSOR},    //select bank 1
    {0x3c, 0x32},   //reserved
    {CLKRC, 0x01},  //Clock Rate Control clk=xvclk/([value]+1) -> divider = 2
        	//Clock Rate Control
                //Bit[7]: Internal frequency doublers ON/OFF selection
                    //0: OFF {set}
                    //1: ON
                //Bit[6]: Reserved
                //Bit[5:0]: Clock divider
                    //CLK = XVCLK/(decimal value of CLKRC[5:0] + 1)
    {COM2, COM2_OUT_DRIVE_3x},  //Output drive select. NOTE: this is probably set to 2x or the datasheet is incorrect
    {REG04, REG04_DEFAULT}, //Bit[7]: Horizontal mirror      0x28 = 0010 1000
                            //Bit[6]: Vertical flip      bit order: 7654 3210
                            //Bit[5]: Not documented
                            //Bit[4]: VREF //Bit[0]
                            //Bit[3]: HREF //Bit[0]
                            //Bit[2]: Reserved
                            //Bit[1:0]: AEC[1:0]
    {COM8, COM8_DEFAULT | COM8_BNDF_EN | COM8_AGC_EN | COM8_AEC_EN},    //0xE5 = 0b11100101
                        	//Bit[7:6]: Reserved                                   76543210
                            //Bit[5]: Banding filter selection
                                //0: OFF {set}
                                //1: ON, set minimum exposure time to 1/120s
                            //Bit[4:3]: Reserved
                            //Bit[2]: AGC auto/manual control selection
                                //0: Manual
                                //1: Auto {set}
                            //Bit[1]: Reserved
                            //Bit[0]: Exposure control
                                //0: Manual
                                //1: Auto  {set}
    {COM9, COM9_AGC_SET(COM9_AGC_GAIN_8x)}, // 0x08 | 0x02 << 5  =  0x08 | 0x40 = 0x48 = 0b01001000
                	//Bit[7:5]: AGC gain ceiling, GH[2:0]                                  76543210
                        //000: 2x
                        //001: 4x
                        //010: 8x {set}
                        //011: 16x
                        //100: 32x
                        //101: 64x
                        //11x: 128x
                    //Bit[4:0]: Reserved  {set to: 0b01000}
    {0x2c, 0x0c},   //reserved
    {0x33, 0x78},   //reserved
    {0x3a, 0x33},   //reserved
    {0x3b, 0xfB},   //reserved
    {0x3e, 0x00},   //reserved
    {0x43, 0x11},   //reserved
    {0x16, 0x10},   //reserved
    {0x39, 0x92},   //reserved
    {0x35, 0xda},   //reserved
    {0x22, 0x1a},   //reserved
    {0x37, 0xc3},   //reserved
    {0x23, 0x00},   //reserved
    {ARCOM2, 0xc0},     //0xC0 = 0b11000000
                        //         76543210
            //Bit[7:3]: Reserved
            //Bit[2]: Zoom window horizontal start point {not set}
            //Bit[1:0]: Reserved
    {0x06, 0x88},   //reserved
    {0x07, 0xc0},   //reserved
    {COM4, 0x87},   //Reserved, there is no COM4 in the datasheet for OV2640
    {0x0e, 0x41},   //reserved
    {0x4c, 0x00},   //reserved
    {0x4a, 0x81},   //reserved
    {0x21, 0x99},   //reserved
    {AEW, 0x40},    //Luminance Signal High Range for AEC/AGC Operation
                    //AEC/AGC values will decrease in auto mode when average luminance 
                    //is greater than AEW[7:0]
    {AEB, 0x38},    //Luminance Signal Low Range for AEC/AGC Operation
                    //AEC/AGC values will increase in auto mode when average luminance 
                    //is less than AEB[7:0]
    {VV, VV_AGC_TH_SET(8,2)},   //Fast Mode Large Step Range Threshold - effective only in AEC/AGC 
                                //fast mode (COM8[7] = 1)
                                //Bit[7:4]: High threshold
                                //Bit[3:0]:Low threshold
                                //Note: AEC/AGC may change in larger steps when luminance average 
                                //is greater than VV[7:4] or less than VV[3:0].
    {0x5c, 0x00},   //reserved
    {0x63, 0x00},   //reserved
    {HISTO_LOW, 0x70},  //Histogram Algorithm Low Level
    {HISTO_HIGH, 0x80}, //Histogram Algorithm High Level
    {0x7c, 0x05},   //reserved
    {0x20, 0x80},   //reserved
    {0x28, 0x30},   //reserved
    {0x6c, 0x00},   //reserved
    {0x6d, 0x80},   //reserved
    {0x6e, 0x00},   //reserved
    {0x70, 0x02},   //reserved
    {0x71, 0x94},   //reserved
    {0x73, 0xc1},   //reserved
    {0x3d, 0x34},   //reserved
    {0x5a, 0x57},   //reserved
    {BD50, 0xbb},   //50Hz Banding AEC 8 LSBs
    {BD60, 0x9c},   //60Hz Banding AEC 8 LSBs
    {COM7, COM7_RES_CIF},   //0x20 = 0b00100000
                                //     76543210
                    	//Bit[7]: SRST
                            //1: Initiates system reset. All registers are set to factory 
                            //default values after which the chip resumes normal 
                            //operation
                        //Bit[6:4]: Resolution selection
                            //000: UXGA (full size) mode
                            //010: CIF mode {set}
                            //100: SVGA mode
                        //Bit[3]: Reserved
                        //Bit[2]: Zoom mode
                        //Bit[1]: Color bar test pattern
                            //0: OFF
                            //1: ON
                        //Bit[0]: Reserved
    {HSTART, 0x11}, //0x11 = 0b00010001
                        //     76543210
            	//Horizontal Window Start MSB 8 bits (3 LSBs in REG32[2:0] (0x32))
                //Bit[10:0]: Selects the start of the horizontal window, each LSB 
                //represents two pixels
    {HSTOP, 0x43},  //0x43 = 0b01000011
                        //     76543210
                //Horizontal Window End MSB 8 bits (3 LSBs in REG32[5:3] (0x32))
                //Bit[10:0]: Selects the end of the horizontal window, each LSB 
                //represents two pixels
    {VSTART, 0x00},
                //Vertical Window Line Start MSB 8 bits (2 LSBs in COM1[1:0] (0x03))
                //Bit[9:0]: Selects the start of the vertical window, each LSB 
                //represents two scan lines.
    {VSTOP, 0x25},  //0x25 = 0b00100101
                            // 76543210
                //Vertical Window Line End MSB 8 bits (2 LSBs in COM1[3:2] (0x03))
                //Bit[9:0]: Selects the end of the vertical window, each LSB 
                //represents two scan lines.
    {REG32, 0x89},  //0x89 = 0b10001001
                            // 76543210
    	//Bit[7:6]: Pixel clock divide option
            //00: No effect on PCLK
            //01: No effect on PCLK
            //10: PCLK frequency divide by 2 {set}
            //11: PCLK frequency divide by 4
        //Bit[5:3]: Horizontal window end position 3 LSBs (8 MSBs in 
            //register HREFEND[7:0] (0x18))
        //Bit[2:0]: Horizontal window start position 3 LSBs (8 MSBs in 
            //register HREFST[7:0] (0x17))
    {0x37, 0xc0},   //reserved
    {BD50, 0xca},   //50Hz Banding AEC 8 LSBs
    {BD60, 0xa8},   //60Hz Banding AEC 8 LSBs
    {0x6d, 0x00},   //reserved
    {0x3d, 0x38},   //reserved

    {BANK_SEL, BANK_DSP},   //Select bank 0
    {0xe5, 0x7f},   //reserved
    {MC_BIST, MC_BIST_RESET | MC_BIST_BOOT_ROM_SEL}, // 0x80 | 0x40 = 0b11000000
                                                            //          76543210
    	//Bit[7]: Microcontroller Reset {set}
        //Bit[6]: Boot ROM select {set}
        //Bit[5]: R/W 1 error for 12K-byte memory
        //Bit[4]: R/W 0 error for 12K-byte memory
        //Bit[3]: R/W 1 error for 512-byte memory
        //Bit[2]: R/W 0 error for 512-byte memory
        //Bit[1]: BIST busy bit for read; One-shot reset of 
            //microcontroller for write
        //Bit[0]: Launch BIST
    {0x41, 0x24},   //reserved
    {RESET, RESET_JPEG | RESET_DVP}, // 0x10 | 0x04 = 0b00010100
                                            //          76543210
            	//Bit[7]: Reserved
                //Bit[6]: Microcontroller
                //Bit[5]: SCCB
                //Bit[4]: JPEG {set}
                //Bit[3]: Reserved
                //Bit[2]: DVP  {set}
                //Bit[1]: IPU
                //Bit[0]: CIF
    {0x76, 0xff},   //reserved
    {0x33, 0xa0},   //reserved
    {0x42, 0x20},   //reserved
    {0x43, 0x18},   //reserved
    {0x4c, 0x00},   //reserved
    {CTRL3, CTRL3_WPC_EN | 0x10 }, // 0x40 | 0x10 = 0b01010000
                                        //            76543210
                //Bit[7]: BPC
                //Bit[6]: WPC {set}   (datasheet does not explain this)
                //Bit[5:0]: Reserved
    {0x88, 0x3f},   //reserved
    {0xd7, 0x03},   //reserved
    {0xd9, 0x10},   //reserved
    {R_DVP_SP, R_DVP_SP_AUTO_MODE | 0x02}, // 0x80 | 0x02 = 0b10000010
                                            //                76543210
    	//Bit[7]: Auto mode {set}
        //Bit[6:0]: DVP output speed control
            //DVP PCLK = sysclk (48)/[6:0] (YUV0); 
            //                  = sysclk (48)/(2*[6:0]) (RAW)
    {0xc8, 0x08},   //reserved
    {0xc9, 0x80},   //reserved

    // This probably writes to addresses that are otherwise locked 
    {BPADDR, 0x00}, //SDE Indirect Register Access: Address
    {BPDATA, 0x00},     //SDE Indirect Register Access: Data
    {BPADDR, 0x03}, //SDE Indirect Register Access: Address
    {BPDATA, 0x48},     //SDE Indirect Register Access: Data
    {BPDATA, 0x48},     //SDE Indirect Register Access: Data
    {BPADDR, 0x08}, //SDE Indirect Register Access: Address
    {BPDATA, 0x20},     //SDE Indirect Register Access: Data
    {BPDATA, 0x10},     //SDE Indirect Register Access: Data
    {BPDATA, 0x0e},     //SDE Indirect Register Access: Data

    // A lot of writing to the same few addresses. Perhaps just used as a delay
    {0x90, 0x00},   //reserved
    {0x91, 0x0e},   //reserved
    {0x91, 0x1a},   //reserved
    {0x91, 0x31},   //reserved
    {0x91, 0x5a},   //reserved
    {0x91, 0x69},   //reserved
    {0x91, 0x75},   //reserved
    {0x91, 0x7e},   //reserved
    {0x91, 0x88},   //reserved
    {0x91, 0x8f},   //reserved
    {0x91, 0x96},   //reserved
    {0x91, 0xa3},   //reserved
    {0x91, 0xaf},   //reserved
    {0x91, 0xc4},   //reserved
    {0x91, 0xd7},   //reserved
    {0x91, 0xe8},   //reserved
    {0x91, 0x20},   //reserved
    {0x92, 0x00},   //reserved
    {0x93, 0x06},   //reserved
    {0x93, 0xe3},   //reserved
    {0x93, 0x05},   //reserved
    {0x93, 0x05},   //reserved
    {0x93, 0x00},   //reserved
    {0x93, 0x04},   //reserved
    {0x93, 0x00},   //reserved
    {0x93, 0x00},   //reserved
    {0x93, 0x00},   //reserved
    {0x93, 0x00},   //reserved
    {0x93, 0x00},   //reserved
    {0x93, 0x00},   //reserved
    {0x93, 0x00},   //reserved
    {0x96, 0x00},   //reserved
    {0x97, 0x08},   //reserved
    {0x97, 0x19},   //reserved
    {0x97, 0x02},   //reserved
    {0x97, 0x0c},   //reserved
    {0x97, 0x24},   //reserved
    {0x97, 0x30},   //reserved
    {0x97, 0x28},   //reserved
    {0x97, 0x26},   //reserved
    {0x97, 0x02},   //reserved
    {0x97, 0x98},   //reserved
    {0x97, 0x80},   //reserved
    {0x97, 0x00},   //reserved
    {0x97, 0x00},   //reserved
    {0xa4, 0x00},   //reserved
    {0xa8, 0x00},   //reserved
    {0xc5, 0x11},   //reserved
    {0xc6, 0x51},   //reserved
    {0xbf, 0x80},   //reserved
    {0xc7, 0x10},   //reserved
    {0xb6, 0x66},   //reserved
    {0xb8, 0xA5},   //reserved
    {0xb7, 0x64},   //reserved
    {0xb9, 0x7C},   //reserved
    {0xb3, 0xaf},   //reserved
    {0xb4, 0x97},   //reserved
    {0xb5, 0xFF},   //reserved
    {0xb0, 0xC5},   //reserved
    {0xb1, 0x94},   //reserved
    {0xb2, 0x0f},   //reserved
    {0xc4, 0x5c},   //reserved

    {CTRL0, 0x0C},   //Sets AEC off and enables YUV module
    {CTRL1, 0xfd},  // 0xFD = 0b11111101
                    //          76543210
        //Module Enable
        	//Bit[7]: CIP
            //Bit[6]: DMY
            //Bit[5]: RAW_GMA
            //Bit[4]: DG
            //Bit[3]: AWB
            //Bit[2]: AWB_GAIN
            //Bit[1]: LENC  {NOT SET}
            //Bit[0]: PRE

    {0x7f, 0x00},   //reserved
    {0xe5, 0x1f},   //reserved
    {0xe1, 0x67},   //reserved
    {0xdd, 0x7f},   //reserved

    {IMAGE_MODE, 0x00},
    //Image Output Format Select
    	//Bit[7]: Reserved
        //Bit[6]: Y8 enable for DVP
        //Bit[5]: Reserved
        //Bit[4]: JPEG output enable
            //0: Non-compressed  {set}
            //1: JPEG output
        //Bit[3:2]: DVP output format
            //00: YUV422    {set}
            //01: RAW10 (DVP)
            //10: RGB565
            //11: Reserved
        //Bit[1]: HREF timing select in DVP JPEG output mode
            //0: HREF is same as sensor
            //1: HREF = VSYNC
        //Bit[0]: Byte swap enable for DVP
            //0: High byte first YUYV (C2[4]=0)
            //YVYU (C2[4] = 1) 
            //1: Low byte first UYVY (C2[4] =0)
            //VYUY (C2[4] =1)
    {RESET, 0x00},
    	//Bit[7]: Reserved
        //Bit[6]: Microcontroller
        //Bit[5]: SCCB
        //Bit[4]: JPEG
        //Bit[3]: Reserved
        //Bit[2]: DVP
        //Bit[1]: IPU
        //Bit[0]: CIF
    {R_BYPASS, R_BYPASS_DSP_EN},
        	//Bypass DSP
                //Bit[7:1]: Reserved
                //Bit[0]: Bypass DSP select
                    //0: DSP {set}
                    //1: Bypass DSP, sensor out directly


    {0, 0}  //This triggers to the write_regs function that transmission is done
};

const DRAM_ATTR uint8_t ov2640_settings_to_cif[][2] = {
    {BANK_SEL, BANK_SENSOR},
    {COM7, COM7_RES_CIF},

    //Set the sensor output window
    {COM1, 0x0A},
    {REG32, REG32_CIF},
    {HSTART, 0x11},
    {HSTOP, 0x43},
    {VSTART, 0x00},
    {VSTOP, 0x25},

    //{CLKRC, 0x00},
    {BD50, 0xca},
    {BD60, 0xa8},
    {0x5a, 0x23},
    {0x6d, 0x00},
    {0x3d, 0x38},
    {0x39, 0x92},
    {0x35, 0xda},
    {0x22, 0x1a},
    {0x37, 0xc3},
    {0x23, 0x00},
    {ARCOM2, 0xc0},
    {0x06, 0x88},
    {0x07, 0xc0},
    {COM4, 0x87},
    {0x0e, 0x41},
    {0x4c, 0x00},
    {BANK_SEL, BANK_DSP},
    {RESET, RESET_DVP},

    //Set the sensor resolution (UXGA, SVGA, CIF)
    {HSIZE8, 0x32},
    {VSIZE8, 0x25},
    {SIZEL, 0x00},

    //Set the image window size >= output size
    {HSIZE, 0x64},
    {VSIZE, 0x4a},
    {XOFFL, 0x00},
    {YOFFL, 0x00},
    {VHYX, 0x00},
    {TEST, 0x00},

    {CTRL2, CTRL2_DCW_EN | 0x1D},
    {CTRLI, CTRLI_LP_DP | 0x00},
    //{R_DVP_SP, 0x08},
    {0, 0}
};

const DRAM_ATTR uint8_t ov2640_settings_to_svga[][2] = {
    {BANK_SEL, BANK_SENSOR},
    {COM7, COM7_RES_SVGA},

    //Set the sensor output window
    {COM1, 0x0A},
    {REG32, REG32_SVGA},
    {HSTART, 0x11},
    {HSTOP, 0x43},
    {VSTART, 0x00},
    {VSTOP, 0x4b},

    //{CLKRC, 0x00},
    {0x37, 0xc0},
    {BD50, 0xca},
    {BD60, 0xa8},
    {0x5a, 0x23},
    {0x6d, 0x00},
    {0x3d, 0x38},
    {0x39, 0x92},
    {0x35, 0xda},
    {0x22, 0x1a},
    {0x37, 0xc3},
    {0x23, 0x00},
    {ARCOM2, 0xc0},
    {0x06, 0x88},
    {0x07, 0xc0},
    {COM4, 0x87},
    {0x0e, 0x41},
    {0x42, 0x03},
    {0x4c, 0x00},
    {BANK_SEL, BANK_DSP},
    {RESET, RESET_DVP},

    //Set the sensor resolution (UXGA, SVGA, CIF)
    {HSIZE8, 0x64},
    {VSIZE8, 0x4B},
    {SIZEL, 0x00},

    //Set the image window size >= output size
    {HSIZE, 0xC8},
    {VSIZE, 0x96},
    {XOFFL, 0x00},
    {YOFFL, 0x00},
    {VHYX, 0x00},
    {TEST, 0x00},

    {CTRL2, CTRL2_DCW_EN | 0x1D},
    {CTRLI, CTRLI_LP_DP | 0x00},
    //{R_DVP_SP, 0x08},
    {0, 0}
};

const DRAM_ATTR uint8_t ov2640_settings_to_uxga[][2] = {
    {BANK_SEL, BANK_SENSOR},    //Select bank 1
    {COM7, COM7_RES_UXGA}, //0b00000000
                    	//Bit[7]: SRST
                            //1: Initiates system reset. All registers are set to factory 
                            //default values after which the chip resumes normal 
                            //operation
                        //Bit[6:4]: Resolution selection
                            //000: UXGA (full size) mode
                            //010: CIF mode {set}
                            //100: SVGA mode
                        //Bit[3]: Reserved
                        //Bit[2]: Zoom mode
                        //Bit[1]: Color bar test pattern
                            //0: OFF
                            //1: ON
                        //Bit[0]: Reserved

    //Set the sensor output window
    {COM1, 0x0F},   //0b00001111
                    //  76543210
    	//Bit[7:6]: Dummy frame control
            //00: Reserved
            //01: Allow 1 dummy frame
            //10: Allow 3 dummy frames
            //11: Allow 7 dummy frames
        //Bit[5:4]: Reserved
        //Bit[3:2]: Vertical window end line control 2 LSBs
            //(8 MSBs in VEND[7:0] (0x1A))
        //Bit[1:0]: Vertical window start line control 2 LSBs
            //(8 MSBs in VSTRT[7:0] (0x19))
    {REG32, REG32_UXGA},    //0b00110110
                            //  76543210
              //Bit[7:6]: Pixel clock divide option
                //00: No effect on PCLK {set}
                //01: No effect on PCLK
                //10: PCLK frequency divide by 2
                //11: PCLK frequency divide by 4
            //Bit[5:3]: Horizontal window end position 3 LSBs (8 MSBs in 
                //register HREFEND[7:0] (0x18))  {110}
            //Bit[2:0]: Horizontal window start position 3 LSBs (8 MSBs in 
                //register HREFST[7:0] (0x17))       {110}       
    {HSTART, 0x11},  //0x11 = 0b00010001
                        //      76543210
            	//Horizontal Window Start MSB 8 bits (3 LSBs in REG32[2:0] (0x32))
                //Bit[10:0]: Selects the start of the horizontal window, each LSB 
                //represents two pixels

    {HSTOP, 0x75}, // 0x75 = 0b01110101
                    //         76543210
            //Horizontal Window End MSB 8 bits (3 LSBs in REG32[5:3] (0x32))
                //Bit[10:0]: Selects the end of the horizontal window, each LSB 
                //represents two pixels
    {VSTART, 0x01},
                //Vertical Window Line Start MSB 8 bits (2 LSBs in COM1[1:0] (0x03))
                //Bit[9:0]: Selects the start of the vertical window, each LSB 
                //represents two scan lines.
    {VSTOP, 0x97},
                //Vertical Window Line End MSB 8 bits (2 LSBs in COM1[3:2] (0x03))
                //Bit[9:0]: Selects the end of the vertical window, each LSB 
                //represents two scan lines.
    
    //{CLKRC, 0x00},  //this was already commented out
    {0x3d, 0x34},
    {BD50, 0xbb},   //50Hz Banding AEC 8 LSBs
    {BD60, 0x9c},   //60Hz Banding AEC 8 LSBs
    {0x5a, 0x57},
    {0x6d, 0x80},
    {0x39, 0x82},
    {0x23, 0x00},
    {0x07, 0xc0},
    {0x4c, 0x00},
    {0x35, 0x88},
    {0x22, 0x0a},
    {0x37, 0x40},
    {ARCOM2, 0xa0}, //0b10100000
                    //  76543210
            //Bit[7:3]: Reserved
            //Bit[2]: Zoom window horizontal start point {not set}
            //Bit[1:0]: Reserved
    {0x06, 0x02},
    {COM4, 0xb7},   //Reserved, there is no COM4 in the datasheet for OV2640
    {0x0e, 0x01},
    {0x42, 0x83},
    {BANK_SEL, BANK_DSP},   //Select bank 0
    {RESET, RESET_DVP}, //0x04 = 0b00000100
                        //         76543210
        //RESET:
        	//Bit[7]: Reserved
            //Bit[6]: Microcontroller
            //Bit[5]: SCCB
            //Bit[4]: JPEG
            //Bit[3]: Reserved
            //Bit[2]: DVP {set}
            //Bit[1]: IPU
            //Bit[0]: CIF

    //Set the sensor resolution (UXGA, SVGA, CIF)
    {HSIZE8, 0xc8}, //Image Horizontal Size HSIZE[10:3]
    {VSIZE8, 0x96}, //Image Vertical Size VSIZE[10:3]
    {SIZEL, 0x00},

    //Set the image window size >= output size
    {HSIZE, 0x90},
    {VSIZE, 0x2c},
    {XOFFL, 0x00},
    {YOFFL, 0x00},
    {VHYX, 0x88},
    {TEST, 0x00},

    {CTRL2, CTRL2_DCW_EN | 0x1d},
    {CTRLI, 0x00},
    //{R_DVP_SP, 0x06},
    {0, 0}
};

const DRAM_ATTR uint8_t ov2640_settings_jpeg3[][2] = {
    {BANK_SEL, BANK_DSP},
    {RESET, RESET_JPEG | RESET_DVP},
    {IMAGE_MODE, IMAGE_MODE_JPEG_EN | IMAGE_MODE_HREF_VSYNC},
    {0xD7, 0x03},
    {0xE1, 0x77},
    {0xE5, 0x1F},
    {0xD9, 0x10},
    {0xDF, 0x80},
    {0x33, 0x80},
    {0x3C, 0x10},
    {0xEB, 0x30},
    {0xDD, 0x7F},
    {RESET, 0x00},
    {0, 0}
};

static const uint8_t ov2640_settings_yuv422[][2] = {
    {BANK_SEL, BANK_DSP},
    {RESET, RESET_DVP},
    {IMAGE_MODE, IMAGE_MODE_YUV422}, //was: IMAGE_MODE_YUV422  now: RAW
    {0xD7, 0x01},
    {0xE1, 0x67},
    {RESET, 0x00},
    {0, 0},
};

static const uint8_t ov2640_settings_rgb565[][2] = {
    {BANK_SEL, BANK_DSP},
    {RESET, RESET_DVP},
    {IMAGE_MODE, IMAGE_MODE_RGB565},
    {0xD7, 0x03},
    {0xE1, 0x77},
    {RESET, 0x00},
    {0, 0},
};

static const uint8_t ov2640_settings_RAW[][2] = {
    {BANK_SEL, BANK_DSP},
    {RESET, RESET_DVP},
    {IMAGE_MODE, IMAGE_MODE_RAW10},
    {CTRL0, 0x01},   //Sets AEC off and enables RAW module
    {R_DVP_SP, R_DVP_SP_AUTO_MODE | 0x01}, //Pixel clock formula is different for RAW, so this is set at half value.
    {0xD7, 0x03},
    {0xE1, 0x77},
    {RESET, 0x00},
    {0, 0},
};

#define NUM_BRIGHTNESS_LEVELS (5)
static const uint8_t brightness_regs[NUM_BRIGHTNESS_LEVELS + 1][5] = {
    {BPADDR, BPDATA, BPADDR, BPDATA, BPDATA },
    {0x00, 0x04, 0x09, 0x00, 0x00 }, /* -2 */
    {0x00, 0x04, 0x09, 0x10, 0x00 }, /* -1 */
    {0x00, 0x04, 0x09, 0x20, 0x00 }, /*  0 */
    {0x00, 0x04, 0x09, 0x30, 0x00 }, /* +1 */
    {0x00, 0x04, 0x09, 0x40, 0x00 }, /* +2 */
};

#define NUM_CONTRAST_LEVELS (5)
static const uint8_t contrast_regs[NUM_CONTRAST_LEVELS + 1][7] = {
    {BPADDR, BPDATA, BPADDR, BPDATA, BPDATA, BPDATA, BPDATA },
    {0x00, 0x04, 0x07, 0x20, 0x18, 0x34, 0x06 }, /* -2 */
    {0x00, 0x04, 0x07, 0x20, 0x1c, 0x2a, 0x06 }, /* -1 */
    {0x00, 0x04, 0x07, 0x20, 0x20, 0x20, 0x06 }, /*  0 */
    {0x00, 0x04, 0x07, 0x20, 0x24, 0x16, 0x06 }, /* +1 */
    {0x00, 0x04, 0x07, 0x20, 0x28, 0x0c, 0x06 }, /* +2 */
};

#define NUM_SATURATION_LEVELS (5)
static const uint8_t saturation_regs[NUM_SATURATION_LEVELS + 1][5] = {
    {BPADDR, BPDATA, BPADDR, BPDATA, BPDATA },
    {0x00, 0x02, 0x03, 0x28, 0x28 }, /* -2 */
    {0x00, 0x02, 0x03, 0x38, 0x38 }, /* -1 */
    {0x00, 0x02, 0x03, 0x48, 0x48 }, /*  0 */
    {0x00, 0x02, 0x03, 0x58, 0x58 }, /* +1 */
    {0x00, 0x02, 0x03, 0x68, 0x68 }, /* +2 */
};

#define NUM_SPECIAL_EFFECTS (7)
static const uint8_t special_effects_regs[NUM_SPECIAL_EFFECTS + 1][5] = {
    {BPADDR, BPDATA, BPADDR, BPDATA, BPDATA },
    {0x00, 0X00, 0x05, 0X80, 0X80 }, /* no effect */
    {0x00, 0X40, 0x05, 0X80, 0X80 }, /* negative */
    {0x00, 0X18, 0x05, 0X80, 0X80 }, /* black and white */
    {0x00, 0X18, 0x05, 0X40, 0XC0 }, /* reddish */
    {0x00, 0X18, 0x05, 0X40, 0X40 }, /* greenish */
    {0x00, 0X18, 0x05, 0XA0, 0X40 }, /* blue */
    {0x00, 0X18, 0x05, 0X40, 0XA6 }, /* retro */
};

#define NUM_WB_MODES (11)
static const uint8_t wb_modes_regs[NUM_WB_MODES + 1][3] = {
    {0XCC, 0XCD, 0XCE },
    {0x5E, 0X41, 0x54 }, /*1 sunny */ 
    {0x65, 0X41, 0x4F }, /*2 cloudy */
    {0x52, 0X41, 0x66 }, /*3 office */
    {0x42, 0X3F, 0x71 }, /*4 home */
    {0x71, 0X61, 0x71 }, /*5 Balanced */
    {0xFF, 0X00, 0x00 }, /*6 R */
    {0x00, 0XFF, 0x00 }, /*7 G */
    {0x00, 0X00, 0xFF }, /*8 B */
    {0x00, 0X88, 0x00 }, /*9 Test1 */
    {0xFF, 0XE1, 0xC8 }, /*10 Calibrated for white LED */
    {0xFF, 0XFF, 0xFF }, /*11 Full-on Equal */
};

#define NUM_AE_LEVELS (5)
static const uint8_t ae_levels_regs[NUM_AE_LEVELS + 1][3] = {
    { AEW,  AEB,  VV  },
    {0x20, 0X18, 0x60 },
    {0x34, 0X1C, 0x00 },
    {0x3E, 0X38, 0x81 },
    {0x48, 0X40, 0x81 },
    {0x58, 0X50, 0x92 },
};

const uint8_t agc_gain_tbl[31] = {
    0x00, 0x10, 0x18, 0x30, 0x34, 0x38, 0x3C, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7A, 0x7C, 0x7E, 0xF0,
    0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

#endif /* _OV2640_SETTINGS_H_ */
