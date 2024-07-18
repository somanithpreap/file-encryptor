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

bool CHECK_NON_ZERO_BUFFER(size_t length, uint8 *buf) {
  if (length == 0)
    ERROR("CHECK_NON_ZERO_BUFFER(): Buffer length cannot be zero.");
  CHECK_NULL_PTR(buf);

  bool non_zero = true;
  for (size_t i = 0; i < length; i++) {
    if (buf++) {
      non_zero = false;
      break;
    }
  }
  return non_zero;
}

void l_rotate_word(uint8 word[4], uint8 positions) {
  if (positions == 0)
    ERROR("l_rotate_word(): Positions to be shifted needs to be non-zero.");
  CHECK_NON_ZERO_BUFFER(4, word);
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
  CHECK_NON_ZERO_BUFFER(4, word);
  positions %= 4;

  for (uint8 p = 0; p < positions; p++) {
    uint8 tmp = word[3];
    for (uint8 i = 3; i > 0; i--)
      word[i] = word[i - 1];
    word[0] = tmp;
  }
}
