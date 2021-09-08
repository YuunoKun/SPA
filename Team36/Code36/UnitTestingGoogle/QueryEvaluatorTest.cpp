#include "pch.h"

#include "QueryEvaluator.h"
#include "PKBAdapter.h"
#include "PKB.h"
#include "Common.h"

namespace UnitTesting {
	class QueryEvaluatorTest : public testing::Test {
	protected:
		QueryEvaluatorTest() {
			PKB::getInstance().resetCache();
			PKB::getInstance().addVariable({ x });
			PKB::getInstance().addVariable({ y });
			PKB::getInstance().addVariable({ z });
			PKB::getInstance().addConstant({ c1 });
			PKB::getInstance().addConstant({ c2 });
			PKB::getInstance().addConstant({ c3 });
			PKB::getInstance().addProcedure(p1);
			PKB::getInstance().addProcedure(p2);
			PKB::getInstance().addProcedure(p3);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addStmt(STMT_READ);
			PKB::getInstance().addStmt(STMT_READ);
			PKB::getInstance().addStmt(STMT_PRINT);
			PKB::getInstance().addStmt(STMT_PRINT);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_CALL);
			PKB::getInstance().addStmt(STMT_CALL);
			PKB::getInstance().addFollows(1, 2);
			PKB::getInstance().addFollows(2, 3);
			PKB::getInstance().addParent(1, 2);
			PKB::getInstance().addParent(1, 3);
			PKB::getInstance().addParent(2, 3);
			PKB::getInstance().addParent(3, 4);
			PKB::getInstance().addModifiesS(1, x);
			PKB::getInstance().addModifiesS(3, y);
			PKB::getInstance().addUsesS(1, x);
			PKB::getInstance().addUsesS(3, y);
		}

		~QueryEvaluatorTest() override {
			PKB::getInstance().resetCache();
		}

		var_name x = "x";
		var_name y = "y";
		var_name z = "z";
		constant c1 = 1;
		constant c2 = 2;
		constant c3 = 3;
		std::string c1s = std::to_string(c1);
		std::string c2s = std::to_string(c2);
		std::string c3s = std::to_string(c3);
		proc_name p1 = "main";
		proc_name p2 = "sub";
		proc_name p3 = "sub1";

		std::string IF1 = "1";
		std::string IF2 = "2";
		std::string WHILE1 = "3";
		std::string	WHILE2 = "4";
		std::string READ1 = "5";
		std::string READ2 = "6";
		std::string PRINT1 = "7";
		std::string PRINT2 = "8";
		std::string ASSIGN1 = "9";
		std::string ASSIGN2 = "10";
		std::string CALL1 = "11";
		std::string CALL2 = "12";

		std::string FOLLOWLEFT1 = "1";
		std::string FOLLOWLEFT2 = "2";
		std::string FOLLOWRIGHT1 = "2";
		std::string FOLLOWRIGHT2 = "3";

		std::string PARENTLEFT1 = "1";
		std::string PARENTLEFT2 = "2";
		std::string PARENTLEFT3 = "3";
		std::string PARENTRIGHT1 = "2";
		std::string PARENTRIGHT2 = "3";
		std::string PARENTRIGHT3 = "4";

		std::list<std::string> stmts = { IF1, IF2, WHILE1, WHILE2, READ1, READ2,
			PRINT1, PRINT2, ASSIGN1, ASSIGN2, CALL1, CALL2 };
		PKBAdapter pkb;
		QueryEvaluator evaluator;

		Synonym commonSynonym1 = { "cs1" };
		Synonym commonSynonym2 = { "cs2" };

		// select v
		std::list<std::string> allVariables = { x, y, z };
		Entity selectVariable = { VARIABLE, commonSynonym1 };
		// select c
		std::list<std::string> allConstant = { c1s, c2s, c3s };
		Entity selectConstant = { CONSTANT, commonSynonym1 };
		// select p
		std::list<std::string> allProcedure = { p1, p2, p3 };
		Entity selectProcedure = { PROCEDURE, commonSynonym1 };
		// select s
		std::list<std::string> allStmt = stmts;
		Entity selectStmt = { STMT, commonSynonym1 };
		// select ifs
		std::list<std::string> allIfs = { IF1, IF2 };
		Entity selectIfs = { IF, commonSynonym1 };
		// select w
		std::list<std::string> allWhile = { WHILE1, WHILE2 };
		Entity selectWhile = { WHILE, commonSynonym1 };
		// select read
		std::list<std::string> allRead = { READ1, READ2 };
		Entity selectRead = { READ, commonSynonym1 };
		// select print
		std::list<std::string> allPrint = { PRINT1, PRINT2 };
		Entity selectPrint = { PRINT, commonSynonym1 };
		// select assign
		std::list<std::string> allAssign = { ASSIGN1, ASSIGN2 };
		Entity selectAssign = { ASSIGN, commonSynonym1 };
		// select call
		std::list<std::string> allCall = { CALL1, CALL2 };
		Entity selectCall = { CALL, commonSynonym1 };

		std::vector<std::list<std::string>> allResult = {
			allVariables , allConstant , allProcedure, allStmt, allIfs,
			allWhile, allRead, allPrint, allAssign, allCall };

		std::vector<Entity> allSelect = {
			selectVariable , selectConstant , selectProcedure, selectStmt, selectIfs,
			selectWhile, selectRead, selectPrint, selectAssign, selectCall };

		std::list<std::string> emptyResult = {};
	};

	//Evaluator Select statement without any relation/Pattern
	TEST_F(QueryEvaluatorTest, evaluateQueryRaw) {
		for (int i = 0; i < allSelect.size(); i++) {
			Query q;
			q.addSelected(allSelect[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), allResult[i]) << "Error at results : " << i + 1;
		}
	}

	//Evaluator Select statement for Follows Relation
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowBooleanTrue) {
		RelType type = FOLLOWS;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { WILD }, { WILD }));
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, "2" }));
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "3" }));
		relations.push_back(RelRef(type, { STMT, "1" }, { WILD }));
		relations.push_back(RelRef(type, { STMT, "2" }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "2" }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "3" }));

		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1;
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowBooleanFalse) {
		RelType type = FOLLOWS;
		std::vector<RelRef> relations;
		//Test false boolean equation
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "2" }));
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "1" }));
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, "3" }));
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, "4" }));
		relations.push_back(RelRef(type, { STMT, "3" }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "1" }));
		relations.push_back(RelRef(type, { STMT, "4" }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "5" }));
		relations.push_back(RelRef(type, { STMT, "5" }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "6" }));

		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowFilterEmpty) {
		RelType type = FOLLOWS;
		std::vector<RelRef> relations;
		relations.push_back(RelRef(type, { STMT, "3" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "4" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "5" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "6" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "7" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "8" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "9" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "10" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "11" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "12" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "1" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "4" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "5" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "6" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "7" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "8" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "9" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "10" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "11" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "12" }));

		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowFilterNoCommonSynonym) {
		RelType type = FOLLOWS;
		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "1" }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "2" }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "2" }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, "2" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "3" }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, "3" }));

		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}

		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, commonSynonym1 });
		synonyms.push_back({ READ, commonSynonym1 });
		synonyms.push_back({ ASSIGN, commonSynonym1 });
		synonyms.push_back({ CALL, commonSynonym1 });

		for (int k = 0; k < synonyms.size(); k++) {
			for (int j = 0; j < synonyms.size(); j++) {
				RelRef relation(type, synonyms[k], synonyms[j]);
				for (int i = 0; i < allSelect.size(); i++) {
					Query q;
					q.addRelation(relation);
					q.addSelected(allSelect[i]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		//Empty result for non-matching header for single column
		for (int k = 0; k < synonyms.size(); k++) {
			std::vector<RelRef> v;
			v.push_back(RelRef(type, { WILD }, synonyms[k]));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { STMT, "1" }, { synonyms[k] }));
			v.push_back(RelRef(type, { STMT, "2" }, { synonyms[k] }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, "2" }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, "3" }));
			for (int j = 0; j < v.size(); j++) {
				for (int i = 0; i < allSelect.size(); i++) {
					Query q;
					q.addRelation(v[j]);
					q.addSelected(allSelect[i]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowFilterCommonSynonym) {
		RelType type = FOLLOWS;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, commonSynonym1 });
		selectedList.push_back({ IF, commonSynonym1 });
		selectedList.push_back({ WHILE, commonSynonym1 });
		selectedList.push_back({ PRINT, commonSynonym1 });
		selectedList.push_back({ READ, commonSynonym1 });
		selectedList.push_back({ ASSIGN, commonSynonym1 });
		selectedList.push_back({ CALL, commonSynonym1 });

		//Test case for Select a such that Follow(selected, a)
		std::list<std::string> result1 = { FOLLOWLEFT1, FOLLOWLEFT2 };
		std::list<std::string> result2 = { FOLLOWLEFT1, FOLLOWLEFT2 };
		std::list<std::string> result3 = { };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, Synonym{"a"} });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(selected, _)
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { WILD });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(a, selected)
		resultList[0] = { FOLLOWRIGHT1, FOLLOWRIGHT2 };
		resultList[1] = { FOLLOWRIGHT1 };
		resultList[2] = { FOLLOWRIGHT2 };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(a, selected)
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow("1", selected)
		resultList[0] = { FOLLOWRIGHT1 };
		resultList[1] = { FOLLOWRIGHT1 };
		resultList[2] = { };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, "1" }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow("2", selected)
		resultList[0] = { FOLLOWRIGHT2 };
		resultList[1] = { };
		resultList[2] = { FOLLOWRIGHT2 };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, "2" }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Follow(anyEmpty, selected)
		std::vector<Entity> emptyList = {
			{ STMT, "3" },{ STMT, "4" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};
		for (int j = 0; j < emptyList.size(); j++) {
			for (int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q;
				q.addRelation(relation);
				q.addSelected(selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select a such that Follow(selected, "2")
		resultList[0] = { FOLLOWLEFT1 };
		resultList[1] = { FOLLOWLEFT1 };
		resultList[2] = { };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, "2" });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(selected, "3")
		resultList[0] = { FOLLOWLEFT2 };
		resultList[1] = { FOLLOWLEFT2 };
		resultList[2] = { };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, "3" });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Follow(selected, anyEmpty)
		emptyList = { { STMT, "1" },{ STMT, "4" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};
		for (int j = 0; j < emptyList.size(); j++) {
			for (int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q;
				q.addRelation(relation);
				q.addSelected(selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
			}
		}
	}

	//Evaluator Select statement for Follows Relation
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTBooleanTrue) {
		RelType type = FOLLOWS_T;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { WILD }, { WILD }));
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, "2" }));
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, "3" }));
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "3" }));
		relations.push_back(RelRef(type, { STMT, "1" }, { WILD }));
		relations.push_back(RelRef(type, { STMT, "2" }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "2" }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "3" }));

		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1;
			}
		}
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTBooleanFalse) {
		RelType type = FOLLOWS_T;
		std::vector<RelRef> relations;
		//Test false boolean equation
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "2" }));
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "1" }));
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, "4" }));
		relations.push_back(RelRef(type, { STMT, "3" }, { WILD }));
		relations.push_back(RelRef(type, { STMT, "4" }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "1" }));
		relations.push_back(RelRef(type, { STMT, "5" }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "5" }));
		relations.push_back(RelRef(type, { STMT, "6" }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "6" }));

		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTFilterEmpty) {
		RelType type = FOLLOWS_T;
		std::vector<RelRef> relations;
		relations.push_back(RelRef(type, { STMT, "3" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "4" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "5" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "6" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "7" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "8" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "9" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "10" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "11" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "12" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "1" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "4" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "5" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "6" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "7" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "8" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "9" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "10" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "11" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "12" }));

		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}


	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTFilterNoCommonSynonym) {
		RelType type = FOLLOWS_T;
		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "1" }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "1" }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "2" }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "2" }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, "2" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "3" }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, "3" }));


		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}

		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, commonSynonym1 });
		synonyms.push_back({ READ, commonSynonym1 });
		synonyms.push_back({ ASSIGN, commonSynonym1 });
		synonyms.push_back({ CALL, commonSynonym1 });

		for (int k = 0; k < synonyms.size(); k++) {
			for (int j = 0; j < synonyms.size(); j++) {
				RelRef relation(type, synonyms[k], synonyms[j]);
				for (int i = 0; i < allSelect.size(); i++) {
					Query q;
					q.addRelation(relation);
					q.addSelected(allSelect[i]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		//Empty result for non-matching header for single column
		for (int k = 0; k < synonyms.size(); k++) {
			std::vector<RelRef> v;
			v.push_back(RelRef(type, { WILD }, synonyms[k]));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { STMT, "1" }, { synonyms[k] }));
			v.push_back(RelRef(type, { STMT, "2" }, { synonyms[k] }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, "2" }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, "3" }));
			for (int j = 0; j < v.size(); j++) {
				for (int i = 0; i < allSelect.size(); i++) {
					Query q;
					q.addRelation(v[j]);
					q.addSelected(allSelect[i]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTFilterCommonSynonym) {
		RelType type = FOLLOWS_T;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, commonSynonym1 });
		selectedList.push_back({ IF, commonSynonym1 });
		selectedList.push_back({ WHILE, commonSynonym1 });
		selectedList.push_back({ PRINT, commonSynonym1 });
		selectedList.push_back({ READ, commonSynonym1 });
		selectedList.push_back({ ASSIGN, commonSynonym1 });
		selectedList.push_back({ CALL, commonSynonym1 });

		//Test case for Select a such that Follow(selected, a)
		std::list<std::string> result1 = { FOLLOWLEFT1, FOLLOWLEFT2 };
		std::list<std::string> result2 = { FOLLOWLEFT1, FOLLOWLEFT2 };
		std::list<std::string> result3 = { };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, Synonym{"a"} });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(selected, _)
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { WILD });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(a, selected)
		resultList[0] = { FOLLOWRIGHT1, FOLLOWRIGHT2 };
		resultList[1] = { FOLLOWRIGHT1 };
		resultList[2] = { FOLLOWRIGHT2 };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(a, selected)
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow("1", selected)
		resultList[0] = { FOLLOWRIGHT1, FOLLOWRIGHT2 };
		resultList[1] = { FOLLOWRIGHT1 };
		resultList[2] = { FOLLOWRIGHT2 };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, "1" }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow("2", selected)
		resultList[0] = { FOLLOWRIGHT2 };
		resultList[1] = { };
		resultList[2] = { FOLLOWRIGHT2 };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, "2" }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Follow(anyEmpty, selected)
		std::vector<Entity> emptyList = {
			{ STMT, "3" },{ STMT, "4" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};
		for (int j = 0; j < emptyList.size(); j++) {
			for (int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q;
				q.addRelation(relation);
				q.addSelected(selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select a such that Follow(selected, "2")
		resultList[0] = { FOLLOWLEFT1 };
		resultList[1] = { FOLLOWLEFT1 };
		resultList[2] = { };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, "2" });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(selected, "3")
		resultList[0] = { FOLLOWLEFT1, FOLLOWLEFT2 };
		resultList[1] = { FOLLOWLEFT2, FOLLOWLEFT2 };
		resultList[2] = { };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, "3" });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Follow(selected, anyEmpty)
		emptyList = { { STMT, "1" },{ STMT, "4" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};
		for (int j = 0; j < emptyList.size(); j++) {
			for (int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q;
				q.addRelation(relation);
				q.addSelected(selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
			}
		}
	}


	//Evaluator Select statement for Follows Relation
	TEST_F(QueryEvaluatorTest, evaluateQueryParentBooleanTrue) {
		RelType type = PARENT;
		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { WILD }, { WILD }));
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, "2" }));
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, "3" }));
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "3" }));
		relations.push_back(RelRef(type, { STMT, "3" }, { STMT, "4" }));
		relations.push_back(RelRef(type, { STMT, "1" }, { WILD }));
		relations.push_back(RelRef(type, { STMT, "2" }, { WILD }));
		relations.push_back(RelRef(type, { STMT, "3" }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "2" }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "3" }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "4" }));

		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1;
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryParentBooleanFalse) {
		RelType type = PARENT;
		std::vector<RelRef> relations;
		//Test false boolean equation
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "2" }));
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "1" }));
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, "4" }));
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, "5" }));
		relations.push_back(RelRef(type, { STMT, "4" }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "1" }));
		relations.push_back(RelRef(type, { STMT, "5" }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "5" }));
		relations.push_back(RelRef(type, { STMT, "6" }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, "6" }));

		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryParentFilterEmpty) {
		RelType type = PARENT;
		std::vector<RelRef> relations;
		relations.push_back(RelRef(type, { STMT, "4" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "5" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "6" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "7" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "8" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "9" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "10" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "11" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "12" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "1" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "5" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "6" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "7" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "8" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "9" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "10" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "11" }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "12" }));

		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryParentFilterNoCommonSynonym) {
		RelType type = PARENT;
		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "1" }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "2" }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, "2" }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, "2" }));
		relations.push_back(RelRef (type, { STMT, Synonym{"a"} }, { STMT, "3" }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, "3" }));

		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}

		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, commonSynonym1 });
		synonyms.push_back({ READ, commonSynonym1 });
		synonyms.push_back({ ASSIGN, commonSynonym1 });
		synonyms.push_back({ CALL, commonSynonym1 });


		for (int k = 0; k < synonyms.size(); k++) {
			for (int j = 0; j < synonyms.size(); j++) {
				RelRef relation(type, synonyms[k], synonyms[j]);
				for (int i = 0; i < allSelect.size(); i++) {
					Query q;
					q.addRelation(relation);
					q.addSelected(allSelect[i]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		//Empty result for non-matching header for single column
		for (int k = 0; k < synonyms.size(); k++) {
			std::vector<RelRef> v;
			v.push_back(RelRef(type, { WILD }, synonyms[k]));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { STMT, "1" }, { synonyms[k] }));
			v.push_back(RelRef(type, { STMT, "2" }, { synonyms[k] }));
			v.push_back(RelRef(type, { STMT, "3" }, { synonyms[k] }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, "2" }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, "3" }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, "4" }));
			for (int j = 0; j < v.size(); j++) {
				for (int i = 0; i < allSelect.size(); i++) {
					Query q;
					q.addRelation(v[j]);
					q.addSelected(allSelect[i]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryParentFilterCommonSynonym) {
		RelType type = PARENT;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, commonSynonym1 });
		selectedList.push_back({ IF, commonSynonym1 });
		selectedList.push_back({ WHILE, commonSynonym1 });
		selectedList.push_back({ PRINT, commonSynonym1 });
		selectedList.push_back({ READ, commonSynonym1 });
		selectedList.push_back({ ASSIGN, commonSynonym1 });
		selectedList.push_back({ CALL, commonSynonym1 });

		//Test case for Select a such that Parent(selected, a)
		std::list<std::string> result1 = { PARENTLEFT1, PARENTLEFT2, PARENTLEFT3 };
		std::list<std::string> result2 = { PARENTLEFT1, PARENTLEFT2 };
		std::list<std::string> result3 = { PARENTLEFT3 };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, Synonym{"a"} });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent(selected, _)
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { WILD });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent(a, selected)
		resultList[0] = { PARENTRIGHT1, PARENTRIGHT2, PARENTRIGHT3 };
		resultList[1] = { PARENTRIGHT1 };
		resultList[2] = { PARENTRIGHT2, PARENTRIGHT3 };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent(a, selected)
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent("1", selected)
		resultList[0] = { PARENTRIGHT1, PARENTRIGHT2 };
		resultList[1] = { PARENTRIGHT1 };
		resultList[2] = { PARENTRIGHT2 };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, "1" }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent("2", selected)
		resultList[0] = { PARENTRIGHT2 };
		resultList[1] = { };
		resultList[2] = { PARENTRIGHT2 };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, "2" }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Parent(anyEmpty, selected)
		std::vector<Entity> emptyList = {
			{ STMT, "4" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};
		for (int j = 0; j < emptyList.size(); j++) {
			for (int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q;
				q.addRelation(relation);
				q.addSelected(selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select a such that Parent(selected, "2")
		resultList[0] = { PARENTLEFT1 };
		resultList[1] = { PARENTLEFT1 };
		resultList[2] = { };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, "2" });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent(selected, "3")
		resultList[0] = { PARENTLEFT1, PARENTLEFT2 };
		resultList[1] = { PARENTLEFT1, PARENTLEFT2 };
		resultList[2] = { };
		for (int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, "3" });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Parent(selected, anyEmpty)
		emptyList = { { STMT, "1" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};
		for (int j = 0; j < emptyList.size(); j++) {
			for (int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q;
				q.addRelation(relation);
				q.addSelected(selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
			}
		}
	}
}