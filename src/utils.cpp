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

  bool non_zero = true;
  for (size_t i = 0; i < length; i++) {
    if (buf++) {
      non_zero = false;
      break;
    }
  }
  return non_zero;
}
