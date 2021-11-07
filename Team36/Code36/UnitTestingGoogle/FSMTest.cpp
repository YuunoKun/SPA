#include "pch.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include "SP/Extractor.h"
#include "SP/FSM.h"
#include "SP/Tokenizer.h"

namespace UnitTesting {
	using namespace SourceProcessor;

	class DummyDesignExtractor : public Extractor {
	public:
		DummyDesignExtractor(PKBSourceInterface& instance) : Extractor(instance) {}
		void startNesting() {}
		void chopNesting() {}
		void endNesting() {}
		void setCondExpr(bool flag) {}
		void addProcedure(proc_name name) {}
		void addStatement(TokenType type) {}
		void addVariable(var_name name) {}
		void addConstant(constant c) {}
		void addStatementUses(var_name name) {}
		void addStatementModifies(var_name name) {}
		void startExpr() {}
		void addExprSegment(std::string str) {}
		void endExpr() {}
		void addCallee(proc_name name) {}
		void validate() {}
		void populatePostValidation() {}
		void populateEntities() {}
		void populateRelations() {}
	};

	TEST(FSM, expectIdentifier) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("bac procedure read print call if then else while abc");
		tokenizer.initTokenStack();
		FSM fsm(tokenizer, &dde);

		std::vector<std::string> expected = { "bac", "procedure", "read", "print", "call", "if", "then", "else", "while", "abc" };
		for (auto s : expected) {
			ASSERT_EQ(fsm.expectIdentifier().getTokenValue(), s);
		}
	}

	TEST(FSM, expectFactor_valid) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("arandomvariable 983425");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expectFactor());
		ASSERT_NO_THROW(fsm_1.expectFactor());

		tokenizer.parseIntoTokens("a");
		tokenizer.initTokenStack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_2.expectFactor());

		tokenizer.parseIntoTokens("0");
		tokenizer.initTokenStack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expectFactor());
	}

	TEST(FSM, expectFactor_invalid) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("{");
		tokenizer.initTokenStack();
		FSM fsm(tokenizer, &dde);
		ASSERT_THROW(fsm.expectFactor(), std::runtime_error);

		tokenizer.parseIntoTokens("01234");
		tokenizer.initTokenStack();
		FSM fsm2(tokenizer, &dde);
		ASSERT_THROW(fsm.expectFactor(), std::runtime_error);
	}

	TEST(FSM, expectTerm) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());
		tokenizer.parseIntoTokens("something*somethingelse/ anotherthing %lastthing ;&&");
		tokenizer.initTokenStack();
		FSM fsm(tokenizer, &dde);
		ASSERT_NO_THROW(fsm.expectTerm());
	}

	TEST(FSM, expectExpression) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("x");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expectExpression());
		ASSERT_FALSE(fsm_1.getTokenizer().hasToken());

		tokenizer.parseIntoTokens("1");
		tokenizer.initTokenStack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_2.expectExpression());
		ASSERT_FALSE(fsm_2.getTokenizer().hasToken());

		tokenizer.parseIntoTokens("(x)+(y)-(1)");
		tokenizer.initTokenStack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expectExpression());
		ASSERT_FALSE(fsm_3.getTokenizer().hasToken());

		tokenizer.parseIntoTokens("2-x+y");
		tokenizer.initTokenStack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_4.expectExpression());
		ASSERT_FALSE(fsm_4.getTokenizer().hasToken());

		tokenizer.parseIntoTokens("x+y*k");
		tokenizer.initTokenStack();
		FSM fsm_5(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_5.expectExpression());
		ASSERT_FALSE(fsm_5.getTokenizer().hasToken());

		tokenizer.parseIntoTokens("x+y*(k-1)/3");
		tokenizer.initTokenStack();
		FSM fsm_6(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_6.expectExpression());
		ASSERT_FALSE(fsm_6.getTokenizer().hasToken());

		tokenizer.parseIntoTokens("x+y*((val-1)*t%(con/m))*a+z-a-b*c+1");
		tokenizer.initTokenStack();
		FSM fsm_7(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_7.expectExpression());
		ASSERT_FALSE(fsm_7.getTokenizer().hasToken());
	}

	TEST(FSM, expectRelationalFactor) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("x");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expectRelationalFactor());

		tokenizer.parseIntoTokens("1");
		tokenizer.initTokenStack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_2.expectRelationalFactor());

		tokenizer.parseIntoTokens("(3)");
		tokenizer.initTokenStack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expectRelationalFactor());

		tokenizer.parseIntoTokens("x+y-1");
		tokenizer.initTokenStack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_4.expectRelationalFactor());
	}

	TEST(FSM, expect_relational_expression) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("1==1");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expectRelationalFactor());

		tokenizer.parseIntoTokens("x!=1");
		tokenizer.initTokenStack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_2.expectRelationalFactor());

		tokenizer.parseIntoTokens("(123) > a");
		tokenizer.initTokenStack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expectRelationalFactor());

		tokenizer.parseIntoTokens("val + 1 >= (k)");
		tokenizer.initTokenStack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_4.expectRelationalFactor());

		tokenizer.parseIntoTokens("m < b * 0");
		tokenizer.initTokenStack();
		FSM fsm_5(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_5.expectRelationalFactor());

		tokenizer.parseIntoTokens("val + 1 <= k + (j-1)*h");
		tokenizer.initTokenStack();
		FSM fsm_6(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_6.expectRelationalFactor());
	}

	TEST(FSM, expectConditionalExpression) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("1==1");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expectConditionalExpression());

		tokenizer.parseIntoTokens("(x > 1) && (y <= k)");
		tokenizer.initTokenStack();
		FSM fsm_2(tokenizer, &dde);
		fsm_2.expectConditionalExpression();

		tokenizer.parseIntoTokens("(aa != 3) || (1 < 2)");
		tokenizer.initTokenStack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expectConditionalExpression());

		tokenizer.parseIntoTokens("!((n>=0)&&((a-1>b)||(k<m*1)))");
		tokenizer.initTokenStack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_4.expectConditionalExpression());

		tokenizer.parseIntoTokens("(4 / k + b) == (7)");
		tokenizer.initTokenStack();
		FSM fsm_5(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_5.expectConditionalExpression());

		tokenizer.parseIntoTokens("((1)+(1)<(1)+(1))&&(((1)-(1)<(1))||((((1)/(1)>=(1)%(1))||(!(1==1)))&&((1)>(1))))");
		tokenizer.initTokenStack();
		FSM fsm_7(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_7.expectConditionalExpression());
	}

	TEST(FSM, expectStatementTypeRead) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("read abc;");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expectStatementTypeRead());
	}

	TEST(FSM, expectStatementTypePrint) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("print xyz;");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expectStatementTypePrint());
	}

	TEST(FSM, expectStatementTypeCall) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("call myProcedure;");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expectStatementTypeCall());
	}

	TEST(FSM, expectStatementTypeWhile) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("while(1==1){x=1;}");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expectStatementTypeWhile());
	}

	TEST(FSM, expectStatementTypeIf) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("if(n>0)then{call someProcedure;}else{print anotherVariable;}");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expectStatementTypeIf());
	}

	TEST(FSM, expectStatementTypeAssign) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("x=1;");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expectStatementTypeAssign());

		tokenizer.parseIntoTokens("val = anotherVal;");
		tokenizer.initTokenStack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_2.expectStatementTypeAssign());

		tokenizer.parseIntoTokens("lost = lost - 1;");
		tokenizer.initTokenStack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expectStatementTypeAssign());
	}

	TEST(FSM, expectStatementList) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("x=1;}");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expectStatementList());

		tokenizer.parseIntoTokens("read x;print x;call myProcedure;}");
		tokenizer.initTokenStack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_2.expectStatementList());

		tokenizer.parseIntoTokens("x=1;if(x>1)then{x=2;}else{x=3;}x=4;while(x==5){x=6;}x=7;}");
		tokenizer.initTokenStack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expectStatementList());
	}

	TEST(FSM, expectProcedure) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("procedure myProcedure{x=y+1;}");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expectProcedure());
	}

	TEST(FSM, optionalFactor) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("arandomvariable 0983425");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_TRUE(fsm_1.optionalFactor());
		ASSERT_TRUE(fsm_1.optionalFactor());

		tokenizer.parseIntoTokens("a");
		tokenizer.initTokenStack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_TRUE(fsm_2.optionalFactor());

		tokenizer.parseIntoTokens("0");
		tokenizer.initTokenStack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_TRUE(fsm_3.optionalFactor());
	}

	TEST(FSM, optionalTerm) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());
		tokenizer.parseIntoTokens("something*somethingelse/ anotherthing %lastthing ;&&");
		tokenizer.initTokenStack();
		FSM fsm(tokenizer, &dde);
		ASSERT_TRUE(fsm.optionalTerm());
	}

	TEST(FSM, optionalExpression) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("x");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_TRUE(fsm_1.optionalExpression());
		tokenizer.parseIntoTokens("1");
		tokenizer.initTokenStack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_TRUE(fsm_2.optionalExpression());

		tokenizer.parseIntoTokens("(x)+(y)-(1)");
		tokenizer.initTokenStack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_TRUE(fsm_3.optionalExpression());

		tokenizer.parseIntoTokens("2-x+y");
		tokenizer.initTokenStack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_TRUE(fsm_4.optionalExpression());

		tokenizer.parseIntoTokens("x+y*k");
		tokenizer.initTokenStack();
		FSM fsm_5(tokenizer, &dde);
		ASSERT_TRUE(fsm_5.optionalExpression());

		tokenizer.parseIntoTokens("x+y*(k-1)/3");
		tokenizer.initTokenStack();
		FSM fsm_6(tokenizer, &dde);
		ASSERT_TRUE(fsm_6.optionalExpression());

		tokenizer.parseIntoTokens("x+y*((val-1)*t%(con/m))*a+z-a-b*c+1");
		tokenizer.initTokenStack();
		FSM fsm_7(tokenizer, &dde);
		ASSERT_TRUE(fsm_7.optionalExpression());
	}

	TEST(FSM, optionalRelationalFactor) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("x");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_TRUE(fsm_1.optionalRelationalFactor());

		tokenizer.parseIntoTokens("1");
		tokenizer.initTokenStack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_TRUE(fsm_2.optionalRelationalFactor());

		tokenizer.parseIntoTokens("(3)");
		tokenizer.initTokenStack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_TRUE(fsm_3.optionalRelationalFactor());

		tokenizer.parseIntoTokens("x+y-1");
		tokenizer.initTokenStack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_TRUE(fsm_4.optionalRelationalFactor());
	}

	TEST(FSM, optionalRelationalExpression) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("1==1");
		tokenizer.initTokenStack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_TRUE(fsm_1.optionalRelationalExpression());

		tokenizer.parseIntoTokens("x!=1");
		tokenizer.initTokenStack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_TRUE(fsm_2.optionalRelationalExpression());

		tokenizer.parseIntoTokens("(123) > a");
		tokenizer.initTokenStack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_TRUE(fsm_3.optionalRelationalExpression());

		tokenizer.parseIntoTokens("val + 1 >= (k)");
		tokenizer.initTokenStack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_TRUE(fsm_4.optionalRelationalExpression());

		tokenizer.parseIntoTokens("m < b * 0");
		tokenizer.initTokenStack();
		FSM fsm_5(tokenizer, &dde);
		ASSERT_TRUE(fsm_5.optionalRelationalExpression());

		tokenizer.parseIntoTokens("val + 1 <= k + (j-1)*h");
		tokenizer.initTokenStack();
		FSM fsm_6(tokenizer, &dde);
		ASSERT_TRUE(fsm_6.optionalRelationalExpression());
	}

	TEST(FSM, keywords_variable_name) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde(PKB::getInstance());

		tokenizer.parseIntoTokens("procedure procedure{call call; print print; read read;if(if<then)then{then=else;}else{else=while;}while(while>procedure){while=if;}}");
		tokenizer.initTokenStack();
		FSM fsm(tokenizer, &dde);
		ASSERT_NO_THROW(fsm.expectProcedure());
	}
}