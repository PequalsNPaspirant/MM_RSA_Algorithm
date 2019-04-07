// Test.cpp : Defines the entry point for the console application.

#include <iostream>

#include "Timer/Timer_Timer.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	//Global function declarations
	void RSA_Template_UnitTest();
	void RSA_UnitTest();

	MM_DEFINE_FLAG(false, arithmeticOperations_power);
}

using namespace mm;

int main(int argc, char* argv[])
{
	MM_RUN_UNIT_TESTS

	//RSA_Template_UnitTest();
	//RSA_UnitTest();

	cout << "\n\n\n" << Timer::getCurrentLocalTimeInNanoSeconds() << " CONGRATULATIONS!!! End of program reached successfully.\n\n\n";

	std::cin.get();
	return 0;
}

