#ifndef _WINTERNITZ
#define _WINTERNITZ

/*
Documentation is in Winternitz.md
*/

#include "../cmacros/c11types.h"
#include "../cmacros/export.h"

/*
This API is orthogonal (i.e. agnostic) to the hash function and key management
employed.

The callback function is called once for each chunk, with the checksum chunks
last.

On signing, the callback function should apply the hash function n times to the
k-th secret signing key.

On verify, the callback function should apply the hash function n times to the
k-th chunk signature.

The 'checksum_bits' should be terminated with a zero value.
*/

void* EXPORT
winternitz(const bool sign, void* state, void* hash(void* state, uint32 n), const uint32* msg, size_t bits, const uint8 chunk_bits, const uint8* checksum_bits);

void* EXPORT
merkle_lamport(const bool sign, void* state, void* hash(void* state, uint32 n), const uint32* msg, size_t bits/*1 bit:, const uint8 chunk_bits, const uint8* checksum_bits*/);

#include "../cmacros/variadic.h"

// winternitz() with 5 arguments is replaced with merkle_lamport() on those 5 arguments.

#define   merkle_lamport_5(a, b, c, d, e) a, b, c, d, e
#define   winternitz_7(a, b, c, d, e, f, g) a, b, c, d, e, f, g
#define   winternitz_5_name() merkle_lamport
#define   winternitz_7_name() winternitz
#define   winternitz(...) NVARIADIC(winternitz, NUMARG7(__VA_ARGS__), __VA_ARGS__)

#endif