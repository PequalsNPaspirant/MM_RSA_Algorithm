#ifndef __BIG_INTEGER_HEADER_GUARD__
#define __BIG_INTEGER_HEADER_GUARD__

#include <vector>
#include <limits>
//There are, some templates defined in the header <utility>; if this header is included, 
//then it suffices to just overload operator== and operator<, and the other operators will be provided by the STL.
#include <utility>
#include <cassert>
using namespace std;

//#include "Maths/Maths_ArithmeticOperations.h"  //GCD, LCM, power/exponent, doModularExponentiation etc
//#include "Logger/Logger.h"

namespace mm {

	/*
	64 bit OS has 64 bit registers. But where the arithmetic oprtations using 64 bit data type
	will be faster on it, is a complex topic. It all depends upon processor architecture and on CPU and ALU etc.
	It all boils down to:
	32 bit arithmetic is at least not slower than 64 bit on most of the processors. Also considering overflow in arithmetic operations,
	32 bit arithmetic can be considered most efficient for BigIntegers.

	*/

	enum PrimalityTest
	{
		Fermat_efficient = 0
		, Fermat_smallPrimes
		, Fermat_bigPrimes
		, Fermat_randomNumbers
		, Fermat_extensive

		, MillerRabin_basic
		, MillerRabin_usingOptimizedBases
	};

	class BigInteger
	{
	public:
		// Different settings for different bases
		// Ranges:
		// unsigned int       : 0 to              4,294,967,295  i.e.  0 to 2^32 - 1 
		// unsigned long long : 0 to 18,446,744,073,709,551,615  i.e.  0 to 2^64 - 1

		// Trial 1:

		// Takes 11 sec to create BigInteger from a 50000 digit decimal number
		typedef unsigned int DigitType;
		typedef unsigned long long ResultType;
		static const unsigned int bitsInNumberSystemBase = 32;
		static const ResultType numberSystemBase = (1ULL << bitsInNumberSystemBase); // OR = ResultType(numeric_limits<DigitType>::max()) + 1; // 4,294,967,295 + 1 = 4,294,967,296
		static const ResultType maskToCalculateRemainder = numberSystemBase - 1;
		static const ResultType ULLMAX = numeric_limits<ResultType>::max();
		// Max value of any digit is 2^32 - 1    ..Thus this can be fit into unsigned int
		// Max product of two digits = (2^32 - 1) * (2^32 - 1) = (2^32 - 1)^2 = 2^64 - 2 * 2^32 * 1 - 1     ...Thus this can be fit into unsigned long long
		//2^64 = 18,446,744,073,709,551,616

		// Trial 2:
		/*
		// Takes 16 sec to create BigInteger from a 50000 digit decimal number
		typedef unsigned long long DigitType;
		typedef unsigned long long ResultType;
		static const unsigned int bitsInNumberSystemBase = 32;
		static const ResultType numberSystemBase = 1ULL << bitsInNumberSystemBase;
		static const ResultType ULLMAX = numeric_limits<ResultType>::max();
		// Max value of any digit is 2^32 - 1    ..Thus this can be EASILY fit into unsigned long long
		// Max product of two digits = (2^32 - 1) * (2^32 - 1) = (2^32 - 1)^2 = 2^64 - 2 * 2^32 * 1 - 1     ...Thus this can be fit into unsigned long long
		*/

		// Trial 3:
		/*
		// Takes 69 sec to create BigInteger from a 50000 digit decimal number
		typedef unsigned int DigitType;
		typedef unsigned int ResultType;
		static const unsigned int bitsInNumberSystemBase = 16;
		static const ResultType numberSystemBase = 1 << bitsInNumberSystemBase;
		static const ResultType ULLMAX = numeric_limits<ResultType>::max();
		// Max value of any digit is 2^16 - 1     ..Thus this can be fit into unsigned int
		// Max product of two digits = (2^16 - 1) * (2^16 - 1) = (2^16 - 1)^2 = 2^32 - 2 * 2^16 * 1 - 1     ...Thus this can be fit into unsigned int
		*/



	public:
		BigInteger();
		BigInteger(const int& number);
		BigInteger(const long& number);
		BigInteger(const long long& number);

		//BigInteger(const DigitType& number);
		BigInteger(const unsigned int& number);
		BigInteger(const unsigned long& number);
		//BigInteger(const ResultType& number);
		BigInteger(const unsigned long long& number);

		BigInteger(const double& number);
		BigInteger(const long double& number);
		BigInteger(const string& numberString, DigitType userBase = 10);
		BigInteger(const vector<DigitType>& digits);
		~BigInteger();

	private:
		bool m_isPositive;
		vector<DigitType> m_digits;

	public:
		static ostream& getLogger();

	public:
		//Other useful public intefaces
		//BigInteger(const BigInteger& copyFrom);
		BigInteger(const BigInteger& copyFrom, bool absolute = false);
		BigInteger& operator=(const BigInteger& assignFrom);
		BigInteger& operator=(const DigitType& singleDigit);

		//Arithmetic Operations
		void operator+=(const BigInteger& rhs);
		friend BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs);

		void operator-=(const BigInteger& rhs);
		friend BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs);
		//Unary '-' operator
		const BigInteger& operator-();

		void operator*=(const BigInteger& rhs);
		friend BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs);

		void operator/=(const BigInteger& rhs);
		friend BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs);

		void operator%=(const BigInteger& rhs);
		friend BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs);

		void operator%=(const DigitType& rhs);
		friend DigitType operator%(const BigInteger& lhs, const DigitType& rhs);

		void divideAndRemainder(const BigInteger& divisor, BigInteger& quotient, BigInteger& remainder) const;
		//void divideAndRemainder_1(const BigInteger& divisor, BigInteger& quotient, BigInteger& remainder) const;
		void divideAndRemainder(const DigitType& divisor, BigInteger& quotient, DigitType& remainder) const;

		friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);
		friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs);
		friend bool operator<(const BigInteger& lhs, const BigInteger& rhs);
		friend bool operator>(const BigInteger& lhs, const BigInteger& rhs);
		BigInteger operator++();
		BigInteger& operator++(int);

		BigInteger absolute() const;
		bool isNegative();
		//const BigInteger negate();
		//const BigInteger toggleSign();

		BigInteger power2(const BigInteger& exponent) const;
		BigInteger power(const BigInteger& exponent) const;
		BigInteger modularExponentiation(const BigInteger& exponent, const BigInteger& modulus);

		long long covertToLongLong() const;
		long double covertToLongDouble();
		long long hashCode();

		operator long long()
		{
			return covertToLongLong();
		}
		operator long double()
		{
			return covertToLongDouble();
		}

		static BigInteger getPrimeNumber(size_t bits, const PrimalityTest& primalityTestMethod);
		static BigInteger getNextPrimeNumber(const BigInteger& number, const PrimalityTest& primalityTestMethod);
		static BigInteger getRandomNumber(size_t bits);
		static BigInteger getRandomNumber2(size_t bits);
		static BigInteger getRandomNumber3(size_t bits);

		bool testDivisibilityByPredefinedPrimes(size_t count = 100) const;

		bool isPrime(const PrimalityTest& primalityTestMethod) const;

		bool MillerRabinPrimalityTest_basic(int iterations) const;
		bool MillerRabinPrimalityTest_usingOptimizedBases(size_t iterations) const;

		bool ManindraAgrawalNeerajKayalNitinSaxenaIITKanpur6August2002PrimesIsInP(int iterations) const;
		bool cyclotomicPrimalityTest(int iterations) const;
		bool ellipticCurvePrimalityTest(int iterations) const;

		bool FermatPrimalityTest_efficient(int iterations) const;
		bool FermatPrimalityTest_withRandomNumbersAsBase(int iterations) const;
		bool FermatPrimalityTest_withSmallPrimeNumbersAsBase(int iterations) const;
		bool FermatPrimalityTest_withBiggerPrimeNumbersAsBase(int iterations) const;
		bool FermatPrimalityTest_extensive(int iterations) const;

		bool LucasLehmerPrimalityTest(int iterations) const;
		bool BailliePSWPrimalityTest(int iterations) const;
		bool SolovayStrassenPrimalityTest(int iterations) const;

		//Print utilities	
		friend ostream& operator << (ostream &out, const BigInteger& bigInt);
		string toString(DigitType userBase = 10) const;
		string toString1(DigitType userBase = 10) const;
		string toString2(DigitType userBase = 10) const;
		string toString3(DigitType userBase = 10) const;
		string toString4(DigitType userBase = 10) const;
		string toString5(DigitType userBase = 10) const;

		//Some useful constants
		static const BigInteger bigIntZero;
		static const BigInteger bigIntOne;
		static const BigInteger bigIntTwo;
		static const BigInteger bigIntBase;
		static const BigInteger bigIntInfinity;

		static const vector<DigitType>& getFirst100Primes();
		static const vector<DigitType>& getFirst1000Primes();
		static const vector<DigitType>& getFirst10000Primes();

		static const vector<size_t> inputDigitsPerBigIntegerDigit;

	private:
		//Other private helper functions
		void initialize(vector<DigitType>& digits, unsigned long long number);
		DigitType convertCharToDecimalNumber(const char& digitIn) const;
		DigitType convertStringToDecimalNumber(const string& numberString, size_t start, size_t end, size_t baseFrom);

		void doMultiplyBySingleDigit(DigitType rhsValue);
		void doAddSingleDigit(DigitType rhsValue);

		static void doMultiply(vector<DigitType>& lhsCumResultVector, const vector<DigitType>& rhsVector);
		static void doAdd(vector<DigitType>& lhsCumResultVector, const vector<DigitType>& rhsVector);
		static void doSubstract(vector<DigitType>& lhsCumResultVector, const vector<DigitType>& rhsVector);
		void addDigitsToRight(const BigInteger& reference, size_t start, size_t end);
		void doDivide(const DigitType& divisor, DigitType& remainder);

		// Following functions are used to convert BigInteger into string representation of decimal digits (base 10)
		static void doMultiply(vector<DigitType>& lhsCumResultVector, const vector<DigitType>& rhsVector, const ResultType& baseIn);
		static void doAdd(vector<DigitType>& lhsCumResultVector, const vector<DigitType>& rhsVector, const ResultType& baseIn);
		
		static void doMultiply(vector<DigitType>& lhsCumResultVector, const ResultType& rhs, const ResultType& baseIn);
		static void doAdd(vector<DigitType>& lhsCumResultVector, const ResultType& rhs, const ResultType& baseIn);
		static void doMultiplyAndAdd(vector<ResultType>& lhsCumResultVector, const ResultType& numToMultiply, const DigitType& numToAdd, const ResultType& baseIn);

		static void convertDecimalToBase(vector<DigitType>& digitVector, size_t targetBase, ResultType decimalNumber);
		char getChar(const DigitType& digitIn) const;

		void resize(const size_t& newSize);
		void removeLeadingZeros();
		static void removeLeadingZeros(vector<DigitType>& vecIn);
		void correctIfNegativeZero();
		friend int compareMagnitudes(const BigInteger& lhs, const BigInteger& rhs);

		static const vector<DigitType> getHardcodedOptimizedPrimalityTetsBases(const BigInteger& limit);
		static size_t getInputDigitsPerBigIntegerDigit(const size_t& baseIn);
		size_t getIndexOfFirstNonZeroDigit()
		{
			size_t index = 0;
			while (index < m_digits.size() && m_digits[index] == 0)
				++index;

			return index;
		}
	};

}

#endif //__BIG_INTEGER_HEADER_GUARD__