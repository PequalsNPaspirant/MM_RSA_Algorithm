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

	struct RSAMethodInitializer
	{
		//TotientFunctionTypes
		enum eTotientFunctionType
		{
			CarmichaelsTotientFunction = 0,
			EulersTotientFunction = 1,

			MaxTotientFunctionType
		};

		//PrivateExponentCalculationMethod
		enum ePrivateExponentCalculationMethod
		{
			ExtendedEuclideanAlgorithm = 0,
			ExtendedEuclideanAlgorithm2 = 1,
			IterateOverMultiplier_k = 2,    //d = (k * TF + 1) / e;
			IterateOverPrivateExponent_d = 3,  //(d * e) % TF == 1

			MaxPrivateExponentCalculationMethod
		};

		RSAMethodInitializer(eTotientFunctionType TotientFunctionTypeIn = eTotientFunctionType(0),
			ePrivateExponentCalculationMethod PrivateExponentCalculationMethodInt = ePrivateExponentCalculationMethod(0))
		{
			TotientFunctionType = TotientFunctionTypeIn;
			PrivateExponentCalculationMethod = PrivateExponentCalculationMethodInt;
		}

		eTotientFunctionType TotientFunctionType;
		ePrivateExponentCalculationMethod PrivateExponentCalculationMethod;
	};


	//Following class contains all trials to implement RSA
	template <typename Type>
	class RSA_Template
	{
	public:
		RSA_Template(unsigned short securityBitLength);
		RSA_Template(Type p_, Type q_);
		RSA_Template(Type p_, Type q_, Type TF_);
		RSA_Template(Type p_, Type q_, Type e_, Type d_);
		RSA_Template(Type p_, Type q_, Type e_, Type d_, Type TF_);

		bool initialize(const RSAMethodInitializer& initializer = RSAMethodInitializer());
		bool generateTotientFunction();
		bool generatePublicExponent(const Type& startSearchFrom = 2);
		bool generatePrivateExponent(Type endSearchAt = std::numeric_limits<Type>::max());

		const Type& getTotientFunction();
		const Type& getPublicExponent();
		const Type& getPrivateExponent();

		void testRSAComplete(const string& message);
		void testRSAComplete(const ustring& message);
		void testRSAComplete(const vector<Type>& messageValues);
		void testRSAComplete();
		void encrypt();
		void decrypt();

	private:
		unsigned int m_securityBitLength;
		Type p; // first prime number
		Type q; // second prime number
		Type e; // public exponent
		Type d; // private exponent
		Type N; // modulus N = p * q
		//Type ETF; // Euler totient function Ø(N) = (p - 1)(q - 1)
		//Type CTF; // Carmichael's totient function λ(N) = lcm(p − 1, q − 1)
		Type TF; // Actual totient function to be used

		vector<Type> m_originalValues;
		vector<Type> m_encryptedValues;
		vector<Type> m_decryptedValues;

		ustring m_originalMessage;
		ustring m_encryptedMessage;
		ustring m_decryptedMessage;

		bool m_init;
		RSAMethodInitializer m_RSAMethodInitializer;

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

	//Constructors

	template <typename Type>
	RSA_Template<Type>::RSA_Template(unsigned short securityBitLength)
		:m_securityBitLength(securityBitLength),
		p(0), q(0), e(0), d(0), N(p * q), TF(0),
		m_init(false)
	{
		size_t size = sizeof(Type);
		MyAssert::myRunTimeAssert(size * 8 > (2 * securityBitLength));
	}

	template <typename Type>
	RSA_Template<Type>::RSA_Template(Type p_, Type q_)
		:p(p_), q(q_), e(0), d(0), N(p * q), TF(0)
	{
	}

	template <typename Type>
	RSA_Template<Type>::RSA_Template(Type p_, Type q_, Type TF_)
		: p(p_), q(q_), e(0), d(0), N(p * q), TF(TF_)
	{
	}

	template <typename Type>
	RSA_Template<Type>::RSA_Template(Type p_, Type q_, Type e_, Type d_)
		: p(p_), q(q_), e(e_), d(d_), N(p * q), TF(0)
	{
	}

	template <typename Type>
	RSA_Template<Type>::RSA_Template(Type p_, Type q_, Type e_, Type d_, Type TF_)
		: p(p_), q(q_), e(e_), d(d_), N(p * q), TF(TF_)
	{
	}

	template <typename Type>
	bool RSA_Template<Type>::initialize(const RSAMethodInitializer& initializer /*= RSAMethodInitializer()*/)
	{
		if (m_init) return m_init;

		m_RSAMethodInitializer = initializer;
		while (!m_init)
		{

#ifdef _DEBUG
			//cout << "\nNext iteration:\n";
			//cout << "\nFor Debugging: p = " << p
			//	<< " q = " << q
			//	<< " e = " << e
			//	<< " d = " << d
			//	<< " N = " << N
			//	<< " F = " << F;
#endif

			if ((p != 0 && q != 0) || (generatePrimeNumbers() && (N = p * q)))
			{
				if (
					(TF != 0 || generateTotientFunction())
					&& (e != 0 || generatePublicExponent())
					&& (d != 0 || generatePrivateExponent(2 * N))
					)
				{
					m_init = true;
					break;
				}
			}
		}

		return m_init;
	}

	//Public static Functions

	template <typename Type>
	bool RSA_Template<Type>::generateTotientFunction()
	{
		if (ArithmeticOperations<Type>::calculteGCD(p, q) != 1)
			return false;
		else //If p and q are relatively prime or co-prime
		{
			switch (m_RSAMethodInitializer.TotientFunctionType)
			{
			case RSAMethodInitializer::eTotientFunctionType::CarmichaelsTotientFunction:
				// Calculate Carmichael's totient function λ(N) = lcm(p − 1, q − 1)
				TF = ArithmeticOperations<Type>::calculteLCM(p - 1, q - 1);
				break;

			case RSAMethodInitializer::eTotientFunctionType::EulersTotientFunction:
				// Calculate Euler totient function Ø(N) = (p - 1)(q - 1)
				TF = (p - 1) * (q - 1);
				break;

			default:
				MyAssert::myRunTimeAssert(false, "Unknown TotientFunctionType");
			}
		}

		return true;
	}

	template <typename Type>
	bool RSA_Template<Type>::generatePublicExponent(const Type& startSearchFrom /*= 2*/)
	{
		bool retVal = false;
		Timer t;
		for (e = startSearchFrom; e < TF; e++) //...what should be limit for e....ideally there is no limit for e....and e can be 1
			if (ArithmeticOperations<Type>::calculteGCD(e, TF) == 1)
			{
				retVal = true;
				break;
			}
		m_publicExponentCalculationDuration = t.getDurationStringTillNowInNanoSeconds();
		return retVal;
	}

	template <typename Type>
	bool RSA_Template<Type>::generatePrivateExponent(Type endSearchAt /*= std::numeric_limits<Type>::max()*/)
	{
		//Make sure public exponent is generated before this
		bool retVal = false;
		Timer t;
		switch (m_RSAMethodInitializer.PrivateExponentCalculationMethod)
		{
		case RSAMethodInitializer::ExtendedEuclideanAlgorithm:
		{
			Type u1, u3, v1, v3, t1, t3, q;
			int iter;
			// Step X1. Initialise 
			u1 = 1;
			u3 = e;
			v1 = 0;
			v3 = TF;
			// Remember odd/even iterations 
			iter = 1;
			// Step X2. Loop while v3 != 0
			while (v3 != 0)
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
			if (u3 == 1) // Error: No inverse exists if u3 != 1
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

		case RSAMethodInitializer::ExtendedEuclideanAlgorithm2:
		{
			/*
			a *x + b*y = gcd(a, b)
			TF*x + e*d = 1
				   e*d = 1 + (-x)TF
				   e*d = 1 (mod TF)
			*/

			d = 0;
			Type newd = 1;
			Type a = TF;
			Type b = e;

			int dsign = 1, newdsign = 1;
			while (b != 0)
			{
				Type quotient = a / b;

				Type tempb = b;
				b = a % b;
				a = tempb;

				//Avoid -ve values as data type is unsigned
				Type tempnewd = newd;
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

			if (a == 1)
			{
				if (dsign == -1)
					d = TF - d;

				retVal = true;
			}

			break;
		}
		case RSAMethodInitializer::IterateOverMultiplier_k:
			/*
			d*e ≡ 1 (mod TF)
			d*e mod TF = 1
			d*e = k*TF + 1
			d = (k*TF + 1) / e  ...provided d is integer
			*/

			for (Type k = 1; k < endSearchAt; k++) //....what should be limit for k...ideally there is no limit for d..so no limit for k
				if ((k * TF + 1) % e == 0)
				{
					d = (k * TF + 1) / e;
					retVal = true;
					break;
				}
			break;

		case RSAMethodInitializer::IterateOverPrivateExponent_d:
			for (d = e; d < endSearchAt; d++)
				if ((d * e) % TF == 1)
				{
					retVal = true;
					break;
				}
			break;

		default:
			MyAssert::myRunTimeAssert(false, "Unknown PrivateExponentCalculationMethod");
		}

		m_privateExponentCalculationDuration = t.getDurationStringTillNowInNanoSeconds();
		return retVal;
	}

	template <typename Type>
	const Type& RSA_Template<Type>::getTotientFunction()
	{
		return TF;
	}

	template <typename Type>
	const Type& RSA_Template<Type>::getPublicExponent()
	{
		return e;
	}

	template <typename Type>
	const Type& RSA_Template<Type>::getPrivateExponent()
	{
		return d;
	}

	//Public Non-static Functions

	template <typename Type>
	void RSA_Template<Type>::testRSAComplete(const string& message)
	{
		testRSAComplete(reinterpret_cast<const unsigned char*>(message.c_str()));
	}

	template <typename Type>
	void RSA_Template<Type>::testRSAComplete(const ustring& message)
	{
		m_originalMessage = message;
		m_originalValues.clear();
		for (int i = 0; i < m_originalMessage.length(); i++)
			m_originalValues.push_back(Type(m_originalMessage[i]));

		testRSAComplete();
	}

	template <typename Type>
	void RSA_Template<Type>::testRSAComplete(const vector<Type>& messageValues)
	{
		m_originalValues = messageValues;
		m_originalMessage.clear();
		for (int i = 0; i < m_originalValues.size(); i++)
			m_originalMessage += char(m_originalValues[i]);

		testRSAComplete();
	}

	template <typename Type>
	void RSA_Template<Type>::testRSAComplete()
	{
		m_encryptedMessage.clear();
		m_decryptedMessage.clear();
		m_encryptedValues.clear();
		m_decryptedValues.clear();

		encrypt();
		decrypt();
		printAllInformation();
	}

	template <typename Type>
	void RSA_Template<Type>::encrypt()
	{
		Timer t;
		for (int i = 0; i < m_originalValues.size(); i++)
		{
			m_originalMessage += char(m_originalValues[i]);
			m_encryptedValues.push_back(ModularOperations::doModularExponentiation(m_originalValues[i], e, N));
			m_encryptedMessage += char(m_encryptedValues[i]);
		}

		m_encryptionDuration = t.getDurationStringTillNowInNanoSeconds();
	}

	template <typename Type>
	void RSA_Template<Type>::decrypt()
	{
		Timer t;
		for (int i = 0; i < m_encryptedValues.size(); i++)
		{
			Type decryptVal = ModularOperations::doModularExponentiation(m_encryptedValues[i], d, N);
			m_decryptedValues.push_back(decryptVal);
			m_decryptedMessage += char(decryptVal);
		}
		m_decryptionDuration = t.getDurationStringTillNowInNanoSeconds();
	}

	//Private Functions:

	//Utility Functions
	template <typename Type>
	bool RSA_Template<Type>::generatePrimeNumbers()
	{
		Timer t;
		p = PrimeNumber<Type>::generateRandomPrimeNumber(m_securityBitLength - 2);
		m_primeNumber1GenerationDuration = t.getDurationStringTillNowInNanoSeconds();
		t.resetTimer();
		q = PrimeNumber<Type>::generateRandomPrimeNumber(m_securityBitLength);
		m_primeNumber2GenerationDuration = t.getDurationStringTillNowInNanoSeconds();

		return true;
	}

	template <typename Type>
	void RSA_Template<Type>::printAllInformation()
	{
		cout.imbue(std::locale(""));
		cout << "\n\np = " << p
			<< " q = " << q
			<< " e = " << e
			<< " d = " << d
			<< " N = " << N
			//<< " ETF = " << ETF
			//<< " CTF = " << CTF
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


	class RSATemplateUnitTest
	{
	public:
		//Unit Testing
		static void TestAllDataTypes();
		static void TestAllAlgorithmsOfRSA();
		static void RSATest_usePrecalculatedPrimes();
		static void RSATest_boundaryConditions();
		static void RSATestWithBigInteger();

		static const string& getTestMessage();
		static const ustring& getTestMessageHavingAllChars();
		static const ustring& getTestMessageHavingAllChars2();
		static const ustring& getTestMessageHavingAllWideChars();
	};

}