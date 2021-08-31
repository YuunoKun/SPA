#include "pch.h"
#include <iostream>
#include "../source/Parser.h"
#include "../source/PKB.h"

namespace UnitTesting {
	// The fixture for testing class Foo.
	class ParserPKBTest : public ::testing::Test {
	protected:

		ParserPKBTest() {
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

	TEST(ParserPKBTest, Sample3TokenizerTest) {
		PKB::getInstance().resetCache();

		SourceProcessor::Parser parser;
		parser.load_file("../UnitTestingGoogle/SPTest/TestSource/Sample3.txt");
		parser.parse();
		ASSERT_EQ(PKB::getInstance().getProcedures().size(), 1);
		ASSERT_EQ(PKB::getInstance().getConstants().size(), 1);
	}

	TEST(ParserPKBTest, Sample4TokenizerTest) {
		PKB::getInstance().resetCache();
		SourceProcessor::Parser parser;
		parser.load_file("../UnitTestingGoogle/SPTest/TestSource/Sample4.txt");
		parser.parse();
		ASSERT_EQ(PKB::getInstance().getProcedures().size(), 2);
		ASSERT_EQ(PKB::getInstance().getConstants().size(), 4);
	}
} // namespace UnitTesting