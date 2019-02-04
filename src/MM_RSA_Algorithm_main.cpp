// Test.cpp : Defines the entry point for the console application.

#include <iostream>

#include "WindowsUtils/WindowsUtils.h"
#include "Timer/Timer.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	//Global function declarations
	void RSA_Template_UnitTest();
	void RSA_UnitTest();
}

using namespace mm;

int main(int argc, char* argv[])
{
	MaximizeWindow();

	MM_RUN_UNIT_TESTS

	//RSA_Template_UnitTest();
	//RSA_UnitTest();

	cout << "\n\n\n" << Timer::getCurrentLocalTimeInNanoSeconds() << " CONGRATULATIONS!!! End of program reached successfully.\n\n\n";

	std::cin.get();
	return 0;
}

