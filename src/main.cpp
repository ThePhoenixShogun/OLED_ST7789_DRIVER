/*
  This code makes use of the tft example sketch adjusted for my ESP32 and my specific OLED
  Make sure to give to .pio--->libdep--->TFT_eSPI and then User_Setup.h to properly configure your pins and driver
  I currently have it configured for the ST7789_DRIVER. In case of color issues, change the RGB order to BGR or try
  inversion. Fair warning, not an expert so theres probably code that could be cut out in the setup, but I don't
  want to touch things I don't know
*/
#include <SPI.h>
#include <TFT_eSPI.h> //need to find the lib for this

//Arduino libs
#include <Arduino.h>
#include <AnimatedGIF.h>
#include <stdint.h>
AnimatedGIF gif;


//PIN LAYOUT

//taken from example, need to stay the same
#define SCLK_PIN GPIO_NUM_18 //needs to be connected to clock pin, so SCK
#define MOST_PIN GPIO_NUM_23 //DIN, MOSI

//Standard GPIO Pins --rec avoiding UART Pins
#define DC_PIN GPIO_NUM_27
#define CS_PIN GPIO_NUM_26
#define RST_PIN GPIO_NUM_25
//go to tft lib UserSetup and also add them there in form TFT_CS....TFT_DC and all that

//Gifs that we can upload
#include "bw_wiggler_128x64.h"
#include "thisisfine_240x179.h"

//Color Defs

#define BLACK      0x0000
#define BLUE       0x001F
#define RED        0xF800
#define GREEN      0x07E0
#define CYAN       0x07FF
#define MAGENTA    0xF81F
#define YELLOW     0xFFE0
#define WHITE      0xFFFF

//size of image, not screen
#define imageWidth 240
#define imageHeight 179

#define GIF_IMAGE thisisfine_240x179

TFT_eSPI tft = TFT_eSPI(); //initliaze tft lib, 

#define NORMAL_SPEED //Access memory

void GIFdraw(GIFDRAW *pDraw);  

void setup() {
  Serial.begin(9600);
  Serial.println("ESP32 Starting Up");

  Serial.println("TFT screen initialising");
  tft.begin();

  #ifdef USE_DMA //preposscer keywords
  tft.initDMA();
  #endif

  //initliazes the screen to check it works
  tft.fillScreen(RED);
  delay(1000);
  tft.fillScreen(GREEN);
  delay(1000);
  tft.fillScreen(BLUE);
  delay(1000);
  gif.begin(LITTLE_ENDIAN_PIXELS); //may be file format, if looks corrupted change this
  
  Serial.println("Image Rendered");
  
}

void loop() {

    if(gif.open((uint8_t *)GIF_IMAGE, sizeof(GIF_IMAGE), GIFdraw)) { //potential prob with GIFDRAW, wrote a function further down

      Serial.printf("succesfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
      tft.startWrite();
      while(gif.playFrame(true, NULL))
      {
        yield();

      }
      gif.close();
      tft.endWrite();


    }


}

void GIFdraw(GIFDRAW *pDraw) {
  uint8_t *s = pDraw->pPixels;
  int x, y;

  if (pDraw->y >= tft.height()) return; // safety check

  // set drawing window for one row
  tft.setAddrWindow(pDraw->iX, pDraw->iY + pDraw->y, pDraw->iWidth, 1);

  // get palette from GIF object
  const uint16_t *palette = (const uint16_t *)pDraw->pPalette; //need to check registers on the esp32 and OLED to check that 16 bytes is correct. May be 18 bytes or 32

  for (x = 0; x < pDraw->iWidth; x++) {
    uint8_t idx = *s++;        // index into palette
    uint16_t color = palette[idx];
    tft.pushColor(color);
  }
}

