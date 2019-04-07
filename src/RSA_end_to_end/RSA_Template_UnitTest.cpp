
#include "RSA_Template.h"

namespace mm {

	void RSATemplateUnitTest::TestAllDataTypes()
	{
		cout << "\n\n-------------------------- TestAllDataTypes -----------------------------------";

		const string message("A first very simple STRING HAVING 1234567890 and -=~!@#$%^&*()_+[]{}\\|;:'\",<.>/?");
		RSA_Template<unsigned int> objRSAUInt(8);
		objRSAUInt.initialize();
		objRSAUInt.testRSAComplete(message);

		RSA_Template<unsigned long> objRSAULong(8);
		objRSAULong.initialize();
		objRSAULong.testRSAComplete(message);

		RSA_Template<unsigned long long> objRSAULongLong(16);
		objRSAULongLong.initialize();
		objRSAULongLong.testRSAComplete(message);
		objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars());
		objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars2());
	}


	void RSATemplateUnitTest::TestAllAlgorithmsOfRSA()
	{
		cout << "\n\n-------------------------- TestAllAlgorithmsOfRSA -----------------------------------";

		const ustring strArray[] = {
			ustring(reinterpret_cast<const unsigned char*>("A first very simple STRING HAVING 1234567890 and -=~!@#$%^&*()_+[]{}\\|;:'\",<.>/?")),
			RSATemplateUnitTest::getTestMessageHavingAllChars(),
			RSATemplateUnitTest::getTestMessageHavingAllChars2()
			//"TODO: Go on adding more strings..."
		};

		int messageCount = sizeof(strArray) / sizeof(string);

		for (int i = 0; i < messageCount; i++)
		{
			const ustring& message = strArray[i];
			for (int TFType = 0; TFType < RSAMethodInitializer::MaxTotientFunctionType; TFType++)
				for (int eCalcMethod = 0; eCalcMethod < RSAMethodInitializer::MaxPrivateExponentCalculationMethod; eCalcMethod++)
				{
					RSAMethodInitializer initializer = RSAMethodInitializer(RSAMethodInitializer::eTotientFunctionType(TFType),
						RSAMethodInitializer::ePrivateExponentCalculationMethod(eCalcMethod));

					cout << "\n\nTotientFunctionType              : " << RSAMethodInitializer::eTotientFunctionType(TFType);
					cout << "\nPrivateExponentCalculationMethod : " << RSAMethodInitializer::ePrivateExponentCalculationMethod(eCalcMethod);

					RSA_Template<unsigned long long> objRSAULongLong(16);
					objRSAULongLong.initialize(initializer);
					objRSAULongLong.testRSAComplete(message);
				}
		}
	}

	void RSATemplateUnitTest::RSATest_usePrecalculatedPrimes()
	{
		cout << "\n\n-------------------------- RSATest_usePrecalculatedPrimes -----------------------------------";

		struct RSAInput
		{
			unsigned long long p, q, e, d;
		};

		RSAInput data[] = {
			{ 2, 7, 5, 5 }, // N = 2 x 7 = 14, Ø(N) = 1 x 6 = 6
			{ 2, 7, 5, 11 }, // N = 14, Ø(N) = 6
			{ 2, 7, 5, 17 }, // N = 14, Ø(N) = 6
			{ 5, 11, 7, 23 }, // N = 55, Ø(N) = 40
			{ 89, 107, 3, 6219 }, // N = 9523, Ø(N) = 9328
			{ 61, 53, 17, 413 } // N = 3233, Ø(N) = 3120
		};

		const string message("A first very simple STRING HAVING 1234567890 and -=~!@#$%^&*()_+[]{}\\|;:'\",<.>/?");

		for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++)
		{
			{
				cout << "\n\n";
				RSA_Template<unsigned long long> objRSAULongLong(data[i].p, data[i].q, data[i].e, data[i].d);
				objRSAULongLong.initialize();
				objRSAULongLong.testRSAComplete(message);
				objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars());
				objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars2());
			}
		}
	}

	void RSATemplateUnitTest::RSATest_boundaryConditions()
	{
		cout << "\n\n-------------------------- RSATest_boundaryConditions -----------------------------------";

		{//ERROR: Original and Decrypted strings are different
			//Since, in this case, RSA works purely on Euler's Totient Function, M should be co-prime with N, 
			// otherwise the decryption can not get us the original message back.
			//p and q are prime numbers                       ...condition 1    It helps to calculate Ø(N) easily.Ø(N) = (p - 1)(q - 1)
			cout << "\n\n\nWhat if p and q are not prime\n";

			unsigned long long p = 100, q = 276;
			//Calculate TF
			/*
			From Euler's product formula: Ø(N) = N (1 - 1/p1) (1 - 1/p2) ... (1 - 1/pi)
			i.e. in another form: Ø(N) = p1^(k1-1) (p1-1) * p2^(k2-1) (p2-1) * ...
			Prime factors of 100 = 2 * 2 * 5 * 5 = 2^2 * 5^2
			Prime factors of 276 = 2 * 2 * 3 * 23 = 2^2 * 3 * 23
			100 * 276 = (2^2 * 5^2) * (2^2 * 3 * 23) = 2^4 * 3 * 5^2 * 23
			Ø(100 * 276) = Ø( 2^4 * 3 * 5^2 * 23 )
						 = 2^3 (2-1) * 3^0 (3-1) * 5^1(5-1) * 23^0(23-1)
						 = 8 * 1 * 1 * 2 * 5 * 4 * 1 * 22
			*/
			unsigned long long TF = 8 * 1 * 1 * 2 * 5 * 4 * 1 * 22; //TF = 7,040
			//Choose e. e should be co-prime with Ø(N), otherwise we can not calculate integer d
			// N = p * q = 27,600

			RSA_Template<unsigned long long> objRSAULongLong(p, q, TF);

			if (objRSAULongLong.generatePublicExponent()) //e = 3
			{
				if (objRSAULongLong.generatePrivateExponent()) //d = 2,347
				{
					string message("A first very simple STRING HAVING 1234567890 and -=~!@#$%^&*()_+[]{}\\|;:'\",<.>/?");
					objRSAULongLong.testRSAComplete(message);
					objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars());
					objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars2());

					cout << "\n\nTest Values co-prime only with p = 100 = 2 * 2 * 5 * 5";
					vector<unsigned long long> messageValueCoPrimeWithOnlyP({ 1, 2, 4, 8, 16, 5, 25, 125, 10, 20, 30, 100, 200, 300 });
					objRSAULongLong.testRSAComplete(messageValueCoPrimeWithOnlyP);

					cout << "\n\nTest Values co-prime only with q = 276 = 2 * 2 * 3 * 23";
					vector<unsigned long long> messageValueCoPrimeWithOnlyQ({ 1, 2, 4, 8, 16, 3, 9, 27, 23, 46, 69, 6, 12, 18, 276, 552, 828 });
					objRSAULongLong.testRSAComplete(messageValueCoPrimeWithOnlyQ);

					cout << "\n\nTest Values co-prime with both p and q";
					vector<unsigned long long> messageValueCoPrimeWithBothPAndQ({ 1, 2, 4, 8, 16, 10, 20, 30, 15, 30, 45, 23, 46, 115, 230 });
					objRSAULongLong.testRSAComplete(messageValueCoPrimeWithBothPAndQ);

					cout << "\n\nTest Values NOT co-prime with p and q";
					vector<unsigned long long> messageValueNotCoPrimeWithPAndQ({ 0, 7, 11, 13, 17, 19, 29, 31, 37, 77, 91, 119 });
					objRSAULongLong.testRSAComplete(messageValueNotCoPrimeWithPAndQ);

				}
				else
					cout << "\nERROR: Unable to calculate private exponent for given situation";
			}
			else
				cout << "\nERROR: Unable to calculate public exponent for given situation";
		}

		{//
			//p != q                                          ...condition 2
			//In this case RSA works purely on Euler's Totient Function
			cout << "\n\nWhat if p == q";
			{//ERROR: Original and Decrypted strings are different
				//In this case if M = 71 or multiples, it is not coprime with N, so it does not work
				unsigned long long p = 71, q = 71;

				// From Euler's product formula: Ø(N) = N (1 - 1/p1) (1 - 1/p2) ... (1 - 1/pi)
				// Ø(N) = Ø(p * q) = Ø(71 ^ 2) = N * (1 - 1/p) = N - N/p = N - p   ..since N = p^2
				unsigned long long N = p * q; //N = 5,041
				unsigned long long TF = N - p; //TF = 4,970 
				RSA_Template<unsigned long long> objRSAULongLong(p, q, TF);

				if (objRSAULongLong.generatePublicExponent()) //e = 3
				{
					if (objRSAULongLong.generatePrivateExponent()) //d = 1,657
					{
						string message("A first very simple STRING HAVING 1234567890 and -=~!@#$%^&*()_+[]{}\\|;:'\",<.>/?");
						objRSAULongLong.testRSAComplete(message);
						objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars());
						objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars2());

						cout << "\n\nTest Values co-prime with p = 71 or q = 71";
						vector<unsigned long long> messageValueCoPrimeWithOnlyP({ 1, 71, 71 * 2, 71 * 3 });
						objRSAULongLong.testRSAComplete(messageValueCoPrimeWithOnlyP);

						cout << "\n\nTest Values NOT co-prime with p and q";
						vector<unsigned long long> messageValueNotCoPrimeWithPAndQ({ 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
						objRSAULongLong.testRSAComplete(messageValueNotCoPrimeWithPAndQ);
					}
					else
						cout << "\nERROR: Unable to calculate private exponent for given situation";
				}
				else
					cout << "\nERROR: Unable to calculate public exponent for given situation";
			}
			{//SUCCESS: Original and Decrypted strings are same!
				unsigned long long p = 601, q = 601;
				unsigned long long N = p * q; //N = 5,041

				// From Euler's product formula: Ø(N) = N (1 - 1/p1) (1 - 1/p2) ... (1 - 1/pi)
				// Ø(N) = Ø(p * q) = Ø(71 ^ 2) = N * (1 - 1/p) = N - N/p = N - p   ..since N = p^2
				unsigned long long TF = N - p; //TF = 4,970 
				RSA_Template<unsigned long long> objRSAULongLong(p, q, TF);

				if (objRSAULongLong.generatePublicExponent()) //e = 3
				{
					if (objRSAULongLong.generatePrivateExponent()) //d = 1,657
					{
						string message("A first very simple STRING HAVING 1234567890 and -=~!@#$%^&*()_+[]{}\\|;:'\",<.>/?");
						objRSAULongLong.testRSAComplete(message);
						objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars());
						objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars2());

						cout << "\n\nTest Values co-prime with p = 601 or q = 601";
						vector<unsigned long long> messageValueCoPrimeWithOnlyP({ 1, 601, 601 * 2, 601 * 3 });
						objRSAULongLong.testRSAComplete(messageValueCoPrimeWithOnlyP);

						cout << "\n\nTest Values NOT co-prime with p and q";
						vector<unsigned long long> messageValueNotCoPrimeWithPAndQ({ 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });
						objRSAULongLong.testRSAComplete(messageValueNotCoPrimeWithPAndQ);
					}
					else
						cout << "\nERROR: Unable to calculate private exponent for given situation";
				}
				else
					cout << "\nERROR: Unable to calculate public exponent for given situation";
			}
		}

		{//SUCCESS: Original and Decrypted strings are same!
			// 1 < e < Ø(N)                                 ...condition 3
			cout << "\n\n\nWhat if e == 1";
			/*
			What if e == 1
			Encryption : C = (M ^ e) mod N = M   ...will not encrypt message
			de mod Ø(N) = 1
			d mod Ø(N) = 1
			d = k * Ø(N) + 1
			Decryption: M = (C ^ d) mod N
			(C ^ d) mod N
			= ( M ^ (k * Ø(N) + 1)) mod N
			= M ^ (k * Ø(N)) mod N    *     M mod N
			= M ^ Ø(N) mod N    *    M mod N
			= 1     *    M mod N
			=  M mod N

			Conclusion : Does not work
			But, encryption results into original message and decryption will also result into original message back
			*/

			unsigned long long p = 89, q = 107, /*e = 3, d = 6219,*/ e = 1, d = 0, N = 9523, /*Ø(N)*/ TF = 9328;
			RSA_Template<unsigned long long> objRSAULongLong(p, q, e, d, TF);

			if (objRSAULongLong.generatePrivateExponent()) // d = 
			{
				string message("A first very simple STRING HAVING 1234567890 and -=~!@#$%^&*()_+[]{}\\|;:'\",<.>/?");
				objRSAULongLong.testRSAComplete(message);
				objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars());
				objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars2());
			}
			else
				cout << "\nERROR: Unable to calculate private exponent for given situation";
		}

		{
			// 1 < e < Ø(N)                                 ...condition 3
			{//ERROR: Unable to calculate private exponent for given situation
				cout << "\n\n\nWhat if e == Ø(N)";
				unsigned long long p = 89, q = 107, /*e = 3, d = 6219,*/ d = 0, N = 9523, /*Ø(N)*/ TF = 9328;
				unsigned long long e = TF;
				RSA_Template<unsigned long long> objRSAULongLong(p, q, e, d, TF);
				if (objRSAULongLong.generatePrivateExponent(2 * N)) // d = 
				{
					assert(false, "We should never reach here!");
				}
				else
					cout << "\nERROR: Unable to calculate private exponent for given situation";
			}
			{//SUCCESS: Original and Decrypted strings are same!
				cout << "\n\n\nWhat if e == N";
				unsigned long long p = 89, q = 107, /*e = 3, d = 6219,*/ d = 0, N = 9523, /*Ø(N)*/ TF = 9328;
				unsigned long long e = N;
				RSA_Template<unsigned long long> objRSAULongLong(p, q, e, d, TF);
				if (objRSAULongLong.generatePrivateExponent())
				{
					string message("A first very simple STRING HAVING 1234567890 and -=~!@#$%^&*()_+[]{}\\|;:'\",<.>/?");
					objRSAULongLong.testRSAComplete(message);
					objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars());
					objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars2());
				}
				else
					cout << "\nERROR: Unable to calculate private exponent for given situation";
			}
		}

		{
			// 1 < e < Ø(N)                                 ...condition 3
			{//SUCCESS: Original and Decrypted strings are same!
				cout << "\n\n\nWhat if e > Ø(N)";
				unsigned long long p = 89, q = 107, /*e = 3, d = 6219,*/ d = 0, N = 9523, /*Ø(N)*/ TF = 9328;
				unsigned long long e = 2 * TF + 1;  // e should be co-prime with TF
				RSA_Template<unsigned long long> objRSAULongLong(p, q, e, d, TF);

				if (objRSAULongLong.generatePrivateExponent()) // d = 
				{
					string message("A first very simple STRING HAVING 1234567890 and -=~!@#$%^&*()_+[]{}\\|;:'\",<.>/?");
					objRSAULongLong.testRSAComplete(message);
					objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars());
					objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars2());
				}
				else
					cout << "\nERROR: Unable to calculate private exponent for given situation";
			}
			{//SUCCESS: Original and Decrypted strings are same!
				cout << "\n\n\nWhat if e > N";
				unsigned long long p = 89, q = 107, /*e = 3, d = 6219,*/ d = 0, N = 9523, /*Ø(N)*/ TF = 9328;
				unsigned long long e = 2 * N + 1;
				RSA_Template<unsigned long long> objRSAULongLong(p, q, e, d, TF);

				if (objRSAULongLong.generatePrivateExponent()) // d = 
				{
					string message("A first very simple STRING HAVING 1234567890 and -=~!@#$%^&*()_+[]{}\\|;:'\",<.>/?");
					objRSAULongLong.testRSAComplete(message);
					objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars());
					objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars2());
				}
				else
					cout << "\nERROR: Unable to calculate private exponent for given situation";
			}
		}

		{//ERROR: Unable to calculate private exponent for given situation
			//	e is co - prime with Ø(N)                      ...condition 4
			cout << "\n\n\nWhat if e is not co - prime with Ø(N)";
			/*
			What if e is not co - prime with Ø(N) ?
			de mod Ø(N) = 1
			de = kØ(N) + 1          ...where k is integer k >= 1
			Lets say e and Ø(N) are not co - prime, i.e.they have a common factor say x and x > 1
			so let e = x * a        ...where x, a are integers, a > 1
			and Ø(N) = x * b        ...where x, b are integers, b > 1
			d * xa = k * xb + 1
			x * (da - kb) = 1
			We can not achieve unity at left side unless x = 1
			Same is true with d
			Conclusion : Does not work

			Not possible to get integer values of d and e such that de mod Ø(N) = 1
			*/

			unsigned long long p = 5, q = 11, e = 4, d = 0, N = 55, /*Ø(N)*/ TF = 40;
			RSA_Template<unsigned long long> objRSAULongLong(p, q, e, d, TF);
			if (objRSAULongLong.generatePrivateExponent(2 * N)) // d = 
			{
				assert(false, "We should never reach here!");
			}
			else
				cout << "\nERROR: Unable to calculate private exponent for given situation";
		}

		{//SUCCESS: Original and Decrypted strings are same!
			//(e*d) mod Ø(N) = 1                              ...condition 5
			cout << "\n\nWhat if e and d do not satisfy (e*d) mod Ø(N) = 1";
			unsigned long long p = 7, q = 19, e = 5, d = 11;
			unsigned long long N = p * q; // N = 133 
			unsigned long long TF = 6 * 18; // Ø(N) = 108 
			//Above set of values do not satisfy ed mod Ø(N) = 1
			//But if we calculate TF as λ(N) = lcm(6, 18) = 18 which satisfies ed mod λ(N) = 1
			//Thus this works because RSA works for any multiple of λ(N) as TF, as long as ed mod λ(N) = 1

			RSA_Template<unsigned long long> objRSAULongLong(p, q, e, d, TF);

			vector<unsigned long long> messageValues(133);
			for (unsigned long long i = 0; i < 133; i++)
				messageValues.push_back(i);

			objRSAULongLong.testRSAComplete(messageValues);

			unsigned long long TFOrig = ArithmeticOperations<unsigned long long>::calculteLCM(p - 1, q - 1);
			//TODO: Test RSA for same p and q but different values of TF = λ(N), 2*λ(N), 3*λ(N), ... etc
			for (int i = 1; i < 5; i++)
			{
				TF = i * TFOrig;
				RSA_Template<unsigned long long> objRSAULongLong(p, q, TF);
				if (objRSAULongLong.generatePublicExponent())
				{
					if (objRSAULongLong.generatePrivateExponent())
					{
						objRSAULongLong.testRSAComplete(messageValues);
					}
					else
						cout << "\nERROR: Unable to calculate private exponent for given situation";
				}
				else
					cout << "\nERROR: Unable to calculate public exponent for given situation";
			}
		}

		{//SUCCESS: Original and Decrypted strings are same!
			/*
			In this case, RSA works purely on Fermat's little theorem
			This wont work if M is divisible by both p and q, i.e. if its divisible by N,
			but in this case, it will be greater than N, which is already ruled out
			Encryption decryption works if M is multiple of only p or q because
			Fermat's little theorem fails for one of them, but succeeds for other value
			This works on fact:
			a mod x = b and a mod y = b then a mod xy = b
			*/

			// M should be co - prime with N i.e with p and q    ...condition 6   ...from Euler's theorem (see proof 2 below)
			// M should not be multiple of p                   ...same as condition 6  ...from Fermat's little theorem (see proof 1 below)

			cout << "\n\n\nWhat if M is not co-prime with N";
			/*
			What if M is not co-prime with N?
			Euler's theorem (M ^ Ø(N)) mod N = 1 doesnt hold true
			M is not co-prime with N only if M = p or M = q
			Lets say M = p
			(M ^ Ø(N)) mod N = 1
			p ^ (p-1)(q-1) = kpq + 1
			p (p ^ ((p-1)(q-1)-1) - kq) = 1
			Above is not possible for any integer value of p
			Conclusion: Does not work
			*/

			unsigned long long p = 5, q = 11, e = 7, d = 23; // N = 55, Ø(N) = 40
			RSA_Template<unsigned long long> objRSAULongLong(p, q, e, d);
			if (objRSAULongLong.generateTotientFunction())
			{
				vector<unsigned long long> messageValues({ 0, 1, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 11, 22, 33, 44 });
				objRSAULongLong.testRSAComplete(messageValues);
			}
			else
				cout << "\nERROR: Unable to generate totient function for given situation";
		}

		{//ERROR: Original and Decrypted strings are different
			// M < N                                           ...condition 7

			cout << "\n\n\nWhat if M > N OR M == N";
			/*
			What if M > N
			Encryption : C = (M ^ e) mod N    ...will always give results < N
			Decryption : M = (C ^ d) mod N    ...will also give results < N
			To make it work, add(M - N) to decrypted values
			Conclusion : works with following changes
			Encryption : C = (M ^ e) mod N
			Decryption : M = ((C ^ d) mod N) + (M - N)
			But remembering(M - N) for every character to be encrypted is practically not possible
			Conclusion : Does not work

			What if M == N
			Encryption : C = (M ^ e) mod N    ...will result into 0
			Decryption : M = (C ^ d) mod N    ...will result into 0
			Conclusion : works with following changes
			Encryption : C = (M ^ e) mod N
			Decryption : M = ((C ^ d) mod N) + (M - N)
			But remembering(M - N) for every character to be encrypted is practically not possible
			Conclusion : Does not work
			*/

			{
				unsigned long long p = 2, q = 7, e = 5, d = 5; //N = 2 x 7 = 14, Ø(N) = 1 x 6 = 6
				char ch[6];
				ch[0] = char(12);
				ch[1] = char(13);
				ch[2] = char(14);
				ch[3] = char(15);
				ch[4] = char(16);
				ch[5] = 0;
				RSA_Template<unsigned long long> objRSAULongLong(p, q, e, d);
				if (objRSAULongLong.generateTotientFunction())
				{
					objRSAULongLong.testRSAComplete(ch);
				}
				else
					cout << "\nERROR: Unable to generate totient function for given situation";
			}
			{
				unsigned long long p = 5, q = 11, e = 7, d = 23; //N = 55, Ø(N) = 40
				char ch[6];
				ch[0] = char(53);
				ch[1] = char(54);
				ch[2] = char(55);
				ch[3] = char(56);
				ch[4] = char(57);
				ch[5] = 0;
				RSA_Template<unsigned long long> objRSAULongLong(p, q, e, d);
				if (objRSAULongLong.generateTotientFunction())
				{
					objRSAULongLong.testRSAComplete(ch);
				}
				else
					cout << "\nERROR: Unable to generate totient function for given situation";
			}
		}

		{
			cout << "\nTest RSA for same p and q but different e and d";
			unsigned long long p = 89, q = 107; // e = 3, d = 6219, N = 9523, Ø(N) = 9328
			RSA_Template<unsigned long long> objRSAULongLong(p, q);
			if (objRSAULongLong.generateTotientFunction())
			{
				unsigned long long startWith = 1;
				for (int i = 0; i < 5; i++)
				{
					if (objRSAULongLong.generatePublicExponent(startWith + 1))
					{
						startWith = objRSAULongLong.getPublicExponent();
						if (objRSAULongLong.generatePrivateExponent(2 * p * q))
						{
							objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars());
							objRSAULongLong.testRSAComplete(RSATemplateUnitTest::getTestMessageHavingAllChars2());
						}
						else
							cout << "\nERROR: Unable to calculate private exponent for given situation";
					}
					else
						cout << "\nERROR: Unable to calculate public exponent for given situation";
				}
			}
			else
				cout << "\nERROR: Unable to calculate Totient Function for given situation";
		}
	}


	const string& RSATemplateUnitTest::getTestMessage()
	{
		static const string msg("A first very simple STRING HAVING 1234567890 and -=~!@#$%^&*()_+[]{}\\|;:'\",<.>/?");
		return msg;
	}

	const ustring& RSATemplateUnitTest::getTestMessageHavingAllChars()
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

	const ustring& RSATemplateUnitTest::getTestMessageHavingAllChars2()
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

	const ustring& RSATemplateUnitTest::getTestMessageHavingAllWideChars()
	{
		static ustring msg = ustring();
		//TODO
		return msg;
	}

	void RSA_Template_UnitTest()
	{
		srand((unsigned)time(0));

		RSATemplateUnitTest::TestAllDataTypes();
		RSATemplateUnitTest::TestAllAlgorithmsOfRSA();
		RSATemplateUnitTest::RSATest_usePrecalculatedPrimes();
		RSATemplateUnitTest::RSATest_boundaryConditions();
	}

}