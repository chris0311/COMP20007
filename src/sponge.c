/* 
sponge.c

Implementations for sponge construction initialisation and manipulation.

Skeleton written by Aidan Dang for COMP20007 Assignment 2 2022
  with Minor modifications by Grady Fitzpatrick
*/
#include <permutation.h>
#include <sponge.h>
#include <string.h>

void sponge_init(sponge_t *sponge) {
  // TODO: fill the rest of this function.
}

void sponge_read(uint8_t *dest, sponge_t const *sponge, uint64_t num) {
  // TODO: fill the rest of this function.
}

void sponge_write(sponge_t *sponge, uint8_t const *src, uint64_t num,
                  bool bw_xor) {
  // TODO: fill the rest of this function.
  // You may use the ^ operator to calculate a bit-wise XOR.
}

void sponge_demarcate(sponge_t *sponge, uint64_t i, uint8_t delimiter) {
  // TODO: fill the rest of this function.
}

void sponge_permute(sponge_t *sponge) {
  // TODO: fill the rest of this function.
  // You should use the permute_384 function from include/permutation.h.
}
