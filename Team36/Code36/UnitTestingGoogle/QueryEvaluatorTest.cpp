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
			PKB::getInstance().addUsesS(std::stoi(USES_LEFT1), USES_RIGHT1);
			PKB::getInstance().addUsesS(std::stoi(USES_LEFT2), USES_RIGHT2);
			PKB::getInstance().addModifiesP(MODIFIESP_LEFT1, MODIFIESP_RIGHT1);
			PKB::getInstance().addModifiesP(MODIFIESP_LEFT2, MODIFIESP_RIGHT2);
			PKB::getInstance().addUsesP(USESP_LEFT1, USESP_RIGHT1);
			PKB::getInstance().addUsesP(USESP_LEFT2, USESP_RIGHT2);
			PKB::getInstance().addExprTree(std::stoi(MODIFIES_LEFT3), EXPRESSION1);
			PKB::getInstance().addExprTree(std::stoi(MODIFIES_LEFT4), EXPRESSION2);
			PKB::getInstance().generateFollowsT();
			PKB::getInstance().generateParentT();
		}

		~QueryEvaluatorTest() override {
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

		Query initQuery(Pattern pattern, Entity selected) {
			Query q;
			q.addPattern(pattern);
			q.addSelected(selected);
			return q;
		}
		Query initQuery(RelRef relation, Entity selected) {
			Query q;
			q.addRelation(relation);
			q.addSelected(selected);
			return q;
		}

		void validatePatterns(std::vector<Pattern> patterns) {
			for (unsigned int i = 0; i < patterns.size(); i++) {
				for (unsigned int j = 0; j < ALL_SELECT.size(); j++) {
					Query q = initQuery(patterns[i], ALL_SELECT[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), ALL_RESULT[j]) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		void validateEmptyPatterns(std::vector<Pattern> patterns) {
			for (unsigned int i = 0; i < patterns.size(); i++) {
				for (unsigned int j = 0; j < ALL_SELECT.size(); j++) {
					Query q = initQuery(patterns[i], ALL_SELECT[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		void validateRelations(std::vector<RelRef> relations) {
			for (unsigned int i = 0; i < relations.size(); i++) {
				for (unsigned int j = 0; j < ALL_SELECT.size(); j++) {
					Query q = initQuery(relations[i], ALL_SELECT[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), ALL_RESULT[j]) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		void validateEmptyRelations(std::vector<RelRef> relations) {
			for (unsigned int i = 0; i < relations.size(); i++) {
				for (unsigned int j = 0; j < ALL_SELECT.size(); j++) {
					Query q = initQuery(relations[i], ALL_SELECT[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		void validateRelations(std::vector<RelRef> relations, std::vector<Entity> selected, std::vector<std::list<std::string>> results) {
			for (unsigned int i = 0; i < relations.size(); i++) {
				for (unsigned int j = 0; j < selected.size(); j++) {
					Query q = initQuery(relations[i], ALL_SELECT[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q), results[j]) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		const var_name x = "x";
		const var_name y = "y";
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

		const std::string EXPRESSION1 = "x";
		const std::string EXPRESSION2 = "x + (y * 5)";
		const std::string EXPRESSION_CONSTANT = "5";

		const std::vector<std::string> MODIFIES_LEFTS = { MODIFIES_LEFT1, MODIFIES_LEFT2, MODIFIES_LEFT3, MODIFIES_LEFT4 };
		const std::vector<std::string> MODIFIES_RIGHTS = { MODIFIES_RIGHT1, MODIFIES_RIGHT2, MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		const std::vector<std::string> EXPRESSIONS = { EXPRESSION1, EXPRESSION2 };

		const std::string USES_LEFT1 = "1";
		const std::string USES_LEFT2 = "3";
		const std::string USES_RIGHT1 = y;
		const std::string USES_RIGHT2 = x;

		const std::vector<std::string> USES_LEFTS = { USES_LEFT1, USES_LEFT2 };
		const std::vector<std::string> USES_RIGHTS = { USES_RIGHT1, USES_RIGHT2 };

		const std::string USESP_LEFT1 = p1;
		const std::string USESP_LEFT2 = p2;
		const std::string USESP_RIGHT1 = y;
		const std::string USESP_RIGHT2 = x;

		const std::vector<std::string> USESP_LEFTS = { USESP_LEFT1, USESP_LEFT2 };
		const std::vector<std::string> USESP_RIGHTS = { USESP_RIGHT1, USESP_RIGHT2 };

		const std::string MODIFIESP_LEFT1 = p1;
		const std::string MODIFIESP_LEFT2 = p2;
		const std::string MODIFIESP_RIGHT1 = y;
		const std::string MODIFIESP_RIGHT2 = x;

		const std::vector<std::string> MODIFIESP_LEFTS = { MODIFIESP_LEFT1, MODIFIESP_LEFT2 };
		const std::vector<std::string> MODIFIESP_RIGHTS = { MODIFIESP_RIGHT1, MODIFIESP_RIGHT2 };

		const std::list<std::string> STMTS = { IF1, IF2, WHILE1, WHILE2, READ1, READ2,
			PRINT1, PRINT2, ASSIGN1, ASSIGN2, CALL1, CALL2 };
		PKBAdapter pkb;
		QueryEvaluator evaluator;

		const Synonym COMMON_SYNONYM1 = { "cs1" };
		const Synonym COMMON_SYNONYM2 = { "cs2" };

		// select v
		const std::list<std::string> ALL_VARIABLE = { x, y, z };
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

		const std::vector<std::list<std::string>> ALL_RESULT = {
			ALL_VARIABLE , ALL_CONSTANT , ALL_PROCEDURE, ALL_STMT, ALL_IF,
			ALL_WHILE, ALL_READ, ALL_PRINT, ALL_ASSIGN, ALL_CALL };

		const std::vector<Entity> ALL_SELECT = {
			SELECT_VARIABLE , SELECT_CONSTANT , SELECT_PROCEDURE, SELECT_STMT, SELECT_IF,
			SELECT_WHILE, SELECT_READ, SELECT_PRINT, SELECT_ASSIGN, SELECT_CALL };

		const std::list<std::string> EMPTY_RESULT = {};

		const Entity WILD_CARD = { WILD };

		const std::vector<Entity> VALID_CONSTANT_STMT_ENTITY = { WILD_CARD,
			{STMT, "1"}, {STMT, "2"}, {STMT, "3"}, {STMT, "4"}, {STMT, "5"}, {STMT, "6"},
			{STMT, "7"}, {STMT, "8"}, {STMT, "9"}, {STMT, "10"}, {STMT, "11"}, {STMT, "12"}
		};

		std::vector<Entity> ALL_VARIABLES = { { VARIABLE, x }, { VARIABLE, y }, { VARIABLE, z } };
	};

	//Evaluator Select statement without any relation/Pattern
	TEST_F(QueryEvaluatorTest, evaluateQueryRaw) {
		for (unsigned int i = 0; i < ALL_SELECT.size(); i++) {
			Query q;
			Entity selected = ALL_SELECT[i];
			q.addSelected(selected);
			EXPECT_EQ(evaluator.evaluateQuery(q), ALL_RESULT[i]) << "Error at results : " << i + 1;
		}
	}

	//Follow Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowBooleanTrue) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowBooleanFalse) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowFilterEmpty) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowFilterNoCommonSynonymTrue) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryFollowFilterNoCommonSynonymFalse) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowFilterCommonSynonym) {
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

		//Test case for Select a such that Follow(selected, a)
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
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], WILD_CARD);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(a, selected)
		resultList[0] = { right1, right2 };
		resultList[1] = { right1 };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(_, selected)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, WILD_CARD, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow("1", selected)
		resultList[0] = { right1 };
		resultList[1] = { right1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left1 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow("2", selected)
		resultList[0] = { right2 };
		resultList[1] = { };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left2 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Follow(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select a such that Follow(selected, "2")
		resultList[0] = { left1 };
		resultList[1] = { left1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Follow(selected, "3")
		resultList[0] = { left2 };
		resultList[1] = { left2 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Follow(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}
	}

	//FollowT Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTBooleanTrue) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTBooleanFalse) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTFilterEmpty) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTFilterNoCommonSynonymTrue) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTFilterNoCommonSynonymFalse) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryFollowTFilterCommonSynonym) {
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

		//Test case for Select a such that FollowT(selected, a)
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
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that FollowT(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], WILD_CARD);
			Query q;
			q.addRelation(relation);
			q.addSelected(selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that FollowT(a, selected)
		resultList[0] = { right1, right2 };
		resultList[1] = { right1 };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that FollowT(_, selected)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, WILD_CARD, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that FollowT("1", selected)
		resultList[0] = { right1, right2 };
		resultList[1] = { right1 };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left1 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that FollowT("2", selected)
		resultList[0] = { right2 };
		resultList[1] = { };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left2 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that FollowT(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select a such that FollowT(selected, "2")
		resultList[0] = { left1 };
		resultList[1] = { left1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that FollowT(selected, "3")
		resultList[0] = { left1, left2 };
		resultList[1] = { left1, left2 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that FollowT(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}
	}

	//Parent Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryParentBooleanTrue) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryParentBooleanFalse) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryParentFilterEmpty) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryParentFilterNoCommonSynonymTrue) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryParentFilterNoCommonSynonymFalse) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryParentFilterCommonSynonym) {
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

		//Test case for Select a such that Parent(selected, a)
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
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], WILD_CARD);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent(a, selected)
		resultList[0] = { right1, right2, right3 };
		resultList[1] = { right1 };
		resultList[2] = { right2, right3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent(_, selected)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, WILD_CARD, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent("1", selected)
		resultList[0] = { right1, right2 };
		resultList[1] = { right1 };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left1 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent("2", selected)
		resultList[0] = { right2 };
		resultList[1] = { };
		resultList[2] = { right2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left2 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent("3", selected)
		resultList[0] = { right3 };
		resultList[1] = { };
		resultList[2] = { right3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left3 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Parent(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);

		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select a such that Parent(selected, "2")
		resultList[0] = { left1 };
		resultList[1] = { left1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent(selected, "3")
		resultList[0] = { left1, left2 };
		resultList[1] = { left1, left2 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Parent(selected, "4")
		resultList[0] = { left3 };
		resultList[1] = { };
		resultList[2] = { left3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right3 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Parent(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);

		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}
	}

	//ParentT Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryParentTBooleanTrue) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryParentTBooleanFalse) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryParentTFilterEmpty) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryParentTFilterNoCommonSynonymTrue) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryParentTFilterNoCommonSynonymFalse) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryParentTFilterCommonSynonym) {
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

		//Test case for Select a such that ParentT(selected, a)
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
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], WILD_CARD);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT(a, selected)
		resultList[0] = { right1, right2, right3 };
		resultList[1] = { right1 };
		resultList[2] = { right2, right3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT(_, selected)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, WILD_CARD, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT("1", selected)
		resultList[0] = { right1, right2, right3 };
		resultList[1] = { right1 };
		resultList[2] = { right2, right3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left1 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT("2", selected)
		resultList[0] = { right2, right3 };
		resultList[1] = { };
		resultList[2] = { right2, right3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left2 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT("3", selected)
		resultList[0] = { right3 };
		resultList[1] = { };
		resultList[2] = { right3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, { STMT, left3 }, selectedList[i]);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that ParentT(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, emptyList[j], selectedList[i]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select a such that ParentT(selected, "2")
		resultList[0] = { left1 };
		resultList[1] = { left1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT(selected, "3")
		resultList[0] = { left1, left2 };
		resultList[1] = { left1, left2 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that ParentT(selected, "4")
		resultList[0] = { left1, left2, left3 };
		resultList[1] = { left1, left2 };
		resultList[2] = { left3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { STMT, right3 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that ParentT(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selectedList.size(); i++) {
				RelRef relation(type, selectedList[i], emptyList[j]);
				Query q = initQuery(relation, selectedList[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}
	}

	//Modifies_S Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesBooleanTrue) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesBooleanFalse) {
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
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k]));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesFilterEmpty) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesFilterNoCommonSynonymTrue) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesFilterNoCommonSynonymFalse) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesFilterCommonSynonym) {
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

		//Test case for Select a such that Modifies_S(selected, a)
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
			std::list<std::string> result = evaluator.evaluateQuery(q);
			result.sort();
			resultList[i].sort();
			EXPECT_EQ(result, resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Modifies_S(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], WILD_CARD);
			Query q = initQuery(relation, selectedList[i]);
			std::list<std::string> result = evaluator.evaluateQuery(q);
			result.sort();
			resultList[i].sort();
			EXPECT_EQ(result, resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Modifies_S(selected, "x")
		resultList[0] = { left3, left1 };
		resultList[1] = { left1 };
		resultList[2] = { };
		resultList[3] = { left3 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, right1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Modifies_S(VARIABLE, "y")
		resultList[0] = { left2, left4 };
		resultList[1] = { };
		resultList[2] = { left2 };
		resultList[3] = { left4 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, right2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that Modifies_S(selected, anyEmpty)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, z });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that Modifies_S(a, selected)
		resultList[0] = { right1, right2 };
		resultList[1] = { right1 };
		resultList[2] = { right2 };
		resultList[3] = { right3, right4 };

		Entity selected(VARIABLE, COMMON_SYNONYM2);
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], selected);
			Query q = initQuery(relation, selected);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]);
		}

		//Test case for Select a such that Modifies_S("1", selected)
		resultList[0] = { right1 };
		resultList[1] = { };
		resultList[2] = { };
		RelRef relation(type, { STMT, left1 }, selected);
		Query q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), resultList[0]);

		//Test case for Select a such that Modifies_S("3", selected)
		resultList[0] = { right2 };
		relation = RelRef(type, { STMT, left2 }, selected);
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), resultList[0]);

		//Test case for Select a such that Modifies_S("9", selected)
		resultList[0] = { right3 };
		relation = RelRef(type, { STMT, left3 }, selected);
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), resultList[0]);

		//Test case for Select a such that Modifies_S("10", selected)
		resultList[0] = { right4 };
		relation = RelRef(type, { STMT, left4 }, selected);
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), resultList[0]);

		//Test case for remaining Select a such that Modifies_S(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);

		for (unsigned int j = 0; j < emptyList.size(); j++) {
			relation = RelRef(type, emptyList[j], selected);
			q = initQuery(relation, selected);
			EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << j + 1;
		}
	}

	//Uses_S Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryUsessBooleanTrue) {
		RelType type = USES_S;
		std::string left1 = USES_LEFT1;
		std::string left2 = USES_LEFT2;
		std::string right1 = USES_RIGHT1;
		std::string right2 = USES_RIGHT2;

		std::vector<RelRef> relations;
		//Test true boolean equation
		relations.push_back(RelRef(type, { STMT, left1 }, { VARIABLE, right1 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { VARIABLE, right2 }));
		relations.push_back(RelRef(type, { STMT, left1 }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left2 }, WILD_CARD));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryUsesBooleanFalse) {
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
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k]));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryUsesFilterEmpty) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryUsesFilterNoCommonSynonymTrue) {
		RelType type = USES_S;
		std::string left1 = USES_LEFT1;
		std::string left2 = USES_LEFT2;
		std::string right1 = USES_RIGHT1;
		std::string right2 = USES_RIGHT2;

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

	TEST_F(QueryEvaluatorTest, evaluateQueryUsesFilterNoCommonSynonymFalse) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryUsesFilterCommonSynonym) {
		RelType type = USES_S;

		std::vector<std::string> lefts = USES_LEFTS;
		std::string left1 = USES_LEFT1;
		std::string left2 = USES_LEFT2;
		std::string right1 = USES_RIGHT1;
		std::string right2 = USES_RIGHT2;

		std::vector<Entity> selectedList;
		selectedList.push_back({ STMT, COMMON_SYNONYM1 });
		selectedList.push_back({ IF, COMMON_SYNONYM1 });
		selectedList.push_back({ WHILE, COMMON_SYNONYM1 });
		selectedList.push_back({ PRINT, COMMON_SYNONYM1 });
		selectedList.push_back({ READ, COMMON_SYNONYM1 });
		selectedList.push_back({ ASSIGN, COMMON_SYNONYM1 });
		selectedList.push_back({ CALL, COMMON_SYNONYM1 });

		//Test case for Select a such that USES_S(selected, a)
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
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that USES_S(selected, _)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], WILD_CARD);
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that USES_S(selected, "x")
		resultList[0] = { left1 };
		resultList[1] = { left1 };
		resultList[2] = { };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, right1 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that USES_S(VARIABLE, "y")
		resultList[0] = { left2 };
		resultList[1] = { };
		resultList[2] = { left2 };
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, right2 });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select a such that USES_S(selected, anyEmpty)
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], { VARIABLE, z });
			Query q = initQuery(relation, selectedList[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
		}

		//Test case for Select a such that USES_S(a, selected)
		resultList[0] = { right1, right2 };
		resultList[1] = { right1 };
		resultList[2] = { right2 };

		Entity selected(VARIABLE, COMMON_SYNONYM2);
		for (unsigned int i = 0; i < selectedList.size(); i++) {
			RelRef relation(type, selectedList[i], selected);
			Query q = initQuery(relation, selected);
			EXPECT_EQ(evaluator.evaluateQuery(q), resultList[i]);
		}

		//Test case for Select a such that USES_S("1", selected)
		resultList[0] = { right1 };
		resultList[1] = { };
		resultList[2] = { };
		RelRef relation(type, { STMT, left1 }, selected);
		Query q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), resultList[0]);

		//Test case for Select a such that USES_S("3", selected)
		resultList[0] = { right2 };
		relation = RelRef(type, { STMT, left2 }, selected);
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), resultList[0]);

		//Test case for remaining Select a such that USES_S(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);

		for (unsigned int j = 0; j < emptyList.size(); j++) {
			relation = RelRef(type, emptyList[j], selected);
			q = initQuery(relation, selected);
			EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << j + 1;
		}
	}

	//MODIFIES_P Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesPBooleanTrue) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesPBooleanFalse) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesPFilterNoCommonSynonymTrue) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryModifiesPFilterCommonSynonym) {
		RelType type = MODIFIES_P;

		std::vector<std::string> lefts = MODIFIESP_LEFTS;
		std::string left1 = MODIFIESP_LEFT1;
		std::string left2 = MODIFIESP_LEFT2;
		std::string right1 = MODIFIESP_RIGHT1;
		std::string right2 = MODIFIESP_RIGHT2;

		Entity selected = { PROCEDURE, COMMON_SYNONYM1 };

		std::list<std::string> result = { left1, left2 };

		//Test case for Select a such that MODIFIES_P(selected, _)
		RelRef relation(type, selected, WILD_CARD);
		Query q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		//Test case for Select a such that MODIFIES_P(selected, "x")
		result = { left1 };
		relation = { type, selected, { VARIABLE, right1 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		//Test case for Select a such that MODIFIES_P(VARIABLE, "y")
		result = { left2 };
		relation = { type, selected, { VARIABLE, right2 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);


		//Test case for Select a such that MODIFIES_P(VARIABLE, "z")
		result = {  };
		relation = { type, selected, { VARIABLE, z } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);


		//Test case for Select a such that MODIFIES_P(a, selected)
		result = { right1, right2 };
		selected = { VARIABLE, COMMON_SYNONYM1 };
		relation = { type, { PROCEDURE, COMMON_SYNONYM2 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);


		//Test case for Select a such that MODIFIES_P("main1", selected)
		result = { right1 };
		relation = { type, { PROCEDURE, left1 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		//Test case for Select a such that MODIFIES_P("sub1", selected)
		result = { right2 };
		selected = { VARIABLE, COMMON_SYNONYM1 };
		relation = { type, { PROCEDURE, left2 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		//Test case for Select a such that MODIFIES_P("sub2", selected)
		result = { };
		relation = { type, { PROCEDURE, p3 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

	}


	//USES_P Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryUsesPBooleanTrue) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryUsesPPBooleanFalse) {
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
	TEST_F(QueryEvaluatorTest, evaluateQueryUsesPPFilterNoCommonSynonymTrue) {
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

	TEST_F(QueryEvaluatorTest, evaluateQueryUsesPFilterCommonSynonym) {
		RelType type = USES_P;

		std::vector<std::string> lefts = USESP_LEFTS;
		std::string left1 = USESP_LEFT1;
		std::string left2 = USESP_LEFT2;
		std::string right1 = USESP_RIGHT1;
		std::string right2 = USESP_RIGHT2;

		Entity selected = { PROCEDURE, COMMON_SYNONYM1 };

		std::list<std::string> result = { left1, left2 };

		//Test case for Select a such that MODIFIES_P(selected, _)
		RelRef relation(type, selected, WILD_CARD);
		Query q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		//Test case for Select a such that MODIFIES_P(selected, "x")
		result = { left1 };
		relation = { type, selected, { VARIABLE, right1 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		//Test case for Select a such that MODIFIES_P(VARIABLE, "y")
		result = { left2 };
		relation = { type, selected, { VARIABLE, right2 } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);


		//Test case for Select a such that MODIFIES_P(VARIABLE, "z")
		result = {  };
		relation = { type, selected, { VARIABLE, z } };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);


		//Test case for Select a such that MODIFIES_P(a, selected)
		result = { right1, right2 };
		selected = { VARIABLE, COMMON_SYNONYM1 };
		relation = { type, { PROCEDURE, COMMON_SYNONYM2 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);


		//Test case for Select a such that MODIFIES_P("main1", selected)
		result = { right1 };
		relation = { type, { PROCEDURE, left1 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		//Test case for Select a such that MODIFIES_P("sub1", selected)
		result = { right2 };
		selected = { VARIABLE, COMMON_SYNONYM1 };
		relation = { type, { PROCEDURE, left2 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		//Test case for Select a such that MODIFIES_P("sub2", selected)
		result = { };
		relation = { type, { PROCEDURE, p3 }, selected };
		q = initQuery(relation, selected);
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

	}


	//Pattern Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorTest, evaluateQueryPatternFilterNoCommonSynonymTrue) {
		Entity assign = { ASSIGN, "a" };
		Entity lhsSynonym = { VARIABLE, Synonym{"a"} };
		Entity lhsX = { VARIABLE, x };
		Entity lhsY = { VARIABLE, y };

		std::vector<Pattern> patterns;
		patterns.push_back(Pattern(assign, lhsX, x, true));
		patterns.push_back(Pattern(assign, lhsX, x, false));
		patterns.push_back(Pattern(assign, lhsY, y, true));
		patterns.push_back(Pattern(assign, lhsY, x, true));

		patterns.push_back(Pattern(assign, lhsSynonym, x, true));
		patterns.push_back(Pattern(assign, lhsSynonym, x, false));
		patterns.push_back(Pattern(assign, lhsSynonym, y, true));
		patterns.push_back(Pattern(assign, lhsY, EXPRESSION_CONSTANT, true));
		patterns.push_back(Pattern(assign, lhsSynonym, EXPRESSION_CONSTANT, true));

		validatePatterns(patterns);
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryPatternFilterNoCommonSynonymFalse) {
		std::string n = "n";
		Entity assign = { ASSIGN, "a" };
		Entity lhsSynonym = { VARIABLE, Synonym{"a"} };
		Entity lhsX = { VARIABLE, x };
		Entity lhsY = { VARIABLE, y };
		Entity lhsZ = { VARIABLE, z };
		Entity lhsN = { VARIABLE, n };

		std::vector<std::string> patternExpr;
		patternExpr.push_back(x);
		patternExpr.push_back(y);
		patternExpr.push_back(z);
		patternExpr.push_back(n);
		std::vector<Pattern> patterns;
		for (auto expr : patternExpr) {
			patterns.push_back(Pattern(assign, lhsZ, expr, false));
			patterns.push_back(Pattern(assign, lhsZ, expr, true));
			patterns.push_back(Pattern(assign, lhsN, expr, false));
			patterns.push_back(Pattern(assign, lhsN, expr, true));
		}

		patterns.push_back(Pattern(assign, lhsX, y, true));
		patterns.push_back(Pattern(assign, lhsX, y, false));
		patterns.push_back(Pattern(assign, lhsY, y, false));
		patterns.push_back(Pattern(assign, lhsY, x, false));
		patterns.push_back(Pattern(assign, lhsSynonym, y, false));
		patterns.push_back(Pattern(assign, lhsSynonym, z, false));
		patterns.push_back(Pattern(assign, lhsSynonym, n, false));
		patterns.push_back(Pattern(assign, lhsSynonym, z, true));
		patterns.push_back(Pattern(assign, lhsSynonym, n, true));
		patterns.push_back(Pattern(assign, lhsSynonym, EXPRESSION_CONSTANT, false));

		validateEmptyPatterns(patterns);
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryPatternFilterCommonSynonym) {
		std::string left1 = MODIFIES_LEFT3;
		std::string left2 = MODIFIES_LEFT4;
		std::string right1 = MODIFIES_RIGHT3;
		std::string right2 = MODIFIES_RIGHT4;

		Entity lhsX = { VARIABLE, x };
		Entity lhsY = { VARIABLE, y };

		Entity assignCommon = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhsCommon = { VARIABLE, COMMON_SYNONYM2 };

		std::vector<Pattern> patterns;
		std::vector<std::list<std::string>> results;
		std::vector<Entity> selected;

		//Handle result for Select a pattern a(_, _)
		patterns.push_back(Pattern(assignCommon, WILD_CARD, "", true));
		selected.push_back(assignCommon);
		results.push_back({ left1, left2 });

		//Handle result for Select a pattern a(v, _)
		patterns.push_back(Pattern(assignCommon, WILD_CARD, "", true));
		selected.push_back(assignCommon);
		results.push_back({ left1, left2 });

		//Handle result for Select a pattern a(_, "x")
		patterns.push_back(Pattern(assignCommon, WILD_CARD, x, false));
		selected.push_back(assignCommon);
		results.push_back({ left1 });

		//Handle result for Select a pattern a(_, _"x"_)
		patterns.push_back(Pattern(assignCommon, WILD_CARD, x, true));
		selected.push_back(assignCommon);
		results.push_back({ left1, left2 });

		//Handle result for Select a pattern a(_, "y")
		patterns.push_back(Pattern(assignCommon, WILD_CARD, y, false));
		selected.push_back(assignCommon);
		results.push_back({ });

		//Handle result for Select a pattern a(_, _"y"_)
		patterns.push_back(Pattern(assignCommon, WILD_CARD, y, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });

		//Handle result for Select a pattern a(v, "x")
		patterns.push_back(Pattern(assignCommon, lhsCommon, x, false));
		selected.push_back(assignCommon);
		results.push_back({ left1 });
		patterns.push_back(Pattern(assignCommon, lhsCommon, x, false));
		selected.push_back(lhsCommon);
		results.push_back({ right1 });

		//Handle result for Select a pattern a(v, _"x"_)
		patterns.push_back(Pattern(assignCommon, lhsCommon, x, true));
		selected.push_back(assignCommon);
		results.push_back({ left1, left2 });
		patterns.push_back(Pattern(assignCommon, lhsCommon, x, true));
		selected.push_back(lhsCommon);
		results.push_back({ right1, right2 });

		//Handle result for Select a pattern a(v, "y")
		patterns.push_back(Pattern(assignCommon, lhsCommon, y, false));
		selected.push_back(assignCommon);
		results.push_back({ });
		patterns.push_back(Pattern(assignCommon, lhsCommon, y, false));
		selected.push_back(lhsCommon);
		results.push_back({ });

		//Handle result for Select a pattern a(v, _"y"_)
		patterns.push_back(Pattern(assignCommon, lhsCommon, y, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });
		patterns.push_back(Pattern(assignCommon, lhsCommon, y, true));
		selected.push_back(lhsCommon);
		results.push_back({ right2 });

		//Handle result for Select a pattern a("x", "x")
		patterns.push_back(Pattern(assignCommon, lhsX, x, false));
		selected.push_back(assignCommon);
		results.push_back({ left1 });

		//Handle result for Select a pattern a("x", _"x"_)
		patterns.push_back(Pattern(assignCommon, lhsX, x, true));
		selected.push_back(assignCommon);
		results.push_back({ left1 });

		//Handle result for Select a pattern a("x", "y")
		patterns.push_back(Pattern(assignCommon, lhsX, y, false));
		selected.push_back(assignCommon);
		results.push_back({ });

		//Handle result for Select a pattern a("x", _"y"_)
		patterns.push_back(Pattern(assignCommon, lhsX, y, true));
		selected.push_back(assignCommon);
		results.push_back({ });

		//Handle result for Select a pattern a("y", "x")
		patterns.push_back(Pattern(assignCommon, lhsY, x, false));
		selected.push_back(assignCommon);
		results.push_back({ });

		//Handle result for Select a pattern a("y", _"x"_)
		patterns.push_back(Pattern(assignCommon, lhsY, x, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });

		//Handle result for Select a pattern a("y", _"y"_)
		patterns.push_back(Pattern(assignCommon, lhsY, y, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });

		//Handle result for Select a pattern a("y", "5")
		patterns.push_back(Pattern(assignCommon, lhsY, EXPRESSION_CONSTANT, false));
		selected.push_back(assignCommon);
		results.push_back({ });

		//Handle result for Select a pattern a("y", _"5"_)
		patterns.push_back(Pattern(assignCommon, lhsY, EXPRESSION_CONSTANT, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });

		//Handle result for Select a pattern a(_, _"5"_)
		patterns.push_back(Pattern(assignCommon, WILD_CARD, EXPRESSION_CONSTANT, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });

		//Handle result for Select a pattern a(v, _"5"_)
		patterns.push_back(Pattern(assignCommon, lhsCommon, EXPRESSION_CONSTANT, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });
		patterns.push_back(Pattern(assignCommon, lhsCommon, EXPRESSION_CONSTANT, true));
		selected.push_back(lhsCommon);
		results.push_back({ right2 });

		for (unsigned int i = 0; i < patterns.size(); i++) {
			Query q = initQuery(patterns[i], selected[i]);
			std::list<std::string> result = evaluator.evaluateQuery(q);
			result.sort();
			results[i].sort();
			EXPECT_EQ(result, results[i]) << "ERROR AT " << i + 1;
		}
	}
}