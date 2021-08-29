#include "pch.h"

#include "PKB.h"
#include "PKB.cpp"
#include "Common.h"

namespace UnitTesting {
	TEST(PKB, getConstants) {
		constant three = 3;
		constant four = 4;
		std::vector<constant> v{ three, four };
		PKB::getInstance().setConstants(v);
		EXPECT_EQ(v, PKB::getInstance().getConstants());
	}

	TEST(PKB, getProcedures) {
		procedure_name first_procedure = "main";
		procedure_name second_procedure = "printY";
		std::vector<procedure_name> v{ first_procedure, second_procedure };
		PKB::getInstance().setProcedures(v);
		EXPECT_EQ(v, PKB::getInstance().getProcedures());
	}
}