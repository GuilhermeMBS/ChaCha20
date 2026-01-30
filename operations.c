#include "operations.h"


void input2state(uint32_t* state, uint8_t* in, int size, int pos) {
    for (int i = 0; i < size; i++) {
        int index = i/4; // The index of the 4-bytes block from the input
        state[pos + index] <<= 8; // pos = offset of the state matrix
        state[pos + index] |= in[index*4 + 3 - i%4]; // Insert Backwards
    }

    return;
}


void create_state(uint32_t* state, uint8_t* key, uint8_t* block, uint8_t* nonce)  {
    
    input2state(state, key, 32, 4);
    input2state(state, block, 4, 12);
    input2state(state, nonce, 12, 13);
    
    return;
}


void print_state(uint32_t* state)  {
    for (int i = 0; i < 16; i++) {
        if (i % 4 == 0) printf("\n");
        printf("%08x ", state[i]);
    }

    return;
}


uint32_t rotate(uint32_t a, int n) {
    uint32_t tmp = a >> (32 - n);
    return (a << n) | tmp;
}


void qround(uint32_t* state, uint32_t a, uint32_t b, uint32_t c, uint32_t d)  {
    state[a] += state[b];
    state[d] ^= state[a];
    state[d] = rotate(state[d], 16);

    state[c] += state[d];
    state[b] ^= state[c];
    state[b] = rotate(state[b], 12);

    state[a] += state[b]; 
    state[d] ^= state[a];
    state[d] = rotate(state[d], 8);
    
    state[c] += state[d]; 
    state[b] ^= state[c];
    state[b] = rotate(state[b], 7);

    return;
}


void inner_block(uint32_t* state) {
    // Columns
    qround(state, 0, 4, 8, 12);
    qround(state, 1, 5, 9, 13);
    qround(state, 2, 6, 10, 14);
    qround(state, 3, 7, 11, 15);

    // Diagonals
    qround(state, 0, 5, 10, 15);
    qround(state, 1, 6, 11, 12);
    qround(state, 2, 7, 8, 13);
    qround(state, 3, 4, 9, 14);
    
    return;
}


void chacha20_block(uint32_t* state, uint8_t* key, uint8_t* block, uint8_t* nonce) {

    create_state(state, key, block, nonce);

    // printf("\n\nINITIAL STATE:");
    // print_state(state);
    
    uint32_t initial_state[16];
    for (int i = 0; i < 16; i++) initial_state[i] = state[i]; // initial_state = state

    for (int i = 0; i < 10; i++) inner_block(state);
    // printf("\n\nSTATE AFTER 20 ROUNDS:");
    // print_state(state);

    for (int i = 0; i < 16; i++) state[i] += initial_state[i];
    // printf("\n\nFINAL STATE:");
    // print_state(state);

    return;
}
