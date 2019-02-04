
#include "RSA.h"

namespace mm {

	void RSAUnitTest::RSATest_usePrecalculatedPrimes()
	{
		cout << "\n\n-------------------------- RSATest_usePrecalculatedPrimes -----------------------------------";

		struct RSAInput
		{
			BigInteger::DigitType p, q, e, d, TF;
		};

		RSAInput data[] = {
			{ 2, 7, 5, 5, 6 }, // N = 2 x 7 = 14, Ø(N) = 1 x 6 = 6
			{ 2, 7, 5, 11, 6 }, // N = 14, Ø(N) = 6
			{ 2, 7, 5, 17, 6 }, // N = 14, Ø(N) = 6
			{ 5, 11, 7, 23, 40 }, // N = 55, Ø(N) = 40
			{ 89, 107, 3, 6219, 9328 }, // N = 9523, Ø(N) = 9328
			{ 61, 53, 17, 413, 3120 } // N = 3233, Ø(N) = 3120
		};

		const string message("A first very simple STRING HAVING 1234567890 and -=~!@#$%^&*()_+[]{}\\|;:'\",<.>/?");

		for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++)
		{
			{
				cout << "\n\n";
				RSA objRSAULongLong(data[i].p, data[i].q, data[i].e, data[i].d, data[i].TF);
				objRSAULongLong.testRSAComplete(message);
				objRSAULongLong.testRSAComplete(RSAUnitTest::getTestMessageHavingAllChars());
				objRSAULongLong.testRSAComplete(RSAUnitTest::getTestMessageHavingAllChars2());
			}
		}
	}

	void RSAUnitTest::TestBasics()
	{
		cout << "\n\n-------------------------- TestBasics -----------------------------------";

		const ustring strArray[] = {
			ustring(reinterpret_cast<const unsigned char*>("A first very simple STRING HAVING 1234567890 and -=~!@#$%^&*()_+[]{}\\|;:'\",<.>/?")),
			RSAUnitTest::getTestMessageHavingAllChars(),
			RSAUnitTest::getTestMessageHavingAllChars2()
			//"TODO: Go on adding more strings..."
		};

		int messageCount = sizeof(strArray) / sizeof(string);

		for (int i = 0; i < messageCount; i++)
		{
			const ustring& message = strArray[i];

			unsigned short bits = 32;
			for (int k = 1; k <= 32; ++k)
			{
				RSA objRSAULongLong(bits + k);
				objRSAULongLong.testRSAComplete(message);
			}
		}
	}

	const ustring& RSAUnitTest::getTestMessageHavingAllChars()
	{
		unsigned char chMin = std::numeric_limits<unsigned char>::min();
		unsigned char chMax = std::numeric_limits<unsigned char>::max();
		static ustring msg = ustring();
		if (msg.size() == 0)
		{
			int size = chMax - chMin + 1;
			msg.resize(size);
			unsigned char ch = chMax; //Start with max so that null char goes at end
			for (int i = 0; i < size; i++)
				msg[i] = ch--;
		}

		return msg;
	}

	const ustring& RSAUnitTest::getTestMessageHavingAllChars2()
	{
		char chMin = std::numeric_limits<char>::min();
		char chMax = std::numeric_limits<char>::max();
		static ustring msg = ustring();
		if (msg.size() == 0)
		{
			int size = chMax - chMin + 1;
			msg.resize(size);
			char ch = chMin;
			for (int i = 0; i < size; i++)
				msg[i] = ch++;
		}

		return msg;
	}

	const ustring& RSAUnitTest::getTestMessageHavingAllWideChars()
	{
		static ustring msg = ustring();
		//TODO
		return msg;
	}

	void RSA_UnitTest()
	{
		srand((unsigned)time(0));

		//RSAUnitTest::RSATest_usePrecalculatedPrimes();
		RSAUnitTest::TestBasics();
		//RSAUnitTest::RSATest_boundaryConditions();
		//RSAUnitTest::RSATestWithBigInteger();
	}

}