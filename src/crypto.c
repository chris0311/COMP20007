/* 
crypto.c

Implementations for cryptography primatives and functions
  making use of them.

Skeleton written by Aidan Dang for COMP20007 Assignment 2 2022
  with Minor modifications by Grady Fitzpatrick
  implementation by <You>
*/
#include <crypto.h>
#include <sponge.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The sponge's rate, in bytes. This represents the maximum number of bytes
// which can be read from or written the start of the sponge's state before a
// permutation must occur.
#define RATE 16
// Delimiter byte value used after absorption of the message
#define DELIMITER_A 0xAD
// Delimiter byte used at the end of the last-used block
#define DELIMITER_B 0X77

// Helpful min function that might be useful.
uint64_t min(uint64_t a, uint64_t b) { return a < b ? a : b; }

void hash(uint8_t *output, uint64_t output_len, uint8_t const *msg,
          uint64_t msg_len) {
    sponge_t sponge;
    sponge_init(&sponge);

    // TODO: fill the rest of this function.
    // Here are some examples of what sponge routines are called for various
    // invocations of this hash function:
    // hash(o, 5, m, 0) performs:
    //   sponge_write(&sponge, m, 0, true);
    //   sponge_demarcate(&sponge, 0, DELIMITER_A);
    //   sponge_demarcate(&sponge, RATE - 1, DELIMITER_B);
    //   sponge_permute(&sponge);
    //   sponge_read(o, &sponge, 5);
    //
    // hash(o, 16, m, 7) performs:
    //   sponge_write(&sponge, m, 7, true);
    //   sponge_demarcate(&sponge, 7, DELIMITER_A);
    //   sponge_demarcate(&sponge, RATE - 1, DELIMITER_B);
    //   sponge_permute(&sponge);
    //   sponge_read(o, &sponge, 16);
    //
    // hash(o, 23, m, 16) performs:
    //   sponge_write(&sponge, m, RATE, true);
    //   sponge_permute(&sponge);
    //   sponge_write(&sponge, m + RATE, 0, true);
    //   sponge_demarcate(&sponge, 0, DELIMITER_A);
    //   sponge_demarcate(&sponge, RATE - 1, DELIMITER_B);
    //   sponge_permute(&sponge);
    //   sponge_read(o, &sponge, RATE);
    //   sponge_permute(&sponge);
    //   sponge_read(o + RATE, &sponge, 7);
    //
    // hash(o, 32, m, 23) performs:
    //   sponge_write(&sponge, m, RATE, true);
    //   sponge_permute(&sponge);
    //   sponge_write(&sponge, m + RATE, 7, true);
    //   sponge_demarcate(&sponge, 7, DELIMITER_A);
    //   sponge_demarcate(&sponge, RATE - 1, DELIMITER_B);
    //   sponge_permute(&sponge);
    //   sponge_read(o, &sponge, RATE);
    //   sponge_permute(&sponge);
    //   sponge_read(o + RATE, &sponge, 16);

    int rate_count = 0;
    // Write the message to the sponge
    //printf("msg_len: %d\n", msg_len);
    while(msg_len > 16){
        sponge_write(&sponge, msg + rate_count*RATE, RATE, true);
        sponge_permute(&sponge);
        msg_len -= RATE;
        rate_count++;
    }
    //printf("msg_len: %d\n", msg_len);
    sponge_write(&sponge, msg + rate_count*RATE, msg_len, true);
    sponge_demarcate(&sponge, msg_len, DELIMITER_A);
    sponge_demarcate(&sponge, RATE-1, DELIMITER_B);
    sponge_permute(&sponge);

    //printf("out_len: %d\n", output_len);
    // Read the output from the sponge
    rate_count = 0;
    while (output_len > 16){
        sponge_read(output + rate_count*RATE, &sponge, RATE);
        sponge_permute(&sponge);
        output_len -= RATE;
        rate_count++;
    }
    //printf("out_len: %d\n", output_len);
    sponge_read(output + rate_count*RATE, &sponge, output_len);
}

void mac(uint8_t *tag, uint64_t tag_len, uint8_t const *key, uint8_t const *msg,
         uint64_t msg_len) {
    sponge_t sponge;
    sponge_init(&sponge);

    // TODO: fill the rest of this function.
    // Your implementation should like very similar to that of the hash
    // function's, but should include a keying phase before the absorbing phase.
    // If you wish, you may also treat this as calculating the hash of the key
    // prepended to the message.

    // Write the key to the sponge
    for (int i = 0; i < 2; i++){
        sponge_write(&sponge, key + i*RATE, RATE, true);
        sponge_permute(&sponge);
    }

    // Write the message to the sponge
    int rate_count = 0;
    // Write the message to the sponge
    while(msg_len > 16){
        sponge_write(&sponge, msg + rate_count*RATE, RATE, true);
        sponge_permute(&sponge);
        msg_len -= RATE;
        rate_count++;
    }
    sponge_write(&sponge, msg + rate_count*RATE, msg_len, true);
    sponge_demarcate(&sponge, msg_len, DELIMITER_A);
    sponge_demarcate(&sponge, RATE-1, DELIMITER_B);
    sponge_permute(&sponge);

    // Read the output from the sponge
    rate_count = 0;
    while (tag_len > 16){
        sponge_read(tag + rate_count*RATE, &sponge, RATE);
        sponge_permute(&sponge);
        tag_len -= RATE;
        rate_count++;
    }
    sponge_read(tag + rate_count*RATE, &sponge, tag_len);
}

void auth_encr(uint8_t *ciphertext, uint8_t *tag, uint64_t tag_len,
               uint8_t const *key, uint8_t const *plaintext,
               uint64_t text_len) {
    sponge_t sponge;
    sponge_init(&sponge);

    // TODO: fill the rest of this function.
    // Your implementation should like very similar to that of the mac function's,
    // but should after each write into the sponge's state, there should
    // immediately follow a read from the sponge's state of the same number of
    // bytes, into the ciphertext buffer.

    // Write the key to the sponge
    for (int i = 0; i < 2; i++){
        sponge_write(&sponge, key + i*RATE, RATE, true);
        sponge_permute(&sponge);
    }

    // Write the message to the sponge
    int rate_count = 0;
    // Write the message to the sponge
    while(text_len > 16){
        sponge_write(&sponge, plaintext + rate_count*RATE, RATE, true);
        sponge_read(ciphertext + rate_count*RATE, &sponge, RATE);
        sponge_permute(&sponge);
        text_len -= RATE;
        rate_count++;
    }
    sponge_write(&sponge, plaintext + rate_count*RATE, text_len, true);
    sponge_read(ciphertext + rate_count*RATE, &sponge, text_len);
    sponge_demarcate(&sponge, text_len, DELIMITER_A);
    sponge_demarcate(&sponge, RATE-1, DELIMITER_B);
    sponge_permute(&sponge);

    // Read the output from the sponge
    rate_count = 0;
    while (tag_len > 16){
        sponge_read(tag + rate_count*RATE, &sponge, RATE);
        sponge_permute(&sponge);
        tag_len -= RATE;
        rate_count++;
    }
    sponge_read(tag + rate_count*RATE, &sponge, tag_len);

}

int auth_decr(uint8_t *plaintext, uint8_t const *key, uint8_t const *ciphertext,
              uint64_t text_len, uint8_t const *tag, uint64_t tag_len) {
    sponge_t sponge;
    sponge_init(&sponge);

    // TODO: fill the rest of this function.
    // The implementation of this function is left as a challenge. It may assist
    // you to know that a ^ b ^ b = a. Remember to return 0 on success, and 1 on
    // failure.

    // Write the key to the sponge
    for (int i = 0; i < 2; i++){
        sponge_write(&sponge, key + i*RATE, RATE, true);
        sponge_permute(&sponge);
    }

    // Write the ciphertext to sponge and decrypt
    // sponge_write(&sponge, ciphertext, 16, true);
    // sponge_read(plaintext, &sponge, 16);
    // sponge_write(&sponge, ciphertext, 16, false);
    // sponge_permute(&sponge);

    // sponge_write(&sponge, ciphertext+16, 16, true);
    // sponge_read(plaintext+16, &sponge, 16);
    // sponge_write(&sponge, ciphertext+16, 16, false);
    // sponge_permute(&sponge);

    // sponge_write(&sponge, ciphertext+32, 1, true);
    // sponge_read(plaintext+32, &sponge, 1);
    // sponge_write(&sponge, ciphertext+32, 1, false);
    // sponge_permute(&sponge);

    int rate_count = 0;
    uint64_t text_len2 = text_len;
    // Write the ciphertext to sponge and decrypt
    while(text_len > 16){
        sponge_write(&sponge, ciphertext + rate_count*RATE, RATE, true);
        sponge_read(plaintext + rate_count*RATE, &sponge, RATE);
        sponge_write(&sponge, ciphertext + rate_count*RATE, RATE, false);
        sponge_permute(&sponge);
        text_len -= RATE;
        rate_count++;
    }
    sponge_write(&sponge, ciphertext + rate_count*RATE, text_len, true);
    sponge_read(plaintext + rate_count*RATE, &sponge, text_len);
    sponge_write(&sponge, ciphertext + rate_count*RATE, text_len, false);
    sponge_permute(&sponge);

    // use the mac system to authenticate message
    uint8_t new_tag[tag_len];
    mac(new_tag, tag_len, key, plaintext, text_len2);
    // check if the tag is the same
    for (int i = 0; i < tag_len; i++){
        if (new_tag[i] != tag[i]){
            return 1;
        }
    }

    return 0;

}