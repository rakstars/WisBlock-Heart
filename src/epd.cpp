/**
 * @file epd.cpp
 * @author Johan Sebastian Macias (johan.macias@rakwireless.com)
 * @brief Application specific functions. Mandatory to have init_app(), 
 *        app_event_handler(), ble_data_handler(), lora_data_handler()
 *        and lora_tx_finished()
 * @version 0.1
 * @date 2022-01-26
 * 
 * @copyright Copyright (c) 2022
 * 
 * Based on: 
 * https://github.com/RAKWireless/WisBlock/blob/master/examples/common/IO/RAK14000-Epaper-Monochrome/RAK14000-Epaper-Monochrome.ino
 * By: taylor.lee (taylor.lee@rakwireless.com)
 * Date: 2021-02-18
 */
#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_EPD.h>

#include "images.h"

#include "app.h"

#define POWER_ENABLE   WB_IO2
#define EPD_MOSI       MOSI  
#define EPD_MISO       -1     // not use
#define EPD_SCK        SCK   
#define EPD_CS         SS    
#define EPD_DC         WB_IO1
#define SRAM_CS        -1     // not use
#define EPD_RESET      -1     // not use
#define EPD_BUSY       WB_IO4

#define LEFT_BUTTON    WB_IO3
#define MIDDLE_BUTTON  WB_IO5
#define RIGHT_BUTTON   WB_IO6

void testdrawtext(int16_t x, int16_t y, char *text, uint16_t text_color, uint32_t text_size);

/*****************************************************/
/*****************************************************/
/* Select the correct display from the defines below */
/* The display type is printed on the back of the    */
/* display.                                          */
/* If you use a different display than the one      */
/* provided by RAKwireless you have to adjust the    */
/* the values matching with your custom display      */
/*****************************************************/
/*****************************************************/
typedef struct  DEPG {
   int  width;
   int  height;
   int  position1_x;
   int  position1_y;  
   int  position2_x;
   int  position2_y;  
   int  position3_x;
   int  position3_y; 
   int  position4_x;
   int  position4_y;   
} DEPG;

DEPG  DEPG_HP = {250,122,40,20,40,30,40,50,90,40};  //use DEPG0213BNS800F41HP as default
//DEPG  DEPG_HP = {212,104,30,15,30,25,30,45,80,30};  //  this is for DEPG0213BNS800F42HP

uint8_t gMsgNum = 0; // which message to show

// 2.13" EPD with SSD1680
Adafruit_SSD1680 display(DEPG_HP.width, DEPG_HP.height, EPD_MOSI,
                         EPD_SCK, EPD_DC, EPD_RESET,
                         EPD_CS, SRAM_CS, EPD_MISO,
                         EPD_BUSY);

void testdrawtext(int16_t x, int16_t y, char *text, uint16_t color);

/**
 * @brief Initialize RAK11200 EPD
 * 
 * @return true If sensor was found and is initialized
 * @return false If sensor initialization failed
 */
bool init_epd(void)
{
    pinMode(POWER_ENABLE, INPUT_PULLUP);
    digitalWrite(POWER_ENABLE, HIGH);

    MYLOG("EPD", "RAK11200 EPD Initialization");

    display.begin();

    // large block of text
    display.clearBuffer();

    display.drawBitmap(45, 10, rak_img, 150, 56, EPD_BLACK);
    testdrawtext(0, 80, "   IoT Made Easy!", (uint16_t)EPD_BLACK, 2);

    display.display(true);

	return true;
}

/**
   @brief Write a text on the display
   @param x x position to start
   @param y y position to start
   @param text text to write
   @param text_color color of text
   @param text_size size of text
*/
void testdrawtext(int16_t x, int16_t y, char *text, uint16_t text_color, uint32_t text_size)
{
  display.setCursor(x, y);
  display.setTextColor(text_color);
  display.setTextSize(text_size);
  display.setTextWrap(true);
  display.print(text);
}

void switch_epd_message()
{
  char str[80];
  if(gMsgNum == 1)
  {
    MYLOG("EPD", "Message #%d", gMsgNum);
    display.clearBuffer();
    //testdrawtext(0, 30, "If I know what love1If I know what love2If I know what love3If I know what love4", (uint16_t)EPD_BLACK, 2);
    sprintf(str, "%.80s", g_user_flash_data.epd_msg_1);
    testdrawtext(0, 40, (char*)str, (uint16_t)EPD_BLACK, 2);
    display.display(true);
  }

  if(gMsgNum == 2)
  {
    MYLOG("EPD", "Message #%d", gMsgNum);
    display.clearBuffer();
    sprintf(str, "%.80s", g_user_flash_data.epd_msg_2);
    testdrawtext(0, 40, (char*)str, (uint16_t)EPD_BLACK, 2);
    display.display(true);
  }

  if(gMsgNum == 3)
  {
    MYLOG("EPD", "Message #%d", gMsgNum);
    display.clearBuffer();
    sprintf(str, "%.80s", g_user_flash_data.epd_msg_3);
    testdrawtext(0, 40, (char*)str, (uint16_t)EPD_BLACK, 2);
    display.display(true);
  }

  if(gMsgNum == 4)
  {
    MYLOG("EPD", "Message #%d", gMsgNum);
    display.clearBuffer();
    sprintf(str, "%.80s", g_user_flash_data.epd_msg_4);
    testdrawtext(0, 40, (char*)str, (uint16_t)EPD_BLACK, 2);
    display.display(true);
  }

  if(gMsgNum == 5)
  {
    MYLOG("EPD", "Message #%d", gMsgNum);
    display.clearBuffer();
    display.drawBitmap(DEPG_HP.position2_x, DEPG_HP.position2_y, rak_img, 150, 56, EPD_BLACK);
    display.display(true);
    gMsgNum = 0;
  }
}