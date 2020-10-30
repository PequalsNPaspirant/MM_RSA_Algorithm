#pragma once

#include <cassert>
#include "BigInteger/BigInteger.h"

namespace mm {

	//Implemented as class template
	template <typename Type>
	class ArithmeticOperations
	{
	public:
		static Type calculteGCD(Type a, Type b);
		static Type calculteLCM(Type a, Type b);
		static Type power(const Type& base, int exponent);

	private:
		ArithmeticOperations();
		~ArithmeticOperations();
	};

	//Implemented as non-templatized class, but individual functions are templatized
	class ModularOperations
	{
	public:
		template <typename Type>
		static Type doModularExponentiation(Type base, Type exponent, Type modulus);
		template <>
		static BigInteger ModularOperations::doModularExponentiation<BigInteger>(BigInteger base, BigInteger exponent, BigInteger modulus);
	};


	template <typename Type>
	Type ArithmeticOperations<Type>::calculteGCD(Type a, Type b)
	{
		while (a != Type{ 0 } && b != Type{ 0 })
		{
			if (a > b)
				//a = a % b;
				a %= b;
			else
				//b = b % a;
				b %= a;
		}

		return a + b;
	}

	template <typename Type>
	Type ArithmeticOperations<Type>::calculteLCM(Type a, Type b)
	{
		//Based on: a * b = gcd(a, b) * lcm(a, b)

		Type gcd = calculteGCD(a, b);
		return (a / gcd) * b; //First do division, then multiplication
	}

	//The following method is most efficient method doing least number of multiplications.
	//This is iterative method and requires same number of multiplications as in case of recursive method
	template <typename Type>
	Type ArithmeticOperations<Type>::power(const Type& base, int exponent)
	{
		Type result = (exponent % 2 == 1) ? base : 1;
		Type factor = base;
		for (exponent /= 2; exponent > 0; exponent /= 2)
		{
			factor = factor * factor;

			if (exponent % 2 == 1)
			{
				if (result == 1)
					result = factor;
				else
					result = result * factor;
			}
		}

		return result;
	}

	template <>
	inline BigInteger ModularOperations::doModularExponentiation<BigInteger>(BigInteger base, BigInteger exponent, BigInteger modulus)
	{
		return base.modularExponentiation(exponent, modulus);
	}


	template <typename Type>
	ArithmeticOperations<Type>::ArithmeticOperations()
	{
	}

	template <typename Type>
	ArithmeticOperations<Type>::~ArithmeticOperations()
	{
	}

	template <typename Type>
	Type ModularOperations::doModularExponentiation(Type base, Type exponent, Type modulus)
	{
		assert(modulus > Type{ 0 });
		if (modulus == Type{ 1 }) return Type{ 0 };
		Type result = 1;
		base = base % modulus;
		while (exponent > Type{ 0 })
		{
			if (exponent % Type{ 2 } == Type{ 1 })
				result = (result * base) % modulus;
			base = (base * base) % modulus;
			exponent = exponent >> 1;
		}

		return result;
	}

	

	double power_recursive(double base, int exponent);
	double power_iterative(double base, int exponent);
	double power1(double base, int exponent, int& numMultiplicatiions);
	double power2(double base, int exponent, int& numMultiplicatiions);
	double power3(double base, int exponent, int& numMultiplicatiions);
	double power4(double base, int exponent, int& numMultiplicatiions);
}