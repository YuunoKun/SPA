#include "pch.h"

#include "RelRef.h"
#include "Common.h"

namespace UnitTesting {
	TEST(RelRef, invalidUsesP) {
		// no error
		std::vector<Entity> assignEntity;
		assignEntity.push_back({ ASSIGN, "assign" });
		assignEntity.push_back({ VARIABLE, "variable" });
		assignEntity.push_back({ PRINT, "print" });
		assignEntity.push_back({ IF, "if" });
		assignEntity.push_back({ WHILE, "while" });
		assignEntity.push_back({ PROCEDURE, "procedure" });
		assignEntity.push_back({ CALL, "call" });
		assignEntity.push_back({ CONSTANT, "constant" });
		for (auto& it : assignEntity) {
			RelRef validRelRefUsesP(USES_P, it, it);
		}

		// invalid UsesP
		try {
			Entity invalidEntityUseP = { READ, "statement" };
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
		assignEntity.push_back({ ASSIGN, "assign" });
		assignEntity.push_back({ VARIABLE, "variable" });
		assignEntity.push_back({ READ, "print" });
		assignEntity.push_back({ IF, "if" });
		assignEntity.push_back({ WHILE, "while" });
		assignEntity.push_back({ PROCEDURE, "procedure" });
		assignEntity.push_back({ CALL, "call" });
		assignEntity.push_back({ CONSTANT, "constant" });
		for (auto& it : assignEntity) {
			RelRef validRelRefModifiesP(MODIFIES_P, it, it);
		}

		// invalid ModifiesP
		try {
			Entity invalidEntityUseP = { PRINT, "statement" };
			assignEntity.push_back({ PRINT, "statement" });
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
		Entity entity( ASSIGN, "assign" );
		RelRef validRelRef(USES_P, entity, entity);

		EXPECT_EQ(validRelRef.getType(), USES_P);
		EXPECT_NE(validRelRef.getType(), MODIFIES_P);
	}

	TEST(RelRef, getClauses) {
		Entity entity1(ASSIGN, "assign");
		Entity entity2(ASSIGN, "assign1");
		RelRef validRelRef(USES_P, entity1, entity2);

		EXPECT_EQ(validRelRef.getFirstClause(), entity1);
		EXPECT_EQ(validRelRef.getSecondClause(), entity2);
	}

	TEST(RelRef, equal) {
		RelRef e1(USES_P,  { WHILE,"test1" }, { ASSIGN, "test2" } );
		RelRef e2(USES_P,  { WHILE,"test1" }, { ASSIGN, "test2" } );
		EXPECT_EQ(e1, e2);

		RelRef e3(MODIFIES_P, { WHILE,"test1" }, { ASSIGN, "test2" } );
		EXPECT_FALSE(e1 == e3);

		RelRef e4(MODIFIES_P, { WHILE,"test1" }, { WHILE,"test11" });
		EXPECT_FALSE(e1 == e4);

		RelRef e5(FOLLOWS, { WHILE,"test1" }, { WHILE,"test13" });
		EXPECT_FALSE(e1 == e5);
	}
}