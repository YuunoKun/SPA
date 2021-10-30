#include "pch.h"

#include "QueryEvaluator.h"
#include "PKBAdapter.h"
#include "PKB.h"
#include "Common.h"
#include "ExprParser.h"

namespace UnitTesting {
	class QueryEvaluatorCFGRelationTest : public testing::Test {
	protected:
		QueryEvaluatorCFGRelationTest() {
			PKB::getInstance().resetCache();
			PKB::getInstance().addVariable({ x });
			PKB::getInstance().addVariable({ y });
			PKB::getInstance().addVariable({ z });
			PKB::getInstance().addProcedure(p1);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addNext(std::stoi(NEXT_LEFT1), std::stoi(NEXT_RIGHT1));
			PKB::getInstance().addNext(std::stoi(NEXT_LEFT2), std::stoi(NEXT_RIGHT2));
			PKB::getInstance().addProcContains(p1, 1);
			PKB::getInstance().addProcContains(p1, 2);
			PKB::getInstance().addProcContains(p1, 3);
			PKB::getInstance().addProcContains(p1, 4);
			PKB::getInstance().addVariable(x);
			PKB::getInstance().addModifiesS(1, x);
			PKB::getInstance().addUsesS(2, x);
			PKB::getInstance().addModifiesS(2, y);
			PKB::getInstance().addUsesS(3, y);

			PKBAdapter::getRelationManager().update();
		}

		~QueryEvaluatorCFGRelationTest() override {
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

		PKBAdapter pkb;
		QueryEvaluator evaluator;
		ExprParser expr_parser;

		const var_name x = "x";
		const var_name y = "y";
		const var_name z = "z";
		const proc_name p1 = "main";

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


	//Next Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryNextBooleanTrue) {
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
	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryNextBooleanFalse) {
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
	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryNextFilterEmpty) {
		RelType type = NEXT;
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
	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryNextFilterNoCommonSynonymTrue) {
		RelType type = NEXT;
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

	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryNextFilterNoCommonSynonymFalse) {
		RelType type = NEXT;

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
	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryNextFilterCommonSynonym) {
		RelType type = NEXT;
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

		//Test case for Select selected such that Next(selected, a)
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

		//Test case for Select selected such that Next(selected, _)
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], WILD_CARD);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Next(a, selected)
		result_list[0] = { right1, right2 };
		result_list[1] = { right1, right2 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Next(_, selected)
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, WILD_CARD, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Next("1", selected)
		result_list[0] = { right1 };
		result_list[1] = { right1 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, { STMT, left1 }, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Next("2", selected)
		result_list[0] = { right2 };
		result_list[1] = { right2 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, { STMT, left2 }, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that Next(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selected_list.size(); i++) {
				RelRef relation(type, emptyList[j], selected_list[i]);
				Query q = initQuery(relation, selected_list[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select selected such that Next(selected, "2")
		result_list[0] = { left1 };
		result_list[1] = { left1 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], { STMT, right1 });
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Next(selected, "3")
		result_list[0] = { left2 };
		result_list[1] = { left2 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], { STMT, right2 });
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that Next(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selected_list.size(); i++) {
				RelRef relation(type, selected_list[i], emptyList[j]);
				Query q = initQuery(relation, selected_list[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}
	}

	//NEXT_T Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryNextTBooleanTrue) {
		RelType type = NEXT_T;
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

	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryNextTBooleanFalse) {
		RelType type = NEXT_T;
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

	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryNextTFilterEmpty) {
		RelType type = NEXT_T;
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

	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryNextTFilterNoCommonSynonymTrue) {
		RelType type = NEXT_T;
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

	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryNextTFilterCommonSynonym) {
		RelType type = NEXT_T;
		std::vector<std::string> lefts = NEXT_LEFTS;
		std::vector<std::string> rights = NEXT_RIGHTS;
		std::string left1 = NEXT_LEFT1;
		std::string left2 = NEXT_LEFT2;
		std::string right1 = NEXT_RIGHT1;
		std::string right2 = NEXT_RIGHT2;


		Entity selected_entity(STMT, COMMON_SYNONYM1);

		//Test case for Select selected such that NextT(selected, a)
		std::list<std::string> result = { left1, left2 };

		RelRef relation(type, selected_entity, { STMT, Synonym{"a"} });
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that NextT(selected, _)
		relation = RelRef(type, selected_entity, WILD_CARD);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that NextT(a, selected)
		result = { right1, right2 };
		relation = RelRef(type, { STMT, Synonym{"a"} }, selected_entity);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that NextT(_, selected)
		relation = RelRef(type, WILD_CARD, selected_entity);
		expectListEqual(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that NextT("1", selected)
		relation = RelRef(type, { STMT, left1 }, selected_entity);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);
		
		//Test case for Select selected such that NextT("2", selected)
		result = { right2 };
		relation = RelRef(type, { STMT, left2 }, selected_entity);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);


		//Test case for remaining Select selected such that NextT(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			RelRef relation(type, emptyList[j], selected_entity);
			Query q = initQuery(relation, selected_entity);
			EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << j + 1;
		}

		//Test case for Select selected such that NextT(selected, "2")
		result = { left1 };
		relation = RelRef(type, selected_entity, { STMT, right1 });
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);


		//Test case for Select selected such that NextT(selected, "3")
		result = { left2, left1 };
		relation = RelRef(type, selected_entity, { STMT, right2 });
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for remaining Select selected such that NextT(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			RelRef relation(type, selected_entity, emptyList[j]);
			Query q = initQuery(relation, selected_entity);
			EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << j + 1;
		}
	}



	//Affects Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryAffectsBooleanTrue) {
		RelType type = AFFECT;
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
	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryAffectsBooleanFalse) {
		RelType type = AFFECT;
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
	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryAffectsFilterEmpty) {
		RelType type = AFFECT;
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
	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryAffectsFilterNoCommonSynonymTrue) {
		RelType type = AFFECT;
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

	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryAffectsFilterNoCommonSynonymFalse) {
		RelType type = AFFECT;

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
	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryAffectsFilterCommonSynonym) {
		RelType type = AFFECT;
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

		//Test case for Select selected such that Affects(selected, a)
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

		//Test case for Select selected such that Affects(selected, _)
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], WILD_CARD);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Affects(a, selected)
		result_list[0] = { right1, right2 };
		result_list[1] = { right1, right2 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, { STMT, Synonym{"a"} }, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Affects(_, selected)
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, WILD_CARD, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			expectListEqual(evaluator.evaluateQuery(q), result_list[i]);
		}

		//Test case for Select selected such that Affects("1", selected)
		result_list[0] = { right1 };
		result_list[1] = { right1 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, { STMT, left1 }, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Affects("2", selected)
		result_list[0] = { right2 };
		result_list[1] = { right2 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, { STMT, left2 }, selected_list[i]);
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that Affects(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selected_list.size(); i++) {
				RelRef relation(type, emptyList[j], selected_list[i]);
				Query q = initQuery(relation, selected_list[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}

		//Test case for Select selected such that Affects(selected, "2")
		result_list[0] = { left1 };
		result_list[1] = { left1 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], { STMT, right1 });
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for Select selected such that Affects(selected, "3")
		result_list[0] = { left2 };
		result_list[1] = { left2 };
		for (unsigned int i = 0; i < selected_list.size(); i++) {
			RelRef relation(type, selected_list[i], { STMT, right2 });
			Query q = initQuery(relation, selected_list[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), result_list[i]) << "Error at results : " << i + 1;
		}

		//Test case for remaining Select selected such that Affects(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			for (unsigned int i = 0; i < selected_list.size(); i++) {
				RelRef relation(type, selected_list[i], emptyList[j]);
				Query q = initQuery(relation, selected_list[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << i + 1;
			}
		}
	}

	//AFFECT_T Relation Test ----------------------------------------------------------------------------------------------------
	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryAffectsTBooleanTrue) {
		RelType type = AFFECT_T;
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

	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryAffectsTBooleanFalse) {
		RelType type = AFFECT_T;
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

	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryAffectsTFilterEmpty) {
		RelType type = AFFECT_T;
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

	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryAffectsTFilterNoCommonSynonymTrue) {
		RelType type = AFFECT_T;
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

	TEST_F(QueryEvaluatorCFGRelationTest, evaluateQueryAffectsTFilterCommonSynonym) {
		RelType type = AFFECT_T;
		std::vector<std::string> lefts = AFFECTS_LEFTS;
		std::vector<std::string> rights = AFFECTS_RIGHTS;
		std::string left1 = AFFECTS_LEFT1;
		std::string left2 = AFFECTS_LEFT2;
		std::string right1 = AFFECTS_RIGHT1;
		std::string right2 = AFFECTS_RIGHT2;


		Entity selected_entity(STMT, COMMON_SYNONYM1);

		//Test case for Select selected such that AffectsT(selected, a)
		std::list<std::string> result = { left1, left2 };

		RelRef relation(type, selected_entity, { STMT, Synonym{"a"} });
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that AffectsT(selected, _)
		relation = RelRef(type, selected_entity, WILD_CARD);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that AffectsT(a, selected)
		result = { right1, right2 };
		relation = RelRef(type, { STMT, Synonym{"a"} }, selected_entity);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that AffectsT(_, selected)
		relation = RelRef(type, WILD_CARD, selected_entity);
		expectListEqual(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that AffectsT("1", selected)
		relation = RelRef(type, { STMT, left1 }, selected_entity);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for Select selected such that AffectsT("2", selected)
		result = { right2 };
		relation = RelRef(type, { STMT, left2 }, selected_entity);
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);


		//Test case for remaining Select selected such that AffectsT(anyEmpty, selected)
		std::vector<Entity> emptyList = getInvalidConstant(lefts);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			RelRef relation(type, emptyList[j], selected_entity);
			Query q = initQuery(relation, selected_entity);
			EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << j + 1;
		}

		//Test case for Select selected such that AffectsT(selected, "2")
		result = { left1 };
		relation = RelRef(type, selected_entity, { STMT, right1 });
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);


		//Test case for Select selected such that AffectsT(selected, "3")
		result = { left2, left1 };
		relation = RelRef(type, selected_entity, { STMT, right2 });
		EXPECT_EQ(evaluator.evaluateQuery(initQuery(relation, selected_entity)), result);

		//Test case for remaining Select selected such that AffectsT(selected, anyEmpty)
		emptyList = getInvalidConstant(rights);
		for (unsigned int j = 0; j < emptyList.size(); j++) {
			RelRef relation(type, selected_entity, emptyList[j]);
			Query q = initQuery(relation, selected_entity);
			EXPECT_EQ(evaluator.evaluateQuery(q), EMPTY_RESULT) << "Error at results : " << j + 1;
		}
	}
}