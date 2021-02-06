#include <M5Stack.h>
#include <Adafruit_AS7341.h>

const uint16_t F1 = M5.lcd.color565(0x76, 0x00, 0xed);
const uint16_t F2 = M5.lcd.color565(0x00, 0x28, 0xff);
const uint16_t F3 = M5.lcd.color565(0x00, 0xd5, 0xff);
const uint16_t F4 = M5.lcd.color565(0x1f, 0xff, 0x00);
const uint16_t F5 = M5.lcd.color565(0xb3, 0xff, 0x00);
const uint16_t F6 = M5.lcd.color565(0xff, 0xdf, 0x00);
const uint16_t F7 = M5.lcd.color565(0xff, 0x4f, 0x00);
const uint16_t F8 = M5.lcd.color565(0xff, 0x00, 0x00);

const int ASTEP = 599;
const int ATIME = 29;
const int FULLSCALE = (ASTEP + 1) * (ATIME * 1);

int32_t left(int index) {
  return index * 40 + 2;
}

int32_t right(int index) {
  return index * 40 + 38;
}

int32_t height(int value) {
  return (value * 190) / FULLSCALE;
}

Adafruit_AS7341 as7341;  

void fillValue(int left, uint16_t color, int value) {
  int32_t h = height(value);
  m5.Lcd.fillRect(left, 192 - h, 36, h, color);
}

as7341_gain_t gainOptions[11] = {AS7341_GAIN_0_5X, AS7341_GAIN_1X, AS7341_GAIN_2X, AS7341_GAIN_4X, AS7341_GAIN_8X, AS7341_GAIN_16X, AS7341_GAIN_32X,
  AS7341_GAIN_64X, AS7341_GAIN_128X, AS7341_GAIN_256X, AS7341_GAIN_512X};
const char* gainNames[11] = {"0.5x", "1x", "2x", "4x", "8x", "16", "32x", "64x", "128x", "256x", "512x"};
int gainIndex = 6;

void readAndDraw() {
  uint16_t readings[12];
  as7341.setGain(gainOptions[gainIndex]);
  as7341.readAllChannels(readings);

  m5.Lcd.fillScreen(TFT_BLACK);
  fillValue(left(0), F1, readings[0]);
  fillValue(left(1), F2, readings[1]);
  fillValue(left(2), F3, readings[2]);
  fillValue(left(3), F4, readings[3]);
  fillValue(left(4), F5, readings[6]);
  fillValue(left(5), F6, readings[7]);
  fillValue(left(6), F7, readings[8]);
  fillValue(left(7), F8, readings[9]);

  uint16_t flicker_freq = as7341.detectFlickerHz();

  M5.Lcd.setTextColor(WHITE, TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(5, 200);
  M5.Lcd.printf("Gain: %s CLEAR: %d", gainNames[gainIndex], readings[10]);  
  M5.Lcd.setCursor(5, 220);
  const char* flicker =
    flicker_freq == 0 ? "None" : 
    flicker_freq == 100 ? "50Hz" : 
    flicker_freq == 120 ? "60Hz" :
    "Unknown";
  M5.Lcd.printf("Freq: %s", flicker);
}

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Power.begin();

  bool success = as7341.begin();

  if (success) {
    as7341.setATIME(ATIME);
    as7341.setASTEP(ASTEP);
  }

    readAndDraw();
}

void loop() {

  if (M5.BtnB.wasPressed()) {
    readAndDraw();
  } 
  else if (M5.BtnA.wasPressed() && gainIndex > 0) {
    --gainIndex;
    readAndDraw();
  }
  else if (M5.BtnC.wasPressed() && gainIndex < 10) {
    ++gainIndex;
    readAndDraw();
  }

  M5.update();
}