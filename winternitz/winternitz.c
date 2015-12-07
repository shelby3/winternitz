#include "winternitz.h"

#undef winternitz

void* EXPORT
winternitz(const bool sign, void* state, winternitz_callback hash, const uint32* msg, size_t bits, const uint8 chunk_bits, const uint8* checksum_bits) {
  if (chunk_bits > SIZEOF(*msg) * 8) return NULL;         // Error: message chunk size larger than *msg word size?   (would require more complex logic below to handle this case and winternitz is not practical for such large chunk sizes)
  if (bits % chunk_bits != 0) return NULL;                // Error: message is smaller than or not a multiple of chunk size? (or chunk_bits==0 && bits!=0)    (works even if bits<0 but bits is an unsigned type)
  uint8 t = 0;
  for(const uint8* p = checksum_bits; *p != 0; p++) t += *p;
  uint32 chunk_max = ((uint32)1 << chunk_bits) - 1;
  uint32 checksum_max = chunk_max * (bits / chunk_bits);
  if (checksum_max > (((uint32)1 << t) - 1)) return NULL; // Error: insufficient checksum_bits?
  // Sign the chunks of the message
  uint32 checksum = 0;
  uint8 cur_bits = 0;
  uint32 cur = 0;
  while (bits != 0) {
    uint32 chunk_val;
    if (cur_bits - chunk_bits >= 0) {
      chunk_val = cur & chunk_max;
      cur_bits -= chunk_bits;
      cur >>= chunk_bits;
    } else if (cur_bits == 0) {
      chunk_val = *msg & chunk_max;
      cur_bits = SIZEOF(*msg) * 8 - chunk_bits;
      cur = *msg >> chunk_bits;
      msg += 1;
    } else {
      chunk_val = cur << (chunk_bits - cur_bits) | (*msg & chunk_max >> cur_bits);
      cur_bits = SIZEOF(*msg) * 8 - (chunk_bits - cur_bits);
      cur = *msg >> (chunk_bits - cur_bits);
      msg += 1;
    }
    state = hash(state, sign ? chunk_max - chunk_val : chunk_val);
    if (state == NULL) return NULL;                       // Error: hash function failed?
    checksum += chunk_val;
    bits -= chunk_bits;
  }
  // Sign the chunks of the checksum
  while (*checksum_bits != 0) {
    chunk_max = ((uint32)1 << *checksum_bits) - 1;
    if (chunk_max > checksum_max) chunk_max = checksum_max;
    state = hash(state, sign ? chunk_max - (checksum & chunk_max) : checksum & chunk_max);
    if (state == NULL) return NULL;                       // Error: hash function failed?
    checksum >>= *checksum_bits;
    checksum_max >>= *checksum_bits;
    checksum_bits += 1;
  }
  return state;
}

void* EXPORT
winternitz2(const bool sign, void* state, winternitz_callback hash, const uint32* msg, size_t bits, const uint8 chunk_bits/*use chunk_bits:, const uint8* checksum_bits*/) {
  if (chunk_bits > SIZEOF(*msg) * 8) return NULL;         // Error: message chunk size larger than *msg word size?   (would require more complex logic below to handle this case and winternitz is not practical for such large chunk sizes)
  if (bits % chunk_bits != 0) return NULL;                // Error: message is smaller than or not a multiple of chunk size? (or chunk_bits==0 && bits!=0)    (works even if bits<0 but bits is an unsigned type)
  uint32 chunk_max = ((uint32)1 << chunk_bits) - 1;
  uint32 checksum_max = chunk_max * (bits / chunk_bits);
  // Sign the chunks of the message
  uint32 checksum = 0;
  uint8 cur_bits = 0;
  uint32 cur = 0;
  while (bits != 0) {
    uint32 chunk_val;
    if (cur_bits - chunk_bits >= 0) {
      chunk_val = cur & chunk_max;
      cur_bits -= chunk_bits;
      cur >>= chunk_bits;
    } else if (cur_bits == 0) {
      chunk_val = *msg & chunk_max;
      cur_bits = SIZEOF(*msg) * 8 - chunk_bits;
      cur = *msg >> chunk_bits;
      msg += 1;
    } else {
      chunk_val = cur << (chunk_bits - cur_bits) | (*msg & chunk_max >> cur_bits);
      cur_bits = SIZEOF(*msg) * 8 - (chunk_bits - cur_bits);
      cur = *msg >> (chunk_bits - cur_bits);
      msg += 1;
    }
    state = hash(state, sign ? chunk_max - chunk_val : chunk_val);
    if (state == NULL) return NULL;                       // Error: hash function failed?
    checksum += chunk_val;
    bits -= chunk_bits;
  }
  // Sign the chunks of the checksum
  while (checksum_max) {
    if (chunk_max > checksum_max) chunk_max = checksum_max;
    state = hash(state, sign ? chunk_max - (checksum & chunk_max) : checksum & chunk_max);
    if (state == NULL) return NULL;                       // Error: hash function failed?
    checksum >>= chunk_bits;
    checksum_max >>= chunk_bits;
  }
  return state;
}

void* EXPORT
merkle_lamport(const bool sign, void* state, winternitz_callback hash, const uint32* msg, size_t bits/*1 bit:, const uint8 chunk_bits, const uint8* checksum_bits*/) {
  uint32 checksum_max = bits;
  // Sign the chunks of the message
  uint32 checksum = 0;
  uint8 cur_bits = 0;
  uint32 cur = 0;
  while (bits != 0) {
    uint32 chunk_val;
    if (cur_bits > 0) {
      chunk_val = cur & 1;
      cur_bits -= 1;
      cur >>= 1;
    } else {
      chunk_val = *msg & 1;
      cur_bits = SIZEOF(*msg) * 8 - 1;
      cur = *msg >> 1;
      msg += 1;
    }
    state = hash(state, sign ? 1 - chunk_val : chunk_val);
    if (state == NULL) return NULL;                       // Error: hash function failed?
    checksum += chunk_val;
    bits -= 1;
  }
  // Sign the chunks of the checksum
  while (checksum_max != 0) {
    state = hash(state, sign ? 1 - (checksum & 1) : checksum & 1);
    if (state == NULL) return NULL;                       // Error: hash function failed?
    checksum >>= 1;
    checksum_max >>= 1;
  }
  return state;
}