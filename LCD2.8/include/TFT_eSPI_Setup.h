#pragma once

#define ILI9341_DRIVER
#define TFT_PARALLEL_8_BIT

// Control pins
#define TFT_CS   26
#define TFT_DC   21
#define TFT_RST  25
#define TFT_WR   27
#define TFT_RD   33

// 8-bit data bus
#define TFT_D0   13
#define TFT_D1   14
#define TFT_D2   16
#define TFT_D3   17
#define TFT_D4   18
#define TFT_D5   19
#define TFT_D6   23
#define TFT_D7   22

#define LOAD_GLCD
#define LOAD_FONT2
