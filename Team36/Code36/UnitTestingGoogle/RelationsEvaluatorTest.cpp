#include "pch.h"

#include "RelationsEvaluator.h"
#include "pkb.h"

namespace UnitTesting {
	TEST(RelationsEvaluatorTest, evaluateRelationType1) {
		PKB::getInstance().resetCache();
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);

		RelationsEvaluator evaluator;
		QueryResult result;
		
		std::vector<RelType> types = { FOLLOWS, PARENT /*FOLLOWS_T, PARENT_T*/};
		
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
	}

	TEST(RelationsEvaluatorTest, evaluateRelationType2) {
		PKB::getInstance().resetCache();
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable("x");

		RelationsEvaluator evaluator;
		QueryResult result;

		std::vector<RelType> types = { MODIFIES_S, USES_S };

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
	}
}