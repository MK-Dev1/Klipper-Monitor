#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "config.h"
#include "logo.h"


Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

bool ON = HIGH;
bool OFF = LOW;

float nozzleCurrent[6] = {
    214.5, 215.2, 213.8, 216.0, 214.1, 59.5
};

float nozzleTarget[6] = {
    220, 220, 0, 220, 220, 60
};

int power[6] = {
  100, 52, -1, 10, 60, 100
};

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
  int power,
  int heaters
) {
  int thickness;
  uint16_t borderColor;
  if (NOZZLES > 1) {
    thickness = selected ? 3 : 1;
    borderColor = selected ? S_NOZZLE : color;
  } else if (SONE) {
    thickness = color == BED ? 1 : 3;
    borderColor = color == BED ? BED : S_NOZZLE;
  } else {
    thickness = 1;
    borderColor = color;
  }

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

  if (heaters <= 3 && GRAPHS) {
    tft.setTextColor(0xFFFFFF);
    tft.drawLine(x + 7, y + 60, x + 7, y + 110, 0xFFFFFF);
    switch (heaters) {
      case 1:
        tft.drawLine(x + 7, y + 110, x + 300, y + 110, 0xFFFFFF);
        break;
      case 2:
        tft.drawLine(x + 7, y + 110, x + 140, y + 110, 0xFFFFFF);
        break;
      case 3:
        tft.drawLine(x + 7, y + 110, x + 90, y + 110, 0xFFFFFF);
        break;
    }
    tft.setCursor(x + 12, y + 80);
    tft.setTextSize(1);
    tft.print("[GRAPH]");
  }
}

void setPos(int n, int p[24]) {
  if (n <= 3) {
    int w = (320-(5*n))/n-(6-(2*n));
    int h = 125;
    for (int i = 0; i < n; i++) {
      p[i*4] = i*w+5 + i*5;
      p[i*4+1] = 5;
      p[i*4+2] = w;
      p[i*4+3] = h;
    }
  } else {
    int w = 100;
    int h = 60;
    for (int i = 0; i < 3; i++) {
      p[i*4] = i*w+5 + i*5;
      p[i*4+1] = 5;
      p[i*4+2] = w;
      p[i*4+3] = h;
    }
    for (int i = 0; i < n-3; i++) {
      p[(i+3)*4] = (320 - ((n-3)*w) - ((n-4)*5)) / 2 + i*(w+5) + 1;
      p[(i+3)*4+1] = 72;
      p[(i+3)*4+2] = w;
      p[(i+3)*4+3] = h;
    }
  }
}

int p[24];
void drawDashboard(int heaters) {
  int w = 100;
  int h = 60;


  int x[3] = {5, 110, 215};

  int y[2] = {5, 72};

  setPos(heaters, p);

  for (int i = 0; i < heaters; i++) {
    if (HEATED_BED && i == heaters-1) {
      drawTemperatureCard(
        "BED", nozzleCurrent[i], nozzleTarget[i],
        p[i*4], p[i*4+1], p[i*4+2], p[i*4+3], BED, false, power[i], heaters
      );
    } else {
      char name[4];
      snprintf(name, sizeof(name), "T%d", i);

      drawTemperatureCard(
        name,
        nozzleCurrent[i],
        nozzleTarget[i],
        p[i*4],
        p[i*4+1],
        p[i*4+2], p[i*4+3],
        NOZZLE,
        selectedNozzle == i,
        power[i],
        heaters
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
  tft.setTextColor(0xFFFF);
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


int heaters;

void setup() {
  pinMode(BACKLIT_PIN, OUTPUT);
  if (BACKLIT_REVERSE) {
    ON = LOW;
    OFF = HIGH;
  }

  heaters = NOZZLES + (HEATED_BED ? 1 : 0);

  SPI.begin();

  tft.begin();
  tft.setSPISpeed(SPI_SPEED);
  tft.setRotation(1);
  tft.setTextWrap(false);
  waitScreen();
  percent = 0;
  tft.fillScreen(BG);
  drawDashboard(heaters);
  digitalWrite(BACKLIT_PIN, ON);
}

void loop() {
  percent += 1;
  if (percent > 100) {
    percent = 0;
  }
  clear();
  drawDashboard(heaters);
  delay(200);
}
