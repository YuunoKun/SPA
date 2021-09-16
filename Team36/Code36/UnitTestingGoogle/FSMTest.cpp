#include "pch.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include "../../source/FSM.h"
#include "../../source/Extractor.h"
#include "../../source/Tokenizer.h"

namespace UnitTesting {

	using namespace SourceProcessor;

	class DummyDesignExtractor : public Extractor {
	public:
		DummyDesignExtractor() {}

		void start_nesting() {}

		void chop_nesting() {}

		void end_nesting() {}

		void add_procedure(proc_name name) {
			std::cout << "Adds procedure : " << name << std::endl;
		}

		void add_statement(TokenType type) {
			std::cout << "Adds statement of type : " << tokenTypeStrings[type] << std::endl;
		}

		void add_variable(var_name name) {
			std::cout << "Adds variable : " << name << std::endl;
		}

		void add_constant(constant c) {
			std::cout << "Adds constant : " << c << std::endl;
		}

		void add_statement_uses(var_name name) {}

		void add_statement_modifies(var_name name) {}

		void start_expr() {}

		void add_expr_segment(std::string str) {}

		void end_expr() {}

		void add_callee(proc_name name) {}

		void validate() {}

		void populate_post_validation() {}

		void populateEntities(PKB&) {}

		void populateRelations(PKB&) {}
	};


	TEST(FSM, expect_factor_valid) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("arandomvariable 0983425");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expect_factor());
		ASSERT_NO_THROW(fsm_1.expect_factor());


		tokenizer.parse_into_tokens("a");
		tokenizer.init_token_stack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_2.expect_factor());

		tokenizer.parse_into_tokens("0");
		tokenizer.init_token_stack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expect_factor());
	}

	TEST(FSM, expect_factor_invalid) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("{");
		tokenizer.init_token_stack();
		FSM fsm(tokenizer, &dde);
		ASSERT_THROW(fsm.expect_factor(), std::runtime_error);
	}

	TEST(FSM, expect_term) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;
		tokenizer.parse_into_tokens("something*somethingelse/ anotherthing %lastthing ;&&");
		tokenizer.init_token_stack();
		FSM fsm(tokenizer, &dde);
		ASSERT_NO_THROW(fsm.expect_term());
	}

	TEST(FSM, expect_expression) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("x");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expect_expression());
		ASSERT_FALSE(fsm_1.get_tokenizer().has_token());

		tokenizer.parse_into_tokens("1");
		tokenizer.init_token_stack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_2.expect_expression());
		ASSERT_FALSE(fsm_2.get_tokenizer().has_token());

		tokenizer.parse_into_tokens("(x)+(y)-(1)");
		tokenizer.init_token_stack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expect_expression());
		ASSERT_FALSE(fsm_3.get_tokenizer().has_token());

		tokenizer.parse_into_tokens("2-x+y");
		tokenizer.init_token_stack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_4.expect_expression());
		ASSERT_FALSE(fsm_4.get_tokenizer().has_token());

		tokenizer.parse_into_tokens("x+y*k");
		tokenizer.init_token_stack();
		FSM fsm_5(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_5.expect_expression());
		ASSERT_FALSE(fsm_5.get_tokenizer().has_token());

		tokenizer.parse_into_tokens("x+y*(k-1)/3");
		tokenizer.init_token_stack();
		FSM fsm_6(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_6.expect_expression());
		ASSERT_FALSE(fsm_6.get_tokenizer().has_token());

		tokenizer.parse_into_tokens("x+y*((val-1)*t%(con/m))*a+z-a-b*c+1");
		tokenizer.init_token_stack();
		FSM fsm_7(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_7.expect_expression());
		ASSERT_FALSE(fsm_7.get_tokenizer().has_token());
	}

	TEST(FSM, expect_relational_factor) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("x");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expect_relational_factor());

		tokenizer.parse_into_tokens("1");
		tokenizer.init_token_stack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_2.expect_relational_factor());

		tokenizer.parse_into_tokens("(3)");
		tokenizer.init_token_stack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expect_relational_factor());

		tokenizer.parse_into_tokens("x+y-1");
		tokenizer.init_token_stack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_4.expect_relational_factor());
	}

	TEST(FSM, expect_relational_expression) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("1==1");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expect_relational_factor());

		tokenizer.parse_into_tokens("x!=1");
		tokenizer.init_token_stack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_2.expect_relational_factor());

		tokenizer.parse_into_tokens("(123) > a");
		tokenizer.init_token_stack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expect_relational_factor());

		tokenizer.parse_into_tokens("val + 1 >= (k)");
		tokenizer.init_token_stack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_4.expect_relational_factor());

		tokenizer.parse_into_tokens("m < b * 0");
		tokenizer.init_token_stack();
		FSM fsm_5(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_5.expect_relational_factor());

		tokenizer.parse_into_tokens("val + 1 <= k + (j-1)*h");
		tokenizer.init_token_stack();
		FSM fsm_6(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_6.expect_relational_factor());
	}

	TEST(FSM, expect_conditional_expression) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("1==1");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expect_conditional_expression());

		tokenizer.parse_into_tokens("(x > 1) && (y <= k)");
		tokenizer.init_token_stack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_2.expect_conditional_expression());

		tokenizer.parse_into_tokens("(aa != 3) || (1 < 2)");
		tokenizer.init_token_stack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expect_conditional_expression());

		tokenizer.parse_into_tokens("!((n>=0)&&((a-1>b)||(k<m*1)))");
		tokenizer.init_token_stack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_4.expect_conditional_expression());

		tokenizer.parse_into_tokens("(4 / k + b) == (7)");
		tokenizer.init_token_stack();
		FSM fsm_5(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_5.expect_conditional_expression());

		tokenizer.parse_into_tokens("((1)+(1)<(1)+(1))&&(((1)-(1)<(1))||((((1)/(1)>=(1)%(1))||(!(1==1)))&&((1)>(1))))");
		tokenizer.init_token_stack();
		FSM fsm_7(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_7.expect_conditional_expression());
	}

	TEST(FSM, expect_statement_type_read) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("read abc;");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expect_statement_type_read());
	}

	TEST(FSM, expect_statement_type_print) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("print xyz;");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expect_statement_type_print());
	}

	TEST(FSM, expect_statement_type_call) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("call myProcedure;");
		//for(Token tk: tokenizer.get_token_chain()) std::cout << tokenTypeStrings[tk.get_token_type()] << std::endl;
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expect_statement_type_call());
	}

	TEST(FSM, expect_statement_type_while) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("while(1==1){x=1;}");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expect_statement_type_while());
	}

	TEST(FSM, expect_statement_type_if) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("if(n>0)then{call someProcedure;}else{print anotherVariable;}");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expect_statement_type_if());
	}

	TEST(FSM, expect_statement_type_assign) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("x=1;");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expect_statement_type_assign());

		tokenizer.parse_into_tokens("val = anotherVal;");
		tokenizer.init_token_stack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_2.expect_statement_type_assign());

		tokenizer.parse_into_tokens("lost = lost - 1;");
		tokenizer.init_token_stack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expect_statement_type_assign());
	}

	TEST(FSM, expect_statement) {
		//
	}

	TEST(FSM, expect_statement_list) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("x=1;}");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expect_statement_list());

		tokenizer.parse_into_tokens("read x;print x;call myProcedure;}");
		tokenizer.init_token_stack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_2.expect_statement_list());

		tokenizer.parse_into_tokens("x=1;if(x>1)then{x=2;}else{x=3;}x=4;while(x==5){x=6;}x=7;}");
		tokenizer.init_token_stack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_3.expect_statement_list());
	}

	TEST(FSM, expect_procedure) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("procedure myProcedure{x=y+1;}");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_NO_THROW(fsm_1.expect_procedure());
	}


	TEST(FSM, optional_factor) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("arandomvariable 0983425");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_TRUE(fsm_1.optional_factor());
		ASSERT_TRUE(fsm_1.optional_factor());


		tokenizer.parse_into_tokens("a");
		tokenizer.init_token_stack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_TRUE(fsm_2.optional_factor());

		tokenizer.parse_into_tokens("0");
		tokenizer.init_token_stack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_TRUE(fsm_3.optional_factor());
	}

	TEST(FSM, optional_term) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;
		tokenizer.parse_into_tokens("something*somethingelse/ anotherthing %lastthing ;&&");
		tokenizer.init_token_stack();
		FSM fsm(tokenizer, &dde);
		ASSERT_TRUE(fsm.optional_term());
	}

	TEST(FSM, optional_expression) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("x");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_TRUE(fsm_1.optional_expression());
		tokenizer.parse_into_tokens("1");
		tokenizer.init_token_stack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_TRUE(fsm_2.optional_expression());

		tokenizer.parse_into_tokens("(x)+(y)-(1)");
		tokenizer.init_token_stack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_TRUE(fsm_3.optional_expression());

		tokenizer.parse_into_tokens("2-x+y");
		tokenizer.init_token_stack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_TRUE(fsm_4.optional_expression());

		tokenizer.parse_into_tokens("x+y*k");
		tokenizer.init_token_stack();
		FSM fsm_5(tokenizer, &dde);
		ASSERT_TRUE(fsm_5.optional_expression());

		tokenizer.parse_into_tokens("x+y*(k-1)/3");
		tokenizer.init_token_stack();
		FSM fsm_6(tokenizer, &dde);
		ASSERT_TRUE(fsm_6.optional_expression());

		tokenizer.parse_into_tokens("x+y*((val-1)*t%(con/m))*a+z-a-b*c+1");
		tokenizer.init_token_stack();
		FSM fsm_7(tokenizer, &dde);
		ASSERT_TRUE(fsm_7.optional_expression());
	}

	TEST(FSM, optional_relational_factor) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("x");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_TRUE(fsm_1.optional_relational_factor());

		tokenizer.parse_into_tokens("1");
		tokenizer.init_token_stack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_TRUE(fsm_2.optional_relational_factor());

		tokenizer.parse_into_tokens("(3)");
		tokenizer.init_token_stack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_TRUE(fsm_3.optional_relational_factor());

		tokenizer.parse_into_tokens("x+y-1");
		tokenizer.init_token_stack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_TRUE(fsm_4.optional_relational_factor());
	}

	TEST(FSM, optional_relational_expression) {
		Tokenizer tokenizer;
		DummyDesignExtractor dde;

		tokenizer.parse_into_tokens("1==1");
		tokenizer.init_token_stack();
		FSM fsm_1(tokenizer, &dde);
		ASSERT_TRUE(fsm_1.optional_relational_expression());

		tokenizer.parse_into_tokens("x!=1");
		tokenizer.init_token_stack();
		FSM fsm_2(tokenizer, &dde);
		ASSERT_TRUE(fsm_2.optional_relational_expression());

		tokenizer.parse_into_tokens("(123) > a");
		tokenizer.init_token_stack();
		FSM fsm_3(tokenizer, &dde);
		ASSERT_TRUE(fsm_3.optional_relational_expression());

		tokenizer.parse_into_tokens("val + 1 >= (k)");
		tokenizer.init_token_stack();
		FSM fsm_4(tokenizer, &dde);
		ASSERT_TRUE(fsm_4.optional_relational_expression());

		tokenizer.parse_into_tokens("m < b * 0");
		tokenizer.init_token_stack();
		FSM fsm_5(tokenizer, &dde);
		ASSERT_TRUE(fsm_5.optional_relational_expression());

		tokenizer.parse_into_tokens("val + 1 <= k + (j-1)*h");
		tokenizer.init_token_stack();
		FSM fsm_6(tokenizer, &dde);
		ASSERT_TRUE(fsm_6.optional_relational_expression());
	}
}