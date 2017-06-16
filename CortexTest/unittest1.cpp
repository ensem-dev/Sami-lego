#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CortexTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: Ici, votre code de test
            int expected;
            int actual;
            expected = 0;
            actual = 0;
            Assert::AreEqual(expected, actual);
		}

   };
}