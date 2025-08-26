# OLED_ST7789_DRIVER

  This code makes use of the tft example sketch adjusted for my ESP32 and my specific OLED
  Make sure to give to .pio--->libdep--->TFT_eSPI and then User_Setup.h to properly configure your pins and driver
  I currently have it configured for the ST7789_DRIVER. In case of color issues, change the RGB order to BGR or try
  inversion. Fair warning, not an expert so theres probably code that could be cut out in the setup, but I don't
  want to touch things I don't know
