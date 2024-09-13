#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ssd1306.h"

/*
 You have to set this config value with menuconfig
 CONFIG_INTERFACE

 for i2c
 CONFIG_MODEL
 CONFIG_SDA_GPIO
 CONFIG_SCL_GPIO
 CONFIG_RESET_GPIO

 for SPI
 CONFIG_CS_GPIO
 CONFIG_DC_GPIO
 CONFIG_RESET_GPIO
*/

#define TAG "SSD1306"

#define IMAGES 10

uint8_t segmentDisplay[IMAGES][192] = {
{
// https://www.iconspng.com/image/5656/seven-segment-display-gray-0
// 'seven-segment-display-gray-0', 32x48px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x06, 0xff, 0xff, 0x60, 
0x07, 0x7f, 0xfe, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x00, 0x00, 0xe0, 0x02, 0x00, 0x00, 0x40, 
0x02, 0x00, 0x00, 0x40, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0xff, 0xff, 0xe0, 
0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
// https://www.iconspng.com/image/5657/seven-segment-display-gray-1
// 'seven-segment-display-gray-1', 32x48px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x60, 
0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x40, 
0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xe0, 
0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
// https://www.iconspng.com/image/5658/seven-segment-display-gray-2
// 'seven-segment-display-gray-2', 32x48px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0x60, 
0x00, 0xff, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x7f, 0xfe, 0xe0, 0x01, 0xff, 0xff, 0xc0, 
0x03, 0xff, 0xff, 0x00, 0x07, 0x7f, 0xfe, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 
0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 
0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 
0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0xff, 0xff, 0x00, 
0x07, 0xff, 0xff, 0x80, 0x07, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
// https://www.iconspng.com/image/5659/seven-segment-display-gray-3
// 'seven-segment-display-gray-3', 32x48px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0x60, 
0x00, 0xff, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x7f, 0xfe, 0xe0, 0x01, 0xff, 0xff, 0x80, 
0x01, 0xff, 0xff, 0x40, 0x00, 0x7f, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0xff, 0xff, 0xe0, 
0x01, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
// https://www.iconspng.com/image/5660/seven-segment-display-gray-4
// 'seven-segment-display-gray-4', 32x48px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x20, 0x06, 0x00, 0x00, 0x60, 
0x07, 0x00, 0x00, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x7f, 0xfe, 0xe0, 0x03, 0xff, 0xff, 0x80, 
0x00, 0xff, 0xff, 0x40, 0x00, 0x7f, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xe0, 
0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
// https://www.iconspng.com/image/5661/seven-segment-display-gray-5
// 'seven-segment-display-gray-5', 32x48px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xc0, 0x06, 0xff, 0xff, 0x80, 
0x07, 0x7f, 0xff, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 
0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 
0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 
0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x7f, 0xfe, 0x00, 0x03, 0xff, 0xff, 0x80, 
0x00, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0xff, 0xff, 0xe0, 
0x01, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
// https://www.iconspng.com/image/5662/seven-segment-display-gray-6
// 'seven-segment-display-gray-6', 32x48px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xc0, 0x06, 0xff, 0xff, 0x80, 
0x07, 0x7f, 0xff, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 
0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 
0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 
0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x7f, 0xfe, 0x00, 0x01, 0xff, 0xff, 0x80, 
0x02, 0xff, 0xff, 0xc0, 0x07, 0x7f, 0xfe, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0xff, 0xff, 0xe0, 
0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
// https://www.iconspng.com/image/5663/seven-segment-display
// 'seven-segment-display-gray-7', 32x48px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0x60, 
0x00, 0xff, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x40, 
0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0xe0, 
0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
// https://www.iconspng.com/image/5664/seven-segment-display-gray-8
// 'seven-segment-display-gray-8', 32x48px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x06, 0xff, 0xff, 0x60, 
0x07, 0x7f, 0xfe, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x7f, 0xfe, 0xe0, 0x01, 0xff, 0xff, 0x80, 
0x02, 0xff, 0xff, 0x40, 0x07, 0x7f, 0xfe, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0xff, 0xff, 0xe0, 
0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
},
{
// https://www.iconspng.com/image/5665/seven-segment-display
// 'seven-segment-display-gray-9', 32x48px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x06, 0xff, 0xff, 0x60, 
0x07, 0x7f, 0xfe, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0, 0x07, 0x7f, 0xfe, 0xe0, 0x03, 0xff, 0xff, 0x80, 
0x00, 0xff, 0xff, 0x40, 0x00, 0x7f, 0xfe, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 
0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0xff, 0xff, 0xe0, 
0x01, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}
};

// If you want show "1234":
// show_digit(&dev, segmentImage, 3, 4);
// show_digit(&dev, segmentImage, 2, 3);
// show_digit(&dev, segmentImage, 1, 2);
// show_digit(&dev, segmentImage, 0, 1);

void show_digit(SSD1306_t * dev, uint8_t *segmentImage, int digit_position, int digit_number) {
	int segmentImageIndex = digit_number * 256;
	int seg = digit_position * 32;
	for (int page=0;page<8;page++) {
		ssd1306_display_image(dev, page, seg, &segmentImage[segmentImageIndex+page*32], 32);
	}
}

void app_main(void)
{
	SSD1306_t dev;

#if CONFIG_I2C_INTERFACE
	ESP_LOGI(TAG, "INTERFACE is i2c");
	ESP_LOGI(TAG, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	ESP_LOGI(TAG, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_I2C_INTERFACE

#if CONFIG_SPI_INTERFACE
	ESP_LOGI(TAG, "INTERFACE is SPI");
	ESP_LOGI(TAG, "CONFIG_MOSI_GPIO=%d",CONFIG_MOSI_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCLK_GPIO=%d",CONFIG_SCLK_GPIO);
	ESP_LOGI(TAG, "CONFIG_CS_GPIO=%d",CONFIG_CS_GPIO);
	ESP_LOGI(TAG, "CONFIG_DC_GPIO=%d",CONFIG_DC_GPIO);
	ESP_LOGI(TAG, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_SPI_INTERFACE

#if CONFIG_FLIP
	dev._flip = true;
	ESP_LOGW(TAG, "Flip upside down");
#endif

#if CONFIG_SSD1306_128x64
	ESP_LOGI(TAG, "Panel is 128x64");
	ssd1306_init(&dev, 128, 64);
#endif // CONFIG_SSD1306_128x64
#if CONFIG_SSD1306_128x32
	ESP_LOGE(TAG, "Panel is 128x32. This demo cannot be run.");
	while(1) { vTaskDelay(1); }
#endif // CONFIG_SSD1306_128x32
	ssd1306_contrast(&dev, 0xff);

	// Allocate memory
	uint8_t *buffer = (uint8_t *)malloc(8*128); // 8 page 128 pixel
	if (buffer == NULL) {
		ESP_LOGE(TAG, "malloc failed");
		while(1) { vTaskDelay(1); }
	}
	uint8_t *segmentImage = (uint8_t *)malloc(IMAGES*8*32); // 10 image 8 page 32 pixel
	if (segmentImage == NULL) {
		ESP_LOGE(TAG, "malloc failed");
		while(1) { vTaskDelay(1); }
	}

	// Convert from segmentDisplay to segmentImage
	for (int imageIndex=0;imageIndex<IMAGES;imageIndex++) {
		ssd1306_clear_screen(&dev, false);
		ssd1306_bitmaps(&dev, 0, 8, segmentDisplay[imageIndex], 32, 48, false);
		vTaskDelay(200 / portTICK_PERIOD_MS);

		// Get from internal buffer to local buffer
		// buffer is [8][128] 8 page 128 pixel
		ssd1306_get_buffer(&dev, buffer);

		// Save from buffer to segmentImage
		// segmentImage is [10][8][32] 10 image 8 page 32 pixel
		int segmentImageIndex = imageIndex * 256;
		for (int page=0;page<8;page++) {
			//ESP_LOGI(TAG, "segmentImageIndex+page*32=%d", segmentImageIndex+page*32);
			memcpy(&segmentImage[segmentImageIndex+page*32], &buffer[page*128], 32);
			//ESP_LOGI(TAG, "page=%d", page);
			//ESP_LOG_BUFFER_HEXDUMP(TAG, &buffer[page*128], 32, ESP_LOG_INFO);
		}

#if 0
		ssd1306_clear_screen(&dev, false);
		for (int page=0;page<8;page++) {
			ssd1306_display_image(&dev, page, 0, &segmentImage[segmentImageIndex+page*32], 32);
			//ESP_LOGI(TAG, "page=%d", page);
			//ESP_LOG_BUFFER_HEXDUMP(TAG, &segmentImage[segmentImageIndex+page*32], 32, ESP_LOG_INFO);
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
#endif
	}

	// I don't use this anymore
	free(buffer);

#if 0
	// Test code
	ssd1306_clear_screen(&dev, false);
	show_digit(&dev, segmentImage, 3, 4);
	show_digit(&dev, segmentImage, 2, 3);
	show_digit(&dev, segmentImage, 1, 2);
	show_digit(&dev, segmentImage, 0, 1);
	vTaskDelay(200);
	show_digit(&dev, segmentImage, 3, 6);
	show_digit(&dev, segmentImage, 2, 5);
	show_digit(&dev, segmentImage, 1, 4);
	show_digit(&dev, segmentImage, 0, 3);
	vTaskDelay(200);
#endif

	// Show initial screen
	int digit1 = 0;
	int digit2 = 0;
	int digit3 = 0;
	int digit4 = 0;
	ssd1306_clear_screen(&dev, false);
	show_digit(&dev, segmentImage, 3, 0);
	show_digit(&dev, segmentImage, 2, 0);
	show_digit(&dev, segmentImage, 1, 0);
	show_digit(&dev, segmentImage, 0, 0);

	while(1) {
		digit4++;
		if (digit4 == 10) {
			digit4 = 0;
			int segmentImageIndex4 = digit4 * 256;
			for (int page=0;page<8;page++) {
				ssd1306_display_image(&dev, page, 96, &segmentImage[segmentImageIndex4+page*32], 32);
			}
			digit3++;
			if (digit3 == 10) {
				digit3 = 0;
				digit2++;
				if (digit2 == 10) {
					digit2 = 0;
					digit1++;
					if (digit1 == 10) {
						digit1 = 0;
						digit2 = 0;
						digit3 = 0;
						digit4 = 0;
					}
					// Update digit1
					show_digit(&dev, segmentImage, 0, digit1);
				}
				// Update digit2
				show_digit(&dev, segmentImage, 1, digit2);
			}
			// Update digit3
			show_digit(&dev, segmentImage, 2, digit3);
		} else {	
			// Update digit4
			show_digit(&dev, segmentImage, 3, digit4);
		}
		vTaskDelay(8);
	} // end while
}
