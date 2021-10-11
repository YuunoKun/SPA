#include "pch.h"

#include "RelRef.h"
#include "Common.h"

namespace UnitTesting {
	TEST(RelRef, invalidUsesP) {
		// invalid UsesP
		std::vector<Entity> assign_entity;
		assign_entity.push_back({ ASSIGN, Synonym{ "assign" } });
		assign_entity.push_back({ READ, Synonym{ "read" } });
		assign_entity.push_back({ IF, Synonym{ "if" } });
		assign_entity.push_back({ WHILE, Synonym{ "while" } });
		assign_entity.push_back({ CALL, Synonym{ "call" } });
		for (auto& it : assign_entity) {
			try {
				RelRef invalidEntityUseP(USES_P, it, it);
				FAIL();
			}
			catch (std::invalid_argument const& err) {
				EXPECT_EQ(err.what(), std::string("Clause Type is invalid for USES_P type"));
			}
			catch (...) {
				// Test case should fail if not caught as std::invalid argument
				FAIL();
			}
		}
	}

	TEST(RelRef, invalidModifiesP) {

		// invalid ModifiesP
		std::vector<Entity> assign_entity;
		assign_entity.push_back({ ASSIGN, Synonym{ "assign" } });
		assign_entity.push_back({ READ, Synonym{ "read" } });
		assign_entity.push_back({ IF, Synonym{ "if" } });
		assign_entity.push_back({ WHILE, Synonym{ "while" } });
		assign_entity.push_back({ CALL, Synonym{ "call" } });
		for (auto& it : assign_entity) {
			try {
				RelRef invalidEntityUseP(MODIFIES_P, it, it);
				FAIL();
			}
			catch (std::invalid_argument const& err) {
				EXPECT_EQ(err.what(), std::string("Clause Type is invalid for MODIFIES_P type"));
			}
			catch (...) {
				// Test case should fail if not caught as std::invalid argument
				FAIL();
			}
		}
	}
	TEST(RelRef, getType) {
		Entity entity1(PROCEDURE, Synonym{ "test" });
		Entity entity2(VARIABLE, Synonym{ "test1" });
		RelRef valid_relref(USES_P, entity1, entity2);

		EXPECT_EQ(valid_relref.getType(), USES_P);
		EXPECT_NE(valid_relref.getType(), MODIFIES_P);
	}

	TEST(RelRef, getClauses) {
		Entity entity1(PROCEDURE, Synonym{ "test" });
		Entity entity2(VARIABLE, Synonym{ "test1" });
		RelRef valid_relref(USES_P, entity1, entity2);

		EXPECT_EQ(valid_relref.getFirstClause(), entity1);
		EXPECT_EQ(valid_relref.getSecondClause(), entity2);
	}

	TEST(RelRef, equal) {
		RelRef e1(USES_P, { PROCEDURE, Synonym{"test1"} }, { VARIABLE, Synonym{ "test2" } });
		RelRef e2(USES_P, { PROCEDURE, Synonym{"test1"} }, { VARIABLE, Synonym{"test2"} });
		EXPECT_EQ(e1, e2);

		RelRef e3(MODIFIES_P, { PROCEDURE, Synonym{"test1"} }, { VARIABLE, Synonym{"test2"} });
		EXPECT_FALSE(e1 == e3);

		RelRef e4(MODIFIES_P, { PROCEDURE, Synonym{"test1"} }, { VARIABLE, Synonym{"test11"} });
		EXPECT_FALSE(e1 == e4);

		RelRef e5(FOLLOWS, { WHILE, Synonym{"test1"} }, { WHILE, Synonym{"test13"} });
		EXPECT_FALSE(e1 == e5);
	}
}