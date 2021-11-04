#include "pch.h"

#include "QueryEvaluator.h"
#include "PKBAdapter.h"
#include "PKB.h"
#include "Common.h"
#include "ExprParser.h"

namespace UnitTesting {
	class QueryEvaluatorMultiClausesTest : public testing::Test {
	protected:
		QueryEvaluatorMultiClausesTest() {
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
			PKB::getInstance().addExprTree(std::stoi(MODIFIES_LEFT3), EXPRESSIONNODE_1);
			PKB::getInstance().addExprTree(std::stoi(MODIFIES_LEFT4), EXPRESSIONNODE_2);
			PKB::getInstance().generateFollowsT();
			PKB::getInstance().generateParentT();
		}

		~QueryEvaluatorMultiClausesTest() override {
			PKB::getInstance().resetCache();
		}

		std::vector<Entity> getInvalidConstant(std::vector<std::string> validStmt) {
			std::vector<Entity> invalid;
			for (Entity it : VALID_CONSTANT_STMT_ENTITY) {
				if (it.getType() == WILD) {
					continue;
				}
				bool is_invalid = true;
				for (std::string valid : validStmt) {
					if (it.getValue() == valid) {
						is_invalid = false;
					}
				}
				if (is_invalid) {
					invalid.push_back(it);
				}
			}
			return invalid;
		}

		Query initQuery(std::vector<RelRef> relations, std::vector<Pattern> patterns, Entity selected) {
			Query q;
			for (auto& it : relations) {
				q.addRelation(it);
			}

			for (auto& it : patterns) {
				q.addPattern(it);
			}
			q.addSelected(selected);
			return q;
		}

		void validate(Query q, std::list<std::string> result, int i, int j) {
			std::list<std::string> query_result = evaluator.evaluateQuery(q);
			result.sort();
			query_result.sort();
			EXPECT_EQ(query_result, result) << "Error at results : " << i + 1 << " : " << j + 1;
		}

		void validate(std::vector<RelRef> relations, std::vector<Pattern> patterns, int i, int j) {

			validate(initQuery(relations, patterns, ALL_SELECT[i % ALL_SELECT.size()]), ALL_RESULT[i % ALL_SELECT.size()], i, j);
			validate(initQuery(relations, patterns, SELECT_BOOLEAN), BOOLEAN_TRUE_RESULT, i, j);
		}

		void validateEmpty(std::vector<RelRef> relations, std::vector<Pattern> patterns, int i, int j) {
			validate(initQuery(relations, patterns, ALL_SELECT[i % ALL_SELECT.size()]), EMPTY_RESULT, i, j);
			validate(initQuery(relations, patterns, SELECT_BOOLEAN), BOOLEAN_FALSE_RESULT, i, j);
		}

		void validate(std::vector<Pattern>& patterns, std::vector<RelRef>& relations) {
			for (unsigned int i = 0; i < relations.size(); i++) {
				for (unsigned int j = 0; j < patterns.size(); j++) {
					validate({ relations[i] }, { patterns[j] }, i, j);
				}
			}
		}

		void validate(std::vector<Pattern>& patterns1, std::vector<Pattern>& patterns2) {
			for (unsigned int i = 0; i < patterns1.size(); i++) {
				for (unsigned int j = 0; j < patterns2.size(); j++) {
					validate({ }, { patterns1[i], patterns2[j] }, i, j);
				}
			}
		}

		void validate(std::vector<RelRef>& relations1, std::vector<RelRef>& relations2) {
			for (unsigned int i = 0; i < relations1.size(); i++) {
				for (unsigned int j = 0; j < relations2.size(); j++) {
					validate({ relations1[i], relations2[j] }, { }, i, j);
				}
			}
		}

		void validateEmpty(std::vector<Pattern>& patterns, std::vector<RelRef>& relations) {
			for (unsigned int i = 0; i < relations.size(); i++) {
				for (unsigned int j = 0; j < patterns.size(); j++) {
					validateEmpty({ relations[i] }, { patterns[j] }, i, j);
				}
			}
		}

		void validateEmpty(std::vector<Pattern>& patterns1, std::vector<Pattern>& patterns2) {
			for (unsigned int i = 0; i < patterns1.size(); i++) {
				for (unsigned int j = 0; j < patterns2.size(); j++) {
					validateEmpty({ }, { patterns1[i], patterns2[j] }, i, j);
				}
			}
		}

		void validateEmpty(std::vector<RelRef>& relations1, std::vector<RelRef>& relations2) {
			for (unsigned int i = 0; i < relations1.size(); i++) {
				for (unsigned int j = 0; j < relations2.size(); j++) {
					validateEmpty({ relations1[i], relations2[j] }, { }, i, j);
				}
			}
		}

		std::vector<RelRef> getAllValidRelation(Synonym s1, Synonym s2) {
			std::vector<RelRef> relations;

			RelType type = FOLLOWS;
			std::string left1 = FOLLOW_LEFT1;
			std::string left2 = FOLLOW_LEFT2;
			std::string right1 = FOLLOW_RIGHT1;
			std::string right2 = FOLLOW_RIGHT2;
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, right1 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, right2 }));

			type = FOLLOWS_T;
			left1 = FOLLOW_LEFT1;
			left2 = FOLLOW_LEFT2;
			right1 = FOLLOW_RIGHT1;
			right2 = FOLLOW_RIGHT2;
			relations.push_back(RelRef(type, WILD_CARD, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, WILD_CARD));

			type = PARENT;
			left1 = PARENT_LEFT1;
			left2 = PARENT_LEFT2;
			std::string left3 = PARENT_LEFT3;
			right1 = PARENT_RIGHT1;
			right2 = PARENT_RIGHT2;
			std::string right3 = PARENT_RIGHT3;
			relations.push_back(RelRef(type, WILD_CARD, { STMT, right1 }));
			relations.push_back(RelRef(type, WILD_CARD, { STMT, right2 }));
			relations.push_back(RelRef(type, WILD_CARD, { STMT, right3 }));

			type = PARENT_T;
			left1 = PARENT_LEFT1;
			left2 = PARENT_LEFT2;
			left3 = PARENT_LEFT3;
			right1 = PARENT_RIGHT1;
			right2 = PARENT_RIGHT2;
			right3 = PARENT_RIGHT3;
			relations.push_back(RelRef(type, { STMT, left1 }, { STMT, s1 }));
			relations.push_back(RelRef(type, { WHILE, s1 }, { STMT, right3 }));

			type = MODIFIES_S;
			left1 = MODIFIES_LEFT1;
			left2 = MODIFIES_LEFT2;
			right1 = MODIFIES_RIGHT1;
			right2 = MODIFIES_RIGHT2;
			relations.push_back(RelRef(type, { STMT, left1 }, { VARIABLE, right1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { VARIABLE, right2 }));

			type = USES_S;
			left1 = USES_LEFT1;
			left2 = USES_LEFT2;
			right1 = USES_RIGHT1;
			right2 = USES_RIGHT2;
			relations.push_back(RelRef(type, { STMT, left2 }, WILD_CARD));
			relations.push_back(RelRef(type, { WHILE, s1 }, { VARIABLE, right2 }));

			return relations;
		}

		std::vector<RelRef> getAllInvalidRelation(Synonym s1, Synonym s2) {
			std::vector<RelRef> relations;

			RelType type = FOLLOWS;
			std::vector<std::string> lefts = FOLLOW_LEFTS;
			std::vector<std::string> rights = FOLLOW_RIGHTS;
			std::string left1 = FOLLOW_LEFT1;
			std::string left2 = FOLLOW_LEFT2;
			std::string right1 = FOLLOW_RIGHT1;
			std::string right2 = FOLLOW_RIGHT2;

			relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right2 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right1 }));

			std::vector<Entity> invalid_lefts = getInvalidConstant(lefts);
			for (unsigned int k = 0; k < invalid_lefts.size(); k++) {
				relations.push_back(RelRef(type, invalid_lefts[k], VALID_CONSTANT_STMT_ENTITY[k % VALID_CONSTANT_STMT_ENTITY.size()]));
			}

			std::vector<Entity> invalid_rights = getInvalidConstant(rights);
			for (unsigned int k = 0; k < invalid_rights.size(); k++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[k % VALID_CONSTANT_STMT_ENTITY.size()], invalid_rights[k]));
			}

			for (Entity it : invalid_lefts) {
				relations.push_back(RelRef(type, it, { STMT, s1 }));
			}

			for (Entity it : invalid_rights) {
				relations.push_back(RelRef(type, { STMT, s1 }, it));
			}

			type = FOLLOWS_T;
			lefts = FOLLOW_LEFTS;
			rights = FOLLOW_RIGHTS;
			left1 = FOLLOW_LEFT1;
			left2 = FOLLOW_LEFT2;
			right1 = FOLLOW_RIGHT1;
			right2 = FOLLOW_RIGHT2;

			invalid_rights = getInvalidConstant(rights);
			for (unsigned int k = 0; k < invalid_rights.size(); k++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[k % VALID_CONSTANT_STMT_ENTITY.size()], invalid_rights[k]));
			}

			left1 = PARENT_LEFT1;
			left2 = PARENT_LEFT2;
			std::string left3 = PARENT_LEFT3;
			right1 = PARENT_RIGHT1;
			right2 = PARENT_RIGHT2;
			std::string right3 = PARENT_RIGHT3;
			lefts = PARENT_LEFTS;
			rights = PARENT_RIGHTS;
			//Test false boolean equation

			relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "2" }));
			relations.push_back(RelRef(type, { STMT, "3" }, { STMT, "3" }));

			type = MODIFIES_S;
			left1 = MODIFIES_LEFT1;
			left2 = MODIFIES_LEFT2;
			right1 = MODIFIES_RIGHT1;
			right2 = MODIFIES_RIGHT2;
			lefts = MODIFIES_LEFTS;

			std::vector<Entity> synonyms = {};
			synonyms.push_back({ CALL, s1 });

			for (unsigned int k = 0; k < synonyms.size(); k++) {
				for (unsigned int j = 0; j < ALL_VARIABLES.size(); j++) {
					relations.push_back(RelRef(type, synonyms[k], ALL_VARIABLES[j]));
				}
			}

			type = USES_S;
			left1 = USES_LEFT1;
			left2 = USES_LEFT2;
			right1 = USES_RIGHT1;
			right2 = USES_RIGHT2;
			lefts = USES_LEFTS;

			invalid_rights = { { VARIABLE, z } };
			for (unsigned int k = 0; k < invalid_rights.size(); k++) {
				for (unsigned int i = 1; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
					relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalid_rights[k]));
				}
			}

			return relations;

		}

		std::vector<Pattern> getAllValidPattern(Synonym s1, Synonym s2) {
			Entity assign = { ASSIGN, s1 };
			Entity lhs_synonym = { VARIABLE, s2 };
			Entity lhs_x = { VARIABLE, x };
			Entity lhs_y = { VARIABLE, y };

			std::vector<Pattern> patterns;
			patterns.push_back(Pattern(assign, lhs_x, "x", true));
			patterns.push_back(Pattern(assign, lhs_x, "x", false));
			patterns.push_back(Pattern(assign, lhs_y, "y", true));
			patterns.push_back(Pattern(assign, lhs_y, "x", true));

			patterns.push_back(Pattern(assign, lhs_synonym, "x", true));
			patterns.push_back(Pattern(assign, lhs_synonym, "x", false));
			patterns.push_back(Pattern(assign, lhs_synonym, "y", true));

			return patterns;
		}

		std::vector<Pattern> getAllInvalidPattern(Synonym s1, Synonym s2) {
			Entity assign = { ASSIGN, s1 };
			Entity lhs_synonym = { VARIABLE, s2 };
			Entity lhs_x = { VARIABLE, x };
			Entity lhs_y = { VARIABLE, y };
			Entity lhs_z = { VARIABLE, z };
			Entity lhs_n = { VARIABLE, "n" };

			std::vector<std::string> pattern_expr;
			pattern_expr.push_back("y");
			pattern_expr.push_back("z");
			pattern_expr.push_back("n");
			std::vector<Pattern> patterns;
			for (auto expr : pattern_expr) {
				patterns.push_back(Pattern(assign, lhs_z, expr, true));
				patterns.push_back(Pattern(assign, lhs_n, expr, false));
				patterns.push_back(Pattern(assign, lhs_n, expr, true));
			}

			patterns.push_back(Pattern(assign, lhs_x, "y", true));
			patterns.push_back(Pattern(assign, lhs_y, "x", false));
			patterns.push_back(Pattern(assign, lhs_synonym, "n", false));
			patterns.push_back(Pattern(assign, lhs_synonym, "z", true));
			patterns.push_back(Pattern(assign, lhs_synonym, "n", true));

			return patterns;
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
		const std::string WHILE2 = "4";
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
		expr EXPRESSIONNODE_1 = expr_parser.parse(EXPRESSION1);
		expr EXPRESSIONNODE_2 = expr_parser.parse(EXPRESSION2);

		const std::vector<std::string> MODIFIES_LEFTS = { MODIFIES_LEFT1, MODIFIES_LEFT2, MODIFIES_LEFT3, MODIFIES_LEFT4 };
		const std::vector<std::string> MODIFIES_RIGHTS = { MODIFIES_RIGHT1, MODIFIES_RIGHT2, MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		const std::vector<std::string> EXPRESSIONS = { EXPRESSION1, EXPRESSION2 };

		const std::string USES_LEFT1 = "1";
		const std::string USES_LEFT2 = "3";
		const std::string USES_RIGHT1 = y;
		const std::string USES_RIGHT2 = x;

		const std::vector<std::string> USES_LEFTS = { USES_LEFT1, USES_LEFT2 };
		const std::vector<std::string> USES_RIGHTS = { USES_RIGHT1, USES_RIGHT2 };

		const std::list<std::string> STMTS = { IF1, IF2, WHILE1, WHILE2, READ1, READ2,
			PRINT1, PRINT2, ASSIGN1, ASSIGN2, CALL1, CALL2 };

		const Synonym COMMON_SYNONYM1 = { "cs1" };
		const Synonym COMMON_SYNONYM2 = { "cs2" };
		const Synonym COMMON_SYNONYM3 = { "cs3" };
		const Synonym COMMON_SYNONYM4 = { "cs4" };
		const Synonym COMMON_SYNONYM5 = { "cs5" };
		const Synonym COMMON_SYNONYM6 = { "cs6" };

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

	//Multiple Clause, Pattern and Relation----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryTwoClausesNoCommonSynonmsTrue) {
		std::vector<RelRef> valid_relation = getAllValidRelation({ "a" }, { "b" });
		std::vector<Pattern> valid_pattern = getAllValidPattern({ "c" }, { "d" });
		std::vector<RelRef> valid_relation1 = getAllValidRelation({ "e" }, { "f" });
		std::vector<Pattern> valid_pattern1 = getAllValidPattern({ "g" }, { "h" });

		validate(valid_pattern, valid_relation);
		validate(valid_pattern, valid_pattern1);
		validate(valid_relation, valid_relation1);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryTwoClausesNoCommonSynonymFalses) {
		std::vector<RelRef> valid_relation = getAllValidRelation({ "a" }, { "b" });
		std::vector<Pattern> valid_pattern = getAllValidPattern({ "c" }, { "d" });
		std::vector<RelRef> invalidRelation = getAllInvalidRelation({ "a1" }, { "b1" });
		std::vector<Pattern> invalidPattern = getAllInvalidPattern({ "c1" }, { "d1" });

		validateEmpty(valid_pattern, invalidRelation);
		validateEmpty(invalidPattern, valid_pattern);
		validateEmpty(invalidPattern, invalidRelation);

		validateEmpty(valid_pattern, invalidPattern);
		validateEmpty(invalidPattern, valid_pattern);
		validateEmpty(invalidPattern, invalidPattern);

		validateEmpty(valid_relation, invalidRelation);
		validateEmpty(invalidRelation, valid_relation);
		validateEmpty(invalidRelation, invalidRelation);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationFilterSingleCommonSynonymsEmpty) {
		Entity assign_common = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhs_common = { VARIABLE, COMMON_SYNONYM2 };
		Entity stmt_common = { STMT,  COMMON_SYNONYM3 };
		Entity stmt_common1 = { STMT,  COMMON_SYNONYM4 };

		//Handle result for Select a pattern a(_, "x") such that Uses(a,_)
		Pattern pattern(assign_common, WILD_CARD, x, false);
		RelRef relation(USES_S, assign_common, WILD_CARD);
		Entity selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, "x") such that Uses(a,_)
		pattern = Pattern(assign_common, lhs_common, x, false);
		relation = RelRef(USES_S, assign_common, WILD_CARD);
		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);

		//Handle result for Select a pattern a(_, "z") such that Modifies(a,_)
		pattern = Pattern(assign_common, WILD_CARD, z, false);
		relation = RelRef(MODIFIES_S, assign_common, WILD_CARD);
		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, _"z"_) such that Modifies(a,_)
		pattern = Pattern(assign_common, lhs_common, z, true);
		relation = RelRef(MODIFIES_S, assign_common, WILD_CARD);
		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);

		//Handle result for Select a pattern a(_, _"x"_) such that Parent(s,a)
		pattern = Pattern(assign_common, WILD_CARD, x, true);
		relation = RelRef(PARENT, stmt_common, assign_common);
		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);

		pattern = Pattern(assign_common, WILD_CARD, x, true);
		relation = RelRef(PARENT, stmt_common, assign_common);
		selected = stmt_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);

		//Handle result for Select a pattern a(_, _"x"_) such that Parent(a,s)
		pattern = Pattern(assign_common, WILD_CARD, x, true);
		relation = RelRef(PARENT, assign_common, stmt_common);
		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);

		pattern = Pattern(assign_common, WILD_CARD, x, true);
		relation = RelRef(PARENT, assign_common, stmt_common);
		selected = stmt_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationJoinSingleCommonSynonymsEmpty) {
		Entity assign_common = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhs_common = { VARIABLE, COMMON_SYNONYM2 };
		Entity lhs_common1 = { VARIABLE, COMMON_SYNONYM4 };
		Entity stmt_common = { STMT,  COMMON_SYNONYM3 };
		Entity stmt_common1 = { STMT,  COMMON_SYNONYM5 };

		//Handle result for Select a pattern a(v, _"z"_) such that Uses(s,v)
		Pattern pattern(assign_common, lhs_common, z, true);
		RelRef relation(USES_S, stmt_common, lhs_common);

		Entity selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
		selected = stmt_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
		selected = lhs_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, "x") such that Uses(a,v1)
		pattern = Pattern(assign_common, lhs_common, x, false);
		relation = RelRef(USES_S, assign_common, lhs_common1);

		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
		selected = lhs_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
		selected = lhs_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, _) such that Parent(s,a)
		pattern = Pattern(assign_common, lhs_common, "", true);
		relation = RelRef(PARENT, stmt_common, assign_common);

		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
		selected = lhs_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
		selected = stmt_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, _) such that Parent(a,s)
		pattern = Pattern(assign_common, lhs_common, "", true);
		relation = RelRef(PARENT, assign_common, stmt_common);

		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
		selected = lhs_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
		selected = stmt_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationFilterDoubleCommonSynonymsEmpty) {
		Entity assign_common = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhs_common = { VARIABLE, COMMON_SYNONYM2 };

		//Handle result for Select a pattern a(v, "y") such that MODIFIES_S(a,v)
		Pattern pattern(assign_common, lhs_common, y, false);
		RelRef relation(MODIFIES_S, assign_common, lhs_common);
		Entity selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
		selected = lhs_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, _"z"_) such that MODIFIES_S(a,v)
		pattern = Pattern(assign_common, lhs_common, z, true);
		relation = RelRef(MODIFIES_S, assign_common, lhs_common);
		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
		selected = lhs_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, _) such that USES_S(a,v)
		pattern = Pattern(assign_common, lhs_common, "", true);
		relation = RelRef(USES_S, assign_common, lhs_common);
		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
		selected = lhs_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), EMPTY_RESULT);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationFilterSingleCommonSynonyms) {
		Entity assign_common = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhs_common = { VARIABLE, COMMON_SYNONYM2 };
		Entity stmt_common = { STMT,  COMMON_SYNONYM3 };

		//Handle result for Select a pattern a(_, "x") such that Modifies(a,v)
		Pattern pattern(assign_common, WILD_CARD, x, false);
		RelRef relation(MODIFIES_S, assign_common, lhs_common);

		std::list<std::string> result = { MODIFIES_LEFT3 };
		Entity selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(_, _"x"_) such that Modifies(a,v)
		pattern = Pattern(assign_common, WILD_CARD, x, true);
		relation = RelRef(MODIFIES_S, assign_common, lhs_common);

		selected = assign_common;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(_, _"y"_) such that Modifies(a,v)
		pattern = Pattern(assign_common, WILD_CARD, y, true);
		relation = RelRef(MODIFIES_S, assign_common, lhs_common);

		selected = assign_common;
		result = { MODIFIES_LEFT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(_, _) such that Modifies(a,v)
		pattern = Pattern(assign_common, WILD_CARD, "", true);
		relation = RelRef(MODIFIES_S, assign_common, lhs_common);

		selected = assign_common;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(v, "x") such that Modifies(a,_)
		pattern = Pattern(assign_common, lhs_common, x, false);
		relation = RelRef(MODIFIES_S, assign_common, WILD_CARD);

		result = { MODIFIES_LEFT3 };
		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(v, _"x"_) such that Modifies(a,_)
		pattern = Pattern(assign_common, lhs_common, x, true);
		relation = RelRef(MODIFIES_S, assign_common, WILD_CARD);

		selected = assign_common;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(v, _"y"_) such that Modifies(a,_)
		pattern = Pattern(assign_common, lhs_common, y, true);
		relation = RelRef(MODIFIES_S, assign_common, WILD_CARD);

		selected = assign_common;
		result = { MODIFIES_LEFT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(v, _) such that Modifies(a,_)
		pattern = Pattern(assign_common, lhs_common, "", true);
		relation = RelRef(MODIFIES_S, assign_common, WILD_CARD);

		selected = assign_common;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(_, "x") such that Modifies(a,_)
		pattern = Pattern(assign_common, lhs_common, x, false);
		relation = RelRef(MODIFIES_S, assign_common, WILD_CARD);
		result = { MODIFIES_LEFT3 };
		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(_, _"x"_) such that Modifies(a,_)
		pattern = Pattern(assign_common, lhs_common, x, true);
		relation = RelRef(MODIFIES_S, assign_common, WILD_CARD);
		selected = assign_common;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(_, _"y"_) such that Modifies(a,_)
		pattern = Pattern(assign_common, lhs_common, y, true);
		relation = RelRef(MODIFIES_S, assign_common, WILD_CARD);
		selected = assign_common;
		result = { MODIFIES_LEFT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationJoinSingleCommonSynonyms) {
		Entity assign_common = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhs_common = { VARIABLE, COMMON_SYNONYM2 };
		Entity stmt_common = { STMT,  COMMON_SYNONYM3 };

		//Handle result for Select a pattern a(v, "x") such that uses(s,v)
		Pattern pattern(assign_common, lhs_common, x, false);
		RelRef relation(USES_S, stmt_common, lhs_common);
		Entity selected = assign_common;
		std::list<std::string> result = { MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = stmt_common;
		result = { USES_LEFT2 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(v, _"x"_) such that uses(s,v)
		pattern = Pattern(assign_common, lhs_common, x, true);
		relation = RelRef(USES_S, stmt_common, lhs_common);

		selected = assign_common;
		result = { MODIFIES_LEFT3, MODIFIES_LEFT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT4, MODIFIES_RIGHT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = stmt_common;
		result = { USES_LEFT1, USES_LEFT2 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(v, _"y"_) such that uses(s,v)
		pattern = Pattern(assign_common, lhs_common, y, true);
		relation = RelRef(USES_S, stmt_common, lhs_common);

		selected = assign_common;
		result = { MODIFIES_LEFT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = stmt_common;
		result = { USES_LEFT1 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(v, _) such that uses(s,v)
		pattern = Pattern(assign_common, lhs_common, "", true);
		relation = RelRef(USES_S, stmt_common, lhs_common);
		selected = assign_common;
		result = { MODIFIES_LEFT3, MODIFIES_LEFT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT4, MODIFIES_RIGHT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = stmt_common;
		result = { USES_LEFT1, USES_LEFT2 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(v, "x") such that modifies(s,v)
		pattern = Pattern(assign_common, lhs_common, x, false);
		relation = RelRef(MODIFIES_S, stmt_common, lhs_common);
		selected = assign_common;
		result = { MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		pattern = Pattern(assign_common, lhs_common, x, false);
		relation = RelRef(MODIFIES_S, stmt_common, lhs_common);
		selected = stmt_common;
		result = { MODIFIES_LEFT3, MODIFIES_LEFT1 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(v, _"x"_) such that modifies(s,v)
		pattern = Pattern(assign_common, lhs_common, x, true);
		relation = RelRef(MODIFIES_S, stmt_common, lhs_common);

		selected = assign_common;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = stmt_common;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3, MODIFIES_LEFT1, MODIFIES_LEFT2 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationFilterDoubleCommonSynonyms) {
		Entity assign_common = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhs_common = { VARIABLE, COMMON_SYNONYM2 };

		//Handle result for Select a pattern a(v, "_") such that MODIFIES_S(a,v)
		Pattern pattern(assign_common, lhs_common, "", true);
		RelRef relation(MODIFIES_S, assign_common, lhs_common);

		std::list<std::string> result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		Entity selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		//Handle result for Select a pattern a(v, "x") such that MODIFIES_S(a,v)
		pattern = Pattern(assign_common, lhs_common, "x", false);
		relation = RelRef(MODIFIES_S, assign_common, lhs_common);

		result = { MODIFIES_LEFT3, };
		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)), result);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationJoinDoubleCommonSynonyms) {
		Entity assign_common = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhs_common = { VARIABLE, COMMON_SYNONYM2 };
		Entity stmt_common = { STMT,  COMMON_SYNONYM3 };
		Entity assign_common1 = { ASSIGN,  COMMON_SYNONYM4 };
		Entity lhs_common1 = { VARIABLE, COMMON_SYNONYM5 };
		Entity stmt_common1 = { STMT,  COMMON_SYNONYM6 };

		Pattern pattern1(assign_common, lhs_common, "", true);
		RelRef relation1(MODIFIES_S, assign_common, lhs_common);
		Pattern pattern2(assign_common1, lhs_common1, "", true);
		RelRef relation2(MODIFIES_S, assign_common1, lhs_common1);
		Pattern pattern3(assign_common1, lhs_common, "", true);

		Entity selected = assign_common;
		std::list<std::string> result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), result);
		selected = assign_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), result);
		selected = lhs_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), result);

		pattern1 = Pattern(assign_common, lhs_common, "", true);
		relation1 = RelRef(MODIFIES_S, stmt_common, lhs_common);
		pattern2 = Pattern(assign_common1, lhs_common1, "", true);
		relation2 = RelRef(MODIFIES_S, stmt_common1, lhs_common1);
		pattern3 = Pattern(assign_common1, lhs_common, "", true);

		selected = assign_common;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), result);
		selected = assign_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), result);

		selected = lhs_common;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), result);
		selected = lhs_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), result);

		selected = stmt_common;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3, MODIFIES_LEFT1, MODIFIES_LEFT2 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), result);
		selected = stmt_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), result);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationJoinDoubleCommonSynonymsEmpty) {
		Entity assign_common = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhs_common = { VARIABLE, COMMON_SYNONYM2 };
		Entity stmt_common = { STMT,  COMMON_SYNONYM3 };
		Entity assign_common1 = { ASSIGN,  COMMON_SYNONYM4 };
		Entity lhs_common1 = { VARIABLE, COMMON_SYNONYM5 };
		Entity stmt_common1 = { STMT,  COMMON_SYNONYM6 };

		Pattern pattern1(assign_common, lhs_common, "", true);
		RelRef relation1(MODIFIES_S, assign_common, lhs_common);
		Pattern pattern2(assign_common1, lhs_common1, "", true);
		RelRef relation2(MODIFIES_S, assign_common1, lhs_common1);
		Pattern pattern3(assign_common1, lhs_common, "z", true);

		Entity selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);
		selected = assign_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);

		selected = lhs_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);
		selected = lhs_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);

		pattern1 = Pattern(assign_common, lhs_common, "", true);
		relation1 = RelRef(MODIFIES_S, stmt_common, lhs_common);
		pattern2 = Pattern(assign_common1, lhs_common1, "", true);
		relation2 = RelRef(MODIFIES_S, stmt_common1, lhs_common1);
		pattern3 = Pattern(assign_common1, lhs_common, "z", true);

		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);
		selected = assign_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);

		selected = lhs_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);
		selected = lhs_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);

		selected = stmt_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);
		selected = stmt_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);

		pattern1 = Pattern(assign_common, lhs_common, "", true);
		relation1 = RelRef(MODIFIES_S, stmt_common, lhs_common);
		pattern2 = Pattern(assign_common1, lhs_common1, "", true);
		relation2 = RelRef(PARENT, stmt_common, stmt_common1);
		pattern3 = Pattern(assign_common1, lhs_common, "z", true);

		selected = assign_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);
		selected = assign_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);

		selected = lhs_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);
		selected = lhs_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);

		selected = stmt_common;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);
		selected = stmt_common1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)), EMPTY_RESULT);
	}
}