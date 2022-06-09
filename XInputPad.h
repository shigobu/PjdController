#ifndef _XINPUTPAD_H_
#define _XINPUTPAD_H_
#include <stdint.h>

typedef struct {
  uint8_t rid;
  uint8_t rsize;
  uint8_t digital_buttons_1;
  uint8_t digital_buttons_2;
  uint8_t lt;
  uint8_t rt;
  int16_t l_x;
  int16_t l_y;
  int16_t r_x;
  int16_t r_y;
  uint8_t reserved_1[6];
} ReportDataXinput;
extern ReportDataXinput XboxButtonData;

#endif
