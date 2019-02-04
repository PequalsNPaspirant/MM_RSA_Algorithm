#ifndef __BIG_INTEGER_HEADER_GUARD__
#define __BIG_INTEGER_HEADER_GUARD__

#include <vector>
#include <limits>
#include <cmath>
using namespace std;

#include "Assert/MyAssert.h"
#include "my_math_lib/ArithmeticOperations.h"

/*
64 bit OS has 64 bit registers. But where the arithmetic oprtations using 64 bit data type 
will be faster on it, is a complex topic. It all depends upon processor architecture and on CPU and ALU etc.
It all boils down to:
32 bit arithmetic is at least not slower than 64 bit on most of the processors. Also considering overflow in arithmetic operations,
32 bit arithmetic can be considered most efficient for BigIntegers.

*/

namespace mm {

	template<typename TypeToHoldResultsOfArithmeticOperations>
	size_t getDefaultBase()
	{
		size_t bits = sizeof(TypeToHoldResultsOfArithmeticOperations) * 8;
		//Ideally, max digit size can be half of above to accomodate overflow in basic arithmatic operations
		return 1 << (bits / 2);
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	class BigIntegerTemplate
	{
	public:
		BigIntegerTemplate(size_t base = getDefaultBase<TypeToHoldResultsOfArithmeticOperations>());
		BigIntegerTemplate(const int& number, size_t base = getDefaultBase<TypeToHoldResultsOfArithmeticOperations>());
		BigIntegerTemplate(const long& number, size_t base = getDefaultBase<TypeToHoldResultsOfArithmeticOperations>());
		BigIntegerTemplate(const long long& number, size_t base = getDefaultBase<TypeToHoldResultsOfArithmeticOperations>());
		BigIntegerTemplate(const string& numberString, size_t stringBase = 10, size_t base = getDefaultBase<TypeToHoldResultsOfArithmeticOperations>());
		~BigIntegerTemplate();

	private:
		bool m_isPositive;
		vector<DigitType> m_digits;
		size_t m_base;

		//Other public useful intefaces
	public:
		BigIntegerTemplate(const BigIntegerTemplate& copyFrom);
		BigIntegerTemplate& operator=(const BigIntegerTemplate& assignFrom);

		//Arithmetic Operations
		void operator+=(const BigIntegerTemplate& rhs);
		template<typename DigitType2, typename TypeToHoldResultsOfArithmeticOperations2>
		friend BigIntegerTemplate<DigitType2, TypeToHoldResultsOfArithmeticOperations2> operator+(const BigIntegerTemplate<DigitType2, TypeToHoldResultsOfArithmeticOperations2>& lhs,
			const BigIntegerTemplate<DigitType2, TypeToHoldResultsOfArithmeticOperations2>& rhs);
		void operator-=(const BigIntegerTemplate& rhs);
		template<typename DigitType2, typename TypeToHoldResultsOfArithmeticOperations2>
		friend BigIntegerTemplate<DigitType2, TypeToHoldResultsOfArithmeticOperations2> operator-(const BigIntegerTemplate<DigitType2, TypeToHoldResultsOfArithmeticOperations2>& lhs,
			const BigIntegerTemplate<DigitType2, TypeToHoldResultsOfArithmeticOperations2>& rhs);
		void operator*=(const BigIntegerTemplate& rhs);
		template<typename DigitType2, typename TypeToHoldResultsOfArithmeticOperations2>
		friend BigIntegerTemplate<DigitType2, TypeToHoldResultsOfArithmeticOperations2> operator*(const BigIntegerTemplate<DigitType2, TypeToHoldResultsOfArithmeticOperations2>& lhs,
			const BigIntegerTemplate<DigitType2, TypeToHoldResultsOfArithmeticOperations2>& rhs);
		//template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations, size_t base>
		//friend bool operator<(const BigIntegerTemplate& lhs, const BigIntegerTemplate& rhs);
		//const BigIntegerTemplate absolute();

		long long covertToDecimal();

		//Print utilities
		template<typename DigitType2, typename TypeToHoldResultsOfArithmeticOperations2>
		friend ostream& operator << (ostream &out, const BigIntegerTemplate<DigitType2, TypeToHoldResultsOfArithmeticOperations2>& bigInt);
		string toString() const;

		//Other private helper functions
	private:
		DigitType getDecimalValue(const char& digitIn) const;
		char getChar(const DigitType& digitIn) const;
		void doMultiply(const vector<DigitType>& rhsVector);
		void doMultiply(DigitType rhsValue);
		void doAdd(const vector<DigitType>& rhsVector);
		void doAdd(DigitType rhsValue);
		void doSubstract(const vector<DigitType>& rhsVector);
		void doSubstractFrom(const vector<DigitType>& rhsVector);
		DigitType convertStringToDecimalNumber(const string& numberString, size_t start, size_t end, size_t baseFrom);
		void convertDecimalToBase(vector<DigitType>& digitVector, DigitType decimalNumber);
		bool isLessThan(const vector<DigitType>& rhsVector) const;
		void resize(const size_t& newSize);
		//void shiftAllDigitsTowardsLeft(size_t positions);
	};

	// ============== Constructors ==================

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::BigIntegerTemplate(size_t base /*= getDefaultBase<TypeToHoldResultsOfArithmeticOperations>()*/)
		: m_isPositive(true), m_base(base), m_digits(0) //Avoid initial memory allocation
	{
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::BigIntegerTemplate(const int& number, size_t base /*= getDefaultBase<TypeToHoldResultsOfArithmeticOperations>()*/)
		: m_isPositive(true), m_base(base), m_digits(0) //Avoid initial memory allocation
	{
		*this = BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>(static_cast<long long>(number), base);
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::BigIntegerTemplate(const long& number, size_t base /*= getDefaultBase<TypeToHoldResultsOfArithmeticOperations>()*/)
		: m_isPositive(true), m_base(base), m_digits(0) //Avoid initial memory allocation
	{
		*this = BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>(static_cast<long long>(number), base);
	}


	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::BigIntegerTemplate(const long long& number, size_t base /*= getDefaultBase<TypeToHoldResultsOfArithmeticOperations>()*/)
		: m_isPositive(true), m_base(base), m_digits(0) //Avoid initial memory allocation
	{
		long long currentNumber = number;
		if (number < 0)
		{
			m_isPositive = false;
			currentNumber = -currentNumber;
		}
		size_t numRequiredDigits = ceil(log(currentNumber + 1.0) / log(base)); //Does not fail if number = 0
		if (numRequiredDigits == 0) numRequiredDigits = 1;
		m_digits.resize(numRequiredDigits, 0);

		for (size_t i = numRequiredDigits; currentNumber > 0; i--)
		{
			m_digits[i - 1] = currentNumber % base; //Assuming DigitType can hold any number less than base, otherwise it should throw exception
			currentNumber /= base;
		}
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::BigIntegerTemplate(const string& numberString, size_t stringBase /*= 10*/, size_t base /*= getDefaultBase<TypeToHoldResultsOfArithmeticOperations>()*/)
		: m_isPositive(true), m_base(base), m_digits(0) //Avoid initial memory allocation
	{
		//Here we assume that the number digits start from 0 and it uses 0 to 9 and a to z or A to Z letters. Max base can be 32.
		//So here we can represent a number with maximum base of 36. This is same as Java BigIntegerTemplate representation.

		if (numberString.empty())
			throw "Invalid input string"; //TODO: Throw better exception

		if (stringBase > 32)
			throw "Invalid base"; //TODO: throw better exception

		size_t baseFrom = stringBase;
		size_t baseTo = m_base;

		int startIndexOfString = 0;
		int endIndexOfString = numberString.length();

		//Check if number is negative
		if (numberString[0] == '-')
		{
			m_isPositive = false;
			startIndexOfString = 1;
		}

		//Remove zeros at the left of number
		while (numberString[startIndexOfString] == '0' && startIndexOfString < endIndexOfString - 1) //Keep last zero digit
			startIndexOfString++;

		//TODO: Handle three cases differently for efficiency?
		size_t numInputDigits = numberString.length() - startIndexOfString;
		size_t numDigits = (baseFrom == baseTo) ? numInputDigits : ceil(log(baseFrom + 1.0) / log(baseTo) * numInputDigits);
		m_digits.resize(numDigits, 0);

		size_t digitsInEachGroup = 1;
		if (baseFrom == baseTo)
		{
			size_t index = 0;
			for (size_t start = startIndexOfString; start < endIndexOfString; start++)
				m_digits[index++] = getDecimalValue(numberString[start]);

		}
		else if (baseFrom < baseTo)
		{
			size_t digitsInEachGroup = numInputDigits / numDigits;
			//DigitType is big enough to store following multiplication
			DigitType multiplier = pow(baseFrom, digitsInEachGroup);

			size_t start = startIndexOfString, end;
			while (start < endIndexOfString)
			{
				if ((end = start + digitsInEachGroup) > endIndexOfString)
				{
					end = endIndexOfString;
					multiplier = pow(baseFrom, end - start);
				}

				doMultiply(multiplier);
				DigitType currentDigit = convertStringToDecimalNumber(numberString, start, end, baseFrom);
				doAdd(currentDigit);

				start = end;
			}
		}
		else // baseFrom > baseTo
		{
			size_t numRequiredDigits = ceil(log(baseFrom + 1.0) / log(baseTo)); //Does not fail if number = 0
			if (numRequiredDigits == 0) numRequiredDigits = 1;

			vector<DigitType> baseVector(numRequiredDigits);
			convertDecimalToBase(baseVector, baseFrom);
			vector<DigitType> currentDigitVector(numRequiredDigits);
			for (size_t start = startIndexOfString; start < endIndexOfString; start++)
			{
				doMultiply(baseVector);
				TypeToHoldResultsOfArithmeticOperations currentDigit = getDecimalValue(numberString[start]);
				convertDecimalToBase(currentDigitVector, currentDigit);
				doAdd(currentDigitVector);
			}
		}
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::BigIntegerTemplate(const BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>& copyFrom)
		:m_isPositive(copyFrom.m_isPositive),
		m_digits(copyFrom.m_digits),
		m_base(copyFrom.m_base)
	{
	}

	// ============== destructor ==================

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::~BigIntegerTemplate()
	{

	}

	// ============== Other Functions ==================

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>&
		BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::operator=(const BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>& assignFrom)
	{
		m_isPositive = assignFrom.m_isPositive;
		m_digits.clear();
		m_digits = assignFrom.m_digits;
		m_base = assignFrom.m_base;

		return *this;
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	ostream& operator << (ostream &out, const BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>& bigInt)
	{
		out << bigInt.toString();
		return out;
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	void BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::doMultiply(const vector<DigitType>& rhsVector)
	{
		//Caller should make sure that m_digits has enough digit places

		const vector<DigitType> copy = m_digits;
		for (size_t start = m_digits.size(); start > 0; start--)
			m_digits[start - 1] = 0;
		size_t rhsDigitPosition = 0;
		for (size_t rhsIndex = rhsVector.size(); rhsIndex > 0; rhsIndex--, rhsDigitPosition++)
		{
			DigitType carry = 0;
			for (size_t start = copy.size(); start > rhsDigitPosition; start--)
			{
				//Handle overflow
				TypeToHoldResultsOfArithmeticOperations result = copy[start - 1] * TypeToHoldResultsOfArithmeticOperations(rhsVector[rhsIndex - 1]) + carry;
				result += m_digits[start - 1 - rhsDigitPosition];
				m_digits[start - 1 - rhsDigitPosition] = result % m_base;
				carry = result / m_base;
			}
		}
	}

	/*
	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations, size_t base>
	void BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations, base>::shiftAllDigitsTowardsLeft(vector<DigitType>& digitVector, size_t positions)
	{
		//Caller should make sure that m_digits has enough digit places
		size_t start = 0;
		for (; (start + positions) < digitVector.size(); start++)
			digitVector[start] = digitVector[start + positions];

		for (; start < m_digits.size(); start++)
			digitVector[start] = 0;
	}*/

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	void BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::doMultiply(DigitType singleDigit)
	{
		//if (singleDigit == m_base)  //Single digit can not be equal to m_base
		//{
		//	shiftAllDigitsTowardsLeft(m_digits, 1);
		//	return;
		//}

		DigitType carry = 0;
		for (size_t start = m_digits.size(); start > 0; start--)
		{
			//Handle overflow
			TypeToHoldResultsOfArithmeticOperations result = m_digits[start - 1] * TypeToHoldResultsOfArithmeticOperations(singleDigit) + carry;
			m_digits[start - 1] = result % m_base;
			carry = result / m_base;
		}

		//Note: carry will never be non zero when we reach here. m_digits always has the number of elements to accomodate multiplication.
		MyAssert::myRunTimeAssert(carry == 0);
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	void BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::doAdd(const vector<DigitType>& rhsVector)
	{
		//We have already ensured that m_digits has equal or more elements than rhsVector
		DigitType carry = 0;
		DigitType rhsValue = 0;
		for (size_t lhsIndex = m_digits.size(), rhsIndex = rhsVector.size(); rhsIndex > 0 || carry != 0; lhsIndex--)
		{
			if (rhsIndex > 0)
				rhsValue = rhsVector[--rhsIndex];
			else
				rhsValue = 0;

			//Handle overflow
			TypeToHoldResultsOfArithmeticOperations result = m_digits[lhsIndex - 1] + rhsValue + TypeToHoldResultsOfArithmeticOperations(carry);
			m_digits[lhsIndex - 1] = result % m_base;
			carry = result / m_base;
		}

		//Note: carry will never be non zero when we reach here. m_digits always has the number of elements to accomodate addition.
		MyAssert::myRunTimeAssert(carry == 0);
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	void BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::doSubstract(const vector<DigitType>& rhsVector)
	{
		//Caller should ensure that absolute(*this) >= absolute(rhs)
		DigitType carry = 0;
		DigitType rhsValue = 0;
		DigitType result = 0;
		for (size_t lhsIndex = m_digits.size(), rhsIndex = rhsVector.size(); lhsIndex > 0 && (rhsIndex > 0 || carry != 0); lhsIndex--)
		{
			if (rhsIndex > 0)
				rhsValue = rhsVector[--rhsIndex];
			else
				rhsValue = 0;

			rhsValue += carry;
			if (m_digits[lhsIndex - 1] < rhsValue)
			{
				result = (m_base - rhsValue) + m_digits[lhsIndex - 1];
				carry = 1;
			}
			else
			{
				result = m_digits[lhsIndex - 1] - rhsValue;
				carry = 0;
			}

			m_digits[lhsIndex - 1] = result;
		}

		//Note: carry will never be non zero when we reach here. m_digits always has the number of elements to accomodate addition.
		MyAssert::myRunTimeAssert(carry == 0);
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	void BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::doSubstractFrom(const vector<DigitType>& rhsVector)
	{
		//Caller should ensure that absolute(*this) < absolute(rhs)
		//Swap m_digits and rhsVector and reuse doSubstract()
		vector<DigitType> copy(m_digits);
		m_digits = rhsVector;
		doSubstract(copy);
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	void BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::doAdd(DigitType singleDigit)
	{
		DigitType carry = singleDigit;
		for (size_t start = m_digits.size(); carry != 0 && start > 0; start--)
		{
			//Handle overflow
			TypeToHoldResultsOfArithmeticOperations result = m_digits[start - 1] + TypeToHoldResultsOfArithmeticOperations(carry);
			m_digits[start - 1] = result % m_base;
			carry = result / m_base;
		}

		//Note: carry will never be non zero when we reach here. m_digits always has the number of elements to accomodate addition.
		MyAssert::myRunTimeAssert(carry == 0);
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	void BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::convertDecimalToBase(vector<DigitType>& digitVector, DigitType decimalNumber)
	{
		DigitType currentNumber = decimalNumber;
		size_t i = digitVector.size();;
		for (; currentNumber > 0; i--)
		{
			digitVector[i - 1] = currentNumber % m_base; //Assuming DigitType can hold any number less than m_base, otherwise it should throw exception
			currentNumber /= m_base;
		}

		//Fill up zeros if any remaining objects in vector
		for (; i > 0; i--)
			digitVector[i - 1] = 0;
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	DigitType BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::getDecimalValue(const char& digitIn) const
	{
		/*
		ASCII codes:
		'0' to '9' : 48 to 57
		'A' to 'Z' : 65 to 90
		'a' to 'z' : 97 to 122
		*/
		DigitType currentUnitPlace = 0;
		if (digitIn <= '9')
			currentUnitPlace = digitIn - '0';
		else if (digitIn <= 'Z')
			currentUnitPlace = digitIn - 'A' + 10;
		else if (digitIn <= 'z')
			currentUnitPlace = digitIn - 'a' + 10;

		return currentUnitPlace;
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	DigitType BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::convertStringToDecimalNumber(const string& numberString, size_t start, size_t end, size_t baseFrom)
	{
		//Convert string to decimal number and then to a number in current base
		//We have already ensured that the range start to end of base "baseFrom" fits into DigitType of base "base"
		DigitType result = 0;
		for (; start < end; start++)
		{
			DigitType currentDigitValue = getDecimalValue(numberString[start]);
			result = result * baseFrom + currentDigitValue;
		}

		return result;
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	char BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::getChar(const DigitType& digitIn) const
	{
		char currentUnitPlace = 0;
		if (digitIn < 10)
			currentUnitPlace = '0' + digitIn;
		else
			currentUnitPlace = 'A' + digitIn - 10;

		return currentUnitPlace;
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	string BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::toString() const
	{
		size_t strLength = m_digits.size() + (m_isPositive ? 0 : 1);
		string retVal(strLength, 0);
		size_t index = 0;
		if (!m_isPositive)
			retVal[index++] = '-';
		bool foundNonZeroDigit = false;
		for (size_t start = 0; start < m_digits.size(); start++)
		{
			if (!foundNonZeroDigit && m_digits[start] == 0)
				continue;
			else
				foundNonZeroDigit = true;
			retVal[index++] = getChar(m_digits[start]);
		}

		if (!foundNonZeroDigit)
			retVal[index++] = '0';

		return retVal.substr(0, index);
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	bool BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::isLessThan(const vector<DigitType>& rhsVector) const
	{
		size_t thisSize = m_digits.size();
		size_t rhsSize = rhsVector.size();
		size_t thisStart = 0;
		size_t rhsStart = 0;

		if (thisSize > rhsSize)
		{
			for (; thisStart < thisSize - rhsSize; thisStart++)
				if (m_digits[thisStart] == 0)
					continue;
				else
					return false;
		}
		else if (thisSize < rhsSize)
		{
			for (; rhsStart < rhsSize - thisSize; rhsStart++)
				if (rhsVector[rhsStart] == 0)
					continue;
				else
					return true;
		}

		for (; thisStart < thisSize && rhsStart < rhsSize; thisStart++, rhsStart++)
			if (m_digits[thisStart] == rhsVector[rhsStart])
				continue;
			else
				return m_digits[thisStart] < rhsVector[rhsStart];
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	void BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::resize(const size_t& newSize)
	{
		vector<DigitType> copy(m_digits);
		size_t copySize = copy.size();
		m_digits.resize(newSize);
		size_t index = newSize;
		for (size_t copyIndex = copySize; index > 0 && copyIndex > 0; index--, copyIndex--)
			m_digits[index - 1] = copy[copyIndex - 1];
		for (; index > 0; index--)
			m_digits[index - 1] = 0;
	}


	//======================= Aithmetic Operations =======================

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	void BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::operator+=(const BigIntegerTemplate& rhs)
	{
		//Addition may need more digits, so allocate 1 extra digit
		resize(m_digits.size() > rhs.m_digits.size() ? m_digits.size() + 1 : rhs.m_digits.size() + 1);

		if (m_isPositive == rhs.m_isPositive)
			doAdd(rhs.m_digits);
		else if (isLessThan(rhs.m_digits))
		{
			m_isPositive = rhs.m_isPositive;
			doSubstractFrom(rhs.m_digits);
		}
		else
			doSubstract(rhs.m_digits);
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>
		operator+(const BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>& lhs,
			const BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>& rhs)
	{
		BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations> result(lhs);
		result += rhs;
		return result;
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	void BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::operator-=(const BigIntegerTemplate& rhs)
	{
		//Addition may need more digits, so allocate 1 extra digit
		resize(m_digits.size() > rhs.m_digits.size() ? m_digits.size() + 1 : rhs.m_digits.size() + 1);

		if (m_isPositive != rhs.m_isPositive)
			doAdd(rhs.m_digits);
		else if (isLessThan(rhs.m_digits))
		{
			m_isPositive = !rhs.m_isPositive;
			doSubstractFrom(rhs.m_digits);
		}
		else
			doSubstract(rhs.m_digits);
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>
		operator-(const BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>& lhs,
			const BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>& rhs)
	{
		BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations> result(lhs);
		result -= rhs;

		return result;
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	void BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::operator*=(const BigIntegerTemplate& rhs)
	{
		resize(m_digits.size() + rhs.m_digits.size());

		if (m_isPositive == rhs.m_isPositive)
			m_isPositive = true;
		else
			m_isPositive = false;

		doMultiply(rhs.m_digits);
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>
		operator*(const BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>& lhs,
			const BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>& rhs)
	{
		BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations> result(lhs);
		result *= rhs;
		return result;
	}

	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations>
	long long BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations>::covertToDecimal()
	{
		long long result = 0;
		for (size_t i = 0; i < m_digits.size(); i++)
			result = result * m_base + m_digits[i];

		if (!m_isPositive)
			result = -result;

		return result;
	}






	/*
	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations, size_t base>
	bool operator<(const BigIntegerTemplate<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations, size_t base>& lhs,
		const BigIntegerTemplate<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations, size_t base>& rhs)
	{

	}
	*/
	/*
	template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations, size_t base>
	const BigIntegerTemplate<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations, size_t base>
	BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations, base>::absolute(
		const BigIntegerTemplate<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations, size_t base>& number)
	{
		return BigIntegerTemplate(number.m_digits);
	}
	*/

	// Warning: Specify numeric limits after class definition. Only the class declaration is not enough.
	//Specify numeric limit
	//namespace std {

	/*
		using ::BigIntegerTemplate; //Another way
		template<>
		template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations, size_t base>
		class numeric_limits<BigIntegerTemplate<DigitType, TypeToHoldResultsOfArithmeticOperations, base>> {
		public:
			static BigIntegerTemplate min() { return BigIntegerTemplate(); };
			static BigIntegerTemplate max() { return BigIntegerTemplate(); };
		};
	*/

	/*
		typedef ::BigIntegerTemplate GlobalBigInteger;
		template<> class numeric_limits<GlobalBigInteger> {
		public:
			template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations, size_t base>
			static GlobalBigInteger min() { return GlobalBigInteger(); };
			template<typename DigitType, typename TypeToHoldResultsOfArithmeticOperations, size_t base>
			static GlobalBigInteger max() { return GlobalBigInteger(); };
			//static GlobalBigInteger lowest() { return GlobalBigInteger(); };
		};
	*/
	//}


}

#endif //__BIG_INTEGER_HEADER_GUARD__