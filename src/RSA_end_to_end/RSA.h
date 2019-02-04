//Goal
//Start to end demo of RSA algorithm including large number arithmatic, modular exponentiation and lot more
//Take an example of real world public key from various web sites
//Need to develop Big integer before this to store large number and do arithmatic on it

#include <iostream>
#include <iomanip>  //for setw
#include <locale> // for std::local
#include <sstream> // for std::stringstream
#include <ctime>
#include <string>
#include <vector>
using namespace std;

#include "Assert/MyAssert.h"
#include "BigInteger/BigInteger.h"
#include "Timer/Timer.h"
#include "my_math_lib/PrimeNumber.h"
#include "my_math_lib/ArithmeticOperations.h"

namespace mm {

	//Let us create our own string of unsigned char (std::string is made up of signed char)
	typedef std::basic_string <unsigned char> ustring;
	//=============================================================================================================

	class RSA
	{
	public:
		RSA(unsigned short securityBitLength = 1024);
		RSA(BigInteger p_, BigInteger q_);
		RSA(BigInteger p_, BigInteger q_, BigInteger e_);
		RSA(BigInteger p_, BigInteger q_, BigInteger e_, BigInteger d_, BigInteger TF_);
		bool generateTotientFunction();
		bool generatePublicExponent(const BigInteger& startSearchFrom = 2LL);
		bool generatePrivateExponent(BigInteger endSearchAt = std::numeric_limits<BigInteger>::max());
		const BigInteger& getTotientFunction();
		const BigInteger& getPublicExponent();
		const BigInteger& getPrivateExponent();
		void testRSAComplete(const string& message);
		void testRSAComplete(const ustring& message);
		void testRSAComplete(const vector<BigInteger>& messageValues);
		void testRSAComplete();
		void encrypt();
		void decrypt();

	private:
		unsigned int m_securityBitLength;
		BigInteger p; // first prime number
		BigInteger q; // second prime number
		BigInteger e; // public exponent
		BigInteger d; // private exponent
		BigInteger N; // modulus N = p * q
		//BigInteger ETF; // Euler totient function Ø(N) = (p - 1)(q - 1)
		//BigInteger CTF; // Carmichael's totient function λ(N) = lcm(p − 1, q − 1)
		BigInteger TF; // Actual totient function to be used
		vector<BigInteger> m_originalValues;
		vector<BigInteger> m_encryptedValues;
		vector<BigInteger> m_decryptedValues;

		ustring m_originalMessage;
		ustring m_encryptedMessage;
		ustring m_decryptedMessage;
		// Book-keeping for printing
		string m_primeNumber1GenerationDuration;
		string m_primeNumber2GenerationDuration;
		string m_privateExponentCalculationDuration;
		string m_publicExponentCalculationDuration;
		string m_encryptionDuration;
		string m_decryptionDuration;

		//Utility Functions
		bool generatePrimeNumbers();
		void printAllInformation();
	};

	class RSAUnitTest
	{
	public:
		//Unit Testing
		static void TestBasics();
		static void RSATest_usePrecalculatedPrimes();
		static const string& getTestMessage();
		static const ustring& getTestMessageHavingAllChars();
		static const ustring& getTestMessageHavingAllChars2();
		static const ustring& getTestMessageHavingAllWideChars();
	};

}