#include <stdio.h>
#include <stdint.h>


void add_block(uint8_t* block);

void input2state(uint32_t* state, uint8_t* in, int size, int pos);

void create_state(uint32_t* state, uint8_t* key, uint8_t* block, uint8_t* nonce);

void print_state(uint32_t* state);

uint32_t rotate(uint32_t a, int n);

void qround(uint32_t* state, uint32_t a, uint32_t b, uint32_t c, uint32_t d);

void inner_block(uint32_t* state);

void chacha20_block(uint32_t* state, uint8_t* key, uint8_t* block, uint8_t* nonce);
