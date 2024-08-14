#include "utils.h"

void ERROR(const char *message, ...) {
  printf("[ERR] ");
  va_list arg;
  va_start(arg, message);
  vprintf(message, arg);
  va_end(arg);
  printf("\n");
  exit(-1);
}

void CHECK_NULL_PTR(void *ptr) {
  if (ptr)
    return;
  ERROR("CHECK_NULL_PTR(): NULL pointer given.");
}

void display_buffer(uint8 buf[16]) {
  for (uint8 i = 0; i < 16; i++)
    printf("%.2X ", buf[i]);
  printf("\n");
}

void l_rotate_word(uint8 word[4], uint8 positions) {
  if (positions == 0)
    ERROR("l_rotate_word(): Positions to be shifted needs to be non-zero.");
  CHECK_NULL_PTR(word);
  positions %= 4;

  for (uint8 p = 0; p < positions; p++) {
    uint8 tmp = word[0];
    for (uint8 i = 0; i < 3; i++)
      word[i] = word[i + 1];
    word[3] = tmp;
  }
}

void r_rotate_word(uint8 word[4], uint8 positions) {
  if (positions == 0)
    ERROR("l_rotate_word(): Positions to be shifted needs to be non-zero.");
  CHECK_NULL_PTR(word);
  positions %= 4;

  for (uint8 p = 0; p < positions; p++) {
    uint8 tmp = word[3];
    for (uint8 i = 3; i > 0; i--)
      word[i] = word[i - 1];
    word[0] = tmp;
  }
}
