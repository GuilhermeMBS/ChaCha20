/**
 * @file chacha.h
 * @brief A stream-based implementation of the ChaCha20 stream cipher.
 *
 * This library implements the ChaCha20 encryption algorithm as described in RFC 8439.
 *
 * @author Guilherme Soares
 * @date 2026
 */

#ifndef CHACHA_H
#define CHACHA_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief The internal state of the ChaCha20 cipher.
 *
 * The structure holds the state initialized by the chacha20_init() as well as its serialization in the keystream array.
 * It keeps the pointer to the keystream so that it always utilizes all of it before making another block.
 */
typedef struct {
    uint32_t state[16];      /**< 4x4 matrix of 32-bit words representing the cipher state. */
    uint8_t keystream[64];   /**< Buffer holding the currently generated 64-byte key block. */
    size_t keystream_idx;    /**< Current read position within the keystream buffer. */
} chacha20_ctx;

/**
 * @brief Initializes the ChaCha20 context.
 *
 * This function wipes the context to remove garbage data, loads the constants,
 * key, nonce, and counter into the state matrix, and prepares the engine for the first block generation.
 *
 * @param ctx     Pointer to the context structure to be initialized.
 * @param key     Pointer to a 32-byte (256-bit) secret key.
 * @param counter Pointer to a 4-byte (32-bit) block counter (usually starts at 1).
 * @param nonce   Pointer to a 12-byte (96-bit) nonce (Number Used Once).
 */
void chacha20_init(chacha20_ctx* ctx, uint8_t* key, uint8_t* counter, uint8_t* nonce);

/**
 * @brief Encrypts or decrypts a chunk of data.
 *
 * Since ChaCha20 is a stream cipher, encryption and decryption are the same operation (XOR). 
 * This function can be called repeatedly to process large streams of data chunk by chunk.
 *
 * @note You can pass the same pointer for both `input` and `output` to perform in-place encryption.
 *
 * @param ctx    Pointer to the active ChaCha20 context.
 * @param input  Pointer to the source data buffer.
 * @param output Pointer to the destination buffer (must be at least `length` bytes).
 * @param length The number of bytes to process.
 */
void chacha20_update(chacha20_ctx* ctx, uint8_t* input, uint8_t* output, size_t length);

/**
 * @brief Securely wipes the context from memory.
 *
 * This function overwrites the internal state (including the key and unused keystream) with zeros.
 * It should be called immediately after the encryption session is finished to prevent sensitive data leaks.
 *
 * @param ctx Pointer to the context to be wiped.
 */
void chacha20_wipe(chacha20_ctx* ctx);

#endif // CHACHA_H