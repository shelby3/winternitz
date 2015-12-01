#ifndef _WINTERNITZ
#define _WINTERNITZ

/*
Winternitz is a one-time use digital signature with selectable tradeoff between
speed and size.

### Signing

The public signature of each k-th chunk of the message being signed, is the
value of a cryptographic hash function H applied repetitively n times to the
k-th secret signature key value, i.e Hⁿ(secretₖ), where n is the value of the
k-th chunk (of the message being signed) subtracted from a chunk’s maximum
possible value. For example, the notation of repetitively 3 times H³(x) means
H(H(H(x))).

The chunks are a selectable number of bits in size, and the message size must
be a multiple of the chunk size.

The public signature also includes the chunk signatures of the checksum. The
checksum is the sum of all the values for the chunks in the signed message.
No checksum of the checksum is needed.

### Verification

A signature verifies if A) its checksum is equal to (or the never occurring
case of greater than) the checksum of the message; and B) if the public
verification key value equals the hash function applied to the concatenation of
all Hⁿ(xₖ), where xₖ is the k-th chunk signature and n is the value of the k-th
chunk from the message followed by the checksum. Since A is equivalent to
checking that the chunk signatures for the checksum have not changed (and per
the explanation that follows the chunk signatures could not have been forged),
then B does A; thus only B needs to be verified.

### Unforgeable

The checksum is required to prevent the construction—from a legitimate
signature—of a forged signature which signs a message other than the message
that was legitimately signed. Chunk signatures can only be plausibly computed in
the forward direction of repetitive hash function application, because the
irreversibility (i.e. confused invertibility) security of a cryptographic hash
function H(x) is the computational implausibility to find the preimage x.

Due to fact that Hⁱ⁺ⁿ(x) = Hⁱ(Hⁿ(x))—e.g. given H²(x) then H⁴(x) = H²(H²(x)) =
H(H(H²(x)))—forged chunk signatures can be computed by forward hashing the
legitimate chunk signatures for chunks that have *lesser* values than their
respective chunks in the legitimately signed message and checksum. Lesser is due
to “subtracted from a chunk’s maximum possible value” in the second paragraph
above; thus lesser values require a greater n number of forward direction hash
function repetitions than the legitimate chunk signature. But such a forgery
requires a *greater* value for the checksum (greater because checksums
accumulate not subtracted from the maximum possible chunk value), yet this would
require lower n number of repetitions for chunk signatures of the checksum— i.e.
requires the implausible computation of the preimage of the legitimate chunk
signature.

In other words, message chunk signatures can only be forged by *decreasing* (not
increasing) the chunk values in the signed message, yet this requires forging
checksum chunk signatures for *increasing* chunk values. Conversely if checksum
chunk values decrease, then message chunk values must increase. In both cases,
increasing a chunk value requires implausible computation of the preimage of the
corresponding legitimately signed chunk signature. Thus a forged signaure is
implausible.

In other words, the signature for the next lesser decrement of the chunk value
is the hash function applied to the signature of the chunk value. Whereas, the
signature for the next greater increment of the chunk value is preimage x of
the hash function H(x) where H(x) is the signature of the chunk value.

### Generalization

Winternitz[1] is a generalization of the Merkle’s improvement[2] to Lamport
signatures, wherein the chunk size is 1 bit. Lamport required two sets of public
and secret keys, one for each of the possible values of the binary chunk. Merkle
invented the checksum to replace the functionality of Lamport’s set for the 0
value case. Robert Winternitz[3] further generalized Merkle’s improvement to
chunks larger than 1 bit.

This implementation reverses the direction of repetitive hash function
application from [1] so as to coincide with [2], which also has the (probably
neglible speed) benefit that verification does not have to subtract chunk values
from the maximum chunk value.

Afaics (2^w - 1 - b_i) would be correct for equation (11) in [1] because the
chunk values b_i will never be greater than 2^w - 1; thus fewer bits needed for
the checksum when used is conjunction with the next paragraph or for cases where
the message is padded with sufficient zeros when meeting the requirement of its
size must be a multiple of the chunk size.

The requirement that the chunk size for the checksum must be equal to the chunk
size for the message[1] is removed in this implementation. This allows for more
flexible optimizations of the tradeoff between speed and size. For example, for
a message of two chunks and a chunk size of 2 bits, equation (6) from [1] says
two chunks of 2 bits each are required for the checksum. But the maximum value
of the checksum (per the improvement of the prior paragraph) is 6. Thus
instead there is the option to use a single chunk of 3 bits for the checksum
but with a maximum value of 6 instead of 7. The average number of hash function
repetitions for a 2-bit chunk with a maximum value of 3 is (0+1+2+3)/4 = 1.5;
whereas, for a 3-bit chunk with a maximum value of 6 is (0+1+2+3+4+5+6)/7 = 3.
That assumes uniform distribution of message values. Since 2 x 1.5 = 3, both
options have the same speed for the hash repetition factor, but the latter
option uses one signature less of size (and potentially impacts speed of the
concatenation for verification).

This implementation expects little-endian format for any I/O of the values
consumed and produced.

[1] D. Bernstein et al, Post-Quantum Cryptography, §1.2 Winternitz one-time signature scheme
    http://www.e-reading.club/bookreader.php/135832/Post_Quantum_Cryptography.pdf#page=45
[2] R. Merkle, A Certified Digital Signature, §4 An Improved One Time Signature
    http://www.merkle.com/papers/Certified1979.pdf#page=18
[3] R. Merkle, A Certified Digital Signature, §5 The Winternitz Improvement
    http://www.merkle.com/papers/Certified1979.pdf#page=20
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