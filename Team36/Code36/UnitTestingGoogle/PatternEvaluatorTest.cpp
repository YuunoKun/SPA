#include "pch.h"

#include "RelationsEvaluator.h"
#include "pkb.h"

namespace UnitTesting {
	TEST(PatternEvaluatorTest, evaluateRelationType1) {
		std::vector<var_name> v = { "x", "y", "z", "y", "z", "z" };
		std::vector<expr> e = { "x", "y*x", "x+y", "z%x", "x-x", "x+y-z" };
		std::vector<stmt_index> s = { 1, 2, 3, 4, 5, 6};

		std::vector<AssignInfo> a;

		for (unsigned int i = 0; i < s.size(); i++) {
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addVariable(v[i]);
			a.push_back({ s[i], v[i] });
			PKB::getInstance().addModifiesS(s[i], v[i]);
			PKB::getInstance().addExprTree(s[i], e[i]);
		}

		PKB::getInstance().resetCache();
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);


		RelationsEvaluator evaluator;
		QueryResult result;

		std::vector<RelType> types = { FOLLOWS, };

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
			for (unsigned int i = 0; i < relations.size(); i++) {
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
			for (unsigned int i = 0; i < relations.size(); i++) {
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
			for (unsigned int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_FALSE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}
	}

}