/*

big integer implementation:
http://grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/6-b14/java/math/BigInteger.java

*/

#include <iostream>
#include <ctime>
#include <string>
#include <cassert>
using namespace std;

#include "BigInteger_Template.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

// ============================================= Testing ===============================================

namespace mm {

	void testConstructionBigIntegerTemplate()
	{
		cout << "\n\nTesting construction of BigIntegerTemplate:";

		long long testValues[] = { 0, 1, 2, 5, 8, 9, 10, 16, 32, 64, 99, 100, 123456, 10000250,
								  -0, -1, -2, -5, -8, -9, -10, -16, -32, -64, -99, -100, -123456, -10000250 };
		for (int i = 0; i < sizeof(testValues) / sizeof(long long); i++)
		{
			BigIntegerTemplate<unsigned int, unsigned int> bigInt1(testValues[i], 10);
			cout << "\nValue: " << i << " BigIntegerTemplate with base 10: " << bigInt1;
			string base10String1 = bigInt1.toString();

			BigIntegerTemplate<unsigned int, unsigned int> bigInt2(testValues[i], 2);
			string base2String1 = bigInt2.toString();

			BigIntegerTemplate<unsigned int, unsigned int> bigInt3(base2String1, 2, 10);
			string base10String2 = bigInt3.toString();

			BigIntegerTemplate<unsigned int, unsigned int> bigInt4(base10String1, 10, 2);
			string base2String2 = bigInt4.toString();

			//cout << "\nValue: " << i << " BigIntegerTemplate with base 2: " << base2String1;
			//cout << "\nValue: " << i << " BigIntegerTemplate with base 2: " << base2String2;
			//cout << "\nValue: " << i << " BigIntegerTemplate with base 10: " << base10String1;
			//cout << "\nValue: " << i << " BigIntegerTemplate with base 10: " << base10String2;

			string string1 = to_string(testValues[i]);
			assert(string1 == base10String1);
			assert(base2String1 == base2String2);
			assert(base10String1 == base10String2);
		}
	}

	void testConstructionBigIntegerTemplateExaustive()
	{
		cout << "\n\nTesting construction of BigIntegerTemplate (Exaustive Test):";
		const int TEST_CASES = 100;
		const int TEST_BASES = 10;
		unsigned int base[TEST_BASES];
		unsigned int maxBase = 32;
		cout << "\nTest bases: ";
		for (int i = 0; i < TEST_BASES; i++)
		{
			base[i] = rand() % (maxBase - 1) + 2; //Avoid base 0 and 1. Generates base values from 2 to 32
			cout << base[i] << ", ";
		}

		long long testRange = 1 << 30;
		long long halfTestRange = testRange / 2;

		for (int testcase = 0; testcase < TEST_CASES; testcase++)
		{
			long long number = halfTestRange - (rand() % testRange); //generates numbers from -halfTestRange to +halfTestRange
			cout << "\nTesting number: " << number;
			for (int i = 0; i < TEST_BASES; i++)
			{
				for (int k = 0; k < TEST_BASES; k++)
				{
					BigIntegerTemplate<unsigned int, unsigned int> bigInt1(number, base[i]);
					string string1 = bigInt1.toString();

					BigIntegerTemplate<unsigned int, unsigned int> bigInt2(number, base[k]);
					string string2 = bigInt2.toString();

					//Convert this string back to BigIntegerTemplate of base[i]
					BigIntegerTemplate<unsigned int, unsigned int> bigInt3(string1, base[i], base[k]);
					string string3 = bigInt3.toString();

					assert(string2 == string3);

					//Convert this string back to BigIntegerTemplate of base[i]
					BigIntegerTemplate<unsigned int, unsigned int> bigInt4(string2, base[k], base[i]);
					string string4 = bigInt4.toString();

					assert(string1 == string4);
				}
			}
		}
	}

	void testConstructionBigIntegerTemplateVeryVeryExaustive(long long number, size_t base1, size_t base2)
	{
		BigIntegerTemplate<unsigned int, unsigned int> bigInt1(number, base1);
		string string1 = bigInt1.toString();

		BigIntegerTemplate<unsigned int, unsigned int> bigInt2(number, base2);
		string string2 = bigInt2.toString();

		//Convert this string back to BigIntegerTemplate of base[i]
		BigIntegerTemplate<unsigned int, unsigned int> bigInt3(string1, base1, base2);
		string string3 = bigInt3.toString();

		assert(string2 == string3);

		//Convert this string back to BigIntegerTemplate of base[i]
		BigIntegerTemplate<unsigned int, unsigned int> bigInt4(string2, base2, base1);
		string string4 = bigInt4.toString();

		assert(string1 == string4);
	}

	void testConstructionBigIntegerTemplateVeryVeryExaustive()
	{
		cout << "\n\nTesting construction of BigIntegerTemplate (Very Very Exaustive Test):";
		const int MAX_DIGITS = 10;
		const int COUNT = 5;
		size_t maxBase = 32;

		long long testRange = 1 << 30;
		long long halfTestRange = testRange / 2;

		for (int digits = 1; digits <= MAX_DIGITS; digits++)
		{
			long long modulus = static_cast<long long>(pow(10, digits));
			for (int iterations = 0; iterations < COUNT; iterations++)
			{
				long long number = rand() % modulus;
				for (int iterations2 = 0; iterations2 < 2; iterations2++) //Test both positive and negative number
				{
					number = -number;
					cout << "\nTesting number: " << number;
					for (size_t base1 = 2; base1 <= maxBase; base1++)
					{
						for (size_t base2 = 2; base2 <= maxBase; base2++)
						{
							testConstructionBigIntegerTemplateVeryVeryExaustive(number, base1, base2);
						}
					}
				}
			}
		}
	}

	void testArithmeticOperationsBigIntegerTemplate(const BigIntegerTemplate<unsigned int, unsigned int>& bigInt1,
		const BigIntegerTemplate<unsigned int, unsigned int>& bigInt2, const long long& number1, const long long& number2)
	{
		//Test addition
		BigIntegerTemplate<unsigned int, unsigned int> bigInt3 = bigInt1 + bigInt2;
		long long result1 = bigInt3.covertToDecimal();
		long long result2 = number1 + number2;
		assert(result1 == result2);
		//Test substraction
		BigIntegerTemplate<unsigned int, unsigned int> bigInt4 = bigInt1 - bigInt2;
		long long result3 = bigInt4.covertToDecimal();
		long long result4 = number1 - number2;
		assert(result3 == result4);
		//Test multiplication
		BigIntegerTemplate<unsigned int, unsigned int> bigInt5 = bigInt1 * bigInt2;
		long long result5 = bigInt5.covertToDecimal();
		long long result6 = number1 * number2;
		assert(result5 == result6);
	}


	void testArithmeticOperationsBigIntegerTemplate(const long long& number1, const long long& number2, const size_t& base)
	{
		const BigIntegerTemplate<unsigned int, unsigned int> bigInt1(number1, base);
		const BigIntegerTemplate<unsigned int, unsigned int> bigInt2(number2, base);
		testArithmeticOperationsBigIntegerTemplate(bigInt1, bigInt2, number1, number2);

		string str1(std::to_string(number1));
		string str2(to_string(number2));
		const BigIntegerTemplate<unsigned int, unsigned int> bigInt3(str1, 10, base);
		const BigIntegerTemplate<unsigned int, unsigned int> bigInt4(str2, 10, base);
		testArithmeticOperationsBigIntegerTemplate(bigInt3, bigInt4, number1, number2);
	}

	void testAdditionSubstractionMultiplicationBigIntegerTemplate()
	{
		cout << "\n\nTesting Arithmetic Operations (Addition & Substraction) on BigIntegerTemplate:";
		const int TEST_CASES = 500;
		const int MAX_DIGITS = 10;
		long long modulus[MAX_DIGITS];
		for (int digits = 0; digits < MAX_DIGITS; digits++)
			modulus[digits] = static_cast<long long>(pow(10, digits));

		size_t maxBase = 32;
		const int COUNT = 4;
		for (int i = 1; i < TEST_CASES; i++)
		{
			size_t index1 = rand() % MAX_DIGITS;
			long long number1 = rand() % modulus[index1];

			size_t index2 = rand() % MAX_DIGITS;
			long long number2 = rand() % modulus[index2];

			for (int iterations = 0; iterations < COUNT; iterations++)
			{
				//Try all +ve & -ve combinations
				number1 = -number1;
				if (iterations % 2 == 0)
					number2 = -number2;

				cout << "\nValues: " << number1 << " & " << number2;

				for (size_t base = 2; base <= maxBase; base++)
				{
					testArithmeticOperationsBigIntegerTemplate(number1, number2, base);
				}
			}
		}
	}


	void testBasicArithmeticOperationsBigIntegerTemplate()
	{
		testAdditionSubstractionMultiplicationBigIntegerTemplate();
	}

	void testExtremeValuesBigIntegerTemplate()
	{
		//BigIntegerTemplate bi = std::numeric_limits<BigIntegerTemplate>::min();
	}

	void BigIntegerTemplateTest()
	{
		{
			//Rough Work:

		}

		//One time seed initialization
		srand((unsigned)time(0));

		testConstructionBigIntegerTemplate();
		testConstructionBigIntegerTemplateExaustive();
		testConstructionBigIntegerTemplateVeryVeryExaustive();
		testBasicArithmeticOperationsBigIntegerTemplate();
	}


	MM_DECLARE_FLAG(BigInteger_Template_unit_test);

	MM_UNIT_TEST(BigInteger_Template_unit_test_1, BigInteger_Template_unit_test)
	{
		BigIntegerTemplateTest();
	}
}
