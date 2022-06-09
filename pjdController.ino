#include <Wire.h>
#include <stdlib.h>

#include "Adafruit_MPR121.h"
#include "XInputPad.h"
#include "tusb.h"

extern "C" {
void sendReportData(void);
}
const int maruButton = 1;
const int batuButton = 2;
const int sikakuButton = 4;
const int sankakuButton = 3;

Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
int8_t startTouchedPosition = 0;
int8_t currTouchedPosition = 0;

void setup() {
  tusb_init();

  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);

  digitalWrite(PIN_LED_R, 1);
  digitalWrite(PIN_LED_G, 1);
  digitalWrite(PIN_LED_B, 1);

  pinMode(maruButton, INPUT_PULLDOWN);
  pinMode(batuButton, INPUT_PULLDOWN);
  pinMode(sikakuButton, INPUT_PULLDOWN);
  pinMode(sankakuButton, INPUT_PULLDOWN);

  if (!cap.begin(0x5A, &Wire)) {
    digitalWrite(PIN_LED_R, 0);

    while (1) {
      ;
    }
  }
  cap.setThreshholds(5, 2);
}

void loop() {
  XboxButtonData.digital_buttons_2 = 0;
  XboxButtonData.digital_buttons_1 = 0;

  XboxButtonData.digital_buttons_2 += (uint)digitalRead(maruButton) << 5;
  XboxButtonData.digital_buttons_2 += (uint)digitalRead(batuButton) << 4;
  XboxButtonData.digital_buttons_2 += (uint)digitalRead(sikakuButton) << 6;
  XboxButtonData.digital_buttons_2 += (uint)digitalRead(sankakuButton) << 7;

  slideBar();
  sendReportData();
  tud_task();  // tinyusb device task
}

void slideBar() {
  const uint32_t slideBarInterval_ms = 1;
  static uint32_t slideBarStart_ms = 0;

  if (millis() - slideBarStart_ms < slideBarInterval_ms) {
    return;  // not enough time
  }
  slideBarStart_ms += slideBarInterval_ms;

  currtouched = cap.touched();

  if (lasttouched == 0 && currtouched == 0) {
    //離れている
    startTouchedPosition = 0;
    currTouchedPosition = 0;
    XboxButtonData.l_x = 0;
  } else if (lasttouched == 0 && currtouched != 0) {
    //触れたとき
    startTouchedPosition = makeTouchedPosition(currtouched);
    XboxButtonData.l_x = 0;
  } else if (lasttouched != 0 && currtouched == 0) {
    //離れたとき
    startTouchedPosition = 0;
    currTouchedPosition = 0;
    XboxButtonData.l_x = 0;
  } else if (lasttouched != 0 && currtouched != 0) {
    //触れている途中
    currTouchedPosition = makeTouchedPosition(currtouched);
    int16_t dist = currTouchedPosition - startTouchedPosition;
    if (dist > 3) {
      dist = 3;
    } else if (dist < -3) {
      dist = -3;
    }
    XboxButtonData.l_x = dist * 10000;
  }

  lasttouched = currtouched;
}

int8_t makeTouchedPosition(uint16_t touched) {
  int8_t bit = 0;
  int8_t min = 0;
  for (bit = 0; bit < 12; bit++) {
    if (touched & (1 << bit)) {
      min = bit;
    }
  }

  int8_t max = 11;
  for (bit = 11; bit >= 0; bit--) {
    if (touched & (1 << bit)) {
      max = bit;
    }
  }

  return (min + max) / 2;
}
