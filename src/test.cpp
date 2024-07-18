#include "aes.h"

int main() {
  State state((uint8 *)"Hello, World!");
  state.print_state();
}
