#include "pch.h"

#include "../source/Parser.h"

namespace UnitTesting {
	// The fixture for testing class Foo.
	class FooTest : public ::testing::Test {
	protected:

		FooTest() {
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


	TEST(ParserTest, ParserLoadsFile) {
		SourceProcessor::Parser parser;
		parser.load_file("SPTest/TestSource/Sample1.txt");
		EXPECT_EQ(parser.get_source_program(), "This is test file without any newline.");
	}

} // namespace UnitTesting