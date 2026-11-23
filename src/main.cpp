#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "config.h"
#include "logo.h"


Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

bool ON = HIGH;
bool OFF = LOW;

float nozzleCurrent[5] = {
    214.5, 215.2, 213.8, 216.0, 214.1
};

float nozzleTarget[5] = {
    220, 220, 0, 220, 220
};

int power[6] = {
  100, 52, -1, 10, 60, 100
};

float bedCurrent = 59.5;
float bedTarget = 60.0;

const int selectedNozzle = 1;

int percent = 35;
int PrevPercent = 34;


void drawTemperatureCard(
  const char* name,
  float current,
  float target,
  int x,
  int y,
  int w,
  int h,
  uint16_t color,
  bool selected,
  int power
) {
  uint16_t borderColor = selected ? S_NOZZLE : color;
  int thickness = selected ? 3 : 1;

  for (int i = 0; i < thickness; i++) {
    tft.drawRoundRect(
      x + i, y + i,
      w - 2 * i, h - 2 * i,
      5, borderColor
    );
  }

  tft.setTextSize(2);
  tft.setTextColor(borderColor);
  tft.setCursor(x + 7, y + 10);
  tft.print(name);

  tft.setTextSize(2);
  tft.setCursor(x + 48, y + 10);
  if (power == -1) {
    tft.setTextColor(OFF_COLOR);
    tft.print("OFF");
  } else {
    tft.setTextColor(NOZZLE_POWER);
    tft.print(power);
    tft.print("%");
  }

  tft.setTextSize(2);
  tft.setTextColor(C_TEMP);
  tft.setCursor(x + 7, y + 34);

  tft.print((int)current);

  tft.setTextColor(SLASH);
  tft.print("/");

  tft.setTextColor(T_TEMP);
  tft.print((int)target);
}


void drawDashboard() {

  int w = 100;
  int h = 60;

  int x[3] = {5, 110, 215};

  int y[2] = {5, 72};


  for (int i = 0; i < 6; i++) {
    if (i != 5) {
      char name[4];
      snprintf(name, sizeof(name), "T%d", i);

      drawTemperatureCard(
        name,
        nozzleCurrent[i],
        nozzleTarget[i],
        x[i % 3],
        y[i < 3 ? 0 : 1],
        w, h,
        NOZZLE,
        selectedNozzle == i,
        power[i]
      );
    } else {
      drawTemperatureCard(
        "BED", bedCurrent, bedTarget,
        x[2], y[1], w, h, BED, false, power[5]
      );
    }
  }


  tft.drawRoundRect(
    5, 140, 310, 90, 5, TIME_PANEL
  );
  tft.setTextColor(ELP);
  tft.setTextSize(2);
  tft.setCursor(14, 150);
  tft.print("ELP:");
  tft.setTextColor(ELP_TIME);
  tft.setCursor(67, 150);
  tft.print("00:00:00");
  tft.setCursor(14, 170);
  tft.setTextColor(ETA);
  tft.print("ETA:");
  tft.setTextColor(ETA_TIME);
  tft.setCursor(67, 170);
  tft.print("00:00:00");

  tft.drawRoundRect(12, 192, 152, 15, 4, PROGRESS_1);
  tft.fillRoundRect(13, 193, abs(percent*1.5), 13, 5, PROGRESS_2);

  tft.setTextSize(2);
  tft.setCursor(170, 150);
  tft.setTextColor(PERCENT);
  tft.print(percent);
  tft.print("%");
  PrevPercent = percent;

  tft.drawRoundRect(
    225, 140, 90, 90, 5, TIME_PANEL
  );
  tft.setTextSize(1);
  tft.setCursor(250, 175);
  tft.setTextColor(0x0000);
  tft.print("[preview]");

  tft.setCursor(14, 213);
  tft.setTextSize(1);
  tft.setTextColor(FILE);
  tft.print("File: ");
  tft.setTextColor(FILE_NAME);
  tft.print("test_file.gcode");
}

void waitScreen() {
  if (BLINKING) {
    digitalWrite(BACKLIT_PIN, OFF);
  }
  tft.fillScreen(BG);
  
  tft.setTextColor(WAITING_COLOR);
  tft.setTextSize(2);
  if (SHOW_LOGO) {
    tft.drawRGBBitmap(
      320/2-(LOGO_WIDTH/2),
      10,
      logo,
      LOGO_WIDTH,
      LOGO_HEIGHT
    );
    tft.setCursor(46, 196);
    tft.print("Waiting for printer");
    tft.setCursor(82, 216);
    tft.print("to initialize");
  } else {
    tft.setCursor(46, 106);
    tft.print("Waiting for printer");
    tft.setCursor(82, 126);
    tft.print("to initialize");
  }

  if (BLINKING) {
    digitalWrite(BACKLIT_PIN, ON);
  }
  delay(5000); //wait
  if (BLINKING) {
    digitalWrite(BACKLIT_PIN, OFF);
  }
}

void clear() {
  if (PrevPercent != percent) {
    //tft.fillRect(170, 150, 46, 18, BG);
    tft.setCursor(170, 150);
    tft.setTextColor(BG);
    tft.setTextSize(2);
    tft.print(PrevPercent);
    tft.print("%");
    tft.fillRect(12, 192, 152, 15, BG);
  }
}

void setup() {
  pinMode(BACKLIT_PIN, OUTPUT);
  if (BACKLIT_REVERSE) {
    ON = LOW;
    OFF = HIGH;
  }

  SPI.begin();

  tft.begin();
  tft.setSPISpeed(SPI_SPEED);
  tft.setRotation(1);
  tft.setTextWrap(false);
  waitScreen();
  percent = 0;
  tft.fillScreen(BG);
  drawDashboard();
  digitalWrite(BACKLIT_PIN, ON);
}

void loop() {
  percent += 1;
  if (percent > 100) {
    percent = 0;
  }
  clear();
  drawDashboard();
  delay(200);
}
