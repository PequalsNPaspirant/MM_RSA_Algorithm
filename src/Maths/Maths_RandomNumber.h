#pragma once

#include <cstdlib>
#include <cmath>
#include <random>
using namespace std;

namespace mm {

	template <typename Type>
	class RandomNumber
	{
	public:
		static Type getRandomNumber(size_t bitCount = 8);
		static Type getnDigitRandomNumber(size_t numDigits);

	private:
		RandomNumber();
		~RandomNumber();
	};

	template <typename Type>
	RandomNumber<Type>::RandomNumber()
	{
	}


	template <typename Type>
	RandomNumber<Type>::~RandomNumber()
	{
	}

	template <typename Type>
	Type RandomNumber<Type>::getRandomNumber(size_t bitCount /*= 8*/)
	{
		//unsigned short numberOfDigits = m_securityBitLength * log10(2.0);
		//return (rand() % Type(pow(10, numberOfDigits)));

		//Type num = Type{ rand() %  static_cast<unsigned long long>(pow(2, bitCount)) };
		//return num + static_cast<Type>( 1ULL << (bitCount - 1) );  //Ensure that leftmost bit is always on so that the number has digits equal to "bitCount" (in base 2)
		
		std::random_device rd;     //Get a random seed from the OS entropy device, or whatever
		std::mt19937_64 eng(rd()); //Use the 64-bit Mersenne Twister 19937 generator
								   //and seed it with entropy.

								   //Define the distribution, by default it goes from 0 to MAX(unsigned long long)
								   //or what have you.
		std::uniform_int_distribution<unsigned long long> distr;

		Type retVal = static_cast<Type>(distr(eng));
		for(unsigned int i = sizeof(size_t) * 8; i <= bitCount; i += sizeof(size_t) * 8)
			retVal += static_cast<Type>(distr(eng));
		return retVal;
	}

	template <typename Type>
	Type RandomNumber<Type>::getnDigitRandomNumber(size_t numDigits)
	{
		return (rand() % Type(pow(10, numDigits))) + Type(pow(10, numDigits));  //Ensure than it has numDigits
	}

}