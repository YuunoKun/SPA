#include "pch.h"

#include "QueryEvaluator.h"
#include "PKBAdapter.h"
#include "PKB.h"
#include "Common.h"
#include "ExprParser.h"

namespace UnitTesting {
	class QueryEvaluatorPatternTest : public testing::Test {
	protected:
		QueryEvaluatorPatternTest() {
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
			PKB::getInstance().addModifiesS(std::stoi(MODIFIES_LEFT1), MODIFIES_RIGHT1);
			PKB::getInstance().addModifiesS(std::stoi(MODIFIES_LEFT2), MODIFIES_RIGHT2);
			PKB::getInstance().addExprTree(std::stoi(MODIFIES_LEFT1), EXPRESSIONNODE_1);
			PKB::getInstance().addExprTree(std::stoi(MODIFIES_LEFT2), EXPRESSIONNODE_2);
		}

		~QueryEvaluatorPatternTest() override {
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

		Query initQuery(Pattern& pattern, Entity selected) {
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
					EXPECT_EQ(evaluator.evaluateQuery(q).front(), ALL_RESULT[j]) << "Error at results : " << i + 1 << " : " << j + 1;
					q = initQuery(patterns[i], SELECT_BOOLEAN);
					EXPECT_EQ(evaluator.evaluateQuery(q).front(), BOOLEAN_TRUE_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}

		void validateEmptyPatterns(std::vector<Pattern> patterns) {
			std::list<Query> querys;
			for (unsigned int i = 0; i < patterns.size(); i++) {
				for (unsigned int j = 0; j < ALL_SELECT.size(); j++) {
					Query q = initQuery(patterns[i], ALL_SELECT[j]);
					EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
					q = initQuery(patterns[i], SELECT_BOOLEAN);
					EXPECT_EQ(evaluator.evaluateQuery(q).front(), BOOLEAN_FALSE_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
				}
			}
		}


		PKBAdapter pkb;
		QueryEvaluator evaluator;
		ExprParser expr_parser;

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


		const std::string MODIFIES_LEFT1 = ASSIGN1;
		const std::string MODIFIES_LEFT2 = ASSIGN2;
		const std::string MODIFIES_RIGHT1 = x;
		const std::string MODIFIES_RIGHT2 = y;

		const std::string EXPRESSION1 = "x";
		const std::string EXPRESSION2 = "x + (y * 5)";
		const std::string EXPRESSION_CONSTANT = "5";
		expr EXPRESSIONNODE_1 = expr_parser.parse(EXPRESSION1);
		expr EXPRESSIONNODE_2 = expr_parser.parse(EXPRESSION2);
		expr EXPRESSIONNODE_CONSTANT = expr_parser.parse(EXPRESSION_CONSTANT);

		const std::vector<std::string> MODIFIES_LEFTS = { MODIFIES_LEFT1, MODIFIES_LEFT2 };
		const std::vector<std::string> MODIFIES_RIGHTS = { MODIFIES_RIGHT1, MODIFIES_RIGHT2 };
		const std::vector<std::string> EXPRESSIONS = { EXPRESSION1, EXPRESSION2 };

		const std::list<std::string> STMTS = { IF1, IF2, WHILE1, WHILE2, READ1, READ2,
			PRINT1, PRINT2, ASSIGN1, ASSIGN2, CALL1, CALL2 };

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

		std::vector<Entity> ALL_VARIABLES = { { VARIABLE, x }, { VARIABLE, y }, { VARIABLE, z } };
	};


	//Pattern Assign Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorPatternTest, evaluateQueryPatternFilterNoCommonSynonymTrue) {
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

	TEST_F(QueryEvaluatorPatternTest, evaluateQueryPatternFilterNoCommonSynonymFalse) {
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

	TEST_F(QueryEvaluatorPatternTest, evaluateQueryPatternFilterCommonSynonym) {
		std::string left1 = MODIFIES_LEFT1;
		std::string left2 = MODIFIES_LEFT2;
		std::string right1 = MODIFIES_RIGHT1;
		std::string right2 = MODIFIES_RIGHT2;

		Entity lhsX = { VARIABLE, x };
		Entity lhsY = { VARIABLE, y };

		Entity assignCommon = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhsCommon = { VARIABLE, COMMON_SYNONYM2 };

		std::vector<Pattern> patterns;
		std::vector<std::list<std::string>> results;
		std::vector<Entity> selected;

		//Handle result for Select a pattern a(_, _) - 1
		patterns.push_back(Pattern(assignCommon, WILD_CARD, "", true));
		selected.push_back(assignCommon);
		results.push_back({ left1, left2 });

		//Handle result for Select a pattern a(v, _) - 2
		patterns.push_back(Pattern(assignCommon, WILD_CARD, "", true));
		selected.push_back(assignCommon);
		results.push_back({ left1, left2 });

		//Handle result for Select a pattern a(_, "x") - 3
		patterns.push_back(Pattern(assignCommon, WILD_CARD, x, false));
		selected.push_back(assignCommon);
		results.push_back({ left1 });

		//Handle result for Select a pattern a(_, _"x"_) - 4
		patterns.push_back(Pattern(assignCommon, WILD_CARD, x, true));
		selected.push_back(assignCommon);
		results.push_back({ left1, left2 });

		//Handle result for Select a pattern a(_, "y") - 5
		patterns.push_back(Pattern(assignCommon, WILD_CARD, y, false));
		selected.push_back(assignCommon);
		results.push_back({ });

		//Handle result for Select a pattern a(_, _"y"_) - 6
		patterns.push_back(Pattern(assignCommon, WILD_CARD, y, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });

		//Handle result for Select a pattern a(v, "x") - 7 8
		patterns.push_back(Pattern(assignCommon, lhsCommon, x, false));
		selected.push_back(assignCommon);
		results.push_back({ left1 });
		patterns.push_back(Pattern(assignCommon, lhsCommon, x, false));
		selected.push_back(lhsCommon);
		results.push_back({ right1 });

		//Handle result for Select a pattern a(v, _"x"_) - 9 10
		patterns.push_back(Pattern(assignCommon, lhsCommon, x, true));
		selected.push_back(assignCommon);
		results.push_back({ left1, left2 });
		patterns.push_back(Pattern(assignCommon, lhsCommon, x, true));
		selected.push_back(lhsCommon);
		results.push_back({ right1, right2 });

		//Handle result for Select a pattern a(v, "y") - 11 12
		patterns.push_back(Pattern(assignCommon, lhsCommon, y, false));
		selected.push_back(assignCommon);
		results.push_back({ });
		patterns.push_back(Pattern(assignCommon, lhsCommon, y, false));
		selected.push_back(lhsCommon);
		results.push_back({ });

		//Handle result for Select a pattern a(v, _"y"_) - 13 14
		patterns.push_back(Pattern(assignCommon, lhsCommon, y, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });
		patterns.push_back(Pattern(assignCommon, lhsCommon, y, true));
		selected.push_back(lhsCommon);
		results.push_back({ right2 });

		//Handle result for Select a pattern a("x", "x") - 15
		patterns.push_back(Pattern(assignCommon, lhsX, x, false));
		selected.push_back(assignCommon);
		results.push_back({ left1 });

		//Handle result for Select a pattern a("x", _"x"_) - 16
		patterns.push_back(Pattern(assignCommon, lhsX, x, true));
		selected.push_back(assignCommon);
		results.push_back({ left1 });

		//Handle result for Select a pattern a("x", "y") - 17
		patterns.push_back(Pattern(assignCommon, lhsX, y, false));
		selected.push_back(assignCommon);
		results.push_back({ });

		//Handle result for Select a pattern a("x", _"y"_) - 18
		patterns.push_back(Pattern(assignCommon, lhsX, y, true));
		selected.push_back(assignCommon);
		results.push_back({ });

		//Handle result for Select a pattern a("y", "x") - 19
		patterns.push_back(Pattern(assignCommon, lhsY, x, false));
		selected.push_back(assignCommon);
		results.push_back({ });

		//Handle result for Select a pattern a("y", _"x"_) - 20
		patterns.push_back(Pattern(assignCommon, lhsY, x, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });

		//Handle result for Select a pattern a("y", _"y"_) - 21
		patterns.push_back(Pattern(assignCommon, lhsY, y, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });

		//Handle result for Select a pattern a("y", "5") - 22
		patterns.push_back(Pattern(assignCommon, lhsY, EXPRESSION_CONSTANT, false));
		selected.push_back(assignCommon);
		results.push_back({ });

		//Handle result for Select a pattern a("y", _"5"_) - 23
		patterns.push_back(Pattern(assignCommon, lhsY, EXPRESSION_CONSTANT, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });

		//Handle result for Select a pattern a(_, _"5"_) - 24
		patterns.push_back(Pattern(assignCommon, WILD_CARD, EXPRESSION_CONSTANT, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });

		//Handle result for Select a pattern a(v, _"5"_) - 25 26
		patterns.push_back(Pattern(assignCommon, lhsCommon, EXPRESSION_CONSTANT, true));
		selected.push_back(assignCommon);
		results.push_back({ left2 });
		patterns.push_back(Pattern(assignCommon, lhsCommon, EXPRESSION_CONSTANT, true));
		selected.push_back(lhsCommon);
		results.push_back({ right2 });

		for (unsigned int i = 0; i < patterns.size(); i++) {
			Query q = initQuery(patterns[i], selected[i]);
			std::list<std::string> result = evaluator.evaluateQuery(q).front();
			result.sort();
			results[i].sort();
			EXPECT_EQ(result, results[i]) << "ERROR AT " << i + 1;
		}
	}
}