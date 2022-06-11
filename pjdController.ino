#include <Wire.h>
#include <stdlib.h>

#include "Adafruit_MPR121.h"
#include "Adafruit_TinyUSB.h"

const int maruButton = 1;
const int batuButton = 2;
const int sikakuButton = 4;
const int sankakuButton = 3;
const int l = D0;
const int r = D1;

Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
int8_t startTouchedPosition = 0;
int8_t currTouchedPosition = 0;

// HID report descriptor using TinyUSB's template
// Single Report (no ID) descriptor
uint8_t const desc_hid_report[] = {TUD_HID_REPORT_DESC_GAMEPAD()};

// USB HID object. For ESP32 these values cannot be changed after this
// declaration desc report, desc len, protocol, interval, use out endpoint
Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report),
                          HID_ITF_PROTOCOL_NONE, 2, false);

// Report payload defined in src/class/hid/hid.h
// - For Gamepad Button Bit Mask see  hid_gamepad_button_bm_t
// - For Gamepad Hat    Bit Mask see  hid_gamepad_hat_t
hid_gamepad_report_t gp;

void setup() {
#if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
  // Manual begin() is required on core without built-in support for TinyUSB
  // such as mbed rp2040
  TinyUSB_Device_Init(0);
#endif

  usb_hid.begin();

  // wait until device mounted
  while (!TinyUSBDevice.mounted()) delay(1);

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
  pinMode(l, INPUT_PULLDOWN);
  pinMode(r, INPUT_PULLDOWN);

  if (!cap.begin(0x5A, &Wire)) {
    digitalWrite(PIN_LED_R, 0);

    while (1) {
      ;
    }
  }
  cap.setThreshholds(5, 2);
}

void loop() {
  if (!usb_hid.ready()) return;

  // Reset buttons
  gp.x = 0;
  gp.y = 0;
  gp.z = 0;
  gp.rz = 0;
  gp.rx = 0;
  gp.ry = 0;
  gp.hat = 0;
  gp.buttons = 0;

  gp.buttons += (uint)digitalRead(maruButton) << 1;
  gp.buttons += (uint)digitalRead(batuButton) << 0;
  gp.buttons += (uint)digitalRead(sikakuButton) << 2;
  gp.buttons += (uint)digitalRead(sankakuButton) << 3;
  gp.buttons += (uint)digitalRead(l) << 4;
  gp.buttons += (uint)digitalRead(r) << 5;

  slideBar();

  usb_hid.sendReport(0, &gp, sizeof(gp));
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
    gp.x = 0;
  } else if (lasttouched == 0 && currtouched != 0) {
    //触れたとき
    startTouchedPosition = makeTouchedPosition(currtouched);
    gp.x = 0;
  } else if (lasttouched != 0 && currtouched == 0) {
    //離れたとき
    startTouchedPosition = 0;
    currTouchedPosition = 0;
    gp.x = 0;
  } else if (lasttouched != 0 && currtouched != 0) {
    //触れている途中
    currTouchedPosition = makeTouchedPosition(currtouched);
    int16_t dist = currTouchedPosition - startTouchedPosition;
    if (dist > 3) {
      dist = 3;
    } else if (dist < -3) {
      dist = -3;
    }
    gp.x = dist * 42;
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
