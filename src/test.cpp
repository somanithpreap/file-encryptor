#include "aes.h"

int main() {
  State state((uint8 *)"Hello, World!");
  state.print_state();
  state.add_round_key((uint8 *)"just random key ");
  state.sub_bytes();
  state.shift_rows();
  state.mix_columns();
  state.print_state();
}
