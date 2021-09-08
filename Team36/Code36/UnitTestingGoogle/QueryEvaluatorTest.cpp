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
			PKB::getInstance().addFollows(std::stoi(FOLLOWLEFT1), std::stoi(FOLLOWRIGHT1));
			PKB::getInstance().addFollows(std::stoi(FOLLOWLEFT2), std::stoi(FOLLOWRIGHT2));
			PKB::getInstance().addParent(std::stoi(PARENTLEFT1), std::stoi(PARENTRIGHT1));
			PKB::getInstance().addParent(std::stoi(PARENTLEFT1), std::stoi(PARENTRIGHT2));
			PKB::getInstance().addParent(std::stoi(PARENTLEFT2), std::stoi(PARENTRIGHT2));
			PKB::getInstance().addParent(std::stoi(PARENTLEFT3), std::stoi(PARENTRIGHT3));
			PKB::getInstance().addModifiesS(std::stoi(MODIFIESLEFT1), MODIFIESRIGHT1);
			PKB::getInstance().addModifiesS(std::stoi(MODIFIESLEFT2), MODIFIESRIGHT2);
			PKB::getInstance().addUsesS(std::stoi(USESLEFT1), USESRIGHT1);
			PKB::getInstance().addUsesS(std::stoi(USESLEFT2), USESRIGHT2);
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

		std::string MODIFIESLEFT1 = "1";
		std::string MODIFIESLEFT2 = "3";
		std::string MODIFIESRIGHT1 = "x";
		std::string MODIFIESRIGHT2 = "y";

		std::string USESLEFT1 = "1";
		std::string USESLEFT2 = "3";
		std::string USESRIGHT1 = "y";
		std::string USESRIGHT2 = "x";

		std::list<std::string> stmts = { IF1, IF2, WHILE1, WHILE2, READ1, READ2,
			PRINT1, PRINT2, ASSIGN1, ASSIGN2, CALL1, CALL2 };
		PKBAdapter pkb;
		QueryEvaluator evaluator;

		Synonym commonSynonym1 = { "cs1" };
		Synonym commonSynonym2 = { "cs2" };

		// select v
		std::list<std::string> allVariable = { x, y, z };
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
			allVariable , allConstant , allProcedure, allStmt, allIfs,
			allWhile, allRead, allPrint, allAssign, allCall };

		std::vector<Entity> allSelect = {
			selectVariable , selectConstant , selectProcedure, selectStmt, selectIfs,
			selectWhile, selectRead, selectPrint, selectAssign, selectCall };

		std::list<std::string> emptyResult = {};

		std::vector<Entity> validConstantEntity = { { WILD },
			{STMT, "1"}, {STMT, "2"}, {STMT, "3"}, {STMT, "4"}, {STMT, "5"}, {STMT, "6"},
			{STMT, "7"}, {STMT, "8"}, {STMT, "9"}, {STMT, "10"}, {STMT, "11"}, {STMT, "12"}
		};

		std::vector<Entity> allVariables = { { VARIABLE, x }, { VARIABLE, y }, { VARIABLE, z } };
	};

	//Evaluator Select statement without any relation/Pattern
	TEST_F(QueryEvaluatorTest, evaluateQueryRaw) {
		for (unsigned int i = 0; i < allSelect.size(); i++) {
			Query q;
			q.addSelected(allSelect[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), allResult[i]) << "Error at results : " << i + 1;
		}
	}

	//Follow Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowBooleanTrue) {
		RelType type = FOLLOWS;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { WILD }, { WILD }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT1 }, { STMT, FOLLOWRIGHT1 }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT2 }, { STMT, FOLLOWRIGHT2 }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT1 }, { WILD }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT2 }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, FOLLOWRIGHT1 }));
		relations.push_back(RelRef(type, { WILD }, { STMT, FOLLOWRIGHT2 }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
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
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, "3" }));
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "2" }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}

		std::vector<Entity> invalidLefts;
		invalidLefts.push_back({ STMT, "3" });
		invalidLefts.push_back({ STMT, "4" });
		invalidLefts.push_back({ STMT, "5" });
		invalidLefts.push_back({ STMT, "6" });
		invalidLefts.push_back({ STMT, "7" });
		invalidLefts.push_back({ STMT, "8" });
		invalidLefts.push_back({ STMT, "9" });
		invalidLefts.push_back({ STMT, "10" });
		invalidLefts.push_back({ STMT, "11" });
		invalidLefts.push_back({ STMT, "12" });
		for (unsigned int k = 0; k < invalidLefts.size(); k++) {
			for (unsigned int i = 0; i < validConstantEntity.size(); i++) {
				for (unsigned int j = 0; j < allSelect.size(); j++) {
					Query q;
					q.addRelation(RelRef(type, invalidLefts[k], validConstantEntity[i]));
					q.addSelected(allSelect[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		std::vector<Entity> invalidRight;
		invalidRight.push_back({ STMT, "1" });
		invalidRight.push_back({ STMT, "4" });
		invalidRight.push_back({ STMT, "5" });
		invalidRight.push_back({ STMT, "6" });
		invalidRight.push_back({ STMT, "7" });
		invalidRight.push_back({ STMT, "8" });
		invalidRight.push_back({ STMT, "9" });
		invalidRight.push_back({ STMT, "10" });
		invalidRight.push_back({ STMT, "11" });
		invalidRight.push_back({ STMT, "12" });
		for (unsigned int k = 0; k < invalidRight.size(); k++) {
			for (unsigned int i = 0; i < validConstantEntity.size(); i++) {
				for (unsigned int j = 0; j < allSelect.size(); j++) {
					Query q;
					q.addRelation(RelRef(type, validConstantEntity[i], invalidRight[k]));
					q.addSelected(allSelect[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
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

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowFilterNoCommonSynonymTrue) {
		RelType type = FOLLOWS;
		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT1 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT1 }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT2 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT2 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, FOLLOWRIGHT1 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, FOLLOWRIGHT1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, FOLLOWRIGHT2 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, FOLLOWRIGHT2 }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryFollowFilterNoCommonSynonymFalse) {
		RelType type = FOLLOWS;
		std::vector<RelRef> relations;
		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, commonSynonym1 });
		synonyms.push_back({ READ, commonSynonym1 });
		synonyms.push_back({ ASSIGN, commonSynonym1 });
		synonyms.push_back({ CALL, commonSynonym1 });

		for (unsigned int k = 0; k < synonyms.size(); k++) {
			for (unsigned int j = 0; j < synonyms.size(); j++) {
				RelRef relation(type, synonyms[k], synonyms[j]);
				for (unsigned int i = 0; i < allSelect.size(); i++) {
					Query q;
					q.addRelation(relation);
					q.addSelected(allSelect[i]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		//Empty result for non-matching header for single column
		for (unsigned int k = 0; k < synonyms.size(); k++) {
			std::vector<RelRef> v;
			v.push_back(RelRef(type, { WILD }, synonyms[k]));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { STMT, FOLLOWLEFT1 }, { synonyms[k] }));
			v.push_back(RelRef(type, { STMT, FOLLOWLEFT2 }, { synonyms[k] }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, FOLLOWRIGHT1 }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, FOLLOWRIGHT2 }));
			for (unsigned int j = 0; j < v.size(); j++) {
				for (unsigned int i = 0; i < allSelect.size(); i++) {
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
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, Synonym{"a"} });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
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
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(_, selected)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { WILD }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow("1", selected)
		resultList[0] = { FOLLOWRIGHT1 };
		resultList[1] = { FOLLOWRIGHT1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, FOLLOWLEFT1 }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow("2", selected)
		resultList[0] = { FOLLOWRIGHT2 };
		resultList[1] = { };
		resultList[2] = { FOLLOWRIGHT2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, FOLLOWLEFT2 }, selectedList[i]);
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
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
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
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, FOLLOWRIGHT1 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(selected, "3")
		resultList[0] = { FOLLOWLEFT2 };
		resultList[1] = { FOLLOWLEFT2 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, FOLLOWRIGHT2 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Follow(selected, anyEmpty)
		emptyList = { { STMT, "1" },{ STMT, "4" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q;
				q.addRelation(relation);
				q.addSelected(selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
			}
		}
	}


	//FollowT Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTBooleanTrue) {
		RelType type = FOLLOWS_T;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { WILD }, { WILD }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT1 }, { STMT, FOLLOWRIGHT1 }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT1 }, { STMT, FOLLOWRIGHT2 }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT2 }, { STMT, FOLLOWRIGHT2 }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT1 }, { WILD }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT2 }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, FOLLOWRIGHT1 }));
		relations.push_back(RelRef(type, { WILD }, { STMT, FOLLOWRIGHT2 }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
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

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}

		std::vector<Entity> invalidLefts;
		invalidLefts.push_back({ STMT, "3" });
		invalidLefts.push_back({ STMT, "4" });
		invalidLefts.push_back({ STMT, "5" });
		invalidLefts.push_back({ STMT, "6" });
		invalidLefts.push_back({ STMT, "7" });
		invalidLefts.push_back({ STMT, "8" });
		invalidLefts.push_back({ STMT, "9" });
		invalidLefts.push_back({ STMT, "10" });
		invalidLefts.push_back({ STMT, "11" });
		invalidLefts.push_back({ STMT, "12" });
		for (unsigned int k = 0; k < invalidLefts.size(); k++) {
			for (unsigned int i = 0; i < validConstantEntity.size(); i++) {
				for (unsigned int j = 0; j < allSelect.size(); j++) {
					Query q;
					q.addRelation(RelRef(type, invalidLefts[k], validConstantEntity[i]));
					q.addSelected(allSelect[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		std::vector<Entity> invalidRight;
		invalidRight.push_back({ STMT, "1" });
		invalidRight.push_back({ STMT, "4" });
		invalidRight.push_back({ STMT, "5" });
		invalidRight.push_back({ STMT, "6" });
		invalidRight.push_back({ STMT, "7" });
		invalidRight.push_back({ STMT, "8" });
		invalidRight.push_back({ STMT, "9" });
		invalidRight.push_back({ STMT, "10" });
		invalidRight.push_back({ STMT, "11" });
		invalidRight.push_back({ STMT, "12" });
		for (unsigned int k = 0; k < invalidRight.size(); k++) {
			for (unsigned int i = 0; i < validConstantEntity.size(); i++) {
				for (unsigned int j = 0; j < allSelect.size(); j++) {
					Query q;
					q.addRelation(RelRef(type, validConstantEntity[i], invalidRight[k]));
					q.addSelected(allSelect[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
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

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}


	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTFilterNoCommonSynonymTrue) {
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
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT1 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT1 }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT1 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT2 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, FOLLOWLEFT2 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, FOLLOWRIGHT1 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, FOLLOWRIGHT1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, FOLLOWRIGHT2 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, FOLLOWRIGHT2 }));


		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTFilterNoCommonSynonymFalse) {
		RelType type = FOLLOWS_T;

		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, commonSynonym1 });
		synonyms.push_back({ READ, commonSynonym1 });
		synonyms.push_back({ ASSIGN, commonSynonym1 });
		synonyms.push_back({ CALL, commonSynonym1 });

		for (unsigned int k = 0; k < synonyms.size(); k++) {
			for (unsigned int j = 0; j < synonyms.size(); j++) {
				RelRef relation(type, synonyms[k], synonyms[j]);
				for (unsigned int i = 0; i < allSelect.size(); i++) {
					Query q;
					q.addRelation(relation);
					q.addSelected(allSelect[i]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		//Empty result for non-matching header for single column
		for (unsigned int k = 0; k < synonyms.size(); k++) {
			std::vector<RelRef> v;
			v.push_back(RelRef(type, { WILD }, synonyms[k]));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { STMT, FOLLOWLEFT1 }, { synonyms[k] }));
			v.push_back(RelRef(type, { STMT, FOLLOWLEFT2 }, { synonyms[k] }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, FOLLOWRIGHT1 }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, FOLLOWRIGHT2 }));
			for (unsigned int j = 0; j < v.size(); j++) {
				for (unsigned int i = 0; i < allSelect.size(); i++) {
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

		//Test case for Select a such that FollowT(selected, a)
		std::list<std::string> result1 = { FOLLOWLEFT1, FOLLOWLEFT2 };
		std::list<std::string> result2 = { FOLLOWLEFT1, FOLLOWLEFT2 };
		std::list<std::string> result3 = { };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, Synonym{"a"} });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that FollowT(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { WILD });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that FollowT(a, selected)
		resultList[0] = { FOLLOWRIGHT1, FOLLOWRIGHT2 };
		resultList[1] = { FOLLOWRIGHT1 };
		resultList[2] = { FOLLOWRIGHT2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that FollowT(_, selected)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { WILD }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that FollowT("1", selected)
		resultList[0] = { FOLLOWRIGHT1, FOLLOWRIGHT2 };
		resultList[1] = { FOLLOWRIGHT1 };
		resultList[2] = { FOLLOWRIGHT2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, FOLLOWLEFT1 }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that FollowT("2", selected)
		resultList[0] = { FOLLOWRIGHT2 };
		resultList[1] = { };
		resultList[2] = { FOLLOWRIGHT2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, FOLLOWLEFT2 }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that FollowT(anyEmpty, selected)
		std::vector<Entity> emptyList = {
			{ STMT, "3" },{ STMT, "4" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q;
				q.addRelation(relation);
				q.addSelected(selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select a such that FollowT(selected, "2")
		resultList[0] = { FOLLOWLEFT1 };
		resultList[1] = { FOLLOWLEFT1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, FOLLOWRIGHT1 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that FollowT(selected, "3")
		resultList[0] = { FOLLOWLEFT1, FOLLOWLEFT2 };
		resultList[1] = { FOLLOWLEFT2, FOLLOWLEFT2 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, FOLLOWRIGHT2 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that FollowT(selected, anyEmpty)
		emptyList = { { STMT, "1" },{ STMT, "4" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q;
				q.addRelation(relation);
				q.addSelected(selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
			}
		}
	}


	//Parent Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryParentBooleanTrue) {
		RelType type = PARENT;
		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { WILD }, { WILD }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { STMT, PARENTRIGHT1 }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { STMT, PARENTRIGHT2 }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT2 }, { STMT, PARENTRIGHT2 }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT3 }, { STMT, PARENTRIGHT3 }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { WILD }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT2 }, { WILD }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT3 }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, PARENTRIGHT1 }));
		relations.push_back(RelRef(type, { WILD }, { STMT, PARENTRIGHT2 }));
		relations.push_back(RelRef(type, { WILD }, { STMT, PARENTRIGHT3 }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
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
		relations.push_back(RelRef(type, { STMT, "1" }, { STMT, "4" }));
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "2" }));
		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "4" }));
		relations.push_back(RelRef(type, { STMT, "3" }, { STMT, "3" }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}

		std::vector<Entity> invalidLefts;
		invalidLefts.push_back({ STMT, "4" });
		invalidLefts.push_back({ STMT, "5" });
		invalidLefts.push_back({ STMT, "6" });
		invalidLefts.push_back({ STMT, "7" });
		invalidLefts.push_back({ STMT, "8" });
		invalidLefts.push_back({ STMT, "9" });
		invalidLefts.push_back({ STMT, "10" });
		invalidLefts.push_back({ STMT, "11" });
		invalidLefts.push_back({ STMT, "12" });
		for (unsigned int k = 0; k < invalidLefts.size(); k++) {
			for (unsigned int i = 0; i < validConstantEntity.size(); i++) {
				for (unsigned int j = 0; j < allSelect.size(); j++) {
					Query q;
					q.addRelation(RelRef(type, invalidLefts[k], validConstantEntity[i]));
					q.addSelected(allSelect[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		std::vector<Entity> invalidRight;
		invalidRight.push_back({ STMT, "1" });
		invalidRight.push_back({ STMT, "5" });
		invalidRight.push_back({ STMT, "6" });
		invalidRight.push_back({ STMT, "7" });
		invalidRight.push_back({ STMT, "8" });
		invalidRight.push_back({ STMT, "9" });
		invalidRight.push_back({ STMT, "10" });
		invalidRight.push_back({ STMT, "11" });
		invalidRight.push_back({ STMT, "12" });
		for (unsigned int k = 0; k < invalidRight.size(); k++) {
			for (unsigned int i = 0; i < validConstantEntity.size(); i++) {
				for (unsigned int j = 0; j < allSelect.size(); j++) {
					Query q;
					q.addRelation(RelRef(type, validConstantEntity[i], invalidRight[k]));
					q.addSelected(allSelect[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
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

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryParentFilterNoCommonSynonymTrue) {
		RelType type = PARENT;
		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT2 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT2 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT3 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT3 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, PARENTRIGHT1 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, PARENTRIGHT1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, PARENTRIGHT2 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, PARENTRIGHT2 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, PARENTRIGHT3 }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { STMT, PARENTRIGHT3 }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryParentFilterNoCommonSynonymFalse) {
		RelType type = PARENT;
		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, commonSynonym1 });
		synonyms.push_back({ READ, commonSynonym1 });
		synonyms.push_back({ ASSIGN, commonSynonym1 });
		synonyms.push_back({ CALL, commonSynonym1 });


		for (unsigned int k = 0; k < synonyms.size(); k++) {
			for (unsigned int j = 0; j < synonyms.size(); j++) {
				RelRef relation(type, synonyms[k], synonyms[j]);
				for (unsigned int i = 0; i < allSelect.size(); i++) {
					Query q;
					q.addRelation(relation);
					q.addSelected(allSelect[i]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		//Empty result for non-matching header for single column
		for (unsigned int k = 0; k < synonyms.size(); k++) {
			std::vector<RelRef> v;
			v.push_back(RelRef(type, { WILD }, synonyms[k]));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { synonyms[k] }));
			v.push_back(RelRef(type, { STMT, PARENTLEFT2 }, { synonyms[k] }));
			v.push_back(RelRef(type, { STMT, PARENTLEFT3 }, { synonyms[k] }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, PARENTRIGHT1 }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, PARENTRIGHT2 }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, PARENTRIGHT3 }));
			for (unsigned int j = 0; j < v.size(); j++) {
				for (unsigned int i = 0; i < allSelect.size(); i++) {
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
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, Synonym{"a"} });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
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
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent(_, selected)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { WILD }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent("1", selected)
		resultList[0] = { PARENTRIGHT1, PARENTRIGHT2 };
		resultList[1] = { PARENTRIGHT1 };
		resultList[2] = { PARENTRIGHT2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, PARENTLEFT1 }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent("2", selected)
		resultList[0] = { PARENTRIGHT2 };
		resultList[1] = { };
		resultList[2] = { PARENTRIGHT2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, PARENTLEFT2 }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent("3", selected)
		resultList[0] = { PARENTRIGHT3 };
		resultList[1] = { };
		resultList[2] = { PARENTRIGHT3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, PARENTLEFT3 }, selectedList[i]);
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
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
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
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, PARENTRIGHT1 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent(selected, "3")
		resultList[0] = { PARENTLEFT1, PARENTLEFT2 };
		resultList[1] = { PARENTLEFT1, PARENTLEFT2 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, PARENTRIGHT2 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent(selected, "4")
		resultList[0] = { PARENTLEFT3 };
		resultList[1] = { };
		resultList[2] = { PARENTLEFT3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, PARENTRIGHT3 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Parent(selected, anyEmpty)
		emptyList = { { STMT, "1" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q;
				q.addRelation(relation);
				q.addSelected(selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
			}
		}
	}

	//ParentT Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryParentTBooleanTrue) {
		RelType type = PARENT_T;
		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { WILD }, { WILD }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { STMT, PARENTRIGHT1 }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { STMT, PARENTRIGHT2 }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { STMT, PARENTRIGHT3 }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT2 }, { STMT, PARENTRIGHT2 }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT2 }, { STMT, PARENTRIGHT3 }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT3 }, { STMT, PARENTRIGHT3 }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { WILD }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT2 }, { WILD }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT3 }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { STMT, PARENTRIGHT1 }));
		relations.push_back(RelRef(type, { WILD }, { STMT, PARENTRIGHT2 }));
		relations.push_back(RelRef(type, { WILD }, { STMT, PARENTRIGHT3 }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1;
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryParentTBooleanFalse) {
		RelType type = PARENT_T;
		std::vector<RelRef> relations;
		//Test false boolean equation

		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "2" }));
		relations.push_back(RelRef(type, { STMT, "3" }, { STMT, "3" }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}

		std::vector<Entity> invalidLefts;
		invalidLefts.push_back({ STMT, "4" });
		invalidLefts.push_back({ STMT, "5" });
		invalidLefts.push_back({ STMT, "6" });
		invalidLefts.push_back({ STMT, "7" });
		invalidLefts.push_back({ STMT, "8" });
		invalidLefts.push_back({ STMT, "9" });
		invalidLefts.push_back({ STMT, "10" });
		invalidLefts.push_back({ STMT, "11" });
		invalidLefts.push_back({ STMT, "12" });
		for (unsigned int k = 0; k < invalidLefts.size(); k++) {
			for (unsigned int i = 0; i < validConstantEntity.size(); i++) {
				for (unsigned int j = 0; j < allSelect.size(); j++) {
					Query q;
					q.addRelation(RelRef(type, invalidLefts[k], validConstantEntity[i]));
					q.addSelected(allSelect[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		std::vector<Entity> invalidRight;
		invalidRight.push_back({ STMT, "1" });
		invalidRight.push_back({ STMT, "5" });
		invalidRight.push_back({ STMT, "6" });
		invalidRight.push_back({ STMT, "7" });
		invalidRight.push_back({ STMT, "8" });
		invalidRight.push_back({ STMT, "9" });
		invalidRight.push_back({ STMT, "10" });
		invalidRight.push_back({ STMT, "11" });
		invalidRight.push_back({ STMT, "12" });
		for (unsigned int k = 0; k < invalidRight.size(); k++) {
			for (unsigned int i = 0; i < validConstantEntity.size(); i++) {
				for (unsigned int j = 0; j < allSelect.size(); j++) {
					Query q;
					q.addRelation(RelRef(type, validConstantEntity[i], invalidRight[k]));
					q.addSelected(allSelect[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}


	}
	TEST_F(QueryEvaluatorTest, evaluateQueryParentTFilterEmpty) {
		RelType type = PARENT_T;
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

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryParentTFilterNoCommonSynonymTrue) {
		RelType type = PARENT_T;
		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT2 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT2 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT3 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, PARENTLEFT3 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, PARENTRIGHT1 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, PARENTRIGHT1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, PARENTRIGHT2 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, PARENTRIGHT2 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, PARENTRIGHT3 }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { STMT, PARENTRIGHT3 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, PARENTRIGHT3 }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryParentTFilterNoCommonSynonymFalse) {
		RelType type = PARENT_T;

		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, commonSynonym1 });
		synonyms.push_back({ READ, commonSynonym1 });
		synonyms.push_back({ ASSIGN, commonSynonym1 });
		synonyms.push_back({ CALL, commonSynonym1 });

		for (unsigned int k = 0; k < synonyms.size(); k++) {
			for (unsigned int j = 0; j < synonyms.size(); j++) {
				RelRef relation(type, synonyms[k], synonyms[j]);
				for (unsigned int i = 0; i < allSelect.size(); i++) {
					Query q;
					q.addRelation(relation);
					q.addSelected(allSelect[i]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		//Empty result for non-matching header for single column
		for (unsigned int k = 0; k < synonyms.size(); k++) {
			std::vector<RelRef> v;
			v.push_back(RelRef(type, { WILD }, synonyms[k]));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { synonyms[k] }, { WILD }));
			v.push_back(RelRef(type, { STMT, PARENTLEFT1 }, { synonyms[k] }));
			v.push_back(RelRef(type, { STMT, PARENTLEFT2 }, { synonyms[k] }));
			v.push_back(RelRef(type, { STMT, PARENTLEFT3 }, { synonyms[k] }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, PARENTRIGHT1 }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, PARENTRIGHT2 }));
			v.push_back(RelRef(type, { synonyms[k] }, { STMT, PARENTRIGHT3 }));
			for (unsigned int j = 0; j < v.size(); j++) {
				for (unsigned int i = 0; i < allSelect.size(); i++) {
					Query q;
					q.addRelation(v[j]);
					q.addSelected(allSelect[i]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryParentTFilterCommonSynonym) {
		RelType type = PARENT_T;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, commonSynonym1 });
		selectedList.push_back({ IF, commonSynonym1 });
		selectedList.push_back({ WHILE, commonSynonym1 });
		selectedList.push_back({ PRINT, commonSynonym1 });
		selectedList.push_back({ READ, commonSynonym1 });
		selectedList.push_back({ ASSIGN, commonSynonym1 });
		selectedList.push_back({ CALL, commonSynonym1 });

		//Test case for Select a such that ParentT(selected, a)
		std::list<std::string> result1 = { PARENTLEFT1, PARENTLEFT2, PARENTLEFT3 };
		std::list<std::string> result2 = { PARENTLEFT1, PARENTLEFT2 };
		std::list<std::string> result3 = { PARENTLEFT3 };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, Synonym{"a"} });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { WILD });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT(a, selected)
		resultList[0] = { PARENTRIGHT1, PARENTRIGHT2, PARENTRIGHT3 };
		resultList[1] = { PARENTRIGHT1 };
		resultList[2] = { PARENTRIGHT2, PARENTRIGHT3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT(_, selected)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { WILD }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT("1", selected)
		resultList[0] = { PARENTRIGHT1, PARENTRIGHT2 };
		resultList[1] = { PARENTRIGHT1 };
		resultList[2] = { PARENTRIGHT2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, PARENTLEFT1 }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT("2", selected)
		resultList[0] = { PARENTRIGHT2 };
		resultList[1] = { };
		resultList[2] = { PARENTRIGHT2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, PARENTLEFT2 }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT("3", selected)
		resultList[0] = { PARENTRIGHT3 };
		resultList[1] = { };
		resultList[2] = { PARENTRIGHT3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, PARENTLEFT3 }, selectedList[i]);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that ParentT(anyEmpty, selected)
		std::vector<Entity> emptyList = {
			{ STMT, "4" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q;
				q.addRelation(relation);
				q.addSelected(selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select a such that ParentT(selected, "2")
		resultList[0] = { PARENTLEFT1 };
		resultList[1] = { PARENTLEFT1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, PARENTRIGHT1 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT(selected, "3")
		resultList[0] = { PARENTLEFT1, PARENTLEFT2 };
		resultList[1] = { PARENTLEFT1, PARENTLEFT2 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, PARENTRIGHT2 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT(selected, "4")
		resultList[0] = { PARENTLEFT3 };
		resultList[1] = { };
		resultList[2] = { PARENTLEFT3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, PARENTRIGHT3 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that ParentT(selected, anyEmpty)
		emptyList = { { STMT, "1" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q;
				q.addRelation(relation);
				q.addSelected(selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
			}
		}
	}


	//Modifies_S Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesBooleanTrue) {
		RelType type = MODIFIES_S;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { WILD }, { WILD }));
		relations.push_back(RelRef(type, { STMT, MODIFIESLEFT1 }, { VARIABLE, MODIFIESRIGHT1 }));
		relations.push_back(RelRef(type, { STMT, MODIFIESLEFT2 }, { VARIABLE, MODIFIESRIGHT2 }));
		relations.push_back(RelRef(type, { STMT, MODIFIESLEFT1 }, { WILD }));
		relations.push_back(RelRef(type, { STMT, MODIFIESLEFT2 }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { VARIABLE, MODIFIESRIGHT1 }));
		relations.push_back(RelRef(type, { WILD }, { VARIABLE, MODIFIESRIGHT2 }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1;
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesBooleanFalse) {
		RelType type = MODIFIES_S;
		std::vector<Entity> variables;
		//Test false boolean equation
		std::vector<Entity> invalidLefts;
		invalidLefts.push_back({ STMT, "2" });
		invalidLefts.push_back({ STMT, "4" });
		invalidLefts.push_back({ STMT, "5" });
		invalidLefts.push_back({ STMT, "6" });
		invalidLefts.push_back({ STMT, "7" });
		invalidLefts.push_back({ STMT, "8" });
		invalidLefts.push_back({ STMT, "9" });
		invalidLefts.push_back({ STMT, "10" });
		invalidLefts.push_back({ STMT, "11" });
		invalidLefts.push_back({ STMT, "12" });
		for (unsigned int k = 0; k < invalidLefts.size(); k++) {
			for (unsigned int i = 0; i < allVariables.size(); i++) {
				for (unsigned int j = 0; j < allSelect.size(); j++) {
					Query q;
					q.addRelation(RelRef(type, invalidLefts[k], allVariables[i]));
					q.addSelected(allSelect[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		std::vector<Entity> invalidRight;
		invalidRight.push_back({ VARIABLE, z });

		for (unsigned int k = 0; k < invalidRight.size(); k++) {
			for (unsigned int i = 0; i < validConstantEntity.size(); i++) {
				for (unsigned int j = 0; j < allSelect.size(); j++) {
					Query q;
					q.addRelation(RelRef(type, validConstantEntity[i], invalidRight[k]));
					q.addSelected(allSelect[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesFilterEmpty) {
		RelType type = MODIFIES_S;
		std::vector<RelRef> relations;
		relations.push_back(RelRef(type, { STMT, "2" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "4" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "5" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "6" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "7" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "8" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "9" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "10" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "11" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "12" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, z }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesFilterNoCommonSynonymTrue) {
		RelType type = MODIFIES_S;
		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { STMT, MODIFIESLEFT1 }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, MODIFIESLEFT2 }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, MODIFIESRIGHT1 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { VARIABLE, MODIFIESRIGHT1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, MODIFIESRIGHT2 }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { VARIABLE, MODIFIESRIGHT2 }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesFilterNoCommonSynonymFalse) {
		RelType type = MODIFIES_S;
		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, commonSynonym1 });
		synonyms.push_back({ READ, commonSynonym1 });
		synonyms.push_back({ ASSIGN, commonSynonym1 });
		synonyms.push_back({ CALL, commonSynonym1 });

		for (unsigned int k = 0; k < synonyms.size(); k++) {
			for (unsigned int j = 0; j < allVariables.size(); j++) {
				RelRef relation(type, synonyms[k], allVariables[j]);
				for (unsigned int i = 0; i < allSelect.size(); i++) {
					Query q;
					q.addRelation(relation);
					q.addSelected(allSelect[i]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesFilterCommonSynonym) {
		RelType type = MODIFIES_S;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, commonSynonym1 });
		selectedList.push_back({ IF, commonSynonym1 });
		selectedList.push_back({ WHILE, commonSynonym1 });
		selectedList.push_back({ PRINT, commonSynonym1 });
		selectedList.push_back({ READ, commonSynonym1 });
		selectedList.push_back({ ASSIGN, commonSynonym1 });
		selectedList.push_back({ CALL, commonSynonym1 });

		//Test case for Select a such that Modifies_S(selected, a)
		std::list<std::string> result1 = { MODIFIESLEFT1, MODIFIESLEFT2 };
		std::list<std::string> result2 = { MODIFIESLEFT1 };
		std::list<std::string> result3 = { MODIFIESLEFT2 };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, Synonym{"a"} });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Modifies_S(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { WILD });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Modifies_S(selected, "x")
		resultList[0] = { MODIFIESLEFT1 };
		resultList[1] = { MODIFIESLEFT1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, MODIFIESRIGHT1 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Modifies_S(VARIABLE, "y")
		resultList[0] = { MODIFIESLEFT2 };
		resultList[1] = { };
		resultList[2] = { MODIFIESLEFT2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, MODIFIESRIGHT2 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Modifies_S(selected, anyEmpty)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, z });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
		}


		//Test case for Select a such that Modifies_S(a, selected)
		resultList[0] = { MODIFIESRIGHT1, MODIFIESRIGHT2 };
		resultList[1] = { MODIFIESRIGHT1 };
		resultList[2] = { MODIFIESRIGHT2 };

		Entity selected(VARIABLE, commonSynonym1);
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], selected);
			Query q;
			q.addRelation(relation);
			q.addSelected(selected);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]);
		}

		resultList[0] = { MODIFIESRIGHT1, MODIFIESRIGHT2 };
		//Test case for Select a such that Modifies_S(_, selected)
		RelRef relation(type, { WILD }, selected);
		Query q;
		q.addRelation(relation);
		q.addSelected(selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), resultList[0]);


		//Test case for Select a such that Modifies_S("1", selected)
		resultList[0] = { MODIFIESRIGHT1 };
		resultList[1] = { };
		resultList[2] = { };
		relation = RelRef(type, { STMT, MODIFIESLEFT1 }, selected);
		q = Query();
		q.addRelation(relation);
		q.addSelected(selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), resultList[0]);


		//Test case for Select a such that Modifies_S("3", selected)
		resultList[0] = { MODIFIESRIGHT2 };
		relation = RelRef(type, { STMT, MODIFIESLEFT2 }, selected);
		q = Query();
		q.addRelation(relation);
		q.addSelected(selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), resultList[0]);


		//Test case for remaining Select a such that Modifies_S(anyEmpty, selected)
		std::vector<Entity> emptyList = {
			{ STMT, "2" },{ STMT, "4" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};

		for (unsigned int j = 0; j < emptyList.size(); j++) {
			relation = RelRef(type, emptyList[j], selected);
			q = Query();
			q.addRelation(relation);
			q.addSelected(selected);
			EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << j + 1;
		}

	}

	//Uses_S Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryUsessBooleanTrue) {
		RelType type = USES_S;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { WILD }, { WILD }));
		relations.push_back(RelRef(type, { STMT, USESLEFT1 }, { VARIABLE, USESRIGHT1 }));
		relations.push_back(RelRef(type, { STMT, USESLEFT2 }, { VARIABLE, USESRIGHT2 }));
		relations.push_back(RelRef(type, { STMT, USESLEFT1 }, { WILD }));
		relations.push_back(RelRef(type, { STMT, USESLEFT2 }, { WILD }));
		relations.push_back(RelRef(type, { WILD }, { VARIABLE, USESRIGHT1 }));
		relations.push_back(RelRef(type, { WILD }, { VARIABLE, USESRIGHT2 }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1;
			}
		}
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryUsesBooleanFalse) {
		RelType type = USES_S;
		//Test false boolean equation
		std::vector<Entity> invalidLefts;
		invalidLefts.push_back({ STMT, "2" });
		invalidLefts.push_back({ STMT, "4" });
		invalidLefts.push_back({ STMT, "5" });
		invalidLefts.push_back({ STMT, "6" });
		invalidLefts.push_back({ STMT, "7" });
		invalidLefts.push_back({ STMT, "8" });
		invalidLefts.push_back({ STMT, "9" });
		invalidLefts.push_back({ STMT, "10" });
		invalidLefts.push_back({ STMT, "11" });
		invalidLefts.push_back({ STMT, "12" });
		for (unsigned int k = 0; k < invalidLefts.size(); k++) {
			for (unsigned int i = 0; i < allVariables.size(); i++) {
				for (unsigned int j = 0; j < allSelect.size(); j++) {
					Query q;
					q.addRelation(RelRef(type, invalidLefts[k], allVariables[i]));
					q.addSelected(allSelect[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		std::vector<Entity> invalidRight;
		invalidRight.push_back({ VARIABLE, z });

		for (unsigned int k = 0; k < invalidRight.size(); k++) {
			for (unsigned int i = 0; i < validConstantEntity.size(); i++) {
				for (unsigned int j = 0; j < allSelect.size(); j++) {
					Query q;
					q.addRelation(RelRef(type, validConstantEntity[i], invalidRight[k]));
					q.addSelected(allSelect[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryUsesFilterEmpty) {
		RelType type = USES_S;
		std::vector<RelRef> relations;
		relations.push_back(RelRef(type, { STMT, "2" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "4" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "5" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "6" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "7" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "8" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "9" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "10" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "11" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, "12" }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, z }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryUsesFilterNoCommonSynonymTrue) {
		RelType type = USES_S;
		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WILD }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { WILD }));
		relations.push_back(RelRef(type, { STMT, USESLEFT1 }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, USESLEFT2 }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, USESRIGHT1 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { VARIABLE, USESRIGHT1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, USESRIGHT2 }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { VARIABLE, USESRIGHT2 }));

		for (unsigned int i = 0; i < relations.size(); i++) {
			for (unsigned int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryUsesFilterNoCommonSynonymFalse) {
		RelType type = USES_S;
		std::vector<RelRef> relations;
		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, commonSynonym1 });
		synonyms.push_back({ READ, commonSynonym1 });
		synonyms.push_back({ ASSIGN, commonSynonym1 });
		synonyms.push_back({ CALL, commonSynonym1 });

		for (unsigned int k = 0; k < synonyms.size(); k++) {
			for (unsigned int j = 0; j < allVariables.size(); j++) {
				RelRef relation(type, synonyms[k], allVariables[j]);
				for (unsigned int i = 0; i < allSelect.size(); i++) {
					Query q;
					q.addRelation(relation);
					q.addSelected(allSelect[i]);
					EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryUsesFilterCommonSynonym) {
		RelType type = USES_S;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, commonSynonym1 });
		selectedList.push_back({ IF, commonSynonym1 });
		selectedList.push_back({ WHILE, commonSynonym1 });
		selectedList.push_back({ PRINT, commonSynonym1 });
		selectedList.push_back({ READ, commonSynonym1 });
		selectedList.push_back({ ASSIGN, commonSynonym1 });
		selectedList.push_back({ CALL, commonSynonym1 });

		//Test case for Select a such that USES_S(selected, a)
		std::list<std::string> result1 = { USESLEFT1, USESLEFT2 };
		std::list<std::string> result2 = { USESLEFT1 };
		std::list<std::string> result3 = { USESLEFT2 };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, Synonym{"a"} });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that USES_S(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { WILD });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that USES_S(selected, "x")
		resultList[0] = { USESLEFT1 };
		resultList[1] = { USESLEFT1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, USESRIGHT1 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that USES_S(VARIABLE, "y")
		resultList[0] = { USESLEFT2 };
		resultList[1] = { };
		resultList[2] = { USESLEFT2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, USESRIGHT2 });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that USES_S(selected, anyEmpty)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, z });
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1;
		}


		//Test case for Select a such that USES_S(a, selected)
		resultList[0] = { USESRIGHT1, USESRIGHT2 };
		resultList[1] = { USESRIGHT1 };
		resultList[2] = { USESRIGHT2 };

		Entity selected(VARIABLE, commonSynonym1);
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], selected);
			Query q;
			q.addRelation(relation);
			q.addSelected(selected);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]);
		}

		resultList[0] = { USESRIGHT1, USESRIGHT2 };
		//Test case for Select a such that USES_S(_, selected)
		RelRef relation(type, { WILD }, selected);
		Query q;
		q.addRelation(relation);
		q.addSelected(selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), resultList[0]);


		//Test case for Select a such that USES_S("1", selected)
		resultList[0] = { USESRIGHT1 };
		resultList[1] = { };
		resultList[2] = { };
		relation = RelRef(type, { STMT, USESLEFT1 }, selected);
		q = Query();
		q.addRelation(relation);
		q.addSelected(selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), resultList[0]);


		//Test case for Select a such that USES_S("3", selected)
		resultList[0] = { USESRIGHT2 };
		relation = RelRef(type, { STMT, USESLEFT2 }, selected);
		q = Query();
		q.addRelation(relation);
		q.addSelected(selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), resultList[0]);


		//Test case for remaining Select a such that USES_S(anyEmpty, selected)
		std::vector<Entity> emptyList = {
			{ STMT, "2" },{ STMT, "4" },{ STMT, "5" },{ STMT, "6" },{ STMT, "7" },
			{ STMT, "8" },{ STMT, "9" },{ STMT, "10" },{ STMT, "11" },{ STMT, "12" }
		};

		for (unsigned int j = 0; j < emptyList.size(); j++) {
			relation = RelRef(type, emptyList[j], selected);
			q = Query();
			q.addRelation(relation);
			q.addSelected(selected);
			EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << j + 1;
		}

	}
}