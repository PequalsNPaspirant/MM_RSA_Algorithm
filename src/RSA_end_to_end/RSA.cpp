//Goal
//Start to end demo of RSA algorithm including large number arithmatic, modular exponentiation and lot more
//Take an example of real world public key from various web sites

#include "RSA.h"

	/*
	TODO:
	DONE : Code refactoring of existing RSAComplete class
	DONE: Test boundary conditions extensively
	DONE: Search all conditions on google:
	DONE: what is upper limit for d?
	Use 3 or more primes instead of just two p and q
	Implement different primality tests and test them
	Implment Big Integer
	Use actual public key of some bank from their website

	*/

	/*
	Factorization Facts:
		With some simple factorization algorithm, 340,282,366,920,938,463,463,374,607,431,768,211,456 can be factored in
			exactly 128 iterations of the innermost while. (The number, of course, is 2^128)
		The more prime factors a composite number has, the smaller those factors have to be. For example,
			919*677=622,163. With the naive algorithm, this takes 157+1=158 iterations to factor.
			A number of roughly the same size comprised of three factors, 73*89*97=630,209 only takes 25+2=27 iterations to factor.
		A 1000-digit number composed of    2 roughly equally-sized factors will take about 10^497 iterations to factor.
		A 1000-digit number composed of  100 roughly equally-sized factors will take about 434,294,481+99≈10^99 iterations.
		A 1000-digit number composed of 1000 roughly equally-sized factors will take about 4+999≈10^3 iterations.

	Encryption Basic:
		M = Message, C = Cypher Text / Ecrypted Message, K = Key

		Encryption	M + K = C
		Decryption	M     = C - K

		Encryption	M * K = C
		Decryption	M     = C / K

		Encryption	M XOR K = C
		Decryption	M       = C XOR K

		Diffie–Hellman key exchange:
		This works on one-way-function   f(x) = (g ^ x) mod p    ...where g and p are public values
		Inverse of this function can not be easily calculated.
		Example:
			Alice and Bob agree to use a modulus p = 23 and base g = 5.
			Alice chooses secret integer a = 6, then sends Bob g^a mod p
				g^a mod p = 5^6 mod 23 = 8
			Bob chooses secret integer b = 15, then sends Alice g^b mod p
				g^b mod p = 5^15 mod 23 = 19
			Alice computes s = (Bob's result)^a mod p
							 = 19^6 mod 23 = 2
							 = (  g^b mod p )^a mod p
							 = (g ^ ab) mod p
			Bob computes s = (Alice's result)^b mod p
						   = 8^15 mod 23 = 2
						   = (  g^a mod p   )^b mod p
						   = (g ^ ab) mod p
			Alice and Bob now know the same secret (the number 2), which is not known to anybody else.
			Both Alice and Bob have arrived at the same value s, because, under mod p,
		Drawback:
			The Sender, if communicates with n persons, has to use different unique keys for every person.
			This is because each person may use the different secret number. So sender has to keep track
			of different keys.

		Public Key Encryption:
			Works on trapdoor-one-way-function.
			Inverse of this function can be easily calculated only if you know trapdoor.

	Fundamental Theorem of Arithmetic:
		Every number > 1 can be expessed in terms of its prime factors.
		The prime factorization for every numbers is unique.
		OR
		By the fundamental theorem of arithmetic, every positive integer has a unique prime factorization.

	Modular Arithmatic:
		Following are equivalent (≡ means "congruent to"):
			M ≡ C (mod N)
			M mod N = C
			M = k * M + C    ...where k is any positive integer

		AB mod N = ((A mod N) * (B mod N)) mod N   ...(1)

		(A ^ e) mod N = ((A mod N) ^ e) mod N      ...(2)

		If   a1 mod N = b1 and
			 a2 mod N = b2
		Then (a1 + a2) mod N = b1 + b2 and
			 (a1 * a2) mod N = b1 * b2

	What is multiplicative inverse modulo OR Modular multiplicative inverse?
		A modular inverse of a value A (mod C) is a number , Ainv, such that (A * Ainv ) mod C = 1.
		We find the modular inverse by using the Extended Euclidean Algorithm.
		In de mod Ø(N) = 1, d is modular inverse of e and vice versa.

	What is Extended Euclidean algorithm
		See the algorithm to calculate private exponent d.

	What is Chinese remaindar theorem?
		Let n1,n2,...,nr be positive integers such that gcd(ni,nj)=1 for i ≠ j. Then the system of linear congruences
		x ≡ c1 (mod n1); x ≡ c2 (mod n2); ... ; x ≡ cr (mod nr)
		has a simultaneous solution which is unique modulo n1n2...nr.
		This is usually solved using Gauss's algorithm:
			Let N=n1n2...nr then
			x ≡ c1N1d1 + c2N2d2 + ... + crNrdr (mod N)
			where Ni = N/ni and di ≡ Ni-1 (mod ni).
		Example:
			There are certain things whose number is unknown. Repeatedly divided by 3, the remainder is 2; by 5 the remainder is 3; and by 7 the remainder is 2. What will be the number?
			n1=3, n2=5, n3=7
			N = n1n2n3 = 3 x 5 x 7 = 105
			c1=2, c2=3, c3=2.
			Now N1 = N/n1 = 35 and so d1 = 35-1 (mod 3) = 2,
			N2 = N/n2 = 21 and so d2 = 21-1 (mod 5) = 1, and
			N3 = N/n3 = 15 and so d3 = 15-1 (mod 7) = 1. Hence
			x = (2 x 35 x 2) + (3 x 21 x 1) + (2 x 15 x 1) = 233 ≡ 23 (mod 105)
		Another example
			x ≡ 1 (mod 3)
			x ≡ 2 (mod 4)
			x ≡ 3 (mod 5)
			Using Gauss's algorithm,
			n1=3, n2=4, n3=5
			N = n1n2n3 = 3 x 4 x 5 = 60
			c1=1, c2=2, c3=3.
			N1 = N/n1 = 20; d1 = 20-1 (mod 3) = 2 [check: 2x20=40≡1 (mod 3)]
			N2 = N/n2 = 15; d2 = 15-1 (mod 4) = 3 [check: 3x15=45≡1 (mod 4)]
			N3 = N/n3 = 12; d3 = 12-1 (mod 5) = 3 [check: 3x12=36≡1 (mod 5)]
			x ≡ c1N1d1 + c2N2d2 + c3N3d3 (mod N)
			x = (1x20x2) + (2x15x3) + (3x12x3) = 238 ≡ 58 (mod 60)
			so a solution is x = 58. Note that this is "a" solution. Any integer that satisfies 58 + 60k for any integer k is also a solution, but the method gives you the unique solution in the range 0 ≤ x < n1n2n3.

	Relation between XOR and mod:
		//TODO: correct below
		XOR is the sum of 2 bits, modulo 2. (2 is base of binary number system)

		For positive integral data types A and B
		A XOR B = (A + B) mod 2  // assumption: (A + B) does not overflow
		A &   B = (A * B) mod 2  // assumption: (A * B) does not overflow

		So above can be re-written as
		Encryption	(M + K) mod K = C
		Decryption	M             = (C + K) mod K

	Fermat's little theorem:
		(M ^ p) mod p = M    ...where p is prime number (and M is not divisible by p)
		if M is divisible by p, then M = pk ...where k is integer k >= 1
		Then (M ^ p) mod p = ((M mod p) ^ p) mod p
						   = ((pk mod p) ^ p) mod p
						   = ((   0    ) ^ p) mod p
						   = 0

	Euler's totient function Ø(N):
		Euler's totient function counts the positive integers up to a given integer N that are relatively prime to N.
			co-prime:- In number theory, two integers a and b are said to be relatively prime,
			mutually prime, or coprime (also spelled co-prime) if the only positive integer that evenly divides
			both of them is 1. That is, the only common positive factor of the two numbers is 1.
			In other words:- Two numbers are called relatively prime, or coprime, if their greatest
			common divisor equals 1.
			Two numbers X and Y are co-prime if X and Y do not have any common prime factor.
			Let’s say if X = a*a*b*c*c*c (a, b, c are prime factors of X) and Y=p*q*q*q*r (p, q, r are prime
			factors of Y) and a, b, c, p, q, r are distinct prime numbers.
		Ø(N) gives the number of co-prime numbers to N in range 1 to N-1
			e.g. Ø(9) = 6 (There are the six numbers 1, 2, 4, 5, 7 and 8 which are co-prime to 9)
		Ø(Prime Number) = Prime Number - 1
			(From the definistion of Prime Number. Prime Number is not divisible by any other number than 1)
		Ø(p * q) = Ø(p) * Ø(q) ...ONLY if p and q are relatively prime ...i.e. if gcd(p, q) = 1
			If p and q are prime numbers: Ø(p * q) = (p - 1) * (q - 1)
		//Use Euler's product formula to calculate Euler's totient function Ø(N) for any N
			if p is prime and k >= 1, then
			Ø(p ^ k) = p ^ k - p ^ (k-1)
			Ø(p ^ k) = p^(k-1) (p-1)
					 = p^k (1 - 1/p)

			The fundamental theorem of arithmetic states that if N > 1 there is a unique expression for N:
				N = p1 ^ k1 * p2 ^ k2 * ... * pi ^ ki
					where p1 < p2 < ... < pi are prime numbers and k1, k2, ..., ki are >= 1
			Using above, we can say:
			Ø(N) = Ø(p1 ^ k1 * p2 ^ k2 * ... * pi ^ ki)
				 = p1^k1 (1 - 1/p1) * p2^k2 (1 - 1/p2) * ...
				 = p1^k1 * p2^k2 * ... * pi^ki (1 - 1/p1) (1 - 1/p2) ... (1 - 1/pi)
			Ø(N) = N (1 - 1/p1) (1 - 1/p2) ... (1 - 1/pi)

			To make integer calculations easy, use Ø(p ^ k) = p^(k-1) (p-1)
			So Ø(N) = p1^(k1-1) (p1-1) * p2^(k2-1) (p2-1) * ...

	Euler's theorem:
		This is generalization of "Fermat's little theorem". Substitute N = p to get Fermat's format.
		If M and N are co-prime or relatively prime
		(M ^ Ø(N)) mod N = 1   OR   M ^ Ø(N) ≡ 1 (mod N)

	Carmichael's totient function:
		In modern RSA, Carmichael's totient function λ(N) = lcm(p − 1, q − 1) is used

	RSA Algorithm:
		Basics:
			RSA uses one way trapdoor function.
			Fermat's little theorem:
				(M ^ p) mod p = M    ...where p is prime number (and M is not divisible by p)
			Euler's theorem:
				(M ^ Ø(N)) mod N = 1   ..where M is co-prime with N

		Choose two large prime numbers: p, q (generally p & q are 2048 bits = 256 bytes) (2^11 = 2048)
			p and q are prime numbers                       ...condition 1  ...from Fermat's little theorem (see proof 1 below). It also helps to calculate Ø(N) easily. Ø(N) = (p - 1)(q - 1)
			p != q                                          ...condition 2
		Calculate composite number N = p * q (generally it is 4096 bits)
		Calculate
			Euler totient function Ø(N) = (p - 1)(q - 1)
			OR Carmichael's totient function λ(N) = lcm(p − 1, q − 1)
		Choose public exponent e such that:
			1 < e < Ø(N)                                 ...condition 3
			e is co-prime with Ø(N)                      ...condition 4
		Choose private exponent d such that:
			(e*d) mod Ø(N) = 1                           ...condition 5
			d = (k * Ø(N) + 1) / e    ...d should be integer i.e. (k * Ø(N) + 1) should devisible by e
		Keys:
			1. Public key: (e, N)
			2. Private key: (d, N)
		Encryption:
			M = Message, C = Encrypted Text / Cypher Text
			C = (M ^ e) mod N
		Decryption:
			M = (C ^ d) mod N
			i.e. M = (   ((M ^ e) mod N)   ^ d) mod N
		Other conditions:
			M should be co-prime with N i.e with p and q    ...condition 6   ...from Euler's theorem (see proof 2 below)
			M should not be multiple of p                   ...same as condition 6  ...from Fermat's little theorem (see proof 1 below)
			1 < M < N                                       ...condition 7  ...For M = 0 or 1, the decryption results are same

	RSA Conditions based on tests and Google results:
		Verified: p and q are prime numbers                       ...condition 1  ...from Fermat's little theorem (see proof 1 below). It also helps to calculate Ø(N) easily. Ø(N) = (p - 1)(q - 1)
		Verified: p != q                                          ...condition 2
		Changed : 1 < e < Ø(N)                                    ...condition 3
			e != 1 and e != Ø(N)
		Verified: e is co-prime with Ø(N)                         ...condition 4
		Changed : (e*d) mod Ø(N) = 1                              ...condition 5

		Changed : M should be co-prime with N i.e with p and q    ...condition 5   ...from Euler's theorem (see proof 2 below)
		Changed : M should not be multiple of p                   ...same as condition 5  ...from Fermat's little theorem (see proof 1 below)
			M can be anything between 1 to N i.e. 1 < M < N
		Verified: M < N                                           ...condition 6
		Verified: d = (k * Ø(N) + 1) / e

	Observations:
		M can be encrypted-decrypted back to M
			if M is co-prime with either p or q or both (M fails when is not co-prime with both p and q)
			So if p and q are co-prime, it works for all M < p*q because M can never be co-prime with both p and q (the first such value is M = p*q)
			But if p and q are not co-prime, then it works for all M except when M is co-prime with p and q both
		If M = 0 or 1 or n-1 there is no security as the ciphertext has the same value.

	Recommendations:
		e > log2(n)
		d > max(p, q)
		In practice, common choices for e are 3, 5, 17, 257 and 65537 (2^16+1). These particular values are chosen because they are primes and make the modular exponentiation operation faster, having only two bits of value 1.
		These five numbers are the first five Fermat numbers, referred to as F0 to F4, where Fx = 2^(2^x) = 1.
		Just be careful, these first five Fermat numbers are prime ("Fermat primes"), but the numbers F5 and above are not prime. For example, F5 = 4294967297 = 641 × 6700417.


	RSA Algorithm Proofs:

	Proof from Wikipedia:


	Proof 1: Using Fermat's little theorem
		If we use e as public exponent to encrypt message M and d as private exponent to decrypt M,
		Encryption:
			( M ^ e ) mod N
		Decryption:
			( (( M ^ e ) mod N) ^ d) mod N
		We should get message M back after decryption, so:
			( (( M ^ e ) mod N) ^ d) mod N = M
			( M ^ de ) mod N = M   ...From Modular Arithmatic (2) above  ...(a)

		Fermat's little theorem:
			(M ^ p) mod p = M    ...where p is prime number (and M is not divisible by p)
		So, if p is a prime number:
			(M ^ p) mod p = M
			(M ^ p-1) mod p = 1
			((M ^ p-1) mod p) ^ q-1 = 1 ^ q-1
			M ^ (p-1)(q-1) mod p = 1            ...(b)
		Similarly, if q is a prime number:
			(M ^ q) mod q = M
			(M ^ q-1) mod q = 1
			M ^ (p-1)(q-1) mod q = 1            ...(c)
		From (b) and (c)
			M ^ (p-1)(q-1) mod pq = 1
			M ^ k(p-1)(q-1) mod pq = 1
			M ^ (1 + k(p-1)(q-1)) mod pq = M     ...(d)

		From (a) and (d), we need following to make it work:
			N = pq and ed = (1 + k(p-1)(q-1))

		Coditions based on this proof:
		1. p and q should be prime numbers.
		2. M should not be divisible by p and q. So M should be co-prime with both p and q.
			In short, M shoudl be co-prime with N which is product of two primes.
		3. e and d should be co-prime with (p-1)(q-1) since e and d are integers in ed = (1 + (p-1)(q-1))

	Proof 2: Using Euler's theorem
		Encryption:
			( M ^ e ) mod N
		Decryption:
			( (( M ^ e ) mod N) ^ d) mod N
		We should get message M back after decryption, so:
			( (( M ^ e ) mod N) ^ d) mod N = M
			( M ^ de ) mod N = M   ...From Modular Arithmatic (2) above  ...(x)

		If N is prime,
			all numbers 1 < x < N are co-prime with N
		If N = pq where p & q are prime numbers,
			all numbers 1 < x < N are co-prime with N EXCEPT p and q
		Euler's theorem, if M is co-prime with N
			(M ^ Ø(N)) mod N = 1
			((M ^ Ø(N)) mod N) ^k = 1 ^k          ...raise both sides by k
			(M ^ kØ(N)) mod N = 1
			((M ^ kØ(N)) mod N) * M = 1 * M       ...multiply both sides by M
			(M ^ (kØ(N) + 1)) mod N = M           ...(y)
		Comparing (x) and (y)
			de = kØ(N) + 1
			de mod Ø(N) = 1     ...Requirement 1
		Also, d = (kØ(N) + 1) / e
		Since d and e both are integers, d and e both should be co-prime with Ø(N)

		Coditions based on this proof:
		2. M shoudl be co-prime with N.
		3. e and d should be co-prime with (p-1)(q-1) since e and d are integers in ed = (1 + (p-1)(q-1))

	Proof 3: Using Euler's theorem in another way
		We know: (d*e) mod Ø(N) = 1
			i.e. Let k be a positive integer, then d*e = k * Ø(N) + 1     ...(1)
		We need to prove:
			M = (   ((M ^ e) mod N)   ^ d) mod N
			M = ( M ^ de ) mod N
		i.e. M ^ de ≡ M (mod N)
		Consider RHS
			RHS = M ^ de
		Substitute value of d*e from (1)
			RHS = ( M ^ (k * Ø(N) + 1) )
			RHS = ( M^1 * M^(k * Ø(N)) )
			RHS = ( M * (M ^ Ø(N))^k )
		From Euler's theorem: if M and N are co-prime (M ^ Ø(N)) mod N = 1  i.e. M ^ Ø(N) # 1 (mod N)
		Since N = p*q and p, q are prime numbers, all other numbers than p, q are relatively prime with N,
		So M is also relatively prime to N (M != p and M != q)
			RHS ≡ ( M * (1)^k ) (mod N)
			RHS = M (mod N)
			RHS = LHS

	Examples:
		Example 1:
			1. P = 2, q = 7
			2. N = p * q = 14
			3. Ø(N) = (p - 1)*(q - 1) = 6
			4. e = 5
			5. d = 5, 11, 17 etc

		Example 2:
			1. P = 5, q = 11
			2. N = p * q = 55
			3. Ø(N) = (p - 1)*(q - 1) = 40
			4. e = 7
			5. d = 23

		Example 3:
			1. P = 89, q = 107
			2. N = p * q = 9523
			3. Ø(N) = (p - 1)*(q - 1) = 9328
			4. e = 3
			5. d = 6219

	Optimal Asymmetric Encryption Padding (OAEP):
		The most critical part of the RSA encryption is the padding scheme. A padding scheme is normally used to increase the length of
		the plain-text in such a way, that a symmetric algorithm can use it. But, even though the RSA can work without a padding scheme,
		it is critical for the security of the data, as without proper padding the cipher-text becomes vulnerable to many different attacks.
		The padding schemes also brings a randomness to the encrypted data. Every time the data is encrypted, the cipher-text is different,
		but after decryption the plain-text remains the same.

		There are two major padding schemes in general use, the PKCS and OAEP (Optimal Asymmetric Encryption Padding). PKCS is very simple
		and is still widely used being an older standard, but, is  vulnerable to some newer attacks. OAEP was designed by Bellare and Rogaway to
		prevent these attacks and is currently recommended for use. But, OAEP is a little complex to implement.

	OAEP Padding Scheme:

						+----------+---------+-------+
				   DB = |  lHash   |    PS   |   M   |
						+----------+---------+-------+
									   |
			 +----------+              V
			 |   seed   |--> MGF ---> xor
			 +----------+              |
				   |                   |
		  +--+     V                   |
		  |00|    xor <----- MGF <-----|
		  +--+     |                   |
			|      |                   |
			V      V                   V
		  +--+----------+----------------------------+
	EM =  |00|maskedSeed|          maskedDB          |
		  +--+----------+----------------------------+

	Keywords:
		DB - Data block to be encrypted, consists of pHash, PS and M.
		pHash - Hash of a predefined parameter list in the form of a byte array. It is used to make sure that the parameters at the encryption side and decryption side are the same, but, in most implementations its ignored and is optional. In that case, the Hash of an empty byte array is used instead.
		PS - A string of '0's followed by a 1. Used to fill the unused space in case, the message is shorter than the maximum allowed message length.
		M - Actual message to be encrypted.
		Seed - A random array of bytes, the length being equal to the length of hash function being used.
		MGF - Mask Generation Function, it is used to generate a variable length hash from a given input random input.
		XOR - Bit-wise Ex-OR operation.
		maskedSeed - The masked seed, which is part of the padded text. It is later (while decoding) used to get the Seed in conjunction with the MGF output of the maskedDB.
		maskedDB - The masked Data Block. It is later (while decoding) used to feed the MGF function which is used to obtain the Seed. It is also used to obtain the DB, by using the MGF output of the Seed.

		Encoding prelims:
		Before encoding, the Hash and the parameter array has to be fixed. For most purposes the parameter string is an empty byte array. Normally three different types of hashes are defined by the standard. SHA1, SHA256 and SHA512. SHA1 is default hashing algorithm. Its important that the parameter array and hash algorithm remains same during decoding, otherwise the decoding should fail.

		Consider:
		hLen = Length of hash function output in bytes.
		k = Length of Modulus in octets.
		PS_Len = k - MessageLength - 2 * hLen - 2

		OAEP Encoding:
		1) Calculate pHash = HASH(Parameter<code>_Array).
		2) Create an array PS filled with '0' of length PS_Len .
		3) Create DB = pHash || PS || 0x01 || M  , || means append operation.
		4) Generate a random octet string Seed of length hLen.
		5) Expand the Seed using the MGF (explained later) function using the Seed as the input randomness and the output length of (k - hLen - 1) to get dbMask.
		6) maskedDB = DB XOR dbMask
		7) seedMask = MGF(maskedDB, hLen)
		8) maskedSeed = Seed XOR seedMask.
		9) EncodedMessage = 0x00 || maskedSeed || maskedDB.

		That's all. The encoded message is simply encrypted using RSA.

		OAEP Decoding:
		The decoding process is just the opposite of the encoding. Because, the length of the maskedSeed is known, we can easily separate the maskedSeed and maskedDB.
		seedMask is generated from maskedDB using MGF, the generated seedMask is XORed with the maskedSeed to get the Seed.
		The Seed is expanded using MGF to get dbMask.
		dbMask and maskedDB are XORed together to get the DB.
		We can easily obtain pHash and M from DB, because the length of pHash is known and the M starts after a sequence of '0's followed by a 1.
		If pHash matches with the Hash of the parameter array during decryption the M is returned as the result. Otherwise the process fails.
		It is important that the decoding algorithm does not allow the user to know the exact nature of the error, otherwise some attacks can be mounted.
		The whole point of the OAEP scheme is to make sure that even 1 bit error in decryption renders the complete packet worthless. This is made sure by the hashes, as even a single bit change causes a complete change in the output bits.

		MGF Function:
		Inputs:
		1) Z = Initial pseudo-random seed.
		2) L = Required length of output.

		Algorithm:

		for(int loop = 0 ; loop <= Loops ; loop++  )
		{
			X = Z || (loop in octets array of size 4, BigEndian)
			MgfOut += HASH (X);
		}
		Return first L octets from MgfOut

	*/

namespace mm {

	//Constructors
	RSA::RSA(unsigned short securityBitLength /*= 1024*/)
		:m_securityBitLength(securityBitLength)
	{
		generatePrimeNumbers();
		generateTotientFunction();
		generatePublicExponent();
		generatePrivateExponent(BigInteger::bigIntTwo *N);
	}

	RSA::RSA(BigInteger p_, BigInteger q_)
		:p(p_), q(q_),
		N(p * q)
	{
		generateTotientFunction();
		generatePublicExponent();
		generatePrivateExponent(BigInteger::bigIntTwo * N);
	}

	RSA::RSA(BigInteger p_, BigInteger q_, BigInteger e_)
		: p(p_), q(q_), e(e_),
		N(p * q)
	{
		generateTotientFunction();
		generatePrivateExponent(BigInteger::bigIntTwo * N);
	}

	RSA::RSA(BigInteger p_, BigInteger q_, BigInteger e_, BigInteger d_, BigInteger TF_)
		: p(p_), q(q_), e(e_), d(d_), N(p * q), TF(TF_)
	{
	}

	//Public static Functions
	bool RSA::generateTotientFunction()
	{
		if (ArithmeticOperations<BigInteger>::calculteGCD(p, q) != BigInteger::bigIntOne)
			return false;
		else //If p and q are relatively prime or co-prime
		{
			// Calculate Carmichael's totient function λ(N) = lcm(p − 1, q − 1)
			TF = ArithmeticOperations<BigInteger>::calculteLCM(p - BigInteger::bigIntOne, q - BigInteger::bigIntOne);

			// Calculate Euler totient function Ø(N) = (p - 1)(q - 1)
			//TF = (p - 1) * (q - 1);
		}
		return true;
	}
	bool RSA::generatePublicExponent(const BigInteger& startSearchFrom /*= 2*/)
	{
		bool retVal = false;
		Timer t;
		for (e = startSearchFrom; e < TF; e++) //...what should be limit for e....ideally there is no limit for e....and e can be 1
			if (ArithmeticOperations<BigInteger>::calculteGCD(e, TF) == BigInteger::bigIntOne)
			{
				retVal = true;
				break;
			}
		m_publicExponentCalculationDuration = t.getDurationStringTillNowInNanoSeconds();
		return retVal;
	}
	bool RSA::generatePrivateExponent(BigInteger endSearchAt /*= std::numeric_limits<BigInteger>::max()*/)
	{
		//Make sure public exponent is generated before this
		bool retVal = false;
		Timer t;
		const int ExtendedEuclideanAlgorithm = 1;
		switch (ExtendedEuclideanAlgorithm)
		{
		case 1:
		{
			BigInteger u1, u3, v1, v3, t1, t3, q;
			int iter;
			// Step X1. Initialise 
			u1 = 1;
			u3 = e;
			v1 = 0;
			v3 = TF;
			// Remember odd/even iterations 
			iter = 1;
			// Step X2. Loop while v3 != 0
			while (v3 != BigInteger::bigIntZero)
			{
				// Step X3. Divide and "Subtract"
				q = u3 / v3;
				t3 = u3 % v3;
				t1 = u1 + q * v1;
				// Swap 
				u1 = v1; v1 = t1; u3 = v3; v3 = t3;
				iter = -iter;
			}
			// Make sure u3 = gcd(u,v) == 1 
			if (u3 == BigInteger::bigIntOne) // Error: No inverse exists if u3 != 1
			{
				// Ensure a positive result
				if (iter < 0)
					d = TF - u1;
				else
					d = u1;
				retVal = true;
			}
		}
		break;
		case 2:
		{
			/*
			a *x + b*y = gcd(a, b)
			TF*x + e*d = 1
			e*d = 1 + (-x)TF
			e*d = 1 (mod TF)
			*/
			d = 0;
			BigInteger newd = BigInteger::bigIntOne;
			BigInteger a = TF;
			BigInteger b = e;
			int dsign = 1, newdsign = 1;
			while (b != BigInteger::bigIntZero)
			{
				BigInteger quotient = a / b;
				BigInteger tempb = b;
				b = a % b;
				a = tempb;
				//Avoid -ve values as data type is unsigned
				BigInteger tempnewd = newd;
				int tempnewdsign = newdsign;
				if (dsign != newdsign)
				{
					newd = d + quotient * newd;
					newdsign = dsign;
				}
				else
				{
					if (d < (quotient * newd))
						newd = quotient * newd - d;
					else
						newd = d - quotient * newd;
					newdsign = -newdsign;
				}
				d = tempnewd;
				dsign = tempnewdsign;
			}

			if (a == BigInteger::bigIntOne)
			{
				if (dsign == -1)
					d = TF - d;
				retVal = true;
			}

			break;
		}
		case 3:
			/*
			d*e ≡ 1 (mod TF)
			d*e mod TF = 1
			d*e = k*TF + 1
			d = (k*TF + 1) / e  ...provided d is integer
			*/
			for (BigInteger k = BigInteger::bigIntOne; k < endSearchAt; k++) //....what should be limit for k...ideally there is no limit for d..so no limit for k
				if ((k * TF + BigInteger::bigIntOne) % e == BigInteger::bigIntZero)
				{
					d = (k * TF + BigInteger::bigIntOne) / e;
					retVal = true;
					break;
				}
			break;
		case 4:
			for (d = e; d < endSearchAt; d++)
				if ((d * e) % TF == BigInteger::bigIntOne)
				{
					retVal = true;
					break;
				}
			break;
		default:
			assert(false, "Unknown PrivateExponentCalculationMethod");
		}
		m_privateExponentCalculationDuration = t.getDurationStringTillNowInNanoSeconds();
		return retVal;
	}
	const BigInteger& RSA::getTotientFunction()
	{
		return TF;
	}

	const BigInteger& RSA::getPublicExponent()
	{
		return e;
	}
	const BigInteger& RSA::getPrivateExponent()
	{
		return d;
	}
	//Public Non-static Functions

	void RSA::testRSAComplete(const string& message)
	{
		testRSAComplete(reinterpret_cast<const unsigned char*>(message.c_str()));
	}

	void RSA::testRSAComplete(const ustring& message)
	{
		m_originalMessage = message;
		m_originalValues.clear();
		for (int i = 0; i < m_originalMessage.length(); i++)
			m_originalValues.push_back(BigInteger(BigInteger::DigitType(m_originalMessage[i])));
		testRSAComplete();
	}

	void RSA::testRSAComplete(const vector<BigInteger>& messageValues)
	{
		m_originalValues = messageValues;
		m_originalMessage.clear();
		for (int i = 0; i < m_originalValues.size(); i++)
			m_originalMessage += char(m_originalValues[i].covertToLongLong());
		testRSAComplete();
	}

	void RSA::testRSAComplete()
	{
		m_encryptedMessage.clear();
		m_decryptedMessage.clear();
		m_encryptedValues.clear();
		m_decryptedValues.clear();
		encrypt();
		decrypt();
		printAllInformation();
	}

	void RSA::encrypt()
	{
		Timer t;
		for (int i = 0; i < m_originalValues.size(); i++)
		{
			m_originalMessage += char(m_originalValues[i].covertToLongLong());
			m_encryptedValues.push_back(m_originalValues[i].modularExponentiation(e, N));
			m_encryptedMessage += char(m_encryptedValues[i].covertToLongLong());
		}
		m_encryptionDuration = t.getDurationStringTillNowInNanoSeconds();
	}

	void RSA::decrypt()
	{
		Timer t;
		for (int i = 0; i < m_encryptedValues.size(); i++)
		{
			BigInteger decryptVal = m_encryptedValues[i].modularExponentiation(d, N);
			m_decryptedValues.push_back(decryptVal);
			m_decryptedMessage += char(decryptVal.covertToLongLong());
		}
		m_decryptionDuration = t.getDurationStringTillNowInNanoSeconds();
	}

	//Utility Functions
	bool RSA::generatePrimeNumbers()
	{
		Timer t;
		p = BigInteger::getPrimeNumber(m_securityBitLength * 0.9, Fermat_efficient);
		m_primeNumber1GenerationDuration = t.getDurationStringTillNowInNanoSeconds();
		t.resetTimer();
		q = BigInteger::getPrimeNumber(m_securityBitLength, Fermat_efficient);
		m_primeNumber2GenerationDuration = t.getDurationStringTillNowInNanoSeconds();

		N = p * q;

		return true;
	}

	void RSA::printAllInformation()
	{
		cout.imbue(std::locale(""));
		cout << "\n\np = " << p
			<< " q = " << q
			<< " e = " << e
			<< " d = " << d
			<< " N = " << N
			<< " TF = " << TF;
		cout << setw(50) << "\nFirst Prime Number generation duration : " << m_primeNumber1GenerationDuration;
		cout << setw(50) << "\nSecond Prime Number generation duration: " << m_primeNumber2GenerationDuration;
		cout << setw(50) << "\nPublic Exponent calculation duration   : " << m_publicExponentCalculationDuration;
		cout << setw(50) << "\nPrivate Exponent calculation duration  : " << m_privateExponentCalculationDuration;
		cout << setw(50) << "\nEncryption duration                    : " << m_encryptionDuration;
		cout << setw(50) << "\nDecryption duration                    : " << m_decryptionDuration;
		if (m_originalValues == m_decryptedValues)
			cout << "\n-------------------- Test Result: SUCCESS! Original and Decrypted messages are same! --------------------";
		else
		{
			cout << "\nOriginal Values : ";
			for (int i = 0; i < m_originalValues.size(); i++)
				cout << m_originalValues[i] << ", ";
			cout << "\nEncrypted Values: ";
			for (int i = 0; i < m_encryptedValues.size(); i++)
				cout << m_encryptedValues[i] << ", ";
			cout << "\nDecrypted Values: ";
			for (int i = 0; i < m_decryptedValues.size(); i++)
				cout << m_decryptedValues[i] << ", ";
			cout << "\nOriginal message : " << m_originalMessage.c_str();
			cout << "\nEncrypted message: " << m_encryptedMessage.c_str();
			cout << "\nDecrypted message: " << m_decryptedMessage.c_str();
			cout << "\n-------------------- Test Result: ERROR! Original and Decrypted messages are different! --------------------";
		}
	}

}