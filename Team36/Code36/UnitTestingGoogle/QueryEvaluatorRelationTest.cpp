#include "pch.h"

#include "QueryEvaluator.h"
#include "PKBAdapter.h"
#include "PKB.h"
#include "Common.h"
#include "ExprParser.h"

namespace UnitTesting {
	class QueryEvaluatorRelationTest : public testing::Test {
	protected:
		QueryEvaluatorRelationTest() {
			PKB::getInstance().resetCache();
			PKB::getInstance().addVariable({ x });
			PKB::getInstance().addVariable({ y });
			PKB::getInstance().addVariable({ x1 });
			PKB::getInstance().addVariable({ y1 });
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
			PKB::getInstance().addFollows(std::stoi(FOLLOW_LEFT1), std::stoi(FOLLOW_RIGHT1));
			PKB::getInstance().addFollows(std::stoi(FOLLOW_LEFT2), std::stoi(FOLLOW_RIGHT2));
			PKB::getInstance().addParent(std::stoi(PARENT_LEFT1), std::stoi(PARENT_RIGHT1));
			PKB::getInstance().addParent(std::stoi(PARENT_LEFT1), std::stoi(PARENT_RIGHT2));
			PKB::getInstance().addParent(std::stoi(PARENT_LEFT2), std::stoi(PARENT_RIGHT2));
			PKB::getInstance().addParent(std::stoi(PARENT_LEFT3), std::stoi(PARENT_RIGHT3));
			PKB::getInstance().addModifiesS(std::stoi(MODIFIES_LEFT1), MODIFIES_RIGHT1);
			PKB::getInstance().addModifiesS(std::stoi(MODIFIES_LEFT2), MODIFIES_RIGHT2);
			PKB::getInstance().addModifiesS(std::stoi(MODIFIES_LEFT3), MODIFIES_RIGHT3);
			PKB::getInstance().addModifiesS(std::stoi(MODIFIES_LEFT4), MODIFIES_RIGHT4);
			PKB::getInstance().addUsesS(std::stoi(USES_LEFT1), USES_RIGHT1_1);
			PKB::getInstance().addUsesS(std::stoi(USES_LEFT1), USES_RIGHT1_2);
			PKB::getInstance().addUsesS(std::stoi(USES_LEFT2), USES_RIGHT2_1);
			PKB::getInstance().addUsesS(std::stoi(USES_LEFT2), USES_RIGHT2_2);
			PKB::getInstance().addModifiesP(MODIFIESP_LEFT1, MODIFIESP_RIGHT1);
			PKB::getInstance().addModifiesP(MODIFIESP_LEFT2, MODIFIESP_RIGHT2);
			PKB::getInstance().addUsesP(USESP_LEFT1, USESP_RIGHT1);
			PKB::getInstance().addUsesP(USESP_LEFT2, USESP_RIGHT2);
			PKB::getInstance().addCallsP(CALLS_LEFT1, CALLS_RIGHT1);
			PKB::getInstance().addCallsP(CALLS_LEFT2, CALLS_RIGHT2);
			PKB::getInstance().addNext(std::stoi(NEXT_LEFT1), std::stoi(NEXT_RIGHT1));
			PKB::getInstance().addNext(std::stoi(NEXT_LEFT2), std::stoi(NEXT_RIGHT2));
			PKB::getInstance().generateFollowsT();
			PKB::getInstance().generateParentT();
			PKB::getInstance().generateCallsPT();
		}

		~QueryEvaluatorRelationTest() override {
			PKB::getInstance().resetCache();
		}

		std::vector<Entity> getInvalidConstant(std::vector<std::string> validStmt) {
			std::vector<Entity> invalid;
			for (Entity it : VALID_CONSTANT_STMT_ENTITY) {
				if (it.getType() == WILD) {
					continue;
				}
				bool isInvalid = true;
				for (std::string valid : validStmt) {
					if (it.getValue() == valid) {
						isInvalid = false;
					}
				}
				if (isInvalid) {
					invalid.push_back(it);
				}
			}
			return invalid;
		}
		Query initQuery(RelRef relation, Entity selected) {
			Query q;
			q.addRelation(relation);
			q.addSelected(selected);
			return q;
		}

		void validateRelations(std::vector<RelRef> relations) {
			for (unsigned int i = 0; i < relations.size(); i++) {
				for (unsigned int j = 0; j < ALL_SELECT.size(); j++) {
					Query q = initQuery(relations[i], ALL_SELECT[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q).front(), ALL_RESULT[j]) << "Error at results : " << i + 1 << " : " << j + 1;
					q = initQuery(relations[i], SELECT_BOOLEAN);
					EXPECT_EQ(evaluator.evaluateQuery(q).front(), BOOLEAN_TRUE_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		void validateEmptyRelations(std::vector<RelRef> relations) {
			for (unsigned int i = 0; i < relations.size(); i++) {
				for (unsigned int j = 0; j < ALL_SELECT.size(); j++) {
					Query q = initQuery(relations[i], ALL_SELECT[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
					q = initQuery(relations[i], SELECT_BOOLEAN);
					EXPECT_EQ(evaluator.evaluateQuery(q).front(), BOOLEAN_FALSE_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		void validateRelations(std::vector<RelRef> relations, std::vector<Entity> selected, std::vector<std::list<std::string>> results) {
			for (unsigned int i = 0; i < relations.size(); i++) {
				for (unsigned int j = 0; j < selected.size(); j++) {
					Query q = initQuery(relations[i], ALL_SELECT[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q).front(), results[j]) << "Error at results : " << i + 1 << " : " << j + 1;
					q = initQuery(relations[i], SELECT_BOOLEAN);
					EXPECT_EQ(evaluator.evaluateQuery(q).front(), BOOLEAN_TRUE_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		PKBAdapter pkb;
		QueryEvaluator evaluator;
		ExprParser expr_parser;

		const var_name x = "x";
		const var_name x1 = "x1";
		const var_name y = "y";
		const var_name y1 = "y1";
		const var_name z = "z";
		const constant c1 = 1;
		const constant c2 = 2;
		const constant c3 = 3;
		const std::string c1s = std::to_string(c1);
		const std::string c2s = std::to_string(c2);
		const std::string c3s = std::to_string(c3);
		const proc_name p1 = "main";
		const proc_name p2 = "sub";
		const proc_name p3 = "sub1";

		const std::string IF1 = "1";
		const std::string IF2 = "2";
		const std::string WHILE1 = "3";
		const std::string	WHILE2 = "4";
		const std::string READ1 = "5";
		const std::string READ2 = "6";
		const std::string PRINT1 = "7";
		const std::string PRINT2 = "8";
		const std::string ASSIGN1 = "9";
		const std::string ASSIGN2 = "10";
		const std::string CALL1 = "11";
		const std::string CALL2 = "12";

		const std::string FOLLOW_LEFT1 = "1";
		const std::string FOLLOW_LEFT2 = "2";
		const std::string FOLLOW_RIGHT1 = "2";
		const std::string FOLLOW_RIGHT2 = "3";

		const std::vector<std::string> FOLLOW_LEFTS = { FOLLOW_LEFT1, FOLLOW_LEFT2 };
		const std::vector<std::string> FOLLOW_RIGHTS = { FOLLOW_RIGHT1, FOLLOW_RIGHT2 };

		const std::string PARENT_LEFT1 = "1";
		const std::string PARENT_LEFT2 = "2";
		const std::string PARENT_LEFT3 = "3";
		const std::string PARENT_RIGHT1 = "2";
		const std::string PARENT_RIGHT2 = "3";
		const std::string PARENT_RIGHT3 = "4";

		const std::vector<std::string> PARENT_LEFTS = { PARENT_LEFT1, PARENT_LEFT2, PARENT_LEFT3 };
		const std::vector<std::string> PARENT_RIGHTS = { PARENT_RIGHT1, PARENT_RIGHT2, PARENT_RIGHT3 };

		const std::string MODIFIES_LEFT1 = "1";
		const std::string MODIFIES_LEFT2 = "3";
		const std::string MODIFIES_RIGHT1 = x;
		const std::string MODIFIES_RIGHT2 = y;

		const std::string MODIFIES_LEFT3 = ASSIGN1;
		const std::string MODIFIES_LEFT4 = ASSIGN2;
		const std::string MODIFIES_RIGHT3 = x;
		const std::string MODIFIES_RIGHT4 = y;

		const std::vector<std::string> MODIFIES_LEFTS = { MODIFIES_LEFT1, MODIFIES_LEFT2, MODIFIES_LEFT3, MODIFIES_LEFT4 };
		const std::vector<std::string> MODIFIES_RIGHTS = { MODIFIES_RIGHT1, MODIFIES_RIGHT2, MODIFIES_RIGHT3, MODIFIES_RIGHT4 };

		const std::string USES_LEFT1 = "1";
		const std::string USES_LEFT2 = "3";
		const std::string USES_RIGHT1_1 = y;
		const std::string USES_RIGHT1_2 = y1;
		const std::string USES_RIGHT2_1 = x;
		const std::string USES_RIGHT2_2 = x1;

		const std::vector<std::string> USES_LEFTS = { USES_LEFT1, USES_LEFT2 };
		const std::vector<std::string> USES_RIGHTS = { USES_RIGHT1_1, USES_RIGHT1_2, USES_RIGHT2_1, USES_RIGHT2_2 };

		const std::string USESP_LEFT1 = p1;
		const std::string USESP_LEFT2 = p2;
		const std::string USESP_RIGHT1 = y;
		const std::string USESP_RIGHT2 = x;

		const std::vector<std::string> USESP_LEFTS = { USESP_LEFT1, USESP_LEFT2 };
		const std::vector<std::string> USESP_RIGHTS = { USESP_RIGHT1, USESP_RIGHT2 };

		const std::string CALLS_LEFT1 = p1;
		const std::string CALLS_LEFT2 = p2;
		const std::string CALLS_RIGHT1 = p2;
		const std::string CALLS_RIGHT2 = p3;

		const std::vector<std::string> CALLS_LEFTS = { CALLS_LEFT1, CALLS_LEFT2 };
		const std::vector<std::string> CALLS_RIGHTS = { CALLS_RIGHT1, CALLS_RIGHT2 };

		const std::string MODIFIESP_LEFT1 = p1;
		const std::string MODIFIESP_LEFT2 = p2;
		const std::string MODIFIESP_RIGHT1 = y;
		const std::string MODIFIESP_RIGHT2 = x;

		const std::vector<std::string> MODIFIESP_LEFTS = { MODIFIESP_LEFT1, MODIFIESP_LEFT2 };
		const std::vector<std::string> MODIFIESP_RIGHTS = { MODIFIESP_RIGHT1, MODIFIESP_RIGHT2 };

		const std::string NEXT_LEFT1 = "1";
		const std::string NEXT_LEFT2 = "2";
		const std::string NEXT_RIGHT1 = "2";
		const std::string NEXT_RIGHT2 = "3";

		const std::vector<std::string> NEXT_LEFTS = { NEXT_LEFT1, NEXT_LEFT2 };
		const std::vector<std::string> NEXT_RIGHTS = { NEXT_RIGHT1, NEXT_RIGHT2 };

		const std::list<std::string> STMTS = { IF1, IF2, WHILE1, WHILE2, READ1, READ2,
			PRINT1, PRINT2, ASSIGN1, ASSIGN2, CALL1, CALL2 };

		const Synonym COMMON_SYNONYM1 = { "cs1" };
		const Synonym COMMON_SYNONYM2 = { "cs2" };

		// select v
		const std::list<std::string> ALL_VARIABLE = { x, y, x1, y1, z };
		const Entity SELECT_VARIABLE = { VARIABLE, COMMON_SYNONYM1 };
		// select c
		const std::list<std::string> ALL_CONSTANT = { c1s, c2s, c3s };
		const Entity SELECT_CONSTANT = { CONSTANT, COMMON_SYNONYM1 };
		// select p
		const std::list<std::string> ALL_PROCEDURE = { p1, p2, p3 };
		const Entity SELECT_PROCEDURE = { PROCEDURE, COMMON_SYNONYM1 };
		// select s
		const std::list<std::string> ALL_STMT = STMTS;
		const Entity SELECT_STMT = { STMT, COMMON_SYNONYM1 };
		// select ifs
		const std::list<std::string> ALL_IF = { IF1, IF2 };
		const Entity SELECT_IF = { IF, COMMON_SYNONYM1 };
		// select w
		const std::list<std::string> ALL_WHILE = { WHILE1, WHILE2 };
		const Entity SELECT_WHILE = { WHILE, COMMON_SYNONYM1 };
		// select read
		const std::list<std::string> ALL_READ = { READ1, READ2 };
		const Entity SELECT_READ = { READ, COMMON_SYNONYM1 };
		// select print
		const std::list<std::string> ALL_PRINT = { PRINT1, PRINT2 };
		const Entity SELECT_PRINT = { PRINT, COMMON_SYNONYM1 };
		// select assign
		const std::list<std::string> ALL_ASSIGN = { ASSIGN1, ASSIGN2 };
		const Entity SELECT_ASSIGN = { ASSIGN, COMMON_SYNONYM1 };
		// select call
		const std::list<std::string> ALL_CALL = { CALL1, CALL2 };
		const Entity SELECT_CALL = { CALL, COMMON_SYNONYM1 };

		// select Boolean
		const Entity SELECT_BOOLEAN = { BOOLEAN };

		const std::vector<std::list<std::string>> ALL_RESULT = {
			ALL_VARIABLE , ALL_CONSTANT , ALL_PROCEDURE, ALL_STMT, ALL_IF,
			ALL_WHILE, ALL_READ, ALL_PRINT, ALL_ASSIGN, ALL_CALL };

		const std::vector<Entity> ALL_SELECT = {
			SELECT_VARIABLE , SELECT_CONSTANT , SELECT_PROCEDURE, SELECT_STMT, SELECT_IF,
			SELECT_WHILE, SELECT_READ, SELECT_PRINT, SELECT_ASSIGN, SELECT_CALL };

		const std::list<std::string> EMPTY_RESULT = {};
		const std::list<std::string> BOOLEAN_TRUE_RESULT = { BOOLEAN_TRUE };
		const std::list<std::string> BOOLEAN_FALSE_RESULT = { BOOLEAN_FALSE };

		const Entity WILD_CARD = { WILD };

		const std::vector<Entity> VALID_CONSTANT_STMT_ENTITY = { WILD_CARD,
			{STMT, "1"}, {STMT, "2"}, {STMT, "3"}, {STMT, "4"}, {STMT, "5"}, {STMT, "6"},
			{STMT, "7"}, {STMT, "8"}, {STMT, "9"}, {STMT, "10"}, {STMT, "11"}, {STMT, "12"}
		};

		std::vector<Entity> ALL_VARIABLES = { { VARIABLE, x }, { VARIABLE, y }, { VARIABLE, x1 }, { VARIABLE, y1 }, { VARIABLE, z } };
	};

	//Evaluator Select statement without any relation/Pattern
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryRaw) {
		for (unsigned int i = 0; i < ALL_SELECT.size(); i++) {
			Query q;
			Entity selected = ALL_SELECT[i];
			q.addSelected(selected);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), ALL_RESULT[i]) << "Error at results : " << i + 1;
		}
	}

	//Follow Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryFollowBooleanTrue) {
		RelType type = FOLLOWS;
		std::string left1 = FOLLOW_LEFT1;
		std::string left2 = FOLLOW_LEFT2;
		std::string right1 = FOLLOW_RIGHT1;
		std::string right2 = FOLLOW_RIGHT2;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, WILD_CARD, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right2 }));
		relations.push_back(RelRef(type, { STMT, left1 }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left2 }, WILD_CARD));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, right1 }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, right2 }));

		validateRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryFollowBooleanFalse) {
		RelType type = FOLLOWS;
		std::vector<std::string> lefts = FOLLOW_LEFTS;
		std::vector<std::string> rights = FOLLOW_RIGHTS;
		std::string left1 = FOLLOW_LEFT1;
		std::string left2 = FOLLOW_LEFT2;
		std::string right1 = FOLLOW_RIGHT1;
		std::string right2 = FOLLOW_RIGHT2;

		std::vector<RelRef> relations;
		//Test false boolean equation
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right2 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right1 }));

		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);
		for (unsigned int k = 0; k < invalidLefts.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, invalidLefts[k], VALID_CONSTANT_STMT_ENTITY[i]));
			}
		}

		std::vector<Entity> invalidRight = getInvalidConstant(rights);
		for (unsigned int k = 0; k < invalidRight.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k]));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryFollowFilterEmpty) {
		RelType type = FOLLOWS;
		std::vector<RelRef> relations;
		std::vector<std::string> lefts = FOLLOW_LEFTS;
		std::vector<std::string> rights = FOLLOW_RIGHTS;

		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);
		for (Entity it : invalidLefts) {
			relations.push_back(RelRef(type, it, { STMT, Synonym{"a"} }));
		}

		std::vector<Entity> invalidRight = getInvalidConstant(rights);
		for (Entity it : invalidRight) {
			relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, it));
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryFollowFilterNoCommonSynonymTrue) {
		RelType type = FOLLOWS;
		std::vector<RelRef> relations;
		std::string left1 = FOLLOW_LEFT1;
		std::string left2 = FOLLOW_LEFT2;
		std::string right1 = FOLLOW_RIGHT1;
		std::string right2 = FOLLOW_RIGHT2;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { IF, Synonym{"b"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WHILE, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { IF, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WHILE, Synonym{"b"} }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, WILD_CARD, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, WILD_CARD, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left1 }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right2 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, right2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryFollowFilterNoCommonSynonymFalse) {
		RelType type = FOLLOWS;

		std::vector<std::string> lefts = FOLLOW_LEFTS;
		std::vector<std::string> rights = FOLLOW_RIGHTS;

		std::vector<RelRef> relations;
		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, COMMON_SYNONYM1 });
		synonyms.push_back({ READ, COMMON_SYNONYM1 });
		synonyms.push_back({ ASSIGN, COMMON_SYNONYM1 });
		synonyms.push_back({ CALL, COMMON_SYNONYM1 });

		for (unsigned int k = 0; k < synonyms.size(); k++) {
			for (unsigned int j = 0; j < synonyms.size(); j++) {
				relations.push_back(RelRef(type, synonyms[k], synonyms[j]));
			}
		}

		//Empty result for non-matching header for single column
		for (unsigned int k = 0; k < synonyms.size(); k++) {
			relations.push_back(RelRef(type, WILD_CARD, synonyms[k]));
			relations.push_back(RelRef(type, { synonyms[k] }, WILD_CARD));
			relations.push_back(RelRef(type, { synonyms[k] }, WILD_CARD));

			for (auto valid : lefts) {
				relations.push_back(RelRef(type, { STMT, valid }, { synonyms[k] }));
			}

			for (auto valid : rights) {
				relations.push_back(RelRef(type, { synonyms[k] }, { STMT, valid }));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryFollowFilterCommonSynonym) {
		RelType type = FOLLOWS;
		std::vector<std::string> lefts = FOLLOW_LEFTS;
		std::vector<std::string> rights = FOLLOW_RIGHTS;
		std::string left1 = FOLLOW_LEFT1;
		std::string left2 = FOLLOW_LEFT2;
		std::string right1 = FOLLOW_RIGHT1;
		std::string right2 = FOLLOW_RIGHT2;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, COMMON_SYNONYM1 });
		selectedList.push_back({ IF, COMMON_SYNONYM1 });
		selectedList.push_back({ WHILE, COMMON_SYNONYM1 });
		selectedList.push_back({ PRINT, COMMON_SYNONYM1 });
		selectedList.push_back({ READ, COMMON_SYNONYM1 });
		selectedList.push_back({ ASSIGN, COMMON_SYNONYM1 });
		selectedList.push_back({ CALL, COMMON_SYNONYM1 });

		//Test case for Select selected such that Follow(selected, a)
		std::list<std::string> result1 = { left1, left2 };
		std::list<std::string> result2 = { left1, left2 };
		std::list<std::string> result3 = { };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, Synonym{"a"} });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Follow(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], WILD_CARD);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Follow(a, selected)
		resultList[0] = { right1, right2 };
		resultList[1] = { right1 };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Follow(_, selected)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, WILD_CARD, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Follow("1", selected)
		resultList[0] = { right1 };
		resultList[1] = { right1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left1 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Follow("2", selected)
		resultList[0] = { right2 };
		resultList[1] = { };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left2 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that Follow(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select selected such that Follow(selected, "2")
		resultList[0] = { left1 };
		resultList[1] = { left1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Follow(selected, "3")
		resultList[0] = { left2 };
		resultList[1] = { left2 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that Follow(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}
	}

	//FollowT Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryFollowTBooleanTrue) {
		RelType type = FOLLOWS_T;
		std::string left1 = FOLLOW_LEFT1;
		std::string left2 = FOLLOW_LEFT2;
		std::string right1 = FOLLOW_RIGHT1;
		std::string right2 = FOLLOW_RIGHT2;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, WILD_CARD, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right2 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right2 }));
		relations.push_back(RelRef(type, { STMT, left1 }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left2 }, WILD_CARD));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, right1 }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, right2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryFollowTBooleanFalse) {
		RelType type = FOLLOWS_T;
		std::vector<std::string> lefts = FOLLOW_LEFTS;
		std::vector<std::string> rights = FOLLOW_RIGHTS;
		std::string left1 = FOLLOW_LEFT1;
		std::string left2 = FOLLOW_LEFT2;
		std::string right1 = FOLLOW_RIGHT1;
		std::string right2 = FOLLOW_RIGHT2;

		std::vector<RelRef> relations;
		//Test false boolean equation
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right1 }));

		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);
		for (unsigned int k = 0; k < invalidLefts.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, invalidLefts[k], VALID_CONSTANT_STMT_ENTITY[i]));
			}
		}

		std::vector<Entity> invalidRight = getInvalidConstant(rights);
		for (unsigned int k = 0; k < invalidRight.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k]));
			}
		}

		validateEmptyRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryFollowTFilterEmpty) {
		RelType type = FOLLOWS_T;
		std::vector<RelRef> relations;
		std::vector<std::string> lefts = FOLLOW_LEFTS;
		std::vector<std::string> rights = FOLLOW_RIGHTS;

		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);
		for (Entity it : invalidLefts) {
			relations.push_back(RelRef(type, it, { STMT, Synonym{"a"} }));
		}

		std::vector<Entity> invalidRight = getInvalidConstant(rights);
		for (Entity it : invalidRight) {
			relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, it));
		}

		validateEmptyRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryFollowTFilterNoCommonSynonymTrue) {
		RelType type = FOLLOWS_T;
		std::string left1 = FOLLOW_LEFT1;
		std::string left2 = FOLLOW_LEFT2;
		std::string right1 = FOLLOW_RIGHT1;
		std::string right2 = FOLLOW_RIGHT2;

		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { IF, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { IF, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WHILE, Synonym{"b"} }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, WILD_CARD, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, WILD_CARD, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left1 }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left1 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right2 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, right2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryFollowTFilterNoCommonSynonymFalse) {
		RelType type = FOLLOWS_T;
		std::vector<std::string> lefts = FOLLOW_LEFTS;
		std::vector<std::string> rights = FOLLOW_RIGHTS;

		std::vector<RelRef> relations;
		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, COMMON_SYNONYM1 });
		synonyms.push_back({ READ, COMMON_SYNONYM1 });
		synonyms.push_back({ ASSIGN, COMMON_SYNONYM1 });
		synonyms.push_back({ CALL, COMMON_SYNONYM1 });

		for (unsigned int k = 0; k < synonyms.size(); k++) {
			for (unsigned int j = 0; j < synonyms.size(); j++) {
				relations.push_back(RelRef(type, synonyms[k], synonyms[j]));
			}
		}

		//Empty result for non-matching header for single column
		for (unsigned int k = 0; k < synonyms.size(); k++) {
			relations.push_back(RelRef(type, WILD_CARD, synonyms[k]));
			relations.push_back(RelRef(type, { synonyms[k] }, WILD_CARD));
			relations.push_back(RelRef(type, { synonyms[k] }, WILD_CARD));

			for (auto valid : lefts) {
				relations.push_back(RelRef(type, { STMT, valid }, { synonyms[k] }));
			}

			for (auto valid : rights) {
				relations.push_back(RelRef(type, { synonyms[k] }, { STMT, valid }));
			}
		}

		validateEmptyRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryFollowTFilterCommonSynonym) {
		RelType type = FOLLOWS_T;
		std::vector<std::string> lefts = FOLLOW_LEFTS;
		std::vector<std::string> rights = FOLLOW_RIGHTS;
		std::string left1 = FOLLOW_LEFT1;
		std::string left2 = FOLLOW_LEFT2;
		std::string right1 = FOLLOW_RIGHT1;
		std::string right2 = FOLLOW_RIGHT2;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, COMMON_SYNONYM1 });
		selectedList.push_back({ IF, COMMON_SYNONYM1 });
		selectedList.push_back({ WHILE, COMMON_SYNONYM1 });
		selectedList.push_back({ PRINT, COMMON_SYNONYM1 });
		selectedList.push_back({ READ, COMMON_SYNONYM1 });
		selectedList.push_back({ ASSIGN, COMMON_SYNONYM1 });
		selectedList.push_back({ CALL, COMMON_SYNONYM1 });

		//Test case for Select selected such that FollowT(selected, a)
		std::list<std::string> result1 = { FOLLOW_LEFT1, FOLLOW_LEFT2 };
		std::list<std::string> result2 = { FOLLOW_LEFT1, FOLLOW_LEFT2 };
		std::list<std::string> result3 = { };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, Synonym{"a"} });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that FollowT(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], WILD_CARD);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that FollowT(a, selected)
		resultList[0] = { right1, right2 };
		resultList[1] = { right1 };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that FollowT(_, selected)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, WILD_CARD, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that FollowT("1", selected)
		resultList[0] = { right1, right2 };
		resultList[1] = { right1 };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left1 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that FollowT("2", selected)
		resultList[0] = { right2 };
		resultList[1] = { };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left2 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that FollowT(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select selected such that FollowT(selected, "2")
		resultList[0] = { left1 };
		resultList[1] = { left1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that FollowT(selected, "3")
		resultList[0] = { left1, left2 };
		resultList[1] = { left1, left2 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that FollowT(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}
	}

	//Parent Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryParentBooleanTrue) {
		RelType type = PARENT;
		std::string left1 = PARENT_LEFT1;
		std::string left2 = PARENT_LEFT2;
		std::string left3 = PARENT_LEFT3;
		std::string right1 = PARENT_RIGHT1;
		std::string right2 = PARENT_RIGHT2;
		std::string right3 = PARENT_RIGHT3;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, WILD_CARD, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right2 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right2 }));
		relations.push_back(RelRef(type, { STMT, left3 }, { STMT, right3 }));
		relations.push_back(RelRef(type, { STMT, left1 }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left2 }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left3 }, WILD_CARD));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, right1 }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, right2 }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, right3 }));

		validateRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryParentBooleanFalse) {
		RelType type = PARENT;
		std::vector<RelRef> relations;
		std::vector<std::string> lefts = PARENT_LEFTS;
		std::vector<std::string> rights = PARENT_RIGHTS;
		std::string left1 = PARENT_LEFT1;
		std::string left2 = PARENT_LEFT2;
		std::string left3 = PARENT_LEFT3;
		std::string right1 = PARENT_RIGHT1;
		std::string right2 = PARENT_RIGHT2;
		std::string right3 = PARENT_RIGHT3;

		//Test false boolean equation
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right3 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right3 }));
		relations.push_back(RelRef(type, { STMT, left3 }, { STMT, right2 }));

		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);

		for (unsigned int k = 0; k < invalidLefts.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, invalidLefts[k], VALID_CONSTANT_STMT_ENTITY[i]));
			}
		}

		std::vector<Entity> invalidRight = getInvalidConstant(rights);

		for (unsigned int k = 0; k < invalidRight.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k]));
			}
		}
		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryParentFilterEmpty) {
		RelType type = PARENT;
		std::vector<RelRef> relations;
		std::vector<std::string> lefts = PARENT_LEFTS;
		std::vector<std::string> rights = PARENT_RIGHTS;

		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);
		for (Entity it : invalidLefts) {
			relations.push_back(RelRef(type, it, { STMT, Synonym{"a"} }));
		}

		std::vector<Entity> invalidRight = getInvalidConstant(rights);
		for (Entity it : invalidRight) {
			relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, it));
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryParentFilterNoCommonSynonymTrue) {
		RelType type = PARENT;
		std::vector<RelRef> relations;

		std::string left1 = PARENT_LEFT1;
		std::string left2 = PARENT_LEFT2;
		std::string left3 = PARENT_LEFT3;
		std::string right1 = PARENT_RIGHT1;
		std::string right2 = PARENT_RIGHT2;
		std::string right3 = PARENT_RIGHT3;

		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { IF, Synonym{"b"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WHILE, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { IF, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WHILE, Synonym{"b"} }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { WHILE, Synonym{"b"} }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, WILD_CARD, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, WILD_CARD, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left1 }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left1 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left3 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left3 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right2 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, right2 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right3 }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { STMT, right3 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryParentFilterNoCommonSynonymFalse) {
		RelType type = PARENT;
		std::vector<std::string> lefts = PARENT_LEFTS;
		std::vector<std::string> rights = PARENT_RIGHTS;

		std::vector<RelRef> relations;
		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, COMMON_SYNONYM1 });
		synonyms.push_back({ READ, COMMON_SYNONYM1 });
		synonyms.push_back({ ASSIGN, COMMON_SYNONYM1 });
		synonyms.push_back({ CALL, COMMON_SYNONYM1 });

		for (unsigned int k = 0; k < synonyms.size(); k++) {
			for (unsigned int j = 0; j < synonyms.size(); j++) {
				relations.push_back(RelRef(type, synonyms[k], synonyms[j]));
			}
		}

		//Empty result for non-matching header for single column
		for (unsigned int k = 0; k < synonyms.size(); k++) {
			relations.push_back(RelRef(type, WILD_CARD, synonyms[k]));
			relations.push_back(RelRef(type, { synonyms[k] }, WILD_CARD));
			relations.push_back(RelRef(type, { synonyms[k] }, WILD_CARD));

			for (auto valid : lefts) {
				relations.push_back(RelRef(type, { STMT, valid }, { synonyms[k] }));
			}

			for (auto valid : rights) {
				relations.push_back(RelRef(type, { synonyms[k] }, { STMT, valid }));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryParentFilterCommonSynonym) {
		RelType type = PARENT;

		std::vector<std::string> lefts = PARENT_LEFTS;
		std::vector<std::string> rights = PARENT_RIGHTS;
		std::string left1 = PARENT_LEFT1;
		std::string left2 = PARENT_LEFT2;
		std::string left3 = PARENT_LEFT3;
		std::string right1 = PARENT_RIGHT1;
		std::string right2 = PARENT_RIGHT2;
		std::string right3 = PARENT_RIGHT3;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, COMMON_SYNONYM1 });
		selectedList.push_back({ IF, COMMON_SYNONYM1 });
		selectedList.push_back({ WHILE, COMMON_SYNONYM1 });
		selectedList.push_back({ PRINT, COMMON_SYNONYM1 });
		selectedList.push_back({ READ, COMMON_SYNONYM1 });
		selectedList.push_back({ ASSIGN, COMMON_SYNONYM1 });
		selectedList.push_back({ CALL, COMMON_SYNONYM1 });

		//Test case for Select selected such that Parent(selected, a)
		std::list<std::string> result1 = { left1, left2, left3 };
		std::list<std::string> result2 = { left1, left2 };
		std::list<std::string> result3 = { left3 };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, Synonym{"a"} });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Parent(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], WILD_CARD);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Parent(a, selected)
		resultList[0] = { right1, right2, right3 };
		resultList[1] = { right1 };
		resultList[2] = { right2, right3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Parent(_, selected)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, WILD_CARD, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Parent("1", selected)
		resultList[0] = { right1, right2 };
		resultList[1] = { right1 };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left1 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Parent("2", selected)
		resultList[0] = { right2 };
		resultList[1] = { };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left2 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Parent("3", selected)
		resultList[0] = { right3 };
		resultList[1] = { };
		resultList[2] = { right3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left3 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that Parent(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);

		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select selected such that Parent(selected, "2")
		resultList[0] = { left1 };
		resultList[1] = { left1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Parent(selected, "3")
		resultList[0] = { left1, left2 };
		resultList[1] = { left1, left2 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Parent(selected, "4")
		resultList[0] = { left3 };
		resultList[1] = { };
		resultList[2] = { left3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right3 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that Parent(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);

		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}
	}

	//ParentT Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryParentTBooleanTrue) {
		RelType type = PARENT_T;
		std::string left1 = PARENT_LEFT1;
		std::string left2 = PARENT_LEFT2;
		std::string left3 = PARENT_LEFT3;
		std::string right1 = PARENT_RIGHT1;
		std::string right2 = PARENT_RIGHT2;
		std::string right3 = PARENT_RIGHT3;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, WILD_CARD, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right2 }));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right3 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right2 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right3 }));
		relations.push_back(RelRef(type, { STMT, left3 }, { STMT, right3 }));
		relations.push_back(RelRef(type, { STMT, left1 }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left2 }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left3 }, WILD_CARD));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, right1 }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, right2 }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, right3 }));

		validateRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryParentTBooleanFalse) {
		RelType type = PARENT_T;
		std::vector<RelRef> relations;
		std::vector<std::string> lefts = PARENT_LEFTS;
		std::vector<std::string> rights = PARENT_RIGHTS;
		//Test false boolean equation

		relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "2" }));
		relations.push_back(RelRef(type, { STMT, "3" }, { STMT, "3" }));

		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);

		for (unsigned int k = 0; k < invalidLefts.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back((RelRef(type, invalidLefts[k], VALID_CONSTANT_STMT_ENTITY[i])));
			}
		}

		std::vector<Entity> invalidRight = getInvalidConstant(rights);

		for (unsigned int k = 0; k < invalidRight.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back((RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k])));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryParentTFilterEmpty) {
		RelType type = PARENT_T;
		std::vector<RelRef> relations;
		std::vector<std::string> lefts = PARENT_LEFTS;
		std::vector<std::string> rights = PARENT_RIGHTS;

		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);
		for (Entity it : invalidLefts) {
			relations.push_back(RelRef(type, it, { STMT, Synonym{"a"} }));
		}

		std::vector<Entity> invalidRight = getInvalidConstant(rights);
		for (Entity it : invalidRight) {
			relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, it));
		}

		validateEmptyRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryParentTFilterNoCommonSynonymTrue) {
		RelType type = PARENT_T;
		std::string left1 = PARENT_LEFT1;
		std::string left2 = PARENT_LEFT2;
		std::string left3 = PARENT_LEFT3;
		std::string right1 = PARENT_RIGHT1;
		std::string right2 = PARENT_RIGHT2;
		std::string right3 = PARENT_RIGHT3;

		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { IF, Synonym{"b"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WHILE, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { IF, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WHILE, Synonym{"b"} }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { WHILE, Synonym{"b"} }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, WILD_CARD, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, WILD_CARD, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left1 }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left1 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left3 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left3 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right2 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, right2 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right3 }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { STMT, right3 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, right3 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryParentTFilterNoCommonSynonymFalse) {
		RelType type = PARENT_T;
		std::vector<std::string> lefts = PARENT_LEFTS;
		std::vector<std::string> rights = PARENT_RIGHTS;

		std::vector<RelRef> relations;
		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, COMMON_SYNONYM1 });
		synonyms.push_back({ READ, COMMON_SYNONYM1 });
		synonyms.push_back({ ASSIGN, COMMON_SYNONYM1 });
		synonyms.push_back({ CALL, COMMON_SYNONYM1 });

		for (unsigned int k = 0; k < synonyms.size(); k++) {
			for (unsigned int j = 0; j < synonyms.size(); j++) {
				relations.push_back(RelRef(type, synonyms[k], synonyms[j]));
			}
		}

		//Empty result for non-matching header for single column
		for (unsigned int k = 0; k < synonyms.size(); k++) {
			relations.push_back(RelRef(type, WILD_CARD, synonyms[k]));
			relations.push_back(RelRef(type, { synonyms[k] }, WILD_CARD));
			relations.push_back(RelRef(type, { synonyms[k] }, WILD_CARD));

			for (auto valid : lefts) {
				relations.push_back(RelRef(type, { STMT, valid }, { synonyms[k] }));
			}

			for (auto valid : rights) {
				relations.push_back(RelRef(type, { synonyms[k] }, { STMT, valid }));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryParentTFilterCommonSynonym) {
		RelType type = PARENT_T;
		std::vector<std::string> lefts = PARENT_LEFTS;
		std::vector<std::string> rights = PARENT_RIGHTS;
		std::string left1 = PARENT_LEFT1;
		std::string left2 = PARENT_LEFT2;
		std::string left3 = PARENT_LEFT3;
		std::string right1 = PARENT_RIGHT1;
		std::string right2 = PARENT_RIGHT2;
		std::string right3 = PARENT_RIGHT3;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, COMMON_SYNONYM1 });
		selectedList.push_back({ IF, COMMON_SYNONYM1 });
		selectedList.push_back({ WHILE, COMMON_SYNONYM1 });
		selectedList.push_back({ PRINT, COMMON_SYNONYM1 });
		selectedList.push_back({ READ, COMMON_SYNONYM1 });
		selectedList.push_back({ ASSIGN, COMMON_SYNONYM1 });
		selectedList.push_back({ CALL, COMMON_SYNONYM1 });

		//Test case for Select selected such that ParentT(selected, a)
		std::list<std::string> result1 = { left1, left2, left3 };
		std::list<std::string> result2 = { left1, left2 };
		std::list<std::string> result3 = { left3 };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, Synonym{"a"} });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that ParentT(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], WILD_CARD);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that ParentT(a, selected)
		resultList[0] = { right1, right2, right3 };
		resultList[1] = { right1 };
		resultList[2] = { right2, right3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that ParentT(_, selected)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, WILD_CARD, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that ParentT("1", selected)
		resultList[0] = { right1, right2, right3 };
		resultList[1] = { right1 };
		resultList[2] = { right2, right3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left1 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that ParentT("2", selected)
		resultList[0] = { right2, right3 };
		resultList[1] = { };
		resultList[2] = { right2, right3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left2 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that ParentT("3", selected)
		resultList[0] = { right3 };
		resultList[1] = { };
		resultList[2] = { right3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left3 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that ParentT(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select selected such that ParentT(selected, "2")
		resultList[0] = { left1 };
		resultList[1] = { left1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that ParentT(selected, "3")
		resultList[0] = { left1, left2 };
		resultList[1] = { left1, left2 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that ParentT(selected, "4")
		resultList[0] = { left1, left2, left3 };
		resultList[1] = { left1, left2 };
		resultList[2] = { left3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right3 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that ParentT(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}
	}

	//Modifies_S Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryModifiesBooleanTrue) {
		RelType type = MODIFIES_S;
		std::string left1 = MODIFIES_LEFT1;
		std::string left2 = MODIFIES_LEFT2;
		std::string right1 = MODIFIES_RIGHT1;
		std::string right2 = MODIFIES_RIGHT2;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { STMT, left1 }, { VARIABLE, right1 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { VARIABLE, right2 }));
		relations.push_back(RelRef(type, { STMT, left1 }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left2 }, WILD_CARD));

		validateRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryModifiesBooleanFalse) {
		RelType type = MODIFIES_S;
		std::vector<Entity> variables;
		//Test false boolean equation
		std::vector<std::string> lefts = MODIFIES_LEFTS;

		std::vector<RelRef> relations;

		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);

		for (unsigned int k = 0; k < invalidLefts.size(); k++) {
			for (unsigned int i = 0; i < ALL_VARIABLES.size(); i++) {
				relations.push_back((RelRef(type, invalidLefts[k], ALL_VARIABLES[i])));
			}
		}

		std::vector<Entity> invalidRight;
		invalidRight.push_back({ VARIABLE, z });

		for (unsigned int k = 0; k < invalidRight.size(); k++) {
			for (unsigned int i = 1; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k]));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryModifiesFilterEmpty) {
		RelType type = MODIFIES_S;

		std::vector<std::string> lefts = MODIFIES_LEFTS;

		std::vector<RelRef> relations;
		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);
		for (Entity it : invalidLefts) {
			relations.push_back(RelRef(type, it, { STMT, Synonym{"a"} }));
		}
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, z }));

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryModifiesFilterNoCommonSynonymTrue) {
		RelType type = MODIFIES_S;

		std::string left1 = MODIFIES_LEFT1;
		std::string left2 = MODIFIES_LEFT2;
		std::string right1 = MODIFIES_RIGHT1;
		std::string right2 = MODIFIES_RIGHT2;

		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, right1 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { VARIABLE, right1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, right2 }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { VARIABLE, right2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryModifiesFilterNoCommonSynonymFalse) {
		RelType type = MODIFIES_S;

		std::vector<RelRef> relations;
		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, COMMON_SYNONYM1 });
		synonyms.push_back({ READ, COMMON_SYNONYM1 });
		synonyms.push_back({ CALL, COMMON_SYNONYM1 });

		for (unsigned int k = 0; k < synonyms.size(); k++) {
			for (unsigned int j = 0; j < ALL_VARIABLES.size(); j++) {
				relations.push_back(RelRef(type, synonyms[k], ALL_VARIABLES[j]));
			}
		}
		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryModifiesFilterCommonSynonym) {
		RelType type = MODIFIES_S;

		std::vector<std::string> lefts = MODIFIES_LEFTS;
		std::string left1 = MODIFIES_LEFT1;
		std::string left2 = MODIFIES_LEFT2;
		std::string left3 = MODIFIES_LEFT3;
		std::string left4 = MODIFIES_LEFT4;
		std::string right1 = MODIFIES_RIGHT1;
		std::string right2 = MODIFIES_RIGHT2;
		std::string right3 = MODIFIES_RIGHT3;
		std::string right4 = MODIFIES_RIGHT4;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, COMMON_SYNONYM1 });
		selectedList.push_back({ IF, COMMON_SYNONYM1 });
		selectedList.push_back({ WHILE, COMMON_SYNONYM1 });
		selectedList.push_back({ ASSIGN, COMMON_SYNONYM1 });
		selectedList.push_back({ PRINT, COMMON_SYNONYM1 });
		selectedList.push_back({ READ, COMMON_SYNONYM1 });
		selectedList.push_back({ CALL, COMMON_SYNONYM1 });

		//Test case for Select selected such that Modifies_S(selected, a)
		std::list<std::string> result1 = { left1, left2, left3, left4 };
		std::list<std::string> result2 = { left1 };
		std::list<std::string> result3 = { left2 };
		std::list<std::string> result4 = { left3, left4 };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, Synonym{"a"} });
			Query q = initQuery(relation, selectedList[i]);
			std::list<std::string> result = evaluator.evaluateQuery(q).front();
			result.sort();
			resultList[i].sort();
			EXPECT_EQ(result, resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Modifies_S(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], WILD_CARD);
			Query q = initQuery(relation, selectedList[i]);
			std::list<std::string> result = evaluator.evaluateQuery(q).front();
			result.sort();
			resultList[i].sort();
			EXPECT_EQ(result, resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Modifies_S(selected, "x")
		resultList[0] = { left3, left1 };
		resultList[1] = { left1 };
		resultList[2] = { };
		resultList[3] = { left3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, right1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Modifies_S(selected, "y")
		resultList[0] = { left2, left4 };
		resultList[1] = { };
		resultList[2] = { left2 };
		resultList[3] = { left4 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, right2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that Modifies_S(selected, anyEmpty)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, z });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Modifies_S(a, selected)
		resultList[0] = { right1, right2 };
		resultList[1] = { right1 };
		resultList[2] = { right2 };
		resultList[3] = { right3, right4 };

		Entity selected(VARIABLE, COMMON_SYNONYM2);
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], selected);
			Query q = initQuery(relation, selected);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]);
		}

		//Test case for Select selected such that Modifies_S("1", selected)
		resultList[0] = { right1 };
		resultList[1] = { };
		resultList[2] = { };
		RelRef relation(type, { STMT, left1 }, selected);
		Query q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[0]);

		//Test case for Select selected such that Modifies_S("3", selected)
		resultList[0] = { right2 };
		relation = RelRef(type, { STMT, left2 }, selected);
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[0]);

		//Test case for Select selected such that Modifies_S("9", selected)
		resultList[0] = { right3 };
		relation = RelRef(type, { STMT, left3 }, selected);
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[0]);

		//Test case for Select selected such that Modifies_S("10", selected)
		resultList[0] = { right4 };
		relation = RelRef(type, { STMT, left4 }, selected);
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[0]);

		//Test case for remaining Select selected such that Modifies_S(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);

		for (unsigned int j = 0; j < emptyList.size(); j++) {
			relation = RelRef(type, emptyList[j], selected);
			q = initQuery(relation, selected);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << j + 1;
		}
	}

	//Uses_S Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryUsessBooleanTrue) {
		RelType type = USES_S;
		std::string left1 = USES_LEFT1;
		std::string left2 = USES_LEFT2;
		std::string right1_1 = USES_RIGHT1_1;
		std::string right1_2 = USES_RIGHT1_2;
		std::string right2_1 = USES_RIGHT2_1;
		std::string right2_2 = USES_RIGHT2_2;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { STMT, left1 }, { VARIABLE, right1_1 }));
		relations.push_back(RelRef(type, { STMT, left1 }, { VARIABLE, right1_2 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { VARIABLE, right2_1 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { VARIABLE, right2_2 }));
		relations.push_back(RelRef(type, { STMT, left1 }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left2 }, WILD_CARD));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryUsesBooleanFalse) {
		RelType type = USES_S;
		//Test false boolean equation
		std::vector<std::string> lefts = USES_LEFTS;

		std::vector<RelRef> relations;

		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);

		for (unsigned int k = 0; k < invalidLefts.size(); k++) {
			for (unsigned int i = 0; i < ALL_VARIABLES.size(); i++) {
				relations.push_back((RelRef(type, invalidLefts[k], ALL_VARIABLES[i])));
			}
		}

		std::vector<Entity> invalidRight;
		invalidRight.push_back({ VARIABLE, z });

		for (unsigned int k = 0; k < invalidRight.size(); k++) {
			for (unsigned int i = 1; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k]));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryUsesFilterEmpty) {
		RelType type = USES_S;
		std::vector<std::string> lefts = USES_LEFTS;

		std::vector<RelRef> relations;
		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);
		for (Entity it : invalidLefts) {
			relations.push_back(RelRef(type, it, { STMT, Synonym{"a"} }));
		}

		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, z }));

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryUsesFilterNoCommonSynonymTrue) {
		RelType type = USES_S;
		std::string left1 = USES_LEFT1;
		std::string left2 = USES_LEFT2;
		std::string right1_1 = USES_RIGHT1_1;
		std::string right1_2 = USES_RIGHT1_2;
		std::string right2_1 = USES_RIGHT2_1;
		std::string right2_2 = USES_RIGHT2_2;

		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, right1_1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, right1_2 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { VARIABLE, right1_1 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { VARIABLE, right1_2 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, right2_1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { VARIABLE, right2_2 }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { VARIABLE, right2_1 }));
		relations.push_back(RelRef(type, { WHILE, Synonym{"a"} }, { VARIABLE, right2_2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryUsesFilterNoCommonSynonymFalse) {
		RelType type = USES_S;
		std::vector<RelRef> relations;
		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, COMMON_SYNONYM1 });
		synonyms.push_back({ READ, COMMON_SYNONYM1 });
		synonyms.push_back({ ASSIGN, COMMON_SYNONYM1 });
		synonyms.push_back({ CALL, COMMON_SYNONYM1 });

		for (unsigned int k = 0; k < synonyms.size(); k++) {
			for (unsigned int j = 0; j < ALL_VARIABLES.size(); j++) {
				relations.push_back(RelRef(type, synonyms[k], ALL_VARIABLES[j]));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryUsesFilterCommonSynonym) {
		RelType type = USES_S;

		std::vector<std::string> lefts = USES_LEFTS;
		std::string left1 = USES_LEFT1;
		std::string left2 = USES_LEFT2;
		std::string right1_1 = USES_RIGHT1_1;
		std::string right1_2 = USES_RIGHT1_2;
		std::string right2_1 = USES_RIGHT2_1;
		std::string right2_2 = USES_RIGHT2_2;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, COMMON_SYNONYM1 });
		selectedList.push_back({ IF, COMMON_SYNONYM1 });
		selectedList.push_back({ WHILE, COMMON_SYNONYM1 });
		selectedList.push_back({ PRINT, COMMON_SYNONYM1 });
		selectedList.push_back({ READ, COMMON_SYNONYM1 });
		selectedList.push_back({ ASSIGN, COMMON_SYNONYM1 });
		selectedList.push_back({ CALL, COMMON_SYNONYM1 });

		//Test case for Select selected such that USES_S(selected, a)
		std::list<std::string> result1 = { left1, left2 };
		std::list<std::string> result2 = { left1 };
		std::list<std::string> result3 = { left2 };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, Synonym{"a"} });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that USES_S(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], WILD_CARD);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that USES_S(selected, "x")
		resultList[0] = { left1 };
		resultList[1] = { left1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, right1_1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, right1_2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}
		//Test case for Select selected such that USES_S(VARIABLE, "y")
		resultList[0] = { left2 };
		resultList[1] = { };
		resultList[2] = { left2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, right2_1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, right2_2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that USES_S(selected, anyEmpty)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, z });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that USES_S(a, selected)
		resultList[0] = { right1_1, right2_1, right1_2, right2_2 };
		resultList[1] = { right1_1, right1_2 };
		resultList[2] = { right2_1, right2_2 };

		Entity selected(VARIABLE, COMMON_SYNONYM2);
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], selected);
			Query q = initQuery(relation, selected);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]);
		}

		//Test case for Select selected such that USES_S("1", selected)
		resultList[0] = { right1_1, right1_2 };
		resultList[1] = { };
		resultList[2] = { };
		RelRef relation(type, { STMT, left1 }, selected);
		Query q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[0]);

		//Test case for Select selected such that USES_S("3", selected)
		resultList[0] = { right2_1, right2_2 };
		relation = RelRef(type, { STMT, left2 }, selected);
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[0]);

		//Test case for remaining Select selected such that USES_S(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);

		for (unsigned int j = 0; j < emptyList.size(); j++) {
			relation = RelRef(type, emptyList[j], selected);
			q = initQuery(relation, selected);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << j + 1;
		}
	}

	//MODIFIES_P Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryModifiesPBooleanTrue) {
		RelType type = MODIFIES_P;
		std::string left1 = MODIFIESP_LEFT1;
		std::string left2 = MODIFIESP_LEFT2;
		std::string right1 = MODIFIESP_RIGHT1;
		std::string right2 = MODIFIESP_RIGHT2;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, { VARIABLE, right1 }));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { VARIABLE, right2 }));
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, WILD_CARD));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, WILD_CARD));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryModifiesPBooleanFalse) {
		RelType type = MODIFIES_P;
		std::string left1 = MODIFIESP_LEFT1;
		std::string left2 = MODIFIESP_LEFT2;
		std::string right1 = MODIFIESP_RIGHT1;
		std::string right2 = MODIFIESP_RIGHT2;
		//Test false boolean equation
		std::vector<std::string> lefts = MODIFIESP_LEFTS;

		std::vector<RelRef> relations;
		relations.push_back(RelRef(type, { PROCEDURE, p3 }, WILD_CARD));
		relations.push_back(RelRef(type, { PROCEDURE, p3 }, { VARIABLE, right1 }));
		relations.push_back(RelRef(type, { PROCEDURE, p3 }, { VARIABLE, right2 }));
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, { VARIABLE, z }));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { VARIABLE, z }));

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryModifiesPFilterNoCommonSynonymTrue) {
		RelType type = MODIFIES_P;
		std::string left1 = MODIFIESP_LEFT1;
		std::string left2 = MODIFIESP_LEFT2;
		std::string right1 = MODIFIESP_RIGHT1;
		std::string right2 = MODIFIESP_RIGHT2;

		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, { VARIABLE, right1 }));
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, { VARIABLE, right2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryModifiesPFilterCommonSynonym) {
		RelType type = MODIFIES_P;

		std::vector<std::string> lefts = MODIFIESP_LEFTS;
		std::string left1 = MODIFIESP_LEFT1;
		std::string left2 = MODIFIESP_LEFT2;
		std::string right1 = MODIFIESP_RIGHT1;
		std::string right2 = MODIFIESP_RIGHT2;

		Entity selected = { PROCEDURE, COMMON_SYNONYM1 };

		std::list<std::string> result = { left1, left2 };

		//Test case for Select selected such that MODIFIES_P(selected, _)
		RelRef relation(type, selected, WILD_CARD);
		Query q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that MODIFIES_P(selected, "x")
		result = { left1 };
		relation = { type, selected, { VARIABLE, right1 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that MODIFIES_P(selected, "y")
		result = { left2 };
		relation = { type, selected, { VARIABLE, right2 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);


		//Test case for Select selected such that MODIFIES_P(selected, "z")
		result = {  };
		relation = { type, selected, { VARIABLE, z } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);


		//Test case for Select selected such that MODIFIES_P(a, selected)
		result = { right1, right2 };
		selected = { VARIABLE, COMMON_SYNONYM1 };
		relation = { type, { PROCEDURE, COMMON_SYNONYM2 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);


		//Test case for Select selected such that MODIFIES_P("main1", selected)
		result = { right1 };
		relation = { type, { PROCEDURE, left1 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that MODIFIES_P("sub1", selected)
		result = { right2 };
		selected = { VARIABLE, COMMON_SYNONYM1 };
		relation = { type, { PROCEDURE, left2 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that MODIFIES_P("sub2", selected)
		result = { };
		relation = { type, { PROCEDURE, p3 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);
	}

	//USES_P Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryUsesPBooleanTrue) {
		RelType type = USES_P;
		std::string left1 = USESP_LEFT1;
		std::string left2 = USESP_LEFT2;
		std::string right1 = USESP_RIGHT1;
		std::string right2 = USESP_RIGHT2;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, { VARIABLE, right1 }));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { VARIABLE, right2 }));
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, WILD_CARD));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, WILD_CARD));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryUsesPPBooleanFalse) {
		RelType type = USES_P;
		std::string left1 = USESP_LEFT1;
		std::string left2 = USESP_LEFT2;
		std::string right1 = USESP_RIGHT1;
		std::string right2 = USESP_RIGHT2;
		//Test false boolean equation
		std::vector<std::string> lefts = USESP_LEFTS;

		std::vector<RelRef> relations;
		relations.push_back(RelRef(type, { PROCEDURE, p3 }, WILD_CARD));
		relations.push_back(RelRef(type, { PROCEDURE, p3 }, { VARIABLE, right1 }));
		relations.push_back(RelRef(type, { PROCEDURE, p3 }, { VARIABLE, right2 }));
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, { VARIABLE, z }));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { VARIABLE, z }));

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryUsesPPFilterNoCommonSynonymTrue) {
		RelType type = USES_P;
		std::string left1 = USESP_LEFT1;
		std::string left2 = USESP_LEFT2;
		std::string right1 = USESP_RIGHT1;
		std::string right2 = USESP_RIGHT2;

		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { VARIABLE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, { VARIABLE, right1 }));
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, { VARIABLE, right2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryUsesPFilterCommonSynonym) {
		RelType type = USES_P;

		std::vector<std::string> lefts = USESP_LEFTS;
		std::string left1 = USESP_LEFT1;
		std::string left2 = USESP_LEFT2;
		std::string right1 = USESP_RIGHT1;
		std::string right2 = USESP_RIGHT2;

		Entity selected = { PROCEDURE, COMMON_SYNONYM1 };

		std::list<std::string> result = { left1, left2 };

		//Test case for Select selected such that MODIFIES_P(selected, _)
		RelRef relation(type, selected, WILD_CARD);
		Query q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that MODIFIES_P(selected, "x")
		result = { left1 };
		relation = { type, selected, { VARIABLE, right1 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that MODIFIES_P(selected, "y")
		result = { left2 };
		relation = { type, selected, { VARIABLE, right2 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);


		//Test case for Select selected such that MODIFIES_P(selected, "z")
		result = {  };
		relation = { type, selected, { VARIABLE, z } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);


		//Test case for Select selected such that MODIFIES_P(a, selected)
		result = { right1, right2 };
		selected = { VARIABLE, COMMON_SYNONYM1 };
		relation = { type, { PROCEDURE, COMMON_SYNONYM2 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);


		//Test case for Select selected such that MODIFIES_P("main1", selected)
		result = { right1 };
		relation = { type, { PROCEDURE, left1 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that MODIFIES_P("sub1", selected)
		result = { right2 };
		selected = { VARIABLE, COMMON_SYNONYM1 };
		relation = { type, { PROCEDURE, left2 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that MODIFIES_P("sub2", selected)
		result = { };
		relation = { type, { PROCEDURE, p3 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

	}

	//Calls Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryCallsBooleanTrue) {
		RelType type = CALLS;
		std::string left1 = CALLS_LEFT1;
		std::string left2 = CALLS_LEFT2;
		std::string right1 = CALLS_RIGHT1;
		std::string right2 = CALLS_RIGHT2;


		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, WILD_CARD, WILD_CARD));
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, { PROCEDURE, right1 }));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { PROCEDURE, right2 }));
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, WILD_CARD));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, WILD_CARD));
		relations.push_back(RelRef(type, WILD_CARD, { PROCEDURE, right1 }));
		relations.push_back(RelRef(type, WILD_CARD, { PROCEDURE, right2 }));

		validateRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryCallsBooleanFalse) {
		RelType type = CALLS;
		std::vector<std::string> lefts = CALLS_LEFTS;
		std::vector<std::string> rights = CALLS_RIGHTS;
		std::string left1 = CALLS_LEFT1;
		std::string left2 = CALLS_LEFT2;
		std::string right1 = CALLS_RIGHT1;
		std::string right2 = CALLS_RIGHT2;

		std::vector<RelRef> relations;
		//Test false boolean equation
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, { PROCEDURE, right2 }));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { PROCEDURE, right1 }));
		relations.push_back(RelRef(type, { PROCEDURE, right1 }, { PROCEDURE, left2 }));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { PROCEDURE, right1 }));

		validateEmptyRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryCallsFilterNoCommonSynonymTrue) {
		RelType type = CALLS;
		std::vector<RelRef> relations;
		std::string left1 = CALLS_LEFT1;
		std::string left2 = CALLS_LEFT2;
		std::string right1 = CALLS_RIGHT1;
		std::string right2 = CALLS_RIGHT2;

		//Have Result for matching header
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, { PROCEDURE, Synonym{"b"} }));
		relations.push_back(RelRef(type, WILD_CARD, { PROCEDURE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, { PROCEDURE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { PROCEDURE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, { PROCEDURE, right1 }));
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, { PROCEDURE, right2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryCallsFilterCommonSynonym) {
		RelType type = CALLS;
		std::vector<std::string> lefts = CALLS_LEFTS;
		std::vector<std::string> rights = CALLS_RIGHTS;
		std::string left1 = CALLS_LEFT1;
		std::string left2 = CALLS_LEFT2;
		std::string right1 = CALLS_RIGHT1;
		std::string right2 = CALLS_RIGHT2;

		Entity selected = { PROCEDURE, COMMON_SYNONYM1 };

		std::list<std::string> result = { left1, left2 };

		//Test case for Select selected such that CALLS(selected, _)
		RelRef relation(type, selected, WILD_CARD);
		Query q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS(selected, a)
		relation = { type, selected, { PROCEDURE, COMMON_SYNONYM2 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS(selected, "sub")
		result = { left1 };
		relation = { type, selected, { PROCEDURE, right1 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS(selected, "sub1")
		result = { left2 };
		relation = { type, selected, { PROCEDURE, right2 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS(selected, "main")
		result = {  };
		relation = { type, selected, { PROCEDURE, p1 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);


		//Test case for Select selected such that CALLS(_, selected)
		result = { right1, right2 };
		relation = { type, WILD_CARD, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS(a, selected)
		result = { right1, right2 };
		relation = { type, { PROCEDURE, COMMON_SYNONYM2 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS("main1", selected)
		result = { right1 };
		relation = { type, { PROCEDURE, left1 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS("sub1", selected)
		result = { right2 };
		selected = { VARIABLE, COMMON_SYNONYM1 };
		relation = { type, { PROCEDURE, left2 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS("sub2", selected)
		result = { };
		relation = { type, { PROCEDURE, p3 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);
	}


	//CallsT Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryCallsTBooleanTrue) {
		RelType type = CALLS_T;
		std::string left1 = CALLS_LEFT1;
		std::string left2 = CALLS_LEFT2;
		std::string right1 = CALLS_RIGHT1;
		std::string right2 = CALLS_RIGHT2;


		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, WILD_CARD, WILD_CARD));
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, { PROCEDURE, right1 }));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { PROCEDURE, right2 }));
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, { PROCEDURE, right2 }));
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, WILD_CARD));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, WILD_CARD));
		relations.push_back(RelRef(type, WILD_CARD, { PROCEDURE, right1 }));
		relations.push_back(RelRef(type, WILD_CARD, { PROCEDURE, right2 }));

		validateRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryCallsTBooleanFalse) {
		RelType type = CALLS_T;
		std::vector<std::string> lefts = CALLS_LEFTS;
		std::vector<std::string> rights = CALLS_RIGHTS;
		std::string left1 = CALLS_LEFT1;
		std::string left2 = CALLS_LEFT2;
		std::string right1 = CALLS_RIGHT1;
		std::string right2 = CALLS_RIGHT2;

		std::vector<RelRef> relations;
		//Test false boolean equation
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { PROCEDURE, right1 }));
		relations.push_back(RelRef(type, { PROCEDURE, right1 }, { PROCEDURE, left2 }));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { PROCEDURE, right1 }));

		validateEmptyRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryCallsTFilterNoCommonSynonymTrue) {
		RelType type = CALLS_T;
		std::vector<RelRef> relations;
		std::string left1 = CALLS_LEFT1;
		std::string left2 = CALLS_LEFT2;
		std::string right1 = CALLS_RIGHT1;
		std::string right2 = CALLS_RIGHT2;

		//Have Result for matching header
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, { PROCEDURE, Synonym{"b"} }));
		relations.push_back(RelRef(type, WILD_CARD, { PROCEDURE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { PROCEDURE, left1 }, { PROCEDURE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { PROCEDURE, left2 }, { PROCEDURE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, { PROCEDURE, right1 }));
		relations.push_back(RelRef(type, { PROCEDURE, Synonym{"a"} }, { PROCEDURE, right2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryCallsTFilterCommonSynonym) {
		RelType type = CALLS_T;
		std::vector<std::string> lefts = CALLS_LEFTS;
		std::vector<std::string> rights = CALLS_RIGHTS;
		std::string left1 = CALLS_LEFT1;
		std::string left2 = CALLS_LEFT2;
		std::string right1 = CALLS_RIGHT1;
		std::string right2 = CALLS_RIGHT2;


		Entity selected = { PROCEDURE, COMMON_SYNONYM1 };

		std::list<std::string> result = { left1, left2 };

		//Test case for Select selected such that CALLS_T(selected, _)
		RelRef relation(type, selected, WILD_CARD);
		Query q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS_T(selected, a)
		relation = { type, selected, { PROCEDURE, COMMON_SYNONYM2 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS_T(selected, "sub")
		result = { left1 };
		relation = { type, selected, { PROCEDURE, right1 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS_T(selected, "sub1")
		result = { left1, left2 };
		relation = { type, selected, { PROCEDURE, right2 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS_T(selected, "main")
		result = {  };
		relation = { type, selected, { PROCEDURE, p1 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);


		//Test case for Select selected such that CALLS_T(_, selected)
		result = { right1, right2 };
		relation = { type, WILD_CARD, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS(a, selected)
		result = { right1, right2 };
		relation = { type, { PROCEDURE, COMMON_SYNONYM2 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS("main1", selected)
		result = { right1, right2 };
		relation = { type, { PROCEDURE, left1 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS("sub1", selected)
		result = { right2 };
		selected = { VARIABLE, COMMON_SYNONYM1 };
		relation = { type, { PROCEDURE, left2 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);

		//Test case for Select selected such that CALLS("sub2", selected)
		result = { };
		relation = { type, { PROCEDURE, p3 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q).front(), result);
	}

	//Next Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryNextBooleanTrue) {
		RelType type = NEXT;
		std::string left1 = NEXT_LEFT1;
		std::string left2 = NEXT_LEFT2;
		std::string right1 = NEXT_RIGHT1;
		std::string right2 = NEXT_RIGHT2;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, WILD_CARD, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right2 }));
		relations.push_back(RelRef(type, { STMT, left1 }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left2 }, WILD_CARD));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, right1 }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, right2 }));

		validateRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryNextBooleanFalse) {
		RelType type = NEXT;
		std::vector<std::string> lefts = NEXT_LEFTS;
		std::vector<std::string> rights = NEXT_RIGHTS;
		std::string left1 = NEXT_LEFT1;
		std::string left2 = NEXT_LEFT2;
		std::string right1 = NEXT_RIGHT1;
		std::string right2 = NEXT_RIGHT2;

		std::vector<RelRef> relations;
		//Test false boolean equation
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right2 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right1 }));

		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);
		for (unsigned int k = 0; k < invalidLefts.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, invalidLefts[k], VALID_CONSTANT_STMT_ENTITY[i]));
			}
		}

		std::vector<Entity> invalidRight = getInvalidConstant(rights);
		for (unsigned int k = 0; k < invalidRight.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k]));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryNextFilterEmpty) {
		RelType type = NEXT;
		std::vector<RelRef> relations;
		std::vector<std::string> lefts = NEXT_LEFTS;
		std::vector<std::string> rights = NEXT_RIGHTS;

		std::vector<Entity> invalidLefts = getInvalidConstant(lefts);
		for (Entity it : invalidLefts) {
			relations.push_back(RelRef(type, it, { STMT, Synonym{"a"} }));
		}

		std::vector<Entity> invalidRight = getInvalidConstant(rights);
		for (Entity it : invalidRight) {
			relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, it));
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryNextFilterNoCommonSynonymTrue) {
		RelType type = NEXT;
		std::vector<RelRef> relations;
		std::string left1 = NEXT_LEFT1;
		std::string left2 = NEXT_LEFT2;
		std::string right1 = NEXT_RIGHT1;
		std::string right2 = NEXT_RIGHT2;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { IF, Synonym{"b"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { WHILE, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { IF, Synonym{"b"} }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { WHILE, Synonym{"b"} }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, WILD_CARD, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, WILD_CARD, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left1 }, { IF, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { WHILE, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right2 }));
		relations.push_back(RelRef(type, { IF, Synonym{"a"} }, { STMT, right2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorRelationTest, evaluateQueryNextFilterNoCommonSynonymFalse) {
		RelType type = NEXT;

		std::vector<std::string> lefts = NEXT_LEFTS;
		std::vector<std::string> rights = NEXT_RIGHTS;

		std::vector<RelRef> relations;
		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, COMMON_SYNONYM1 });
		synonyms.push_back({ READ, COMMON_SYNONYM1 });
		synonyms.push_back({ ASSIGN, COMMON_SYNONYM1 });
		synonyms.push_back({ CALL, COMMON_SYNONYM1 });

		for (unsigned int k = 0; k < synonyms.size(); k++) {
			for (unsigned int j = 0; j < synonyms.size(); j++) {
				relations.push_back(RelRef(type, synonyms[k], synonyms[j]));
			}
		}

		//Empty result for non-matching header for single column
		for (unsigned int k = 0; k < synonyms.size(); k++) {
			relations.push_back(RelRef(type, WILD_CARD, synonyms[k]));
			relations.push_back(RelRef(type, { synonyms[k] }, WILD_CARD));
			relations.push_back(RelRef(type, { synonyms[k] }, WILD_CARD));

			for (auto valid : lefts) {
				relations.push_back(RelRef(type, { STMT, valid }, { synonyms[k] }));
			}

			for (auto valid : rights) {
				relations.push_back(RelRef(type, { synonyms[k] }, { STMT, valid }));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorRelationTest, evaluateQueryNextFilterCommonSynonym) {
		RelType type = NEXT;
		std::vector<std::string> lefts = NEXT_LEFTS;
		std::vector<std::string> rights = NEXT_RIGHTS;
		std::string left1 = NEXT_LEFT1;
		std::string left2 = NEXT_LEFT2;
		std::string right1 = NEXT_RIGHT1;
		std::string right2 = NEXT_RIGHT2;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, COMMON_SYNONYM1 });
		selectedList.push_back({ IF, COMMON_SYNONYM1 });
		selectedList.push_back({ WHILE, COMMON_SYNONYM1 });
		selectedList.push_back({ PRINT, COMMON_SYNONYM1 });
		selectedList.push_back({ READ, COMMON_SYNONYM1 });
		selectedList.push_back({ ASSIGN, COMMON_SYNONYM1 });
		selectedList.push_back({ CALL, COMMON_SYNONYM1 });

		//Test case for Select selected such that Next(selected, a)
		std::list<std::string> result1 = { left1, left2 };
		std::list<std::string> result2 = { left1, left2 };
		std::list<std::string> result3 = { };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> resultList = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, Synonym{"a"} });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Next(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], WILD_CARD);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Next(a, selected)
		resultList[0] = { right1, right2 };
		resultList[1] = { right1 };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Next(_, selected)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, WILD_CARD, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Next("1", selected)
		resultList[0] = { right1 };
		resultList[1] = { right1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left1 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Next("2", selected)
		resultList[0] = { right2 };
		resultList[1] = { };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left2 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that Next(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select selected such that Next(selected, "2")
		resultList[0] = { left1 };
		resultList[1] = { left1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Next(selected, "3")
		resultList[0] = { left2 };
		resultList[1] = { left2 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q).front(), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that Next(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}
	}
}