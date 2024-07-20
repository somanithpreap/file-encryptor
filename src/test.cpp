#include "aes.h"

int main() {
  // Initialize State with buffers and print the state
  State state((uint8 *)"Hello, World!");
  printf("Original State:\n");
  state.print();

  // Test the block transformation and inverse functions and print
  state.add_round_key((uint8 *)"just random key ");
  printf("\nAfter round key addition:\n");
  state.print();
  state.add_round_key((uint8 *)"just random key ");
  printf("After round key addition (inverse):\n");
  state.print();

  state.sub_bytes(false);
  printf("\nAfter byte substitutions:\n");
  state.print();
  state.sub_bytes(true);
  printf("After inverse byte substitutions:\n");
  state.print();

  state.shift_rows(false);
  printf("\nAfter row shifts:\n");
  state.print();
  state.shift_rows(true);
  printf("After inverse row shifts:\n");
  state.print();

  state.mix_columns(false);
  printf("\nAfter column mixing:\n");
  state.print();
  state.mix_columns(true);
  printf("After inverse column mixing:\n");
  state.print();
}
