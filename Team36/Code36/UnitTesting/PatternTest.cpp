#include "stdafx.h"
#include "CppUnitTest.h"

#include "Pattern.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(PatternTest) {
public:

	TEST_CLASS_INITIALIZE(methodName) {
		// test class initialization  code
	}

	TEST_METHOD(TestPattern) {
		Assert::AreEqual("Hi", "Hi");
	}
	};
}