#include "operations.h"


void serialize_state(uint32_t* state, uint8_t* key_stream);

void chacha20_encrypt(uint8_t* key, uint8_t* block, uint8_t* nonce, FILE* plaintext, FILE* encrypted_message);