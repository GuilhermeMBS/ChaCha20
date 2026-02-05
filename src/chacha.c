#include "chacha.h"


static inline uint32_t rotate(uint32_t a, int n) {
    return (a << n) | (a >> (32 - n));
}

// Performs the ChaCha Quarter Round operation.
static inline void qround(uint32_t* state, uint32_t a, uint32_t b, uint32_t c, uint32_t d)  {
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
}

// Inserts the array of 1-byte elements as 4-bytes little endian elements into the state matrix starting from position `pos`.
static void load_little_endian(uint32_t* state, uint8_t* input, int size, int pos) {
    for (int i = 0; i < size; i++) {
        int index = i/4; // The index of the 4-bytes block from the input
        state[pos + index] <<= 8; // pos = offset of the state matrix
        state[pos + index] |= input[index*4 + 3 - i%4]; // Insert Backwards
    }
}

static void inner_block(uint32_t* state) {
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
}

static void serialize_state(uint32_t* state, uint8_t* key_stream) {
    uint32_t mask[16];
    for (int i = 0; i < 16; i++) mask[i] = state[i];

    for (int i = 0; i < 64; i++) {
        int index = i/4;
        key_stream[i] = mask[index] & 0xFF;
        mask[index] >>= 8;
    }
}

/**
 * Generates the next 64-byte block of keystream.
 * 1. Copies the internal state to a working state.
 * 2. Runs 20 rounds (10 iterations of inner_block).
 * 3. Adds the original state to the working state.
 * 4. Serializes the result into the context's keystream buffer.
 * 5. Increments the block counter.
 */
static void generate_block(chacha20_ctx* ctx) {
    uint32_t working_state[16];

    for (int i = 0; i < 16; i++) working_state[i] = ctx->state[i];
    for (int i = 0; i < 10; i++) inner_block(working_state);
    for (int i = 0; i < 16; i++) working_state[i] += ctx->state[i];

    serialize_state(working_state, ctx->keystream);
    ctx->state[12]++;
    
    ctx->keystream_idx = 0;
}

void chacha20_wipe(chacha20_ctx* ctx) {
    for (int i = 0; i < 16; i++) ctx->state[i] = 0;
    for (int i = 0; i < 64; i++) ctx->keystream[i] = 0;
    ctx->keystream_idx = 0;
}

// Just create the state matrix of the context, but does not serialize it
void chacha20_init(chacha20_ctx* ctx, uint8_t* key, uint8_t* counter, uint8_t* nonce) {
    chacha20_wipe(ctx);

    // Constants "expand 32-byte k"
    ctx->state[0] = 0x61707865;
    ctx->state[1] = 0x3320646e;
    ctx->state[2] = 0x79622d32;
    ctx->state[3] = 0x6b206574;

    load_little_endian(ctx->state, key, 32, 4);
    load_little_endian(ctx->state, counter, 4, 12);
    load_little_endian(ctx->state, nonce, 12, 13);

    ctx->keystream_idx = 64; // So the first call forces a new block generation (serialization + block update)
}

void chacha20_update(chacha20_ctx* ctx, uint8_t* input, uint8_t* output, size_t length) {    
    for (size_t i = 0; i < length; i++) {
        if (ctx->keystream_idx == 64) generate_block(ctx);
        output[i] = input[i] ^ ctx->keystream[ctx->keystream_idx];
        ctx->keystream_idx++;
    }
}
