#pragma once
#include <Arduino.h>


// SPI
#define TFT_CS   PA4
#define TFT_DC   PB0
#define TFT_RST  PB1
#define SPI_SPEED 60000000 // 60Mhz



// Hardware
#define NOZZLES 5          // Amount of nozzles
#define HEATED_BED true    // Heated bed?



// If only one nozzle should it
// be treated as selected one or
// normal? true - selected
// [it only affects style of nozzle card]
#define SONE true



// Show temp graphs?
// Works only for printers with max
// 3 total heaters [nozzles + bed]
#define GRAPHS true
#define GRAPH_MAX 250
#define BED_GRAPH_MAX 120



// Backlight
// if this is true then screen backlight
// will turn off while waiting screen is
// being drawn and while transitioning from
// waiting screen to status screen
// BACKLIT_PIN defines where backlit pin is
// connected, BACKLIT_REVERSE reverses the
// ON and OFF if you have different display
// true  -->  HIGH - ON   LOW  - OFF
// false -->  LOW  - ON   HIGH - OFF
#define BLINKING  true
#define BACKLIT_PIN  PB12
#define BACKLIT_REVERSE false



// Waiting screen
#define SHOW_LOGO true    // show logo? if not waiting message will
                          // be moved to the middle of screen
                          // [you can change logo to custom one
                          // for instructions on how to do that check
                          // customlogo.txt]



// Colors
#define BG              0x0000  // background
#define WAITING_COLOR   0xFFFF  // waiting for printer [...] message


#define NOZZLE          0x07FF  // border of nozzle card
#define S_NOZZLE        0x07E0  // border of selected nozzle card
#define NOZZLE_POWER    0xF0F0  // nozzle power [%]
#define OFF_COLOR       0xF000  // nozzle power ["OFF"]
#define BED             0xFD20  // border of bed card

#define C_TEMP          0xFFFF  // current temperature
#define SLASH           0x9CF3  // slash between temperatures
#define T_TEMP          0xFFE0  // target temperature


#define TIME_PANEL      0x9CF3  // panel at the bottom

#define ELP             0xFFFF  // ELP text ["ELP: "]
#define ETA             0xFFFF  // ETA text ["ETA: "]
#define ELP_TIME        0xFFFF  // ELP time ["00:00:00"]
#define ETA_TIME        0xFFFF  // ETA time ["00:00:00"]

#define PROGRESS_1      0xFFFF  // border of progress bar
#define PROGRESS_2      0x07E0  // progress bar

#define PERCENT         0xFD20  // percent of finished print

#define FILE            0xFFFF  // file text ["File: "]
#define FILE_NAME       0xFFFF  // file name ["text.gcode"]