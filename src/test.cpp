#include <iostream>
#include "aes.h"

int main() {
  uint8 a = 0x57;
  uint8 b = 0x10;
  printf("%hhu\n", __GF_mul__(a, b));
}
