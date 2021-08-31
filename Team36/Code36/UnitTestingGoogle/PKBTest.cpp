#include "pch.h"

#include "PKB.h"
#include "PKB.cpp"
#include "Common.h"
#include <iostream>

namespace UnitTesting {
	class PKBTest : public ::testing::Test {
	protected:
		PKBTest() {
		}

		// If the constructor and destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		virtual void SetUp() override {
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
	TEST(PKB, getConstants) {
		PKB::getInstance().resetCache();

		constant three = 3;
		constant four = 4;
		std::vector<constant> v{ three, four };
		PKB::getInstance().setConstants(v);
		EXPECT_EQ(v, PKB::getInstance().getConstants());
	}

	TEST(PKB, getConstants_second) {
		PKB::getInstance().resetCache();

		constant two = 2;
		std::vector<constant> v{ two };
		PKB::getInstance().setConstants(v);
		EXPECT_EQ(v, PKB::getInstance().getConstants());
	}
	TEST(PKB, getProcedures) {
		PKB::getInstance().resetCache();

		procedure_name first_procedure = "main";
		procedure_name second_procedure = "printY";
		std::vector<procedure_name> v{ first_procedure, second_procedure };
		PKB::getInstance().setProcedures(v);
		EXPECT_EQ(v, PKB::getInstance().getProcedures());
	}
	TEST(PKB, resetCache) {
		PKB::getInstance().resetCache();

		std::vector<constant> vc{ 3 };
		std::vector<procedure_name> vp{ "main" };
		PKB::getInstance().setConstants(vc);
		PKB::getInstance().setProcedures(vp);
		PKB::getInstance().resetCache();
		EXPECT_EQ(PKB::getInstance().getConstants().size(), 0);
		EXPECT_EQ(PKB::getInstance().getProcedures().size(), 0);
	}
}