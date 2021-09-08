#include "pch.h"

#include "RelationsEvaluator.h"
#include "pkb.h"

namespace UnitTesting {
	TEST(RelationsEvaluatorTest, evaluateRelationType1) {
		PKB::getInstance().resetCache();
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);

		RelationsEvaluator evaluator;
		QueryResult result;
		
		std::vector<RelType> types = { FOLLOWS, PARENT /*FOLLOWS_T, PARENT_T*/};

		//Negative Test Case
		for (auto type : types) {
			RelRef relation1(type, { WILD }, { WILD });
			RelRef relation2(type, { STMT, "1" }, { STMT, "2" });
			RelRef relation3(type, { STMT, "1" }, { WILD });
			RelRef relation4(type, { WILD }, { STMT, "2" });
			RelRef relation5(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"a"} });
			RelRef relation6(type, { WILD }, { STMT, Synonym{"a"} });
			RelRef relation7(type, { STMT, Synonym{"a"} }, { WILD });
			RelRef relation8(type, { STMT, "1" }, { STMT, Synonym{"a"} });
			RelRef relation9(type, { STMT, Synonym{"a"} }, { STMT, "2" });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4,
				relation5,relation6,relation7 ,relation8,relation9 };
			for (int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_FALSE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addParent(1, 2);

		//Positive Test Case
		for (auto type : types) {
			RelRef relation1(type, { WILD }, { WILD });
			RelRef relation2(type, { STMT, "1" }, { STMT, "2" });
			RelRef relation3(type, { STMT, "1" }, { WILD });
			RelRef relation4(type, { WILD }, { STMT, "2" });
			RelRef relation5(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"a"} });
			RelRef relation6(type, { WILD }, { STMT, Synonym{"a"} });
			RelRef relation7(type, { STMT, Synonym{"a"} }, { WILD });
			RelRef relation8(type, { STMT, "1" }, { STMT, Synonym{"a"} });
			RelRef relation9(type, { STMT, Synonym{"a"} }, { STMT, "2" });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4,
				relation5,relation6,relation7 ,relation8,relation9 };
			for (int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_TRUE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}

		//Negative Test Case
		for (auto type : types) {
			RelRef relation1(type, { STMT, "2" }, { STMT, "1" });
			RelRef relation2(type, { STMT, "2" }, { WILD });
			RelRef relation3(type, { WILD }, { STMT, "1" });
			RelRef relation4(type, { STMT, "2" }, { STMT, Synonym{"a"} });
			RelRef relation5(type, { STMT, Synonym{"a"} }, { STMT, "1" });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4, relation5 };
			for (int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_FALSE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}
	}

	TEST(RelationsEvaluatorTest, evaluateRelationType2) {
		PKB::getInstance().resetCache();
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable("x");
		PKB::getInstance().addVariable("y");

		RelationsEvaluator evaluator;
		QueryResult result;

		std::vector<RelType> types = { MODIFIES_S, USES_S };

		//Negative Test Case
		for (auto type : types) {
			RelRef relation1(type, { WILD }, { WILD });
			RelRef relation2(type, { STMT, "1" }, { VARIABLE, "x" });
			RelRef relation3(type, { STMT, "1" }, { WILD });
			RelRef relation4(type, { WILD }, { VARIABLE, "x" });
			RelRef relation5(type, { STMT, Synonym{"a"} }, { VARIABLE, Synonym{"a"} });
			RelRef relation6(type, { WILD }, { VARIABLE, Synonym{"a"} });
			RelRef relation7(type, { STMT, Synonym{"a"} }, { WILD });
			RelRef relation8(type, { STMT, "1" }, { VARIABLE, Synonym{"a"} });
			RelRef relation9(type, { STMT, Synonym{"a"} }, { VARIABLE, "x" });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4,
				relation5,relation6,relation7 ,relation8,relation9 };
			for (int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_FALSE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}
		PKB::getInstance().addModifiesS(1, "x");
		PKB::getInstance().addUsesS(1, "x");

		//Positive Test Case
		for (auto type : types) {
			RelRef relation1(type, { WILD }, { WILD });
			RelRef relation2(type, { STMT, "1" }, { VARIABLE, "x" });
			RelRef relation3(type, { STMT, "1" }, { WILD });
			RelRef relation4(type, { WILD }, { VARIABLE, "x" });
			RelRef relation5(type, { STMT, Synonym{"a"} }, { VARIABLE, Synonym{"a"} });
			RelRef relation6(type, { WILD }, { VARIABLE, Synonym{"a"} });
			RelRef relation7(type, { STMT, Synonym{"a"} }, { WILD });
			RelRef relation8(type, { STMT, "1" }, { VARIABLE, Synonym{"a"} });
			RelRef relation9(type, { STMT, Synonym{"a"} }, { VARIABLE, "x" });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4,
				relation5,relation6,relation7 ,relation8,relation9 };
			for (int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_TRUE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}

		//Negative Test Case
		for (auto type : types) {
			RelRef relation1(type, { STMT, "1" }, { VARIABLE, "y" });
			RelRef relation2(type, { STMT, "2" }, { WILD });
			RelRef relation3(type, { WILD }, { VARIABLE, "y" });
			RelRef relation4(type, { STMT, "2" }, { VARIABLE, Synonym{"a"} });
			RelRef relation5(type, { STMT, Synonym{"a"} }, { VARIABLE, "y" });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4, relation5 };
			for (int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_FALSE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}
	}
}