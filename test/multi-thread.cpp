// #include "../src/aes.h"
// #include <thread>
// using namespace std;

// #define NUM_THREADS 2

int main() {

}

// int main() {
//   // A single threaded test of encrypting 64 bytes of data using one AES
//   // instance
//   uint8 key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
//                    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

//   uint8 inputs[16][16] = {
//       {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2,
//        0xe0, 0x37, 0x07, 0x34},
//       {0x4e, 0x34, 0x50, 0xe3, 0x11, 0x86, 0x2b, 0xef, 0x74, 0x46, 0xb0, 0xb4,
//        0xe1, 0x6c, 0x59, 0x16},
//       {0x4b, 0xa8, 0xa7, 0x21, 0x01, 0x46, 0x8e, 0x09, 0x9e, 0x98, 0x79, 0x18,
//        0x3f, 0x8a, 0x66, 0xdd},
//       {0x41, 0x7d, 0x77, 0xf6, 0x09, 0xa4, 0x16, 0x8d, 0x1a, 0x28, 0x1e, 0xb8,
//        0x8e, 0xcd, 0xfb, 0x13},
//       {0x44, 0xe2, 0x42, 0xb4, 0x99, 0xf7, 0x5f, 0xf8, 0x10, 0x48, 0x9f, 0x19,
//        0x8d, 0xe6, 0xba, 0x76},
//       {0x5e, 0xb1, 0x98, 0x82, 0xa6, 0x7f, 0xa0, 0x40, 0xbe, 0x36, 0xa6, 0x3d,
//        0x66, 0xbb, 0xa8, 0x53},
//       {0x6a, 0xd8, 0x76, 0xd8, 0xca, 0x3a, 0xa7, 0xe9, 0xf9, 0x0b, 0x40, 0x75,
//        0xc9, 0x69, 0x48, 0xfd},
//       {0xe4, 0xc4, 0xb4, 0xb7, 0x36, 0x7e, 0x22, 0xbb, 0x3a, 0xa2, 0x58, 0xbf,
//        0xf9, 0x30, 0xb9, 0x66},
//       {0x3f, 0xd6, 0xb2, 0x9b, 0x28, 0xa7, 0x12, 0xcd, 0xd3, 0x35, 0xb3, 0x6a,
//        0x46, 0xfe, 0x28, 0x15},
//       {0x31, 0xa1, 0xe2, 0xa9, 0x9f, 0x99, 0x1c, 0x95, 0x78, 0x26, 0xba, 0x56,
//        0x25, 0x59, 0x15, 0x25},
//       {0x05, 0x7b, 0x3a, 0x2e, 0x0b, 0x9e, 0x93, 0x33, 0x37, 0xbd, 0xf0, 0xf9,
//        0x33, 0x83, 0x43, 0x67},
//       {0xad, 0x1d, 0x67, 0x41, 0x47, 0x94, 0x35, 0x8c, 0x0a, 0x1a, 0x97, 0x42,
//        0x9d, 0xda, 0x12, 0x86},
//       {0xee, 0x88, 0x8c, 0xcd, 0xf7, 0x2e, 0x4e, 0x23, 0xb3, 0x14, 0xcb, 0xec,
//        0xd2, 0x46, 0xca, 0x2a},
//       {0x6d, 0x84, 0x92, 0xb8, 0x47, 0xd3, 0xc4, 0xe6, 0x82, 0x91, 0x1b, 0xfe,
//        0x2c, 0xcb, 0x48, 0xe0},
//       {0x82, 0xc7, 0xd0, 0xd9, 0x76, 0x73, 0xa6, 0x9c, 0xcc, 0x74, 0x82, 0xc2,
//        0x7f, 0x3b, 0x67, 0x2b},
//       {0x9a, 0xe4, 0xf3, 0x75, 0x40, 0xc5, 0x68, 0x9d, 0xc4, 0x6c, 0xee, 0x4e,
//        0xe8, 0xc0, 0xd3, 0xa6},
//   };

//   // Initialize AES instances and create round keys
//   AES<16> instances[NUM_THREADS];
//   uint8 round_keys[AES128_NR + 1][4][4];
//   key_expansion(16, key, round_keys);

//   printf("Original Buffer:\n");
//   for (uint8 i = 0; i < 16; i++)
//     display_buffer(inputs[i]);

//   // Thread handling
//   thread *threads[NUM_THREADS];
//   uint8 holders[16][16];

//   for (uint8 i = 0; i < 8; i++) {
//     for (uint8 j = 0; j < NUM_THREADS; j++)
//       threads[j] =
//           new thread(&AES<16>::encrypt, instances[j].state, inputs[2 * i + j],
//                      holders[2 * i + j], round_keys);
//     for (uint8 j = 0; j < NUM_THREADS; j++) threads[j]->join();
//   }

//   printf("After Encryption:\n");
//   for (uint8 i = 0; i < 16; i++) display_buffer(holders[i]);
// }
