#include "pch.h"

#include "QueryEvaluator.h"
#include "PKBAdapter.h"
#include "PKB.h"
#include "Common.h"

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

		void validate(std::vector<Pattern> patterns, std::vector<RelRef> relations) {
			for (unsigned int k = 0; k < patterns.size(); k++) {
				for (unsigned int i = 0; i < patterns.size(); i++) {
					for (unsigned int j = 0; j < ALL_SELECT.size(); j++) {
						Query q = initQuery({ relations[k] }, { patterns[i] }, ALL_SELECT[j]);
						EXPECT_EQ(evaluator.evaluateQuery(q).front(), ALL_RESULT[j]) << "Error at results : " << i + 1 << " : " << j + 1;
						q = initQuery({ relations[k] }, { patterns[i] }, SELECT_BOOLEAN);
						EXPECT_EQ(evaluator.evaluateQuery(q).front(), BOOLEAN_TRUE_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
					}
				}
			}
		}

		void validate(std::vector<Pattern> patterns, std::vector<RelRef> relations, std::vector<Entity> selected, std::vector<std::list<std::string>> results) {
			for (unsigned int i = 0; i < patterns.size(); i++) {
				Query q = initQuery({ relations[i] }, { patterns[i] }, selected[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q).front(), results[i]) << "Error at results : " << i + 1;
				q = initQuery({ relations[i] }, { patterns[i] }, SELECT_BOOLEAN);
				EXPECT_EQ(evaluator.evaluateQuery(q).front(), BOOLEAN_TRUE_RESULT) << "Error at results : " << i + 1;
			}
		}

		void validate(std::vector<Pattern> patterns1, std::vector<Pattern> patterns2) {
			for (unsigned int k = 0; k < patterns1.size(); k++) {
				for (unsigned int i = 0; i < patterns2.size(); i++) {
					for (unsigned int j = 0; j < ALL_SELECT.size(); j++) {
						Query q = initQuery({ }, { patterns1[k], patterns2[i] }, ALL_SELECT[j]);
						EXPECT_EQ(evaluator.evaluateQuery(q).front(), ALL_RESULT[j]) << "Error at results : " << i + 1 << " : " << j + 1;
						q = initQuery({ }, { patterns1[k], patterns2[i] }, SELECT_BOOLEAN);
						EXPECT_EQ(evaluator.evaluateQuery(q).front(), BOOLEAN_TRUE_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
					}
				}
			}
		}

		void validate(std::vector<RelRef> relations1, std::vector<RelRef> relations2) {
			for (unsigned int k = 0; k < relations1.size(); k++) {
				for (unsigned int i = 0; i < relations2.size(); i++) {
					for (unsigned int j = 0; j < ALL_SELECT.size(); j++) {
						Query q = initQuery({ relations1[k], relations2[i] }, {  }, ALL_SELECT[j]);
						EXPECT_EQ(evaluator.evaluateQuery(q).front(), ALL_RESULT[j]) << "Error at results : " << i + 1 << " : " << j + 1;
						q = initQuery({ relations1[k], relations2[i] }, {  }, SELECT_BOOLEAN);
						EXPECT_EQ(evaluator.evaluateQuery(q).front(), BOOLEAN_TRUE_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
					}
				}
			}
		}

		void validateEmpty(std::vector<Pattern>& patterns, std::vector<RelRef> relations) {
			for (unsigned int k = 0; k < patterns.size(); k++) {
				for (unsigned int i = 0; i < patterns.size(); i++) {
					for (unsigned int j = 0; j < ALL_SELECT.size(); j++) {
						Query q = initQuery({ relations[k] }, { patterns[i] }, ALL_SELECT[j]);
						EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
						q = initQuery({ relations[k] }, { patterns[i] }, SELECT_BOOLEAN);
						EXPECT_EQ(evaluator.evaluateQuery(q).front(), BOOLEAN_FALSE_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;

					}
				}
			}
		}

		void validateEmpty(std::vector<Pattern> patterns1, std::vector<Pattern> patterns2) {
			for (unsigned int k = 0; k < patterns1.size(); k++) {
				for (unsigned int i = 0; i < patterns2.size(); i++) {
					for (unsigned int j = 0; j < ALL_SELECT.size(); j++) {
						Query q = initQuery({ }, { patterns1[k], patterns2[i] }, ALL_SELECT[j]);
						EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
						q = initQuery({ }, { patterns1[k], patterns2[i] }, SELECT_BOOLEAN);
						EXPECT_EQ(evaluator.evaluateQuery(q).front(), BOOLEAN_FALSE_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
					}
				}
			}
		}

		void validateEmpty(std::vector<RelRef> relations1, std::vector<RelRef> relations2) {
			for (unsigned int k = 0; k < relations1.size(); k++) {
				for (unsigned int i = 0; i < relations2.size(); i++) {
					for (unsigned int j = 0; j < ALL_SELECT.size(); j++) {
						Query q = initQuery({ relations1[k], relations2[i] }, {  }, ALL_SELECT[j]);
						EXPECT_EQ(evaluator.evaluateQuery(q).front(), EMPTY_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
						q = initQuery({ relations1[k], relations2[i] }, {  }, BOOLEAN);
						EXPECT_EQ(evaluator.evaluateQuery(q).front(), BOOLEAN_FALSE_RESULT) << "Error at results : " << i + 1 << " : " << j + 1;
					}
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
			relations.push_back(RelRef(type, WILD_CARD, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right2 }));
			relations.push_back(RelRef(type, { STMT, left1 }, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left2 }, WILD_CARD));
			relations.push_back(RelRef(type, WILD_CARD, { STMT, right1 }));
			relations.push_back(RelRef(type, WILD_CARD, { STMT, right2 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, s2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, s2 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { IF, s2 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { WHILE, s2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { IF, s2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { WHILE, s2 }));
			relations.push_back(RelRef(type, WILD_CARD, { STMT, s1 }));
			relations.push_back(RelRef(type, WILD_CARD, { IF, s1 }));
			relations.push_back(RelRef(type, WILD_CARD, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { IF, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left1 }, { STMT, s1 }));
			relations.push_back(RelRef(type, { STMT, left1 }, { IF, s1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { STMT, s1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, right1 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, right1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, right2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, right2 }));

			type = FOLLOWS_T;
			left1 = FOLLOW_LEFT1;
			left2 = FOLLOW_LEFT2;
			right1 = FOLLOW_RIGHT1;
			right2 = FOLLOW_RIGHT2;
			relations.push_back(RelRef(type, WILD_CARD, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right1 }));
			relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right2 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right2 }));
			relations.push_back(RelRef(type, { STMT, left1 }, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left2 }, WILD_CARD));
			relations.push_back(RelRef(type, WILD_CARD, { STMT, right1 }));
			relations.push_back(RelRef(type, WILD_CARD, { STMT, right2 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, s2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, s2 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { IF, s2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { IF, s2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { WHILE, s2 }));
			relations.push_back(RelRef(type, WILD_CARD, { STMT, s1 }));
			relations.push_back(RelRef(type, WILD_CARD, { IF, s1 }));
			relations.push_back(RelRef(type, WILD_CARD, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { IF, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left1 }, { STMT, s1 }));
			relations.push_back(RelRef(type, { STMT, left1 }, { IF, s1 }));
			relations.push_back(RelRef(type, { STMT, left1 }, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { STMT, s1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, right1 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, right1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, right2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, right2 }));

			type = PARENT;
			left1 = PARENT_LEFT1;
			left2 = PARENT_LEFT2;
			std::string left3 = PARENT_LEFT3;
			right1 = PARENT_RIGHT1;
			right2 = PARENT_RIGHT2;
			std::string right3 = PARENT_RIGHT3;
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
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, s2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, s2 }));
			relations.push_back(RelRef(type, { WHILE, s1 }, { STMT, s2 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { IF, s2 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { WHILE, s2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { IF, s2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { WHILE, s2 }));
			relations.push_back(RelRef(type, { WHILE, s1 }, { WHILE, s2 }));
			relations.push_back(RelRef(type, WILD_CARD, { STMT, s1 }));
			relations.push_back(RelRef(type, WILD_CARD, { IF, s1 }));
			relations.push_back(RelRef(type, WILD_CARD, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { IF, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { WHILE, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left1 }, { STMT, s1 }));
			relations.push_back(RelRef(type, { STMT, left1 }, { IF, s1 }));
			relations.push_back(RelRef(type, { STMT, left1 }, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { STMT, s1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, left3 }, { STMT, s1 }));
			relations.push_back(RelRef(type, { STMT, left3 }, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, right1 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, right1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, right2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, right2 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, right3 }));
			relations.push_back(RelRef(type, { WHILE, s1 }, { STMT, right3 }));

			type = PARENT_T;
			left1 = PARENT_LEFT1;
			left2 = PARENT_LEFT2;
			left3 = PARENT_LEFT3;
			right1 = PARENT_RIGHT1;
			right2 = PARENT_RIGHT2;
			right3 = PARENT_RIGHT3;
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
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, s2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, s2 }));
			relations.push_back(RelRef(type, { WHILE, s1 }, { STMT, s2 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { IF, s2 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { WHILE, s2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { IF, s2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { WHILE, s2 }));
			relations.push_back(RelRef(type, { WHILE, s1 }, { WHILE, s2 }));
			relations.push_back(RelRef(type, WILD_CARD, { STMT, s1 }));
			relations.push_back(RelRef(type, WILD_CARD, { IF, s1 }));
			relations.push_back(RelRef(type, WILD_CARD, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { IF, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { WHILE, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left1 }, { STMT, s1 }));
			relations.push_back(RelRef(type, { STMT, left1 }, { IF, s1 }));
			relations.push_back(RelRef(type, { STMT, left1 }, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { STMT, s1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, left3 }, { STMT, s1 }));
			relations.push_back(RelRef(type, { STMT, left3 }, { WHILE, s1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, right1 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, right1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, right2 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, right2 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { STMT, right3 }));
			relations.push_back(RelRef(type, { WHILE, s1 }, { STMT, right3 }));
			relations.push_back(RelRef(type, { IF, s1 }, { STMT, right3 }));

			type = MODIFIES_S;
			left1 = MODIFIES_LEFT1;
			left2 = MODIFIES_LEFT2;
			right1 = MODIFIES_RIGHT1;
			right2 = MODIFIES_RIGHT2;
			relations.push_back(RelRef(type, WILD_CARD, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left1 }, { VARIABLE, right1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { VARIABLE, right2 }));
			relations.push_back(RelRef(type, { STMT, left1 }, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left2 }, WILD_CARD));
			relations.push_back(RelRef(type, WILD_CARD, { VARIABLE, right1 }));
			relations.push_back(RelRef(type, WILD_CARD, { VARIABLE, right2 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { VARIABLE, s1 }));
			relations.push_back(RelRef(type, { IF, s1 }, { VARIABLE, s1 }));
			relations.push_back(RelRef(type, { WHILE, s1 }, { VARIABLE, s1 }));
			relations.push_back(RelRef(type, WILD_CARD, { VARIABLE, s1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { IF, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { WHILE, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left1 }, { VARIABLE, s1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { VARIABLE, s1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { VARIABLE, right1 }));
			relations.push_back(RelRef(type, { IF, s1 }, { VARIABLE, right1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { VARIABLE, right2 }));
			relations.push_back(RelRef(type, { WHILE, s1 }, { VARIABLE, right2 }));

			type = USES_S;
			left1 = USES_LEFT1;
			left2 = USES_LEFT2;
			right1 = USES_RIGHT1;
			right2 = USES_RIGHT2;
			relations.push_back(RelRef(type, WILD_CARD, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left1 }, { VARIABLE, right1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { VARIABLE, right2 }));
			relations.push_back(RelRef(type, { STMT, left1 }, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left2 }, WILD_CARD));
			relations.push_back(RelRef(type, WILD_CARD, { VARIABLE, right1 }));
			relations.push_back(RelRef(type, WILD_CARD, { VARIABLE, right2 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { VARIABLE, s1 }));
			relations.push_back(RelRef(type, { IF, s1 }, { VARIABLE, s1 }));
			relations.push_back(RelRef(type, { WHILE, s1 }, { VARIABLE, s1 }));
			relations.push_back(RelRef(type, WILD_CARD, { VARIABLE, s1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { IF, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { WHILE, s1 }, WILD_CARD));
			relations.push_back(RelRef(type, { STMT, left1 }, { VARIABLE, s1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { VARIABLE, s1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { VARIABLE, right1 }));
			relations.push_back(RelRef(type, { IF, s1 }, { VARIABLE, right1 }));
			relations.push_back(RelRef(type, { STMT, s1 }, { VARIABLE, right2 }));
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

			for (Entity it : invalidLefts) {
				relations.push_back(RelRef(type, it, { STMT, s1 }));
			}

			for (Entity it : invalidRight) {
				relations.push_back(RelRef(type, { STMT, s1 }, it));
			}

			std::vector<Entity> synonyms;
			synonyms.push_back({ PRINT, s1 });
			synonyms.push_back({ READ, s1 });
			synonyms.push_back({ ASSIGN, s1 });
			synonyms.push_back({ CALL, s1 });

			for (unsigned int k = 0; k < synonyms.size(); k++) {
				for (unsigned int j = 0; j < synonyms.size(); j++) {
					relations.push_back(RelRef(type, synonyms[k], synonyms[j]));
				}
			}

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

			type = FOLLOWS_T;
			lefts = FOLLOW_LEFTS;
			rights = FOLLOW_RIGHTS;
			left1 = FOLLOW_LEFT1;
			left2 = FOLLOW_LEFT2;
			right1 = FOLLOW_RIGHT1;
			right2 = FOLLOW_RIGHT2;

			relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right1 }));

			invalidLefts = getInvalidConstant(lefts);
			for (unsigned int k = 0; k < invalidLefts.size(); k++) {
				for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
					relations.push_back(RelRef(type, invalidLefts[k], VALID_CONSTANT_STMT_ENTITY[i]));
				}
			}

			invalidRight = getInvalidConstant(rights);
			for (unsigned int k = 0; k < invalidRight.size(); k++) {
				for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
					relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k]));
				}
			}

			for (Entity it : invalidLefts) {
				relations.push_back(RelRef(type, it, { STMT, s1 }));
			}

			for (Entity it : invalidRight) {
				relations.push_back(RelRef(type, { STMT, s1 }, it));
			}

			synonyms = {};
			synonyms.push_back({ PRINT, s1 });
			synonyms.push_back({ READ, s1 });
			synonyms.push_back({ ASSIGN, s1 });
			synonyms.push_back({ CALL, s1 });

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

			type = PARENT;
			left1 = PARENT_LEFT1;
			left2 = PARENT_LEFT2;
			std::string left3 = PARENT_LEFT3;
			right1 = PARENT_RIGHT1;
			right2 = PARENT_RIGHT2;
			std::string right3 = PARENT_RIGHT3;

			relations.push_back(RelRef(type, { STMT, left1 }, { STMT, right3 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right1 }));
			relations.push_back(RelRef(type, { STMT, left2 }, { STMT, right3 }));
			relations.push_back(RelRef(type, { STMT, left3 }, { STMT, right2 }));

			invalidLefts = getInvalidConstant(lefts);

			for (unsigned int k = 0; k < invalidLefts.size(); k++) {
				for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
					relations.push_back(RelRef(type, invalidLefts[k], VALID_CONSTANT_STMT_ENTITY[i]));
				}
			}

			invalidRight = getInvalidConstant(rights);

			for (unsigned int k = 0; k < invalidRight.size(); k++) {
				for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
					relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k]));
				}
			}

			for (Entity it : invalidLefts) {
				relations.push_back(RelRef(type, it, { STMT, s1 }));
			}

			for (Entity it : invalidRight) {
				relations.push_back(RelRef(type, { STMT, s1 }, it));
			}

			synonyms = {};
			synonyms.push_back({ PRINT, s1 });
			synonyms.push_back({ READ, s1 });
			synonyms.push_back({ ASSIGN, s1 });
			synonyms.push_back({ CALL, s1 });

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

			type = PARENT_T;
			left1 = PARENT_LEFT1;
			left2 = PARENT_LEFT2;
			left3 = PARENT_LEFT3;
			right1 = PARENT_RIGHT1;
			right2 = PARENT_RIGHT2;
			right3 = PARENT_RIGHT3;
			lefts = PARENT_LEFTS;
			rights = PARENT_RIGHTS;
			//Test false boolean equation

			relations.push_back(RelRef(type, { STMT, "2" }, { STMT, "2" }));
			relations.push_back(RelRef(type, { STMT, "3" }, { STMT, "3" }));

			invalidLefts = getInvalidConstant(lefts);

			for (unsigned int k = 0; k < invalidLefts.size(); k++) {
				for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
					relations.push_back((RelRef(type, invalidLefts[k], VALID_CONSTANT_STMT_ENTITY[i])));
				}
			}

			invalidRight = getInvalidConstant(rights);

			for (unsigned int k = 0; k < invalidRight.size(); k++) {
				for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
					relations.push_back((RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k])));
				}
			}

			for (Entity it : invalidLefts) {
				relations.push_back(RelRef(type, it, { STMT, s1 }));
			}

			for (Entity it : invalidRight) {
				relations.push_back(RelRef(type, { STMT, s1 }, it));
			}

			synonyms = {};
			synonyms.push_back({ PRINT, s1 });
			synonyms.push_back({ READ, s1 });
			synonyms.push_back({ ASSIGN, s1 });
			synonyms.push_back({ CALL, s1 });

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

			type = MODIFIES_S;
			left1 = MODIFIES_LEFT1;
			left2 = MODIFIES_LEFT2;
			right1 = MODIFIES_RIGHT1;
			right2 = MODIFIES_RIGHT2;
			lefts = MODIFIES_LEFTS;

			invalidLefts = getInvalidConstant(lefts);

			for (unsigned int k = 0; k < invalidLefts.size(); k++) {
				for (unsigned int i = 0; i < ALL_VARIABLES.size(); i++) {
					relations.push_back((RelRef(type, invalidLefts[k], ALL_VARIABLES[i])));
				}
			}

			invalidRight = { { VARIABLE, z } };

			for (unsigned int k = 0; k < invalidRight.size(); k++) {
				for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
					relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k]));
				}
			}

			for (Entity it : invalidLefts) {
				relations.push_back(RelRef(type, it, { STMT, s1 }));
			}
			relations.push_back(RelRef(type, { STMT, s1 }, { VARIABLE, z }));

			synonyms = {};
			synonyms.push_back({ PRINT, s1 });
			synonyms.push_back({ READ, s1 });
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

			invalidLefts = getInvalidConstant(lefts);

			for (unsigned int k = 0; k < invalidLefts.size(); k++) {
				for (unsigned int i = 0; i < ALL_VARIABLES.size(); i++) {
					relations.push_back((RelRef(type, invalidLefts[k], ALL_VARIABLES[i])));
				}
			}

			invalidRight = { { VARIABLE, z } };

			for (unsigned int k = 0; k < invalidRight.size(); k++) {
				for (unsigned int i = 0; i < VALID_CONSTANT_STMT_ENTITY.size(); i++) {
					relations.push_back(RelRef(type, VALID_CONSTANT_STMT_ENTITY[i], invalidRight[k]));
				}
			}

			for (Entity it : invalidLefts) {
				relations.push_back(RelRef(type, it, { STMT, s1 }));
			}

			relations.push_back(RelRef(type, { STMT, s1 }, { VARIABLE, z }));

			synonyms = { };
			synonyms.push_back({ PRINT, s1 });
			synonyms.push_back({ READ, s1 });
			synonyms.push_back({ ASSIGN, s1 });
			synonyms.push_back({ CALL, s1 });

			for (unsigned int k = 0; k < synonyms.size(); k++) {
				for (unsigned int j = 0; j < ALL_VARIABLES.size(); j++) {
					relations.push_back(RelRef(type, synonyms[k], ALL_VARIABLES[j]));
				}
			}

			return relations;
		}

		std::vector<Pattern> getAllValidPattern(Synonym s1, Synonym s2) {
			Entity assign = { ASSIGN, s1 };
			Entity lhsSynonym = { VARIABLE, s2 };
			Entity lhsX = { VARIABLE, x };
			Entity lhsY = { VARIABLE, y };

			std::vector<Pattern> patterns;
			patterns.push_back(Pattern(assign, lhsX, "x", true));
			patterns.push_back(Pattern(assign, lhsX, "x", false));
			patterns.push_back(Pattern(assign, lhsY, "y", true));
			patterns.push_back(Pattern(assign, lhsY, "x", true));

			patterns.push_back(Pattern(assign, lhsSynonym, "x", true));
			patterns.push_back(Pattern(assign, lhsSynonym, "x", false));
			patterns.push_back(Pattern(assign, lhsSynonym, "y", true));

			return patterns;
		}

		std::vector<Pattern> getAllInvalidPattern(Synonym s1, Synonym s2) {
			Entity assign = { ASSIGN, s1 };
			Entity lhsSynonym = { VARIABLE, s2 };
			Entity lhsX = { VARIABLE, x };
			Entity lhsY = { VARIABLE, y };
			Entity lhsZ = { VARIABLE, z };
			Entity lhsN = { VARIABLE, "n" };

			std::vector<std::string> patternExpr;
			patternExpr.push_back("x");
			patternExpr.push_back("y");
			patternExpr.push_back("z");
			patternExpr.push_back("n");
			std::vector<Pattern> patterns;
			for (auto expr : patternExpr) {
				patterns.push_back(Pattern(assign, lhsZ, expr, false));
				patterns.push_back(Pattern(assign, lhsZ, expr, true));
				patterns.push_back(Pattern(assign, lhsN, expr, false));
				patterns.push_back(Pattern(assign, lhsN, expr, true));
			}

			patterns.push_back(Pattern(assign, lhsX, "y", true));
			patterns.push_back(Pattern(assign, lhsX, "y", false));
			patterns.push_back(Pattern(assign, lhsY, "y", false));
			patterns.push_back(Pattern(assign, lhsY, "x", false));
			patterns.push_back(Pattern(assign, lhsSynonym, "y", false));
			patterns.push_back(Pattern(assign, lhsSynonym, "z", false));
			patterns.push_back(Pattern(assign, lhsSynonym, "n", false));
			patterns.push_back(Pattern(assign, lhsSynonym, "z", true));
			patterns.push_back(Pattern(assign, lhsSynonym, "n", true));

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
		expr* EXPRESSIONNODE_1 = expr_parser.parse(EXPRESSION1);
		expr* EXPRESSIONNODE_2 = expr_parser.parse(EXPRESSION2);

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
		std::vector<RelRef> validRelation = getAllValidRelation({ "a" }, { "b" });
		std::vector<Pattern> validPattern = getAllValidPattern({ "c" }, { "d" });
		std::vector<RelRef> validRelation1 = getAllValidRelation({ "e" }, { "f" });
		std::vector<Pattern> validPattern1 = getAllValidPattern({ "g" }, { "h" });

		validate(validPattern, validRelation);
		validate(validPattern, validPattern1);

		// Don't do this unless needed, it 172 * 172 * 10 = 295,840 test case combination
		//validate(validRelation, validRelation1);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryTwoClausesNoCommonSynonymFalses) {
		std::vector<RelRef> validRelation = getAllValidRelation({ "a" }, { "b" });
		std::vector<Pattern> validPattern = getAllValidPattern({ "c" }, { "d" });
		std::vector<RelRef> invalidRelation = getAllInvalidRelation({ "a1" }, { "b1" });
		std::vector<Pattern> invalidPattern = getAllInvalidPattern({ "c1" }, { "d1" });

		validateEmpty(validPattern, invalidRelation);
		validateEmpty(invalidPattern, validPattern);
		validateEmpty(invalidPattern, invalidRelation);

		validateEmpty(validPattern, invalidPattern);
		validateEmpty(invalidPattern, validPattern);
		validateEmpty(invalidPattern, invalidPattern);

		// Don't do this unless needed, it 172 * 172 * 10 = 295,840 test case combination each
		//validateEmpty(validRelation, invalidRelation);
		//validateEmpty(invalidRelation, validRelation);
		//validateEmpty(invalidRelation, invalidRelation);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationFilterSingleCommonSynonymsEmpty) {
		Entity assignCommon = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhsCommon = { VARIABLE, COMMON_SYNONYM2 };
		Entity stmtCommon = { STMT,  COMMON_SYNONYM3 };
		Entity stmtCommon1 = { STMT,  COMMON_SYNONYM4 };

		//Handle result for Select a pattern a(_, "x") such that Uses(a,_)
		Pattern pattern(assignCommon, WILD_CARD, x, false);
		RelRef relation(USES_S, assignCommon, WILD_CARD);
		Entity selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, "x") such that Uses(a,_)
		pattern = Pattern(assignCommon, lhsCommon, x, false);
		relation = RelRef(USES_S, assignCommon, WILD_CARD);
		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);

		//Handle result for Select a pattern a(_, "z") such that Modifies(a,_)
		pattern = Pattern(assignCommon, WILD_CARD, z, false);
		relation = RelRef(MODIFIES_S, assignCommon, WILD_CARD);
		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, _"z"_) such that Modifies(a,_)
		pattern = Pattern(assignCommon, lhsCommon, z, true);
		relation = RelRef(MODIFIES_S, assignCommon, WILD_CARD);
		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);

		//Handle result for Select a pattern a(_, _"x"_) such that Parent(s,a)
		pattern = Pattern(assignCommon, WILD_CARD, x, true);
		relation = RelRef(PARENT, stmtCommon, assignCommon);
		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);

		pattern = Pattern(assignCommon, WILD_CARD, x, true);
		relation = RelRef(PARENT, stmtCommon, assignCommon);
		selected = stmtCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);

		//Handle result for Select a pattern a(_, _"x"_) such that Parent(a,s)
		pattern = Pattern(assignCommon, WILD_CARD, x, true);
		relation = RelRef(PARENT, assignCommon, stmtCommon);
		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);

		pattern = Pattern(assignCommon, WILD_CARD, x, true);
		relation = RelRef(PARENT, assignCommon, stmtCommon);
		selected = stmtCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationJoinSingleCommonSynonymsEmpty) {
		Entity assignCommon = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhsCommon = { VARIABLE, COMMON_SYNONYM2 };
		Entity lhsCommon1 = { VARIABLE, COMMON_SYNONYM4 };
		Entity stmtCommon = { STMT,  COMMON_SYNONYM3 };
		Entity stmtCommon1 = { STMT,  COMMON_SYNONYM5 };

		//Handle result for Select a pattern a(v, _"z"_) such that Uses(s,v)
		Pattern pattern(assignCommon, lhsCommon, z, true);
		RelRef relation(USES_S, stmtCommon, lhsCommon);

		Entity selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
		selected = stmtCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
		selected = lhsCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, "x") such that Uses(a,v1)
		pattern = Pattern(assignCommon, lhsCommon, x, false);
		relation = RelRef(USES_S, assignCommon, lhsCommon1);

		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
		selected = lhsCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
		selected = lhsCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, _) such that Parent(s,a)
		pattern = Pattern(assignCommon, lhsCommon, "", true);
		relation = RelRef(PARENT, stmtCommon, assignCommon);

		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
		selected = lhsCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
		selected = stmtCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, _) such that Parent(a,s)
		pattern = Pattern(assignCommon, lhsCommon, "", true);
		relation = RelRef(PARENT, assignCommon, stmtCommon);

		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
		selected = lhsCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
		selected = stmtCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationFilterDoubleCommonSynonymsEmpty) {
		Entity assignCommon = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhsCommon = { VARIABLE, COMMON_SYNONYM2 };

		//Handle result for Select a pattern a(v, "y") such that MODIFIES_S(a,v)
		Pattern pattern(assignCommon, lhsCommon, y, false);
		RelRef relation(MODIFIES_S, assignCommon, lhsCommon);
		Entity selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
		selected = lhsCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, _"z"_) such that MODIFIES_S(a,v)
		pattern = Pattern(assignCommon, lhsCommon, z, true);
		relation = RelRef(MODIFIES_S, assignCommon, lhsCommon);
		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
		selected = lhsCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);

		//Handle result for Select a pattern a(v, _) such that USES_S(a,v)
		pattern = Pattern(assignCommon, lhsCommon, "", true);
		relation = RelRef(USES_S, assignCommon, lhsCommon);
		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
		selected = lhsCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), EMPTY_RESULT);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationFilterSingleCommonSynonyms) {
		Entity assignCommon = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhsCommon = { VARIABLE, COMMON_SYNONYM2 };
		Entity stmtCommon = { STMT,  COMMON_SYNONYM3 };

		//Handle result for Select a pattern a(_, "x") such that Modifies(a,v)
		Pattern pattern(assignCommon, WILD_CARD, x, false);
		RelRef relation(MODIFIES_S, assignCommon, lhsCommon);

		std::list<std::string> result = { MODIFIES_LEFT3 };
		Entity selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(_, _"x"_) such that Modifies(a,v)
		pattern = Pattern(assignCommon, WILD_CARD, x, true);
		relation = RelRef(MODIFIES_S, assignCommon, lhsCommon);

		selected = assignCommon;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(_, _"y"_) such that Modifies(a,v)
		pattern = Pattern(assignCommon, WILD_CARD, y, true);
		relation = RelRef(MODIFIES_S, assignCommon, lhsCommon);

		selected = assignCommon;
		result = { MODIFIES_LEFT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(_, _) such that Modifies(a,v)
		pattern = Pattern(assignCommon, WILD_CARD, "", true);
		relation = RelRef(MODIFIES_S, assignCommon, lhsCommon);

		selected = assignCommon;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(v, "x") such that Modifies(a,_)
		pattern = Pattern(assignCommon, lhsCommon, x, false);
		relation = RelRef(MODIFIES_S, assignCommon, WILD_CARD);

		result = { MODIFIES_LEFT3 };
		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(v, _"x"_) such that Modifies(a,_)
		pattern = Pattern(assignCommon, lhsCommon, x, true);
		relation = RelRef(MODIFIES_S, assignCommon, WILD_CARD);

		selected = assignCommon;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(v, _"y"_) such that Modifies(a,_)
		pattern = Pattern(assignCommon, lhsCommon, y, true);
		relation = RelRef(MODIFIES_S, assignCommon, WILD_CARD);

		selected = assignCommon;
		result = { MODIFIES_LEFT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(v, _) such that Modifies(a,_)
		pattern = Pattern(assignCommon, lhsCommon, "", true);
		relation = RelRef(MODIFIES_S, assignCommon, WILD_CARD);

		selected = assignCommon;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(_, "x") such that Modifies(a,_)
		pattern = Pattern(assignCommon, lhsCommon, x, false);
		relation = RelRef(MODIFIES_S, assignCommon, WILD_CARD);
		result = { MODIFIES_LEFT3 };
		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(_, _"x"_) such that Modifies(a,_)
		pattern = Pattern(assignCommon, lhsCommon, x, true);
		relation = RelRef(MODIFIES_S, assignCommon, WILD_CARD);
		selected = assignCommon;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(_, _"y"_) such that Modifies(a,_)
		pattern = Pattern(assignCommon, lhsCommon, y, true);
		relation = RelRef(MODIFIES_S, assignCommon, WILD_CARD);
		selected = assignCommon;
		result = { MODIFIES_LEFT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationJoinSingleCommonSynonyms) {
		Entity assignCommon = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhsCommon = { VARIABLE, COMMON_SYNONYM2 };
		Entity stmtCommon = { STMT,  COMMON_SYNONYM3 };

		//Handle result for Select a pattern a(v, "x") such that uses(s,v)
		Pattern pattern(assignCommon, lhsCommon, x, false);
		RelRef relation(USES_S, stmtCommon, lhsCommon);
		Entity selected = assignCommon;
		std::list<std::string> result = { MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = stmtCommon;
		result = { USES_LEFT2 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(v, _"x"_) such that uses(s,v)
		pattern = Pattern(assignCommon, lhsCommon, x, true);
		relation = RelRef(USES_S, stmtCommon, lhsCommon);

		selected = assignCommon;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = stmtCommon;
		result = { USES_LEFT2, USES_LEFT1 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(v, _"y"_) such that uses(s,v)
		pattern = Pattern(assignCommon, lhsCommon, y, true);
		relation = RelRef(USES_S, stmtCommon, lhsCommon);

		selected = assignCommon;
		result = { MODIFIES_LEFT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = stmtCommon;
		result = { USES_LEFT1 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(v, _) such that uses(s,v)
		pattern = Pattern(assignCommon, lhsCommon, "", true);
		relation = RelRef(USES_S, stmtCommon, lhsCommon);
		selected = assignCommon;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = stmtCommon;
		result = { USES_LEFT2, USES_LEFT1 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(v, "x") such that modifies(s,v)
		pattern = Pattern(assignCommon, lhsCommon, x, false);
		relation = RelRef(MODIFIES_S, stmtCommon, lhsCommon);
		selected = assignCommon;
		result = { MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		pattern = Pattern(assignCommon, lhsCommon, x, false);
		relation = RelRef(MODIFIES_S, stmtCommon, lhsCommon);
		selected = stmtCommon;
		result = { MODIFIES_LEFT3, MODIFIES_LEFT1 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(v, _"x"_) such that modifies(s,v)
		pattern = Pattern(assignCommon, lhsCommon, x, true);
		relation = RelRef(MODIFIES_S, stmtCommon, lhsCommon);

		selected = assignCommon;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = stmtCommon;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3, MODIFIES_LEFT1, MODIFIES_LEFT2 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationFilterDoubleCommonSynonyms) {
		Entity assignCommon = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhsCommon = { VARIABLE, COMMON_SYNONYM2 };

		//Handle result for Select a pattern a(v, "_") such that MODIFIES_S(a,v)
		Pattern pattern(assignCommon, lhsCommon, "", true);
		RelRef relation(MODIFIES_S, assignCommon, lhsCommon);

		std::list<std::string> result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		Entity selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		//Handle result for Select a pattern a(v, "x") such that MODIFIES_S(a,v)
		pattern = Pattern(assignCommon, lhsCommon, "x", false);
		relation = RelRef(MODIFIES_S, assignCommon, lhsCommon);

		result = { MODIFIES_LEFT3, };
		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation }, { pattern }, selected)).front(), result);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationJoinDoubleCommonSynonyms) {
		Entity assignCommon = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhsCommon = { VARIABLE, COMMON_SYNONYM2 };
		Entity stmtCommon = { STMT,  COMMON_SYNONYM3 };
		Entity assignCommon1 = { ASSIGN,  COMMON_SYNONYM4 };
		Entity lhsCommon1 = { VARIABLE, COMMON_SYNONYM5 };
		Entity stmtCommon1 = { STMT,  COMMON_SYNONYM6 };

		Pattern pattern1(assignCommon, lhsCommon, "", true);
		RelRef relation1(MODIFIES_S, assignCommon, lhsCommon);
		Pattern pattern2(assignCommon1, lhsCommon1, "", true);
		RelRef relation2(MODIFIES_S, assignCommon1, lhsCommon1);
		Pattern pattern3(assignCommon1, lhsCommon, "", true);

		Entity selected = assignCommon;
		std::list<std::string> result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), result);
		selected = assignCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), result);
		selected = lhsCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), result);

		pattern1 = Pattern(assignCommon, lhsCommon, "", true);
		relation1 = RelRef(MODIFIES_S, stmtCommon, lhsCommon);
		pattern2 = Pattern(assignCommon1, lhsCommon1, "", true);
		relation2 = RelRef(MODIFIES_S, stmtCommon1, lhsCommon1);
		pattern3 = Pattern(assignCommon1, lhsCommon, "", true);

		selected = assignCommon;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), result);
		selected = assignCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), result);

		selected = lhsCommon;
		result = { MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), result);
		selected = lhsCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), result);

		selected = stmtCommon;
		result = { MODIFIES_LEFT4, MODIFIES_LEFT3, MODIFIES_LEFT1, MODIFIES_LEFT2 };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), result);
		selected = stmtCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), result);
	}

	TEST_F(QueryEvaluatorMultiClausesTest, evaluateQueryPatternRelationJoinDoubleCommonSynonymsEmpty) {
		Entity assignCommon = { ASSIGN,  COMMON_SYNONYM1 };
		Entity lhsCommon = { VARIABLE, COMMON_SYNONYM2 };
		Entity stmtCommon = { STMT,  COMMON_SYNONYM3 };
		Entity assignCommon1 = { ASSIGN,  COMMON_SYNONYM4 };
		Entity lhsCommon1 = { VARIABLE, COMMON_SYNONYM5 };
		Entity stmtCommon1 = { STMT,  COMMON_SYNONYM6 };

		Pattern pattern1(assignCommon, lhsCommon, "", true);
		RelRef relation1(MODIFIES_S, assignCommon, lhsCommon);
		Pattern pattern2(assignCommon1, lhsCommon1, "", true);
		RelRef relation2(MODIFIES_S, assignCommon1, lhsCommon1);
		Pattern pattern3(assignCommon1, lhsCommon, "z", true);

		Entity selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);
		selected = assignCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);

		selected = lhsCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);
		selected = lhsCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);

		pattern1 = Pattern(assignCommon, lhsCommon, "", true);
		relation1 = RelRef(MODIFIES_S, stmtCommon, lhsCommon);
		pattern2 = Pattern(assignCommon1, lhsCommon1, "", true);
		relation2 = RelRef(MODIFIES_S, stmtCommon1, lhsCommon1);
		pattern3 = Pattern(assignCommon1, lhsCommon, "z", true);

		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);
		selected = assignCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);

		selected = lhsCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);
		selected = lhsCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);

		selected = stmtCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);
		selected = stmtCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);

		pattern1 = Pattern(assignCommon, lhsCommon, "", true);
		relation1 = RelRef(MODIFIES_S, stmtCommon, lhsCommon);
		pattern2 = Pattern(assignCommon1, lhsCommon1, "", true);
		relation2 = RelRef(PARENT, stmtCommon, stmtCommon1);
		pattern3 = Pattern(assignCommon1, lhsCommon, "z", true);

		selected = assignCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);
		selected = assignCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);

		selected = lhsCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);
		selected = lhsCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);

		selected = stmtCommon;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);
		selected = stmtCommon1;
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ relation1, relation2 }, { pattern1, pattern2, pattern3 }, selected)).front(), EMPTY_RESULT);
	}
}