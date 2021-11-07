#include "pch.h"

#include "PQL/QueryEvaluator/QueryEvaluator.h"
#include "PKB/PKBAdapter.h"
#include "PKB/PKB.h"
#include "Common/Common.h"
#include "Common/ExprParser.h"

namespace UnitTesting {
	class QueryEvaluatorCFGBipRelationTest : public testing::Test {
	protected:
		QueryEvaluatorCFGBipRelationTest() {
			PKB::getInstance().resetCache();

			PKB::getInstance().addVariable({ x });
			PKB::getInstance().addVariable({ y });
			PKB::getInstance().addVariable({ z });
			PKB::getInstance().addProcedure(p1);
			PKB::getInstance().addProcedure(p2);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addNext(std::stoi(NEXT_LEFT1), std::stoi(NEXT_RIGHT1));
			PKB::getInstance().addNext(std::stoi(NEXT_LEFT2), std::stoi(NEXT_RIGHT2));
			PKB::getInstance().addProcContains(p1, 1);
			PKB::getInstance().addProcContains(p1, 2);
			PKB::getInstance().addProcContains(p1, 3);
			PKB::getInstance().addProcContains(p2, 4);
			PKB::getInstance().addVariable(x);
			PKB::getInstance().addModifiesS(1, x);
			PKB::getInstance().addUsesS(2, x);
			PKB::getInstance().addModifiesS(2, y);
			PKB::getInstance().addUsesS(3, y);

			cfg1 = new CFG();
			cfg1->addLine(1);
			cfg1->addLine(2);
			cfg1->addLine(3);

			cfg2 = new CFG();
			cfg2->addLine(4);
			PKB::getInstance().addCFGBip(cfg1);
			PKB::getInstance().addCFGBip(cfg2);

			PKBAdapter::getRelationManager().reset();
		}

		~QueryEvaluatorCFGBipRelationTest() override {
			PKB::getInstance().resetCache();
			delete cfg1;
			delete cfg2;
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

		Query initQuery(RelRef relation, Entity selected) {
			Query q;
			q.addRelation(relation);
			q.addSelected(selected);
			return q;
		}
		void expectListEqual(std::list<std::string> r1, std::list<std::string> r2) {
			r1.sort();
			r2.sort();
			EXPECT_EQ(r1, r2);
		}

		void validate(Query q, std::list<std::string> result, int i) {
			EXPECT_EQ(evaluator.evaluateQuery(q), result) << "Error at results : " << i + 1;
		}

		void validate(RelRef relation, int i) {
			validate(initQuery(relation, SELECT_BOOLEAN), BOOLEAN_TRUE_RESULT, i);
		}

		void validateEmpty(RelRef relation, int i) {
			validate(initQuery(relation, SELECT_BOOLEAN), BOOLEAN_FALSE_RESULT, i);
		}

		void validateRelations(std::vector<RelRef> relations) {
			for (unsigned int i = 0; i < relations.size(); i++) {
				validate(relations[i], i);
			}
		}

		void validateEmptyRelations(std::vector<RelRef> relations) {
			for (unsigned int i = 0; i < relations.size(); i++) {
				validateEmpty(relations[i], i);
			}
		}

		void validateRelations(std::vector<RelRef> relations, std::vector<Entity> selected, std::vector<std::list<std::string>> results) {
			for (unsigned int i = 0; i < relations.size(); i++) {
				for (unsigned int j = 0; j < selected.size(); j++) {
					validate(initQuery(relations[i], selected[j]), results[j], i);
				}
			}
		}
		CFG* cfg1;
		CFG* cfg2;

		PKBAdapter pkb;
		QueryEvaluator evaluator;
		ExprParser expr_parser;

		const var_name x = "x";
		const var_name y = "y";
		const var_name z = "z";
		const proc_name p1 = "main";
		const proc_name p2 = "sub";

		const std::string NEXT_LEFT1 = "1";
		const std::string NEXT_LEFT2 = "2";
		const std::string NEXT_RIGHT1 = "2";
		const std::string NEXT_RIGHT2 = "3";

		const std::vector<std::string> NEXT_LEFTS = { NEXT_LEFT1, NEXT_LEFT2 };
		const std::vector<std::string> NEXT_RIGHTS = { NEXT_RIGHT1, NEXT_RIGHT2 };

		const std::string AFFECTS_LEFT1 = "1";
		const std::string AFFECTS_LEFT2 = "2";
		const std::string AFFECTS_RIGHT1 = "2";
		const std::string AFFECTS_RIGHT2 = "3";

		const std::vector<std::string> AFFECTS_LEFTS = { AFFECTS_LEFT1, AFFECTS_LEFT2 };
		const std::vector<std::string> AFFECTS_RIGHTS = { AFFECTS_RIGHT1, AFFECTS_RIGHT2 };

		const Synonym COMMON_SYNONYM1 = { "cs1" };
		const Synonym COMMON_SYNONYM2 = { "cs2" };

		const Entity SELECT_BOOLEAN = { BOOLEAN };

		const std::list<std::string> EMPTY_RESULT = {};
		const std::list<std::string> BOOLEAN_TRUE_RESULT = { BOOLEAN_TRUE };
		const std::list<std::string> BOOLEAN_FALSE_RESULT = { BOOLEAN_FALSE };

		const Entity WILD_CARD = { WILD };

		const std::vector<Entity> VALID_CONSTANT_STMT_ENTITY = { WILD_CARD,
			{STMT, "1"}, {STMT, "2"}, {STMT, "3"}, {STMT, "4"}
		};

		std::vector<Entity> ALL_VARIABLES = { { VARIABLE, x }, { VARIABLE, y }, { VARIABLE, z } };
	};

	//NextBip Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryNextBipBooleanTrue) {
		RelType type = NEXT_BIP;
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
	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryNextBipBooleanFalse) {
		RelType type = NEXT_BIP;
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

		std::vector<Entity> invalid_lefts = getInvalidConstant(lefts);
		for (unsigned int k = 0; k < invalid_lefts.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, invalid_lefts[k], VALID_CONSTANT_STMT_ENTITY[i]));
			}
		}

		std::vector<Entity> invalid_rights = getInvalidConstant(rights);
		for (unsigned int k = 0; k < invalid_rights.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalid_rights[k]));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryNextBipFilterEmpty) {
		RelType type = NEXT_BIP;
		std::vector<RelRef> relations;
		std::vector<std::string> lefts = NEXT_LEFTS;
		std::vector<std::string> rights = NEXT_RIGHTS;

		std::vector<Entity> invalid_lefts = getInvalidConstant(lefts);
		for (Entity it : invalid_lefts) {
			relations.push_back(RelRef(type, it, { STMT, Synonym{"a"} }));
		}

		std::vector<Entity> invalid_rights = getInvalidConstant(rights);
		for (Entity it : invalid_rights) {
			relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, it));
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryNextBipFilterNoCommonSynonymTrue) {
		RelType type = NEXT_BIP;
		std::vector<RelRef> relations;
		std::string left1 = NEXT_LEFT1;
		std::string left2 = NEXT_LEFT2;
		std::string right1 = NEXT_RIGHT1;
		std::string right2 = NEXT_RIGHT2;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { ASSIGN, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { ASSIGN, Synonym{"b"} }));
		relations.push_back(RelRef(type, { ASSIGN, Synonym{"a"} }, { ASSIGN, Synonym{"b"} }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, WILD_CARD, { ASSIGN, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { ASSIGN, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left1 }, { ASSIGN, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { ASSIGN, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { ASSIGN, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right2 }));
		relations.push_back(RelRef(type, { ASSIGN, Synonym{"a"} }, { STMT, right2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryNextBipFilterNoCommonSynonymFalse) {
		RelType type = NEXT_BIP;

		std::vector<std::string> lefts = NEXT_LEFTS;
		std::vector<std::string> rights = NEXT_RIGHTS;

		std::vector<RelRef> relations;
		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, COMMON_SYNONYM1 });
		synonyms.push_back({ READ, COMMON_SYNONYM1 });
		synonyms.push_back({ IF, COMMON_SYNONYM1 });
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
	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryNextBipFilterCommonSynonym) {
		RelType type = NEXT_BIP;
		std::vector<std::string> lefts = NEXT_LEFTS;
		std::vector<std::string> rights = NEXT_RIGHTS;
		std::string left1 = NEXT_LEFT1;
		std::string left2 = NEXT_LEFT2;
		std::string right1 = NEXT_RIGHT1;
		std::string right2 = NEXT_RIGHT2;

		std::vector<Entity> selected_list;
		selected_list.push_back({ STMT, COMMON_SYNONYM1 });
		selected_list.push_back({ ASSIGN, COMMON_SYNONYM1 });
		selected_list.push_back({ WHILE, COMMON_SYNONYM1 });
		selected_list.push_back({ PRINT, COMMON_SYNONYM1 });
		selected_list.push_back({ READ, COMMON_SYNONYM1 });
		selected_list.push_back({ IF, COMMON_SYNONYM1 });
		selected_list.push_back({ CALL, COMMON_SYNONYM1 });

		//Test case for Select selected such that NextBip(selected, a)
		std::list<std::string> result1 = { left1, left2 };
		std::list<std::string> result2 = { left1, left2 };
		std::list<std::string> result3 = { };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> result_list = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], { STMT, Synonym{"a"} });
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that NextBip(selected, _)
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], WILD_CARD);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that NextBip(a, selected)
		result_list[0] = { right1, right2 };
		result_list[1] = { right1, right2 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that NextBip(_, selected)
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, WILD_CARD, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that NextBip("1", selected)
		result_list[0] = { right1 };
		result_list[1] = { right1 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, { STMT, left1 }, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that NextBip("2", selected)
		result_list[0] = { right2 };
		result_list[1] = { right2 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, { STMT, left2 }, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that NextBip(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selected_list.size(); i++) {
				RelRef relation(type, emptyList[j], selected_list[i]);
				Query q = initQuery(relation, selected_list[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select selected such that NextBip(selected, "2")
		result_list[0] = { left1 };
		result_list[1] = { left1 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], { STMT, right1 });
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that NextBip(selected, "3")
		result_list[0] = { left2 };
		result_list[1] = { left2 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], { STMT, right2 });
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that NextBip(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selected_list.size(); i++) {
				RelRef relation(type, selected_list[i], emptyList[j]);
				Query q = initQuery(relation, selected_list[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}
	}

	//NEXT_BIP_T Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryNextBipTBooleanTrue) {
		RelType type = NEXT_BIP_T;
		std::string left1 = NEXT_LEFT1;
		std::string left2 = NEXT_LEFT2;
		std::string right1 = NEXT_RIGHT1;
		std::string right2 = NEXT_RIGHT2;

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

	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryNextBipTBooleanFalse) {
		RelType type = NEXT_BIP_T;
		std::vector<std::string> lefts = NEXT_LEFTS;
		std::vector<std::string> rights = NEXT_RIGHTS;
		std::string left1 = NEXT_LEFT1;
		std::string left2 = NEXT_LEFT2;
		std::string right1 = NEXT_RIGHT1;
		std::string right2 = NEXT_RIGHT2;

		std::vector<RelRef> relations;
		//Test false boolean equation
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right1 }));

		std::vector<Entity> invalid_lefts = getInvalidConstant(lefts);
		for (unsigned int k = 0; k < invalid_lefts.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, invalid_lefts[k], VALID_CONSTANT_STMT_ENTITY[i]));
			}
		}

		std::vector<Entity> invalid_rights = getInvalidConstant(rights);
		for (unsigned int k = 0; k < invalid_rights.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalid_rights[k]));
			}
		}

		validateEmptyRelations(relations);
	}

	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryNextBipTFilterEmpty) {
		RelType type = NEXT_BIP_T;
		std::vector<RelRef> relations;
		std::vector<std::string> lefts = NEXT_LEFTS;
		std::vector<std::string> rights = NEXT_RIGHTS;

		std::vector<Entity> invalid_lefts = getInvalidConstant(lefts);
		for (Entity it : invalid_lefts) {
			relations.push_back(RelRef(type, it, { STMT, Synonym{"a"} }));
		}

		std::vector<Entity> invalid_rights = getInvalidConstant(rights);
		for (Entity it : invalid_rights) {
			relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, it));
		}

		validateEmptyRelations(relations);
	}

	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryNextBipTFilterNoCommonSynonymTrue) {
		RelType type = NEXT_BIP_T;
		std::string left1 = NEXT_LEFT1;
		std::string left2 = NEXT_LEFT2;
		std::string right1 = NEXT_RIGHT1;
		std::string right2 = NEXT_RIGHT2;

		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryNextBipTFilterCommonSynonym) {
		RelType type = NEXT_BIP_T;
		std::vector<std::string> lefts = NEXT_LEFTS;
		std::vector<std::string> rights = NEXT_RIGHTS;
		std::string left1 = NEXT_LEFT1;
		std::string left2 = NEXT_LEFT2;
		std::string right1 = NEXT_RIGHT1;
		std::string right2 = NEXT_RIGHT2;

		Entity selected_entity(STMT, COMMON_SYNONYM1);

		//Test case for Select selected such that NextBipT(selected, a)
		std::list<std::string> result = { left1, left2 };

		RelRef relation(type, selected_entity, { STMT, Synonym{"a"} });
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that NextBipT(selected, _)
		relation = RelRef(type, selected_entity, WILD_CARD);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that NextBipT(a, selected)
		result = { right1, right2 };
		relation = RelRef(type, { STMT, Synonym{"a"} }, selected_entity);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that NextBipT(_, selected)
		relation = RelRef(type, WILD_CARD, selected_entity);
		expectListEqual(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that NextBipT("1", selected)
		relation = RelRef(type, { STMT, left1 }, selected_entity);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that NextBipT("2", selected)
		result = { right2 };
		relation = RelRef(type, { STMT, left2 }, selected_entity);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for remaining Select selected such that NextBipT(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			RelRef relation(type, emptyList[j], selected_entity);
			Query q = initQuery(relation, selected_entity);
			EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << j + 1;
		}

		//Test case for Select selected such that NextBipT(selected, "2")
		result = { left1 };
		relation = RelRef(type, selected_entity, { STMT, right1 });
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that NextBipT(selected, "3")
		result = { left1, left2 };
		relation = RelRef(type, selected_entity, { STMT, right2 });
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for remaining Select selected such that NextBipT(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			RelRef relation(type, selected_entity, emptyList[j]);
			Query q = initQuery(relation, selected_entity);
			EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << j + 1;
		}
	}

	//AffectsBip Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryAffectsBipBooleanTrue) {
		RelType type = AFFECT_BIP;
		std::string left1 = NEXT_LEFT1;
		std::string left2 = AFFECTS_LEFT2;
		std::string right1 = AFFECTS_RIGHT1;
		std::string right2 = AFFECTS_RIGHT2;

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
	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryAffectsBipBooleanFalse) {
		RelType type = AFFECT_BIP;
		std::vector<std::string> lefts = AFFECTS_LEFTS;
		std::vector<std::string> rights = AFFECTS_RIGHTS;
		std::string left1 = AFFECTS_LEFT1;
		std::string left2 = AFFECTS_LEFT2;
		std::string right1 = AFFECTS_RIGHT1;
		std::string right2 = AFFECTS_RIGHT2;

		std::vector<RelRef> relations;
		//Test false boolean equation
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right2 }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right1 }));

		std::vector<Entity> invalid_lefts = getInvalidConstant(lefts);
		for (unsigned int k = 0; k < invalid_lefts.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, invalid_lefts[k], VALID_CONSTANT_STMT_ENTITY[i]));
			}
		}

		std::vector<Entity> invalid_rights = getInvalidConstant(rights);
		for (unsigned int k = 0; k < invalid_rights.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalid_rights[k]));
			}
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryAffectsBipFilterEmpty) {
		RelType type = AFFECT_BIP;
		std::vector<RelRef> relations;
		std::vector<std::string> lefts = AFFECTS_LEFTS;
		std::vector<std::string> rights = AFFECTS_RIGHTS;

		std::vector<Entity> invalid_lefts = getInvalidConstant(lefts);
		for (Entity it : invalid_lefts) {
			relations.push_back(RelRef(type, it, { STMT, Synonym{"a"} }));
		}

		std::vector<Entity> invalid_rights = getInvalidConstant(rights);
		for (Entity it : invalid_rights) {
			relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, it));
		}

		validateEmptyRelations(relations);
	}
	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryAffectsBipFilterNoCommonSynonymTrue) {
		RelType type = AFFECT_BIP;
		std::vector<RelRef> relations;
		std::string left1 = AFFECTS_LEFT1;
		std::string left2 = AFFECTS_LEFT2;
		std::string right1 = AFFECTS_RIGHT1;
		std::string right2 = AFFECTS_RIGHT2;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { ASSIGN, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { ASSIGN, Synonym{"b"} }));
		relations.push_back(RelRef(type, { ASSIGN, Synonym{"a"} }, { ASSIGN, Synonym{"b"} }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, WILD_CARD, { ASSIGN, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { ASSIGN, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left1 }, { ASSIGN, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { ASSIGN, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { ASSIGN, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right2 }));
		relations.push_back(RelRef(type, { ASSIGN, Synonym{"a"} }, { STMT, right2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryAffectsBipFilterNoCommonSynonymFalse) {
		RelType type = AFFECT_BIP;

		std::vector<std::string> lefts = AFFECTS_LEFTS;
		std::vector<std::string> rights = AFFECTS_RIGHTS;

		std::vector<RelRef> relations;
		//Empty result for non-matching header for double column
		std::vector<Entity> synonyms;
		synonyms.push_back({ PRINT, COMMON_SYNONYM1 });
		synonyms.push_back({ READ, COMMON_SYNONYM1 });
		synonyms.push_back({ IF, COMMON_SYNONYM1 });
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
	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryAffectsBipFilterCommonSynonym) {
		RelType type = AFFECT_BIP;
		std::vector<std::string> lefts = AFFECTS_LEFTS;
		std::vector<std::string> rights = AFFECTS_RIGHTS;
		std::string left1 = AFFECTS_LEFT1;
		std::string left2 = AFFECTS_LEFT2;
		std::string right1 = AFFECTS_RIGHT1;
		std::string right2 = AFFECTS_RIGHT2;

		std::vector<Entity> selected_list;
		selected_list.push_back({ STMT, COMMON_SYNONYM1 });
		selected_list.push_back({ ASSIGN, COMMON_SYNONYM1 });
		selected_list.push_back({ WHILE, COMMON_SYNONYM1 });
		selected_list.push_back({ PRINT, COMMON_SYNONYM1 });
		selected_list.push_back({ READ, COMMON_SYNONYM1 });
		selected_list.push_back({ IF, COMMON_SYNONYM1 });
		selected_list.push_back({ CALL, COMMON_SYNONYM1 });

		//Test case for Select selected such that AffectsBip(selected, a)
		std::list<std::string> result1 = { left1, left2 };
		std::list<std::string> result2 = { left1, left2 };
		std::list<std::string> result3 = { };
		std::list<std::string> result4 = { };
		std::list<std::string> result5 = { };
		std::list<std::string> result6 = { };
		std::list<std::string> result7 = { };

		std::vector<std::list<std::string>> result_list = { result1, result2, result3,
			result4, result5, result6, result7 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], { STMT, Synonym{"a"} });
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that AffectsBip(selected, _)
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], WILD_CARD);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that AffectsBip(a, selected)
		result_list[0] = { right1, right2 };
		result_list[1] = { right1, right2 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that AffectsBip(_, selected)
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, WILD_CARD, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			expectListEqual(evaluator.evaluateQuery(q), result_list[i]);
		}

		//Test case for Select selected such that AffectsBip("1", selected)
		result_list[0] = { right1 };
		result_list[1] = { right1 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, { STMT, left1 }, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that AffectsBip("2", selected)
		result_list[0] = { right2 };
		result_list[1] = { right2 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, { STMT, left2 }, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that AffectsBip(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selected_list.size(); i++) {
				RelRef relation(type, emptyList[j], selected_list[i]);
				Query q = initQuery(relation, selected_list[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select selected such that AffectsBip(selected, "2")
		result_list[0] = { left1 };
		result_list[1] = { left1 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], { STMT, right1 });
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that AffectsBip(selected, "3")
		result_list[0] = { left2 };
		result_list[1] = { left2 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], { STMT, right2 });
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that AffectsBip(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selected_list.size(); i++) {
				RelRef relation(type, selected_list[i], emptyList[j]);
				Query q = initQuery(relation, selected_list[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}
	}

	//AFFECT_BIP_T Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryAffectsBipTBooleanTrue) {
		RelType type = AFFECT_BIP_T;
		std::string left1 = AFFECTS_LEFT1;
		std::string left2 = AFFECTS_LEFT2;
		std::string right1 = AFFECTS_RIGHT1;
		std::string right2 = AFFECTS_RIGHT2;

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

	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryAffectsBipTBooleanFalse) {
		RelType type = AFFECT_BIP_T;
		std::vector<std::string> lefts = AFFECTS_LEFTS;
		std::vector<std::string> rights = AFFECTS_RIGHTS;
		std::string left1 = AFFECTS_LEFT1;
		std::string left2 = AFFECTS_LEFT2;
		std::string right1 = AFFECTS_RIGHT1;
		std::string right2 = AFFECTS_RIGHT2;

		std::vector<RelRef> relations;
		//Test false boolean equation
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right1 }));

		std::vector<Entity> invalid_lefts = getInvalidConstant(lefts);
		for (unsigned int k = 0; k < invalid_lefts.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, invalid_lefts[k], VALID_CONSTANT_STMT_ENTITY[i]));
			}
		}

		std::vector<Entity> invalid_rights = getInvalidConstant(rights);
		for (unsigned int k = 0; k < invalid_rights.size(); k++) {
			for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
				relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalid_rights[k]));
			}
		}

		validateEmptyRelations(relations);
	}

	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryAffectsBipTFilterEmpty) {
		RelType type = AFFECT_BIP_T;
		std::vector<RelRef> relations;
		std::vector<std::string> lefts = AFFECTS_LEFTS;
		std::vector<std::string> rights = AFFECTS_RIGHTS;

		std::vector<Entity> invalid_lefts = getInvalidConstant(lefts);
		for (Entity it : invalid_lefts) {
			relations.push_back(RelRef(type, it, { STMT, Synonym{"a"} }));
		}

		std::vector<Entity> invalid_rights = getInvalidConstant(rights);
		for (Entity it : invalid_rights) {
			relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, it));
		}

		validateEmptyRelations(relations);
	}

	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryAffectsBipTFilterNoCommonSynonymTrue) {
		RelType type = AFFECT_BIP_T;
		std::string left1 = AFFECTS_LEFT1;
		std::string left2 = AFFECTS_LEFT2;
		std::string right1 = AFFECTS_RIGHT1;
		std::string right2 = AFFECTS_RIGHT2;

		std::vector<RelRef> relations;
		//Have Result for matching header
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, Synonym{"b"} }));
		relations.push_back(RelRef(type, WILD_CARD, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, WILD_CARD));
		relations.push_back(RelRef(type, { STMT, left1 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, left2 }, { STMT, Synonym{"a"} }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right1 }));
		relations.push_back(RelRef(type, { STMT, Synonym{"a"} }, { STMT, right2 }));

		validateRelations(relations);
	}

	TEST_F(QueryEvaluatorCFGBipRelationTest, evaluateQueryAffectsBipTFilterCommonSynonym) {
		RelType type = AFFECT_BIP_T;
		std::vector<std::string> lefts = AFFECTS_LEFTS;
		std::vector<std::string> rights = AFFECTS_RIGHTS;
		std::string left1 = AFFECTS_LEFT1;
		std::string left2 = AFFECTS_LEFT2;
		std::string right1 = AFFECTS_RIGHT1;
		std::string right2 = AFFECTS_RIGHT2;

		Entity selected_entity(STMT, COMMON_SYNONYM1);

		//Test case for Select selected such that AffectsBipT(selected, a)
		std::list<std::string> result = { left1, left2 };

		RelRef relation(type, selected_entity, { STMT, Synonym{"a"} });
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that AffectsBipT(selected, _)
		relation = RelRef(type, selected_entity, WILD_CARD);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that AffectsBipT(a, selected)
		result = { right1, right2 };
		relation = RelRef(type, { STMT, Synonym{"a"} }, selected_entity);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that AffectsBipT(_, selected)
		relation = RelRef(type, WILD_CARD, selected_entity);
		expectListEqual(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that AffectsBipT("1", selected)
		relation = RelRef(type, { STMT, left1 }, selected_entity);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that AffectsBipT("2", selected)
		result = { right2 };
		relation = RelRef(type, { STMT, left2 }, selected_entity);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for remaining Select selected such that AffectsBipT(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			RelRef relation(type, emptyList[j], selected_entity);
			Query q = initQuery(relation, selected_entity);
			EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << j + 1;
		}

		//Test case for Select selected such that AffectsBipT(selected, "2")
		result = { left1 };
		relation = RelRef(type, selected_entity, { STMT, right1 });
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that AffectsBipT(selected, "3")
		result = { left1, left2 };
		relation = RelRef(type, selected_entity, { STMT, right2 });
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for remaining Select selected such that AffectsBipT(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			RelRef relation(type, selected_entity, emptyList[j]);
			Query q = initQuery(relation, selected_entity);
			EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << j + 1;
		}
	}
}