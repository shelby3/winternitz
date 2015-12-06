# Winternitz

Winternitz is a one-time use digital signature with selectable tradeoff between speed and size.

### Signing

The public signature of each k-th chunk of the message being signed, is the value of a cryptographic hash function H applied recursively n times to the k-th secret signature key value, i.e Hⁿ(secretₖ), where n is the value of the k-th chunk (of the message being signed) subtracted from a chunk’s maximum possible value. For example, the notation of recursively 3 times H³(x) means H(H(H(x))).

The chunks are a selectable number of bits in size, and the message size must be a multiple of the chunk size.

The public signature also includes the chunk signatures of the checksum. The checksum is the sum of all the values for the chunks in the signed message. No checksum of the checksum is needed.

### Verification

A signature verifies if A) its checksum is equal to (or the never occurring case of greater than) the checksum of the message; and B) if the public verification key value equals the hash function applied to the concatenation of all Hⁿ(xₖ), where xₖ is the k-th chunk signature and n is the value of the k-th chunk from the message followed by the checksum. Since A is equivalent to checking that the chunk signatures for the checksum have not changed (and per the explanation that follows the chunk signatures could not have been forged), then B does A; thus only B needs to be verified.

### Unforgeability

The checksum is required to prevent adaptive chosen message attack which is the construction—from a legitimate signature—of a forged signature which signs a message other than the message that was legitimately signed. Chunk signatures can only be plausibly computed in the forward direction of recursive hash function application, because the irreversibility (i.e. confused invertibility) security of a cryptographic hash function H(x) is the computational implausibility (i.e. [intractable](https://en.wikipedia.org/wiki/Computational_complexity_theory#Intractability) complexity of computation required) to find the preimage x.

Due to fact that Hⁱ⁺ⁿ(x) = Hⁱ(Hⁿ(x))—e.g. given H²(x) then H⁴(x) = H²(H²(x)) = H(H(H²(x)))—forged chunk signatures can be computed by forward hashing the legitimate chunk signatures for chunks that have *lesser* values than their respective chunks in the legitimately signed message and checksum. Lesser is due to “subtracted from a chunk’s maximum possible value” in the second paragraph above; thus lesser values require a greater n number of forward direction hash function repetitions than the legitimate chunk signature. But such a forgery requires a *greater* value for the checksum (greater because checksums accumulate not subtracted from the maximum possible chunk value), yet this would require lower n number of repetitions for chunk signatures of the checksum— i.e. requires the intractable computation of the preimage of the legitimate chunk signature.

In other words, message chunk signatures can only be forged by *decreasing* (not increasing) the chunk values in the signed message, yet this requires forging checksum chunk signatures for *increasing* chunk values. Conversely if checksum chunk values decrease, then message chunk values must increase. In both cases, increasing a chunk value requires intractable computation of the preimage of the corresponding legitimately signed chunk signature. Thus a forged signaure is implausible.

In other words, the signature for the next lesser decrement of the chunk value is the hash function applied to the signature of the chunk value. Whereas, the signature for the next greater increment of the chunk value is preimage x of the hash function H(x) where H(x) is the signature of the chunk value.

The bit security is equal to the number of bits of the cryptographic hash function, because only the second preimage resistance is required to prevent an adaptive chosen message attack[1][2]. Collision resistance is not required because the birthday attack[3] does not generate collisions for a specified hash function value.

However, assuming the signed message is the value of a cryptographic hash function, the collision resistance (which is at most but oft-less than 2<sup>*bits*/2</sup>) does apply to the bit length *bits* of the signed message in the case where random message preimages are acceptable to the attacker.

The bit security of hash-based signatures does not diminish significantly in a practical quantum computing model[4][5].

### Generalization

Winternitz[6] is a generalization of the Merkle’s improvement[7] to Lamport signatures, wherein the chunk size is 1 bit. Lamport required two sets of public and secret keys, one for each of the possible values of the binary chunk. Merkle invented the checksum to replace the functionality of Lamport’s set for the 0 value case. Robert Winternitz[8] further generalized Merkle’s improvement to chunks larger than 1 bit.

This implementation reverses the direction of recursive hash function application from [6] so as to coincide with [7], which also has the (probably neglible speed) benefit that verification does not have to subtract chunk values from the maximum chunk value.

Afaics (2ʷ-1 - bᵢ) would be correct for equation (11) in [6] because the chunk values bᵢ will never be greater than 2ʷ-1; thus fewer bits (replacing (6) in [6] with t₂ = ⌈log₂(t₁(2ʷ-1)) ÷ w⌉) needed for the checksum when used is conjunction with the next paragraph or for cases where the message is padded with sufficient zeros when meeting the requirement of its size must be a multiple of the chunk size.

The requirement that the chunk size for the checksum must be equal to the chunk size for the message[6] is removed in this implementation. This allows for more flexible optimizations of the tradeoff between speed and size. For example, for a message of two chunks and a chunk size of 2 bits, equation (6) from [6] says two chunks of 2 bits each are required for the checksum. But the maximum value of the checksum (per the improvement of the prior paragraph) is 6. Thus instead there is the option to use a single chunk of 3 bits for the checksum but with a maximum value of 6 instead of 7. The average number of hash function repetitions for a 2-bit chunk with a maximum value of 3 is (0+1+2+3) ÷ 4 = 1.5; whereas, for a 3-bit chunk with a maximum value of 6 is (0+1+2+3+4+5+6) ÷ 7 = 3. That assumes uniform distribution of message values. Since 2 × 1.5 = 3, both options have the same speed for the hash repetition factor, but the latter option uses one signature less of size (and potentially impacts speed of the concatenation for verification).

### Binary compatibility

This implementation expects little-endian format for any I/O of the values consumed and produced.

 # | References
---|---
[1]|D. Bernstein et al, Post-Quantum Cryptography, Hash-based Digital Signature Schemes, §7.2 Security of the Lamport–Diffie one-time signature scheme<br/>http://www.e-reading.club/bookreader.php/135832/Post_Quantum_Cryptography.pdf#page=90
[2]|D. Naor et al, One-Time Signatures Revisited: Have They Become Practical?, §4.2 Selecting Hash Functions for FMTseq<br/>https://eprint.iacr.org/2005/442.pdf#page=8
[3]|http://crypto.stackexchange.com/questions/3295/how-does-a-birthday-attack-on-a-hashing-algorithm-work
[4]|D. Bernstein et al, Post-Quantum Cryptography, Hash-based Digital Signature Schemes, §7.4 The security level of MSS: Using quantum computers<br/>http://www.e-reading.club/bookreader.php/135832/Post_Quantum_Cryptography.pdf#page=96
[5]|D. Bernstein, Cost analysis of hash collisions<br/>http://cr.yp.to/hash/collisioncost-20090823.pdf
[6]|D. Bernstein et al, Post-Quantum Cryptography, Hash-based Digital Signature Schemes, §1.2 Winternitz one-time signature scheme<br/>http://www.e-reading.club/bookreader.php/135832/Post_Quantum_Cryptography.pdf#page=45
[7]|R. Merkle, A Certified Digital Signature, §4 An Improved One Time Signature<br/>http://www.merkle.com/papers/Certified1979.pdf#page=18
[8]|R. Merkle, A Certified Digital Signature, §5 The Winternitz Improvement<br/>http://www.merkle.com/papers/Certified1979.pdf#page=20