#include "pch.h"

#include "PKB.h"
#include "PKB.cpp"
#include "Common.h"

namespace UnitTesting {
	TEST(PKB, getConstants) {
		PKB pkb = pkb.getInstance();
		constant three = 3;
		constant four = 4;
		std::vector<constant> v{ three, four };
		pkb.setConstants(v);
		EXPECT_EQ(v, pkb.getConstants());
	}

	TEST(PKB, getProcedures) {
		PKB pkb = pkb.getInstance();
		procedure_name first_procedure = "main";
		procedure_name second_procedure = "printY";
		std::vector<procedure_name> v{ first_procedure, second_procedure };
		pkb.setProcedures(v);
		EXPECT_EQ(v, pkb.getProcedures());
	}
}