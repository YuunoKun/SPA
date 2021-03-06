#include "pch.h"

#include "PQL/QueryEvaluator/RelationsEvaluator.h"
#include "PKB/PKB.h"

namespace UnitTesting {
	//Type 1 test relation that take in stmt (left) and stmt (right)
	//The following relations is tested : FOLLOWS, PARENT, FOLLOWS_T, PARENT_T, NEXT
	TEST(RelationsEvaluatorTest, evaluateRelationType1) {
		PKBAdapter pkb;

		PKB::getInstance().resetCache();
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		proc_name p = "p";
		PKB::getInstance().addProcedure(p);
		PKB::getInstance().addProcContains(p, 1);
		PKB::getInstance().addProcContains(p, 2);
		pkb.getRelationManager().reset();

		RelationsEvaluator evaluator;
		QueryResult result;

		std::vector<RelType> types = { FOLLOWS, PARENT, FOLLOWS_T, PARENT_T, NEXT, NEXT_T };

		//Negative Test Case
		for (auto type : types) {
			RelRef relation1(type, { WILD }, { WILD });
			RelRef relation2(type, { STMT, "1" }, { STMT, "2" });
			RelRef relation3(type, { STMT, "1" }, { WILD });
			RelRef relation4(type, { WILD }, { STMT, "2" });
			RelRef relation5(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"b"} });
			RelRef relation6(type, { WILD }, { STMT, Synonym{"a"} });
			RelRef relation7(type, { STMT, Synonym{"a"} }, { WILD });
			RelRef relation8(type, { STMT, "1" }, { STMT, Synonym{"a"} });
			RelRef relation9(type, { STMT, Synonym{"a"} }, { STMT, "2" });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4,
				relation5,relation6,relation7 ,relation8,relation9 };
			for (unsigned int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_FALSE(result.haveResult()) << type << "  ERROR AT RELATION : " << i + 1;
			}
		}
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addNext(1, 2);
		PKB::getInstance().addFollowsT(1, 2);
		PKB::getInstance().addParentT(1, 2);
		PKB::getInstance().addProcContains(p, 1);
		PKB::getInstance().addProcContains(p, 2);
		pkb.getRelationManager().reset();

		//Positive Test Case
		for (auto type : types) {
			RelRef relation1(type, { WILD }, { WILD });
			RelRef relation2(type, { STMT, "1" }, { STMT, "2" });
			RelRef relation3(type, { STMT, "1" }, { WILD });
			RelRef relation4(type, { WILD }, { STMT, "2" });
			RelRef relation5(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"b"} });
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

	//Type 2 test relation that take in stmt (left) and variable (right)
	//The following relations is tested : MODIFIES_S, USES_S
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
			RelRef relation1(type, { STMT, "1" }, { VARIABLE, "x" });
			RelRef relation2(type, { STMT, "1" }, { WILD });
			RelRef relation3(type, { STMT, Synonym{"a"} }, { VARIABLE, Synonym{"a"} });
			RelRef relation4(type, { STMT, Synonym{"a"} }, { WILD });
			RelRef relation5(type, { STMT, "1" }, { VARIABLE, Synonym{"a"} });
			RelRef relation6(type, { STMT, Synonym{"a"} }, { VARIABLE, "x" });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4,
				relation5,relation6 };
			for (unsigned int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_FALSE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}
		PKB::getInstance().addModifiesS(1, "x");
		PKB::getInstance().addUsesS(1, "x");

		//Positive Test Case
		for (auto type : types) {
			RelRef relation1(type, { STMT, "1" }, { VARIABLE, "x" });
			RelRef relation2(type, { STMT, "1" }, { WILD });
			RelRef relation3(type, { STMT, Synonym{"a"} }, { VARIABLE, Synonym{"a"} });
			RelRef relation4(type, { STMT, Synonym{"a"} }, { WILD });
			RelRef relation5(type, { STMT, "1" }, { VARIABLE, Synonym{"a"} });
			RelRef relation6(type, { STMT, Synonym{"a"} }, { VARIABLE, "x" });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4,
				relation5,relation6 };
			for (unsigned int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_TRUE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}

		//Negative Test Case
		for (auto type : types) {
			RelRef relation1(type, { STMT, "1" }, { VARIABLE, "y" });
			RelRef relation2(type, { STMT, "2" }, { WILD });
			RelRef relation3(type, { STMT, "2" }, { VARIABLE, Synonym{"a"} });
			RelRef relation4(type, { STMT, Synonym{"a"} }, { VARIABLE, "y" });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4 };
			for (unsigned int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_FALSE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}
	}

	//Type 3 test relation that take in procedure (left) and variable (right)
	//The following relations is tested : MODIFIES_P, USES_P
	TEST(RelationsEvaluatorTest, evaluateRelationType3) {
		PKB::getInstance().resetCache();
		proc_name main1 = "main1";
		proc_name main2 = "main2";
		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addVariable("x");
		PKB::getInstance().addVariable("y");

		RelationsEvaluator evaluator;
		QueryResult result;

		std::vector<RelType> types = { MODIFIES_P, USES_P };

		//Negative Test Case
		for (auto type : types) {
			RelRef relation1(type, { PROCEDURE, main1 }, { VARIABLE, "x" });
			RelRef relation2(type, { PROCEDURE, main1 }, { WILD });
			RelRef relation3(type, { PROCEDURE, Synonym{"a"} }, { VARIABLE, Synonym{"a"} });
			RelRef relation4(type, { PROCEDURE, Synonym{"a"} }, { WILD });
			RelRef relation5(type, { PROCEDURE, main1 }, { VARIABLE, Synonym{"a"} });
			RelRef relation6(type, { PROCEDURE, Synonym{"a"} }, { VARIABLE, "x" });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4,
				relation5,relation6 };
			for (unsigned int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_FALSE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}
		PKB::getInstance().addModifiesP(main1, "x");
		PKB::getInstance().addUsesP(main1, "x");

		//Positive Test Case
		for (auto type : types) {
			RelRef relation1(type, { PROCEDURE, main1 }, { VARIABLE, "x" });
			RelRef relation2(type, { PROCEDURE, main1 }, { WILD });
			RelRef relation3(type, { PROCEDURE, Synonym{"a"} }, { VARIABLE, Synonym{"a"} });
			RelRef relation4(type, { PROCEDURE, Synonym{"a"} }, { WILD });
			RelRef relation5(type, { PROCEDURE, main1 }, { VARIABLE, Synonym{"a"} });
			RelRef relation6(type, { PROCEDURE, Synonym{"a"} }, { VARIABLE, "x" });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4,
				relation5,relation6 };
			for (unsigned int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_TRUE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}

		//Negative Test Case
		for (auto type : types) {
			RelRef relation1(type, { PROCEDURE, main1 }, { VARIABLE, "y" });
			RelRef relation2(type, { PROCEDURE, main2 }, { WILD });
			RelRef relation3(type, { PROCEDURE, main2 }, { VARIABLE, Synonym{"a"} });
			RelRef relation4(type, { PROCEDURE, Synonym{"a"} }, { VARIABLE, "y" });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4 };
			for (unsigned int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_FALSE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}
	}

	//Type 4 test relation that take in procedure (left) and procedure (right)
	//The following relations is tested : CALLS, CALLS_T
	TEST(RelationsEvaluatorTest, evaluateRelationType4) {
		PKB::getInstance().resetCache();
		proc_name main1 = "main1";
		proc_name main2 = "main2";
		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);

		RelationsEvaluator evaluator;
		QueryResult result;

		std::vector<RelType> types = { CALLS, CALLS_T };

		//Negative Test Case
		for (auto type : types) {
			RelRef relation1(type, { WILD }, { WILD });
			RelRef relation2(type, { PROCEDURE, main1 }, { PROCEDURE, main2 });
			RelRef relation3(type, { PROCEDURE, main1 }, { WILD });
			RelRef relation4(type, { WILD }, { PROCEDURE, main2 });
			RelRef relation5(type, { STMT, Synonym{"a"} }, { PROCEDURE, Synonym{"b"} });
			RelRef relation6(type, { WILD }, { PROCEDURE, Synonym{"a"} });
			RelRef relation7(type, { PROCEDURE, Synonym{"a"} }, { WILD });
			RelRef relation8(type, { PROCEDURE, main1 }, { PROCEDURE, Synonym{"a"} });
			RelRef relation9(type, { PROCEDURE, Synonym{"a"} }, { PROCEDURE, main2 });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4,
				relation5,relation6,relation7 ,relation8,relation9 };
			for (unsigned int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_FALSE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}
		PKB::getInstance().addCallsP(main1, main2);
		PKB::getInstance().addCallsPT(main1, main2);

		//Positive Test Case
		for (auto type : types) {
			RelRef relation1(type, { WILD }, { WILD });
			RelRef relation2(type, { PROCEDURE, main1 }, { PROCEDURE, main2 });
			RelRef relation3(type, { PROCEDURE, main1 }, { WILD });
			RelRef relation4(type, { WILD }, { PROCEDURE, main2 });
			RelRef relation5(type, { PROCEDURE, Synonym{"a"} }, { PROCEDURE, Synonym{"b"} });
			RelRef relation6(type, { WILD }, { PROCEDURE, Synonym{"a"} });
			RelRef relation7(type, { PROCEDURE, Synonym{"a"} }, { WILD });
			RelRef relation8(type, { PROCEDURE, main1 }, { PROCEDURE, Synonym{"a"} });
			RelRef relation9(type, { PROCEDURE, Synonym{"a"} }, { PROCEDURE, main2 });

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
			RelRef relation1(type, { PROCEDURE, main2 }, { PROCEDURE, main1 });
			RelRef relation2(type, { PROCEDURE, main2 }, { WILD });
			RelRef relation3(type, { WILD }, { PROCEDURE, main1 });
			RelRef relation4(type, { PROCEDURE, main2 }, { PROCEDURE, Synonym{"a"} });
			RelRef relation5(type, { PROCEDURE, Synonym{"a"} }, { PROCEDURE, main1 });

			std::vector<RelRef> relations{ relation1, relation2, relation3, relation4, relation5 };
			for (unsigned int i = 0; i < relations.size(); i++) {
				QueryResult result;
				evaluator.evaluateRelation(result, relations[i]);
				EXPECT_FALSE(result.haveResult()) << "ERROR AT RELATION : " << i + 1;
			}
		}
	}
}