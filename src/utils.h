#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

void ERROR(const char *message, ...);
void CHECK_NULL_PTR(void *ptr);
bool CHECK_NON_ZERO_BUFFER(size_t length, uint8 *buf);

void display_buffer(uint8 buf[16]);

void l_rotate_word(uint8 word[4], uint8 positions);
void r_rotate_word(uint8 word[4], uint8 positions);
