#include "pch.h"
#include <iostream>
#include "../../source/Parser.h"
#include "../../source/PKB.h"

namespace UnitTesting {
	// The fixture for testing class Foo.
	class ParserTest : public ::testing::Test {
	protected:

		ParserTest() {
			// You can do set-up work for each test here.
		}

		// If the constructor and destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}

		// Class members declared here can be used by all tests in the test suite
		// for Foo.
	};

	TEST(ParserTest, Sample3TokenizerTest) {
	}

	TEST(ParserTest, Sample4TokenizerTest) {
	}
} // namespace UnitTesting