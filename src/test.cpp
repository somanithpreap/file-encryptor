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

  state.add_round_key((uint8*)"just random key");
  state.sub_bytes(false);
  state.shift_rows(false);
  state.mix_columns(false);

  uint8 enc[16];
  state.serialize(enc);
  for (uint8 i = 0; i < 16; i++) printf("%c", enc[i]);
  printf("\n");

  state.mix_columns(true);
  state.shift_rows(true);
  state.sub_bytes(true);
  state.add_round_key((uint8*)"just random key");

  state.serialize(enc);
  for (uint8 i = 0; i < 16; i++) printf("%c", enc[i]);
  printf("\n");

  //#define len 33
  //unsigned char word[len];
  //for (uint8 i = 0; i < len; i++) scanf("%c", &word[i]);
  //for (uint8 i = 0; i < len; i++) printf("%.2X ", word[i]);
  //printf("\n");

  //Before testing for the output of key_expansion, you should undo all the comment lines that has working code first 
  /*
  uint8 myIn[16] = {45, 137, 223, 14, 56, 78, 190, 123, 9, 200, 176, 89, 34, 150, 2, 73};
  uint8 myKey[32] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};
  AES aes1(myIn, myKey);
  aes1.key_expansion(myKey);
  */
  
}
