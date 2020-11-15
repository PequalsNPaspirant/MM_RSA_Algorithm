#include <iostream>
#include <ctime>
#include <cmath>
#include <cassert>
#include <string>
using namespace std;

#include "BigInteger.h"
#include "Timer/Timer_Timer.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	// ============================================= Testing ===============================================


	void testSmallNumbersArithmeticOperations(const BigInteger& bigInt1, const BigInteger& bigInt2, const long long& number1, const long long& number2)
	{
		//Test addition
		BigInteger bigInt3 = bigInt1 + bigInt2;
		long long result1 = bigInt3.covertToLongLong();
		long long result2 = number1 + number2;
		assert(result1 == result2);
		//Test substraction
		BigInteger bigInt4 = bigInt1 - bigInt2;
		long long result3 = bigInt4.covertToLongLong();
		long long result4 = number1 - number2;
		assert(result3 == result4);
		//Test multiplication
		BigInteger bigInt5 = bigInt1 * bigInt2;
		long long result5 = bigInt5.covertToLongLong();
		long long result6 = number1 * number2;
		assert(result5 == result6);
		//Test division
		BigInteger bigIntQuotient, bigIntRemainder;
		if (bigInt2 != 0LL)
		{
			bigInt1.divideAndRemainder(bigInt2, bigIntQuotient, bigIntRemainder);
			long long result7 = bigIntQuotient.covertToLongLong();
			long long result8 = bigIntRemainder.covertToLongLong();
			long long result9 = number1 / number2;
			long long result10 = number1 % number2;
			assert(result7 == result9);
			assert(result8 == result10);
		}
	}

	void testSmallNumbers(const long long& number1, const long long& number2)
	{
		const BigInteger bigInt1(number1);
		string bigInt1str(bigInt1.toString());
		string str1(to_string(number1));
		assert(bigInt1str == str1);

		long long result1 = bigInt1.covertToLongLong();
		assert(result1 == number1);

		const BigInteger bigInt2(number2);
		string bigInt2str(bigInt2.toString());
		string str2(to_string(number2));
		assert(bigInt2str == str2);

		long long result2 = bigInt2.covertToLongLong();
		assert(result2 == number2);

		const BigInteger bigInt3(str1);
		string bigInt3str(bigInt3.toString());
		assert(bigInt3str == str1);
		assert(bigInt1 == bigInt3);

		const BigInteger bigInt4(str2);
		string bigInt4str(bigInt4.toString());
		assert(bigInt4str == str2);
		assert(bigInt2 == bigInt4);

		testSmallNumbersArithmeticOperations(bigInt1, bigInt2, number1, number2);
		testSmallNumbersArithmeticOperations(bigInt3, bigInt4, number1, number2);

		bool comparisonResult1 = number1 < number2;
		bool comparisonResult2 = bigInt1 < bigInt2;
		bool comparisonResult3 = bigInt3 < bigInt4;
		assert(comparisonResult1 == comparisonResult2);
		assert(comparisonResult1 == comparisonResult3);
	}

	void testSmallNumbers()
	{
		cout << "\n\ntestSmallNumbers----------------------------------\n";

		//Predefined numbers
		long long testValues[] = { 0, 1, 2, 5, 8, 9, 10, 16, 32, 64, 99, 100, 123456, 10000250,
			-0, -1, -2, -5, -8, -9, -10, -16, -32, -64, -99, -100, -123456, -10000250, -16337, -730 };
		size_t count = sizeof(testValues) / sizeof(long long);
		for (int i = 0; i < count; i++)
			for (int k = 0; k < count; k++)
			{
				//cout << "\nValue: " << testValues[i];
				testSmallNumbers(testValues[i], testValues[k]);
			}

		//Random numbers
		cout << "\n\nTesting Arithmetic Operations (Addition, Substraction, Multiplication and division) on BigInteger:";
		const int TEST_CASES = 1000;
		const int MAX_DIGITS = 10;
		long long modulus[MAX_DIGITS];
		for (int digits = 0; digits < MAX_DIGITS; digits++)
			modulus[digits] = (BigInteger::DigitType)pow(10, digits);

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

				//cout << "\nValues: " << number1 << " & " << number2;
				testSmallNumbers(number1, number2);
			}
		}
	}

	void testVeryLargeNumberConstruction()
	{
		cout << "\n\ntestVeryLargeNumberConstruction----------------------------------\n";
		const int iterations = 1000;
		for (int i = 0; i < iterations; i++)
		{
			int numDigits = 50 + rand() % 50;  //generates numbers with 50 to 100 digits
			string random1;
			for (int k = 0; k < numDigits; k++)
				random1 += char('0' + rand() % 10);
			cout << "\nNumber: " << random1;
			BigInteger bigInt1(random1);
			string random2 = bigInt1.toString();
			//Trim leading zeros
			random1.erase(random1.begin(), random1.begin() + random1.find_first_not_of('0'));
			assert(random1 == random2);
		}

		//Test construction from hex number
		BigInteger bigInt1("0", 16);
		string hexStr1 = bigInt1.toString();
		cout << "\nHex:     " << hexStr1;
		cout << "\nDecimal: " << bigInt1.toString();
	}

	void testVeryLargeNumberArithmetic(const BigInteger& a, const BigInteger& b, const BigInteger& k)
	{
		string as(a.toString());
		cout << "\na = " << as;

		string bs(b.toString());
		cout << "\nb = " << bs;

		BigInteger c = a + b;
		string cs(c.toString());
		cout << "\nc = a + b = " << cs;

		BigInteger d = c - a;
		string ds(d.toString());
		cout << "\nd = c - a = " << ds;
		assert(d == b);
		assert(ds == bs);

		BigInteger e = c - b;
		string es(e.toString());
		cout << "\ne = c - b = " << es;
		assert(e == a);
		assert(es == as);

		BigInteger f = a * b;
		string fs(f.toString());
		cout << "\nf = a * b = " << fs;

		if (a != BigInteger::bigIntZero)
		{
			BigInteger g, h;
			f.divideAndRemainder(a, g, h);
			string gs(g.toString());
			string hs(h.toString());
			cout << "\ng = f / a = " << gs;
			cout << "\nh = f % a = " << hs;
			assert(g == b);
			assert(gs == bs);
			assert(h == BigInteger::bigIntZero);
			assert(hs == "0");
		}

		if (b != BigInteger::bigIntZero)
		{
			BigInteger i, j;
			f.divideAndRemainder(b, i, j);
			string is(i.toString());
			string js(j.toString());
			cout << "\ni = f / b = " << is;
			cout << "\nj = f % b = " << js;
			assert(i == a);
			assert(is == as);
			assert(j == BigInteger::bigIntZero);
			assert(js == "0");
		}

		string ks(k.toString());
		cout << "\nk = " << ks;

		//Find min of a, b, k
		BigInteger lhs(a);
		BigInteger rhs(b);
		BigInteger min(k);
		string mins(min.toString());
		assert(min == k);
		assert(mins == ks);

		if (min.absolute() > a.absolute())
		{
			min = a;
			lhs = k;
		}
		if (min.absolute() > b.absolute())
		{
			min = b;
			rhs = k;
		}

		//The tally wont match if min is not less than both lhs and rhs
		if (min.absolute() == lhs.absolute() || min.absolute() == rhs.absolute())
			return;

		string lhss(lhs.toString());
		cout << "\nlhs = " << lhss;
		string rhss(rhs.toString());
		cout << "\nrhs = " << rhss;
		mins = min.toString();
		cout << "\nmin = " << mins;

		BigInteger dividend = lhs * rhs + min;
		string dividends(dividend.toString());
		cout << "\ndividend = lhs * rhs + min = " << dividends;

		bool isLhsNegative = lhs.isNegative();
		bool isRhsNegative = rhs.isNegative();
		bool isMinNegative = min.isNegative();

		if (dividend != BigInteger::bigIntZero && lhs != BigInteger::bigIntZero && rhs != BigInteger::bigIntZero)
		{
			BigInteger div1, rem1;
			dividend.divideAndRemainder(lhs, div1, rem1);
			string div1s(div1.toString());
			string rem1s(rem1.toString());
			cout << "\ndiv1 = dividend / lhs = " << div1s;
			cout << "\nrem1 = dividend % lhs = " << rem1s;

			BigInteger div2, rem2;
			dividend.divideAndRemainder(rhs, div2, rem2);
			string div2s(div2.toString());
			string rem2s(rem2.toString());
			cout << "\ndiv2 = dividend / rhs = " << div2s;
			cout << "\nrem2 = dividend % rhs = " << rem2s;

			if (min == BigInteger::bigIntZero
				|| (!isLhsNegative && !isRhsNegative && !isMinNegative)
				|| (isLhsNegative && isRhsNegative && !isMinNegative))
			{
				assert(div1 == rhs);
				assert(div1s == rhss);
				assert(rem1 == min);
				assert(rem1s == mins);

				assert(div2 == lhs);
				assert(div2s == lhss);
				assert(rem2 == min);
				assert(rem2s == mins);
			}
			else if ((!isLhsNegative && !isRhsNegative && isMinNegative)
				|| (isLhsNegative && isRhsNegative && isMinNegative)
				|| ((isLhsNegative || isRhsNegative) && !isMinNegative))
			{
				assert(div1.absolute() == (rhs.absolute() - BigInteger::bigIntOne));
				assert((rem1.absolute() + min.absolute()) == lhs.absolute());

				assert(div2.absolute() == (lhs.absolute() - BigInteger::bigIntOne));
				assert((rem2.absolute() + min.absolute()) == rhs.absolute());
			}
		}
	}

	void testVeryLargeNumberArithmetic()
	{
		//Predefined numbers (Specially the ones which failed during dev testing)
		struct testSet
		{
			testSet(const string& _first, const string& _second, const string& _third)
				: first(_first), second(_second), third(_third)
			{}

			string first;
			string second;
			string third;
		};

		const testSet testValues[] = {
			{ "0", "0", "0" },
			{ "0", "0", "1" },
			{ "0", "1", "0" },
			{ "0", "1", "1" },
			{ "1", "0", "0" },
			{ "1", "0", "1" },
			{ "1", "1", "0" },
			{ "1", "1", "1" },

			{ "12345", "0", "1" },
			{ "12345", "1", "1" },
			{ "0", "12345", "1" },
			{ "1", "12345", "1" },
			{ "4611686014132420609", "633437442543876639", "267546423334887" }
		};

		for (int i = 0; i < sizeof(testValues) / sizeof(testSet); i++)
		{
			testVeryLargeNumberArithmetic(testValues[i].first, testValues[i].second, testValues[i].third);
			testVeryLargeNumberArithmetic(testValues[i].first, testValues[i].second, "-" + testValues[i].third);
			testVeryLargeNumberArithmetic(testValues[i].first, "-" + testValues[i].second, testValues[i].third);
			testVeryLargeNumberArithmetic(testValues[i].first, "-" + testValues[i].second, "-" + testValues[i].third);
			testVeryLargeNumberArithmetic("-" + testValues[i].first, testValues[i].second, testValues[i].third);
			testVeryLargeNumberArithmetic("-" + testValues[i].first, testValues[i].second, "-" + testValues[i].third);
			testVeryLargeNumberArithmetic("-" + testValues[i].first, "-" + testValues[i].second, testValues[i].third);
			testVeryLargeNumberArithmetic("-" + testValues[i].first, "-" + testValues[i].second, "-" + testValues[i].third);
		}

		struct testSetBigInt
		{
			testSetBigInt(const BigInteger& _first, const BigInteger& _second, const BigInteger& _third)
				: first(_first), second(_second), third(_third)
			{}

			BigInteger first;
			BigInteger second;
			BigInteger third;
		};

		testSetBigInt testValuesBigInt[] = {
			{ BigInteger(vector<BigInteger::DigitType>({ 1, 0, 0 })), BigInteger(vector<BigInteger::DigitType>({ 0, 294967295, 131551774 })), BigInteger(vector<BigInteger::DigitType>({ 124586, 25689745 })) },
			{ BigInteger(vector<BigInteger::DigitType>({ 1, 0, 0 })), BigInteger(vector<BigInteger::DigitType>({ 0, 1702788861 })), BigInteger(vector<BigInteger::DigitType>({ 0, 124586 })) },
			{ BigInteger(vector<BigInteger::DigitType>({ 1, 0, 0, 0 })), BigInteger(vector<BigInteger::DigitType>({ 0, BigInteger::DigitType(BigInteger::numberSystemBase - 1), BigInteger::DigitType(BigInteger::numberSystemBase - 1) })), BigInteger(vector<BigInteger::DigitType>({ 0, BigInteger::DigitType(BigInteger::numberSystemBase - 1) })) }
		};

		for (int i = 0; i < sizeof(testValuesBigInt) / sizeof(testSet); i++)
		{
			testVeryLargeNumberArithmetic(testValuesBigInt[i].first, testValuesBigInt[i].second, testValuesBigInt[i].third);
			testVeryLargeNumberArithmetic(testValuesBigInt[i].first, testValuesBigInt[i].second, -testValuesBigInt[i].third);
			testVeryLargeNumberArithmetic(testValuesBigInt[i].first, -testValuesBigInt[i].second, testValuesBigInt[i].third);
			testVeryLargeNumberArithmetic(testValuesBigInt[i].first, -testValuesBigInt[i].second, -testValuesBigInt[i].third);
			testVeryLargeNumberArithmetic(-testValuesBigInt[i].first, testValuesBigInt[i].second, testValuesBigInt[i].third);
			testVeryLargeNumberArithmetic(-testValuesBigInt[i].first, testValuesBigInt[i].second, -testValuesBigInt[i].third);
			testVeryLargeNumberArithmetic(-testValuesBigInt[i].first, -testValuesBigInt[i].second, testValuesBigInt[i].third);
			testVeryLargeNumberArithmetic(-testValuesBigInt[i].first, -testValuesBigInt[i].second, -testValuesBigInt[i].third);
		}

		//Random numbers
		const int TEST_CASES = 100;
		const size_t bits = 1024;
		for (int i = 1; i < TEST_CASES; i++)
		{
			BigInteger a = BigInteger::getRandomNumber(bits);
			BigInteger b = BigInteger::getRandomNumber2(bits);
			BigInteger k = BigInteger::getRandomNumber3(bits);
			testVeryLargeNumberArithmetic(a, b, k);
		}
	}

	void testBigIntPower(const BigInteger& a)
	{
		string as(a.toString());
		cout << "\na = " << as;

		BigInteger b[10];
		string bs[10];
		for (int i = 0; i < 10; i++)
		{
			b[i] = a.power(BigInteger(BigInteger::DigitType(i)));
			bs[i] = b[i].toString();

			BigInteger t = a.power2(BigInteger(BigInteger::DigitType(i)));
			string ts(t.toString());

			cout << "\nb = a ^ " << i << " = " << bs[i];
			assert(t == b[i]);
			assert(ts == bs[i]);
		}
		int testCase = 0;
		for (int i = 1; i < 10; i++)
		{
			cout << "\nTest Case: " << ++testCase;
			if (b[i - 1] != BigInteger::bigIntZero)
				continue;
			BigInteger c, d;
			b[i].divideAndRemainder(b[i - 1], c, d);
			string cs(c.toString());
			string ds(d.toString());

			assert(c == a);
			assert(cs == as);

			assert(d == BigInteger::bigIntZero);
			assert(ds == "0");
		}

		for (int i = 1; i < 10; i++)
		{
			cout << "\nTest Case: " << ++testCase;
			if (a != BigInteger::bigIntZero)
				continue;
			BigInteger c, d;
			b[i].divideAndRemainder(a, c, d);
			string cs(c.toString());
			string ds(d.toString());

			assert(c == b[i - 1]);
			assert(cs == bs[i - 1]);

			assert(d == BigInteger::bigIntZero);
			assert(ds == "0");
		}
	}

	void testBigIntPower()
	{
		//Predefined numbers (Specially the ones which failed during dev testing)
		const string testValues[] = {
			"0",
			"1",
			"000010",
			"880",
			"10000000000000000000000000000000000000000001"
		};
		for (int i = 0; i < sizeof(testValues) / sizeof(string); i++)
		{
			BigInteger a = BigInteger(testValues[i]);
			testBigIntPower(a);

			BigInteger b = BigInteger("-" + testValues[i]);
			testBigIntPower(b);
		}

		//Random numbers
		size_t increment = 10;
		for (size_t bits = 10; bits <= 1000; bits += increment)
		{
			if (bits == 200)
				increment = 100;
			cout << "\nbits = " << bits << " -------------------------------------------------------";
			BigInteger a = BigInteger::getRandomNumber(bits);
			testBigIntPower(a);
		}
	}

	void testVeryLargeNumbers()
	{
		testVeryLargeNumberConstruction();
		testVeryLargeNumberArithmetic();
		testBigIntPower();
	}

	void testExtraLargeNumbers_test1_parsing()
	{
		/*
		My Tests:
		time in milli seconds

		Test														My BigInteger    JAVA BigInteger    .NET BigInteger
		----------------------------------------------------------------------------------------------------------------------------
		Generate 500,000 digit random number						   14 ms
		Create 5,000 digit number from string						  164 ms		     8 ms
		Create 50,000 digit number from string - debug			   10,428 ms		    58 ms
		Create 50,000 digit number from string - release			  102 ms	        58 ms
		Create 100,000 digit number from string - release			  484 ms		   166 ms
		Create 500,000 digit number from string - release			7,764 ms		 3,614 ms
		Create 500,000 digit number from string - release x64		2,280 ms
		Create 500,000 digit number from string - release x64		1,325 ms
		Create 1,000,000 digit number from string - release			4,120 ms		14,553 ms

		Convert BigInt to string 1,000,000 digit				  116,290 ms		 1,763 ms
		*/
		cout << "\n\ntestExtraLargeNumbers()";

		Timer t;

		// Test 1: Parsing: Converting two strings of length 500000 representing numbers in base 10 to the internal representation.
		//Generate random sequence of decimal digits
		const size_t numDigits = 50000;
		string random(numDigits, '0');
		for (int k = 0; k < numDigits; k++)
			random[k] = char('0' + rand() % 10);

		cout << "\nTest 1: Parsing: Converting a string of length " << numDigits << " representing numbers in base 10 to the internal representation: ";
		t.resetTimer();
		BigInteger num(random);
		cout << t.getDurationStringTillNowInNanoSeconds();

		//Convert BigInteger back to string
		cout << "\nTime required to convert BigInteger back to string: ";
		t.resetTimer();
		string str(num.toString());
		cout << t.getDurationStringTillNowInNanoSeconds();

		//cout << "\n1st String: " << random;
		//cout << "\n2nd String: " << str;

		assert(random == str);

		/*
		Results:
		Test 1: Parsing: Converting a string of length 1000000 representing numbers in base 10 to the internal representation: 7,299,721,855 nanoseconds
		Time required to convert BigInteger back to string: 116,290,321,431 nanoseconds

		Test 1: Parsing: Converting a string of length 500000 representing numbers in base 10 to the internal representation: 2,135,200,513 nanoseconds
		Time required to convert BigInteger back to string: 28,404,088,456 nanoseconds

		Test 1: Parsing: Converting a string of length 1000000 representing numbers in base 10 to the internal representation: 8,052,430,309 nanoseconds
		Time required to convert BigInteger back to string: 113,723,386,212 nanoseconds

		Test 1: Parsing: Converting a string of length 500000 representing numbers in base 10 to the internal representation: 2,393,273,685 nanoseconds
		Time required to convert BigInteger back to string: 28,504,641,148 nanoseconds

		Test 1: Parsing: Converting a string of length 500000 representing numbers in base 10 to the internal representation: 2,092,657,944 nanoseconds
		Time required to convert BigInteger back to string: 28,422,124,829 nanoseconds

		--When doDivide() - % was replaced by multiplication and substraction
		Test 1: Parsing: Converting a string of length 500000 representing numbers in base 10 to the internal representation: 2,906,235,543 nanoseconds
		Time required to convert BigInteger back to string: 33,532,390,012 nanoseconds

		Test 1: Parsing: Converting a string of length 500000 representing numbers in base 10 to the internal representation: 2,375,695,361 nanoseconds
		Time required to convert BigInteger back to string: 33,165,674,755 nanoseconds

		--When doDivide() skips first zero digits:
		Test 1: Parsing: Converting a string of length 500000 representing numbers in base 10 to the internal representation: 2,024,057,313 nanoseconds
		Time required to convert BigInteger back to string: 15,619,945,584 nanoseconds

		--commented out conversion of Digit to string, but still it taked same time
		Test 1: Parsing: Converting a string of length 500000 representing numbers in base 10 to the internal representation: 2,019,683,448 nanoseconds
		Time required to convert BigInteger back to string: 15,368,757,969 nanoseconds

		--Separated pre-calc and post-calc time. These times are other than while loop:
		Test 1: Parsing: Converting a string of length 500000 representing numbers in base 10 to the internal representation: 2,030,807,818 nanoseconds
		Time required to convert BigInteger back to string:
		Pre calc time: 194,997 nanoseconds
		Post calc time: 80,044 nanoseconds 15,558,613,805 nanoseconds

		--When in toString() comparing with BigInteger::bigIntZero replaced by m_digits[] != 0
		Test 1: Parsing: Converting a string of length 500000 representing numbers in base 10 to the internal representation: 2,027,114,857 nanoseconds
		Time required to convert BigInteger back to string: 14,926,523,377 nanoseconds

		--When in doDivide() adition is changed into bitwise OR
		Test 1: Parsing: Converting a string of length 500000 representing numbers in base 10 to the internal representation: 2,017,798,709 nanoseconds
		Time required to convert BigInteger back to string: 14,893,399,216 nanoseconds

		--with third toString function:
		Test 1: Parsing: Converting a string of length 500000 representing numbers in base 10 to the internal representation: 2,284,829,771 nanoseconds
		Time required to convert BigInteger back to string: 15,975,506,499 nanoseconds

		--When 10^18 is used as divisor and using long division


		*/
	}

	void testExtraLargeNumbers_test2_addition()
	{
		Timer t;
		
		cout << "\n\nTest: Generating 100000 digit random big integer (Takes 14,721,756 nanoseconds; 380,121 ns): ";
		size_t bits = static_cast<size_t>(100000 * (log(10) / log(2)));
		t.resetTimer();		
		BigInteger b1 = BigInteger::getRandomNumber(bits);
		cout << t.getDurationStringTillNowInNanoSeconds();

		size_t trials = 100000;
		cout << "\nRepeating addition of bigintegers " << trials << " times\n";
		BigInteger result(b1);
		t.resetTimer();
		for (size_t i = 0; i < trials; i++)
		{
			result += b1;
		}
		cout << t.getDurationStringTillNowInNanoSeconds();
	}

	void testExtraLargeNumbers_test5_factorial()
	{
		Timer t;
		cout << "\n\nTest: Generating 50000! (Takes 6,108,231,991 nanoseconds): ";

		t.resetTimer();
		BigInteger factorial = BigInteger::bigIntOne;
		for (BigInteger::DigitType num = 1; num < 50000; ++num)
			factorial = factorial * BigInteger{ num };

		cout << t.getDurationStringTillNowInNanoSeconds();
		cout << "\nfactorial of 50000 = " << factorial.toString();
	}

	void testExtraLargeNumbers()
	{
		/*

		Comparison --
		The following time measurements are a results of some benchmarks run on my shitty 1.65GHz Dual Core computer. ^^
		Test 1: Parsing: Converting two strings of length 500000 representing numbers in base 10 to the internal representation.
		Test 2: Add: Adding a 100000 decimal digit number to another equally sized number, 100000 times. (MidBig-size cumulative benchmark.)
		Test 3: Sub: Same as the addition experiment, but with subtractions.
		Test 4: MidMul: Multiplying a 300 decimal digit number to a growing product, initially a 300 decimal digit number. (Mid-size cumulative benchmark.)
		Test 5: TinyMul: The naive straightforward way to calculate 50000! (Small-size cumulative benchmark.)
		Test 6: BigMul: Multiplication of two 500000 decimal digit numbers. (Big-size benchmark.)
		Test 7: MidDiv: Dividing a 400000 decimal digit number by a 4000 decimal digit number a 1000 times. (Mid-size cumulative benchmark.)
		Test 8: BigDiv: Dividing a 400000 decimal digit number by a 200000 decimal digit number. (Big-size benchmark.)
		Test 9: toString: Converting the internal representation (having in decimal 213237 digits) to a decimal number string.

		Test No: Test        BigInteger    BigInt    Apint     LargeInteger    My BigInteger
		---------------------------------------------------------------------------------------
		Test 1:  Parsing     31.602s       8.497s    0.049s    14.054s         1.325s
		Test 2:  Add         6.394s        3.006s    27.279s   6.322s          6.xxxs
		Test 3:  Sub         5.618s        2.243s    24.35s    6.026s
		Test 4:  MidMul      2.676s        2.259s    40.433s   3.176s
		Test 5:  TinyMul     10.683s       1.468s    35.535s   6.879s
		Test 6:  BigMul      12.332s       0.655s    0.289s    1.266s
		Test 7:  MidDiv      9.936s        7.558s    297.022s  3239.432s
		Test 8:  BigDiv      3.647s        2.536s    0.563s    5.335s
		Test 9:  toString    16.912s       4.614s    0.029s    15.116s

		Comments
		Parsing: Apint is by far the fastest parser, this because it internally (probably) uses a power of 10 as base. BigInteger is by far the slowest.
		Add: BigInt is fastest due to its mutability. Apint is by far the slowest, since it uses a power of 10 base.
		Sub: The same goes here. Apint is slow when it comes to simple arithmetic operations.
		MidMul: A bit surprisingly Apint is the slowest here, while BigInt is the fastest.
		TinyMul: Not surprisingly BigInt is the fastest (since it's designed around this user case) and Apint the slowest.
		BigMul: BigInteger is by far the slowest since it utilizes a naive O(n2) algorithm, whereas BigInt uses (a parallel) Karatsuba, and Apint probably uses FFT making it the fastest.
		MidDiv: What the heck happened to LargeInteger!? 50 minutes runtime!? Well that's simply unacceptable. BigInt and BigInetger are the fastest here. Apint (and definitely LargeInteger) probably makes unwise algorithm choices.
		BigDiv: LargeInteger is the slowest, but by no means bad. Apint is once again fastest, this is really the area where it shines.
		toString: Both BigInteger and LargeInteger are just slow. LargeInteger also uses for some unknown reason a recursive approach, which could cause Stackoverflow if you don't increase your stack size (terrible design choice indeed...). Once again Apint is the fastest due to its internal representation
		*/

		testExtraLargeNumbers_test1_parsing();
		//testExtraLargeNumbers_test2_addition();

		//testExtraLargeNumbers_test5_factorial();
	}

	void testBigIntegerInitialization()
	{
		BigInteger b1 = 0;
		BigInteger b2 = 0U;
		BigInteger b3 = 0L;
		BigInteger b4 = 0UL;
		BigInteger b5 = 0LL;
		BigInteger b6 = 0ULL;

		BigInteger b7 = 1;
		BigInteger b8 = 1U;
		BigInteger b9 = 1L;
		BigInteger b10 = 1UL;
		BigInteger b11 = 1LL;
		BigInteger b12 = 1ULL;

		BigInteger b13 = -1;
		BigInteger b14 = -1L;
		BigInteger b15 = -1LL;
	}

	void generatePrimeNumbers(const int count)
	{
		//There are n/log(n) prime numbers between 1 and n 
		vector<int> primes(count);
		int currentIndex = 0;
		for (int n = 2; currentIndex != count; n++)
		{
			int squareRootN = static_cast<int>(sqrt(n));
			int i = 0;
			while (true)
			{
				if (i == currentIndex || primes[i] > squareRootN)
				{
					primes[currentIndex++] = n;
					break;
				}
				else if (n % primes[i] == 0)
					break;

				i++;
			}
		}

		cout << "\n\nstatic const vector<DigitType> first" << count << "Primes = {\n\t";
		int groupEnd = 100;
		for (int i = 0; i < primes.size(); i++)
		{
			if (primes[i] > groupEnd)
			{
				cout << "\n\t";
				groupEnd += 100;
			}
			cout << primes[i];
			if (i < primes.size() - 1)
				cout << ", ";
		}
		cout << "\n};";
	}

	void FindNextPrime(const BigInteger& referenceNumber, const PrimalityTest& primalityTestMethod)
	{
		cout << "\n\nFinding next prime. Reference Number: " << referenceNumber.toString();
		BigInteger prime = BigInteger::getNextPrimeNumber(referenceNumber, primalityTestMethod);
		string primeStr(prime.toString());
		cout << "\nPrime number = " << primeStr;
	}

	BigInteger::ResultType generateRandomNumber(size_t digits)
	{
		BigInteger::ResultType number = 0;
		for (size_t i = 0; i < digits; i++)
			number = number * 10 + (1 + (rand() % 9));

		return number;
	}

	void generateBigPrimeNumbers()
	{
		//Find random primes of various different digit size
		//unsigned long long max is 18,446,744,073,709,551,615 - a 20 digit number
		for (size_t digits = 1; digits < 20; ++digits)
		{
			BigInteger::getLogger() << ("\n\n=================== Exercise: generating prime number of " + to_string(digits) + " digits");
			BigInteger::ResultType randomNumber = generateRandomNumber(digits);
			FindNextPrime(randomNumber, Fermat_efficient);
			FindNextPrime(randomNumber, MillerRabin_basic);
		}

		//Find random primes of various different bit size
		vector<size_t> bits = { 
			8, 16, 24, 32, 40, 48, 56, 64, 72, 80,
			100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,
			1024, 2048, 3072, 4096
		};
		for (size_t i = 0; i < bits.size(); ++i)
		{
			BigInteger::getLogger() << ("\n\n=================== Exercise: generating prime number of length " + to_string(bits[i]) + " bits");
			BigInteger randomNumber = BigInteger::getRandomNumber(bits[i]);
			FindNextPrime(randomNumber, Fermat_efficient);
			FindNextPrime(randomNumber, MillerRabin_basic);
		}
	}

	//void generateBigPrimeNumbers1024bits()
	//{
	//	//Find 10 random primes of 1024 bits
	//	BigInteger::getLogger() << "\n=================== Exercise: Find 10 prime numbers of 1024 bits";
	//	for (int i = 0; i < 10; i++)
	//	{
	//		BigInteger::getLogger() << ("\n" + to_string(i + 1) + " th prime number: ");
	//		//_sleep(4000);
	//		BigInteger randomNumber = BigInteger::getRandomNumber(1024);
	//		FindNextPrime(randomNumber, Fermat_efficient);
	//		FindNextPrime(randomNumber, MillerRabin_basic);
	//	}
	//}

	//This function is not used
	void FindRandomPrime(const size_t& bits, const PrimalityTest& primalityTestMethod)
	{
		BigInteger::getLogger() << ("Finding random prime of size " + to_string(bits) + " bits");
		BigInteger prime = BigInteger::getPrimeNumber(bits, primalityTestMethod);
		string primeStr(prime.toString());
		BigInteger::getLogger() << ("Prime number = " + primeStr);
	}

	void testPredefinedPrimeNumbers(const vector<string>& numberStrings)
	{
		for (size_t i = 0; i < numberStrings.size(); i++)
		{
			cout << "\nTesting " << numberStrings[i].length() << " digit number: " << numberStrings[i];
			BigInteger::getLogger() << ("Testing " + to_string(numberStrings[i].length()) + " digit number: " + numberStrings[i]);
			BigInteger b(numberStrings[i]);
			BigInteger::getLogger() << (b.isPrime(Fermat_efficient) ? "Result: Fermat_efficient - Prime" : "Result: Fermat_efficient - NOT Prime");
			BigInteger::getLogger() << (b.isPrime(MillerRabin_basic) ? "Result: MillerRabin_basic - Prime" : "Result: MillerRabin_basic - NOT Prime");
			BigInteger::getLogger() << (b.isPrime(MillerRabin_usingOptimizedBases) ? "Result: MillerRabin_usingOptimizedBases - Prime" : "Result: MillerRabin_usingOptimizedBases - NOT Prime");
		}
	}

	void testPredefinedPrimeNumbers()
	{
		const vector<string> numberStrings = {
			"2",
			"3"
		};
		testPredefinedPrimeNumbers(numberStrings);
	}

	void testPredefinedCarmichaelNumbers()
	{
		const vector<string> CarmichaelNumberStrings = {
			"561", //Smallest
			//Arnault 1994, strong pseudoprime to all bases up to 306
			"29674495668685510550154174642905332730771991799853043350995075531276838753171770199594238596428121188033664754218345562493168782883",
			"2887148238050771212671429597130393991977609459279722700926516024197432303799152733116328983144639225941977803110929349655578418949441740933805615113979999421542416933972905423711002751042080134966731755152859226962916775325475044445856101949404200039904432116776619949629539250452698719329070373564032273701278453899126120309244841494728976885406024976768122077071687938121709811322297802059565867",
			//Strong pseudoprime to first 100 bases
			"197475704297076769879318479365782605729426528421984294554780711762857505669370986517424096751829488980502254269692200841641288349940843678305321105903510536750100514089183274534482451736946316424510377404498460285069545777656519289729095553895011368091845754887799208568313368087677010037387886257331969473598709629563758316982529541918503729974147573401550326647431929654622465970387164330994694720288156577774827473110333350092369949083055692184067330157343079442986832268281420578909681133401657075403304506177944890621300718745594728786819988830295725434492922853465829752746870734788604359697581532651202427729467",
			//Strong pseudoprime to first 150 bases
			"2504564851231996223418611498583553580586431162725714036294663419005627942030045018144967085826016995812748308972856014960994030057096300272690934546847718913274308904632162037753108744756079484071197757334474410667077275268095650646955133107287726653142089063491101528203219286279619714745365476456016263876476506935633902632378276445418807506643579691598485378380707876204179521868647423847956174718793857337275560326966743283833603259339320266954292802259534246253628396748505321522751546284902630334444060405092248083998624231344891540701484875133564504847404605998928272819163447443703835478321841022013831138852193839499198235152203734163783925867691241340516136948911294063782761240713332883707486122030071233696262539897861764349350102562679795879652984483644711085101952997260985769555028200212433179592354351467963621580674595217679045289986395851940360535530808265791863676735166100444465385760336851651312776349197351443263637225179385994064241024523629682623814114793546441523513505946466820080716059467",
			//Strong pseudoprime to first 168 bases
			"2809386136371438866496346539320857607283794588353401165473007394921174159995576890097633348301878401799853448496604965862616291048652062065394646956750323263193037964463262192320342740843556773326129475220032687577421757298519461662249735906372935033549531355723541168448473213797808686850657266188854910604399375221284468243956369013816289853351613404802033369894673267294395882499368769744558478456847832293372532910707925159549055961870528474205973317584333504757691137280936247019772992086410290579840045352494329434008415453636962234340836064927449224611783307531275541463776950841504387756099277118377038405235871794332425052938384904092351280663156507379159650872073401637805282499411435158581593146712826943388219341340599170371727498381901415081480224172469034841153893464174362543356514320522139692755430021327765409775374978255770027259819794532960997484676733140078317807018465818200888425898964847614568913543667470861729894161917981606153150551439410460813448153180857197888310572079656577579695814664713878369660173739371415918888444922272634772987239224582905405240454751027613993535619787590841842251056960329294514093407010964283471430374834873427180817297408975879673867"
		};

		testPredefinedPrimeNumbers(CarmichaelNumberStrings);
	}

	void generateAndTestCarmichaelNumbers()
	{
		for (BigInteger::DigitType i = 0; i < 1000; i++)
		{
			BigInteger first(6 * i + 1);
			BigInteger second(12 * i + 1);
			BigInteger third(18 * i + 1);
			if (first.isPrime(Fermat_efficient) && second.isPrime(Fermat_efficient) && third.isPrime(Fermat_efficient))
			{
				BigInteger CarmichaelNumber = first * second * third;
				cout << ("\ni = " + to_string(i) + " Checking if Carmichael number: " + CarmichaelNumber.toString() + " is prime. Result: ");
				BigInteger::getLogger() << ("i = " + to_string(i) + " Checking if Carmichael number: " + CarmichaelNumber.toString() + " is prime...");
				BigInteger::getLogger() << (CarmichaelNumber.isPrime(Fermat_efficient) ? "Result: Fermat_efficient - Prime" : "Result: Fermat_efficient - NOT Prime");
				BigInteger::getLogger() << (CarmichaelNumber.isPrime(MillerRabin_basic) ? "Result: MillerRabin_basic - Prime" : "Result: MillerRabin - NOT Prime");
				BigInteger::getLogger() << (CarmichaelNumber.isPrime(MillerRabin_usingOptimizedBases) ? "Result: MillerRabin_usingOptimizedBases - Prime" : "Result: MillerRabin - NOT Prime");
			}
		}
	}

	void testFermatPseudoPrimes()
	{

	}

	void testAbsoluteFermatPseudoPrimes()
	{

	}

	void testMillerRabinPseudoPrimes()
	{

	}

	void BigInteger_UnitTest()
	{
		cout << "\n\n~~~~~~~~~~~~~~~~~~ BigInteger_UnitTest() ~~~~~~~~~~~~~~~~~~~~~~~\n";

		//One time seed initialization
		srand((unsigned)time(0));

		//Some utility functions:
		generatePrimeNumbers(1000);

		testBigIntegerInitialization();
		testSmallNumbers();
		testVeryLargeNumbers();
		testExtraLargeNumbers();

		generateBigPrimeNumbers();
		//generateBigPrimeNumbers1024bits();
		testPredefinedPrimeNumbers();

		generateAndTestCarmichaelNumbers();
		testPredefinedCarmichaelNumbers();
		testFermatPseudoPrimes();
		testAbsoluteFermatPseudoPrimes();
		testMillerRabinPseudoPrimes();

		//BigInteger::getLogger().close();
	}


	MM_DECLARE_FLAG(BigInteger_unit_test);

	MM_UNIT_TEST(BigInteger_unit_test_1, BigInteger_unit_test)
	{
		BigInteger_UnitTest();
	}


	/*

	Exercise: 0
	Finding random prime of size 1024 bits
	Starting search for prime number. Random start picked: 152564409342158655892030353916160958471561643832741652410427911065655629791472479777203231780482243084108892653826057873979490485909871135292523125994860425225254198804432498647815003362596328543121674113918363276602881099513989506761572429529888902117998218887866982675898655232688409853430489837221657

	Trying next odd number. Trial: 865
	Divisibility Result by first 10000 primes successful! Time required : 281,560 microseconds
	FermatPrimalityTest_withSmallPrimeNumbersAsBase successful! Iterations: 20 Time required: 136,988,836 microseconds
	FermatPrimalityTest_withBiggerPrimeNumbersAsBase successful! Iterations: 20 Time required: 137,408,660 microseconds
	Trying with base: 81810537064603
	Trying with base: 379135931002314542833930
	Trying with base: 10259173081412212505711239501827383469549019
	Trying with base: 41651335170428192531486762009082261271946369748175249
	Trying with base: 678177458444702285257843475095846041216511865277980360131648004001107679
	Trying with base: 378755923991049965942660468826117798099699301115059722571797983869554487359187986
	Trying with base: 44607953271093241168402567110122146938866831656928731178095416580621865960880852197175492185801391691
	Trying with base: 77731255143683636887635287815373330700352791711333544836047251159989505888734416437714233303905009657993760111
	Trying with base: 3980635775480663787040027707661993292269226773667838006031296687931060240361948289179315659386565224679108865973589729105823164169
	Trying with base: 11601799720513407027301645721523326339245374152981214492544356764685801604499090720002510072503223420274300378855981791241665047705105209422
	Trying with base: 88116113727607723570860120638794277091812399242976292891930822220708495097672080103422300803456134873388545159611611728099874145975142987028310453615920687595
	Trying with base: 1079451380547927202894422096565104594815953549555759831863244703862163055644633045336667301901861229453416478773017405985676370393040806497407003386129423252860736381232
	Trying with base: 30439518374161715285181819705960521008604383920518159293859269109693068858377874900075498001905057631647441217366437907854525818225345744277177060170461280735811506251483468499482056859307
	Trying with base: 19750805451138615271503526450895047347570877264954932049084226476749207171058306780345507134211149279479589500095764720748135739090966347143312019198350181483990618656880234280462878947816062861516
	Trying with base: 1933710120074768916086239741356258628218317612841698744389309223553253395259149507986479330869073233324412958980365487929948948905892036627450734685663224635086109687985848097349019734497248068167103846895969376420097
	Trying with base: 4145742861652368863047062766501196502929034927264136399259507214730233901161669510883416503760418177182257500236880286099928785166661053835736694719422836855044802401863340234461425386537997271694267442870268645457695697014092
	Trying with base: 112042646542066788869797641999661809467558250199310808622537392460302987126878548819965699132757827232677089451856274116585636882592110943198825265471349121299007127806607884478199018915609673055082780990876321842586199442690716399620460197734871
	Trying with base: 253626133108702274948357103619930912933939439557527592983914269653774882847381204370943384805379851653973222481657380424505889456324001707583649737896782444114534248139021731146883663230588130408062257439130029022171167629419757021087270051773252953466859
	Trying with base: 7585252879905812325187626075480695954312708573349043295840071847757610580911849661995524611361019083430941033735303954215794557653877303630859348789107429452432608428417015958435075142076666595307667186708839728221295007230791979044789156569247680414142416563965546266101938
	Trying with base: 36027304811735652084874057398562302471863795608894700553615262035631746413310662687649292877263070141713589366006780195668556965278501897831310944088513135773000833282793179781025685019766280538633822125645602047648127376141600699049276833677645996105664860072671606115145704865268849
	FermatPrimalityTest_withRandomNumbersAsBase successful! Iterations: 20 Time required: 139,419,139 microseconds
	Found prime number after testing 865 odd numbers!
	Prime number = 152564409342158655892030353916160958471561643832741652410427911065655629791472479777203231780482243084108892653826057873979490485909871135292523125994860425225254198804432498647815003362596328543121674113918363276602881099513989506761572429529888902117998218887866982675898655232688409853430489837223387

	Time required: 1,560 sec
	Prime number = 505743691611943740568871846669576023556342307067906800317491881605208694400462062955033419712776166246168675155934694393956916965814970835280059164087168152579867248370838347514999671452326083276829487765857998378645049356805819072246825431242628168687736463088089966764502493348943125347053544144926771
	*/

}