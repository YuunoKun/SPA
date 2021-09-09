#include "pch.h"

#include "RelRef.h"
#include "Common.h"

namespace UnitTesting {
	TEST(RelRef, invalidUsesP) {
		// no error
		std::vector<Entity> assignEntity;
		assignEntity.push_back({ ASSIGN, Synonym{ "assign" } });
		assignEntity.push_back({ VARIABLE, Synonym{ "variable" } });
		assignEntity.push_back({ PRINT, Synonym{ "print" } });
		assignEntity.push_back({ IF, Synonym{ "if" } });
		assignEntity.push_back({ WHILE, Synonym{ "while" } });
		assignEntity.push_back({ PROCEDURE, Synonym{ "procedure" } });
		assignEntity.push_back({ CALL, Synonym{ "call" } });
		assignEntity.push_back({ CONSTANT, Synonym{ "constant" } });
		for (auto& it : assignEntity) {
			RelRef validRelRefUsesP(USES_P, it, it);
		}

		// invalid UsesP
		try {
			Entity invalidEntityUseP = { READ, Synonym{ "statement" } };
			RelRef invalidRelRefUsesP(USES_P, invalidEntityUseP, invalidEntityUseP);
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Clause Type is invalid for USES_P type"));
		}
		catch (...) {
			// Test case should fail if not caught as std::invalid argument
			FAIL();
		}
	}

	TEST(RelRef, invalidModifiesP) {
		// no error
		std::vector<Entity> assignEntity;
		assignEntity.push_back({ ASSIGN, Synonym{ "assign" } });
		assignEntity.push_back({ VARIABLE, Synonym{ "variable" } });
		assignEntity.push_back({ READ, Synonym{ "read" } });
		assignEntity.push_back({ IF, Synonym{ "if" } });
		assignEntity.push_back({ WHILE, Synonym{ "while" } });
		assignEntity.push_back({ PROCEDURE, Synonym{ "procedure" } });
		assignEntity.push_back({ CALL, Synonym{ "call" } });
		assignEntity.push_back({ CONSTANT, Synonym{ "constant" } });
		for (auto& it : assignEntity) {
			RelRef validRelRefModifiesP(MODIFIES_P, it, it);
		}

		// invalid ModifiesP
		try {
			Entity invalidEntityUseP = { PRINT, Synonym{ "statement" } };
			assignEntity.push_back({ PRINT, Synonym{ "statement" } });
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Clause Type is invalid for MODIFIES_P type"));
		}
		catch (...) {
			// Test case should fail if not caught as std::invalid argument
			FAIL();
		}
	}
	TEST(RelRef, getType) {
		Entity entity(ASSIGN, Synonym{ "assign" });
		RelRef validRelRef(USES_P, entity, entity);

		EXPECT_EQ(validRelRef.getType(), USES_P);
		EXPECT_NE(validRelRef.getType(), MODIFIES_P);
	}

	TEST(RelRef, getClauses) {
		Entity entity1(ASSIGN, Synonym{ "assign" });
		Entity entity2(ASSIGN, Synonym{ "assign1" });
		RelRef validRelRef(USES_P, entity1, entity2);

		EXPECT_EQ(validRelRef.getFirstClause(), entity1);
		EXPECT_EQ(validRelRef.getSecondClause(), entity2);
	}

	TEST(RelRef, equal) {
		RelRef e1(USES_P, { WHILE, Synonym{"test1"} }, { ASSIGN, Synonym{ "test2" } });
		RelRef e2(USES_P, { WHILE, Synonym{"test1"} }, { ASSIGN, Synonym{"test2"} });
		EXPECT_EQ(e1, e2);

		RelRef e3(MODIFIES_P, { WHILE, Synonym{"test1"} }, { ASSIGN, Synonym{"test2"} });
		EXPECT_FALSE(e1 == e3);

		RelRef e4(MODIFIES_P, { WHILE, Synonym{"test1"} }, { WHILE, Synonym{"test11"} });
		EXPECT_FALSE(e1 == e4);

		RelRef e5(FOLLOWS, { WHILE, Synonym{"test1"} }, { WHILE, Synonym{"test13"} });
		EXPECT_FALSE(e1 == e5);
	}
}