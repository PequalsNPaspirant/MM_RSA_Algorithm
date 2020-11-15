#pragma once

/*
This class provides all utility functions related to Prime Numbers

From RSAPaper.pdf -
To find a 100-digit random prime number, generate (odd) 100-digit random
numbers until a prime number is found. By the prime number theorem [7], about
(ln 10^100)/2 = 115 numbers will be tested before a prime is found.

*/

#include <iostream>
#include <iomanip>  //for setw
#include <cmath>
#include <cassert>
using namespace std;

#include "Maths/Maths_RandomNumber.h"
#include "Timer/Timer_Timer.h"

namespace mm {

	//PrimalityTest
	enum ePrimalityTest
	{
		BruteForcePrimalityTest = 0,

		MaxPrimalityTest
	};

	template <typename Type>
	class PrimeNumber
	{
	public:
		static Type generateRandomPrimeNumber(size_t bitCount, ePrimalityTest primalityTest = ePrimalityTest());
		static Type generateNextPrimeNumber(Type referenceNumber, ePrimalityTest primalityTest = ePrimalityTest());
		static bool checkIfPrimeNumber(Type number, ePrimalityTest primalityTest = ePrimalityTest());

		//Primality tests
		static bool deterministic_BruteForcePrimalityTest(Type number);

	private:
		PrimeNumber();
		~PrimeNumber();
	};

	//Constructor
	template <typename Type>
	PrimeNumber<Type>::PrimeNumber()
	{
	}

	//Destructor
	template <typename Type>
	PrimeNumber<Type>::~PrimeNumber()
	{
	}

	//Public Functions

	template <typename Type>
	Type PrimeNumber<Type>::generateRandomPrimeNumber(size_t bitCount, ePrimalityTest primalityTest /*= ePrimalityTest()*/)
	{
		return generateNextPrimeNumber(RandomNumber<Type>::getRandomNumber(bitCount), primalityTest);
	}

	template <typename Type>
	Type PrimeNumber<Type>::generateNextPrimeNumber(Type referenceNumber, ePrimalityTest primalityTest /*= ePrimalityTest()*/)
	{
		//Timer t;
		referenceNumber = referenceNumber % Type{ 2 } == Type{ 0 } ? referenceNumber + Type{ 1 } : referenceNumber + Type{ 2 };
		while (!checkIfPrimeNumber(referenceNumber, primalityTest))
			referenceNumber += 2;

		//cout << setw(50) << "\nPrime Number Generation Duration: " << t.getDurationStringTillNow();
		return referenceNumber;
	}

	template <typename Type>
	bool PrimeNumber<Type>::checkIfPrimeNumber(Type number, ePrimalityTest primalityTest /*= ePrimalityTest()*/)
	{
		switch (primalityTest)
		{
		case BruteForcePrimalityTest:
			return deterministic_BruteForcePrimalityTest(number);

		default:
			mm_assert(false, "Unknown PrimalityTest");
		}

		return false;
	}


	//Primality tests
	template <typename Type>
	bool PrimeNumber<Type>::deterministic_BruteForcePrimalityTest(Type number)
	{
		if (number > Type{ 1 })
		{
			if (number % Type{ 2 } == Type{ 0 })
				return false;

			long double numberDouble = static_cast<long double>(number);
			Type limit = static_cast<Type>(sqrt(numberDouble)) + Type{ 1 };
			for (Type i = Type{ 3 }; i < limit; i += Type{ 2 })
				if (number % i == Type{ 0 })
					return false;
			return true;
		}

		return false;
	}

}