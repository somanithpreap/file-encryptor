#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uint8;

void ERROR(const char *message, ...);
void CHECK_NULL_PTR(void *ptr);

void display_buffer(uint8 buf[16]);

void l_rotate_word(uint8 word[4], uint8 positions);
void r_rotate_word(uint8 word[4], uint8 positions);
