#include "pch.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include "../../source/DesignExtractor.h"

namespace UnitTesting {

	using namespace SourceProcessor;


	TEST(DesignExtractor, full_behavior_test) {
		DesignExtractor* extractor = new DesignExtractor();


		/*
		* procedure main {
		1	mainX = 1;
		2	read readVar;
		3	print printVar;
		4	beforeIf = beforeIf * mainX;
		5	if(mainIfCond==13) then {
		6		beforeCall = beforeCall + 2;
		7		call p2;
		8		afterCall = afterCall + 4;
			} else {
		9		beforeWhile = beforeWhile;
		10		while(whileCond < 15) {
		11			inWhile = inWhile;
				}
		12		afterWhile = afterWhile;}
		13	afterIf = afterIf;}
		* 
		* procedure p2 {
		14	p2Var = p2Var - 11;
		* }
		*/

		// procedure main {
		extractor->add_procedure("main");
		
		// 1  mainX = 1;
		extractor->add_statement(TokenType::ASSIGN);
		extractor->add_variable("mainX");
		extractor->add_statement_modifies("mainX");
		extractor->start_expr();
		extractor->add_constant(1);
		extractor->add_expr_segment("1");
		extractor->end_expr();
		 
		// 2  read readVar;
		extractor->add_statement(TokenType::READ);
		extractor->add_variable("readVar");
		extractor->add_statement_modifies("readVar");

		// 3  print printVar;
		extractor->add_statement(TokenType::PRINT);
		extractor->add_variable("printVar");
		extractor->add_statement_uses("printVar");

		// 4  beforeIf = beforeIf * mainX;
		extractor->add_statement(TokenType::ASSIGN);
		extractor->add_variable("beforeIf");
		extractor->add_statement_modifies("beforeIf");
		extractor->start_expr();
		extractor->add_statement_uses("beforeIf");
		extractor->add_statement_uses("mainX");
		extractor->add_expr_segment("beforeIf*mainX");
		extractor->end_expr();

		// 5  if(mainIfCond==13) then {
		extractor->add_statement(TokenType::IF);
		extractor->add_variable("mainIfCond");
		extractor->add_statement_uses("mainIfCond");
		extractor->add_constant(13);
		extractor->start_nesting();

		// 5->6  beforeCall = beforeCall + 2;
		extractor->add_statement(TokenType::ASSIGN);
		extractor->add_variable("beforeCall");
		extractor->add_statement_modifies("beforeCall");
		extractor->start_expr();
		extractor->add_statement_uses("beforeCall");
		extractor->add_constant(2);
		extractor->add_expr_segment("beforeCall+2");
		extractor->end_expr();

		// 5->7  call p2;
		extractor->add_statement(TokenType::CALL);
		extractor->add_callee("p2");

		// 5->8  afterCall = afterCall + 4;
		extractor->add_statement(TokenType::ASSIGN);
		extractor->add_variable("afterCall");
		extractor->add_statement_modifies("afterCall");
		extractor->start_expr();
		extractor->add_statement_uses("afterCall");
		extractor->add_constant(4);
		extractor->add_expr_segment("afterCall+4");
		extractor->end_expr();

		// } else {
		extractor->chop_nesting();

		// 5->9  beforeWhile = beforeWhile;
		extractor->add_statement(TokenType::ASSIGN);
		extractor->add_variable("beforeWhile");
		extractor->add_statement_modifies("beforeWhile");
		extractor->start_expr();
		extractor->add_statement_uses("beforeWhile");
		extractor->add_expr_segment("beforeWhile");
		extractor->end_expr();

		// 5->10  while(whileCond < 15) {
		extractor->add_statement(TokenType::WHILE);
		extractor->add_variable("whileCond");
		extractor->add_statement_uses("whileCond");
		extractor->add_constant(15);
		extractor->start_nesting();

		// 5->10->11 inWhile = inWhile;
		extractor->add_statement(TokenType::ASSIGN);
		extractor->add_variable("inWhile");
		extractor->add_statement_modifies("inWhile");
		extractor->start_expr();
		extractor->add_statement_uses("inWhile");
		extractor->add_expr_segment("inWhile");
		extractor->end_expr();

		// }
		extractor->end_nesting();


		// 5->12 afterWhile = afterWhile;
		extractor->add_statement(TokenType::ASSIGN);
		extractor->add_variable("afterWhile");
		extractor->add_statement_modifies("afterWhile");
		extractor->start_expr();
		extractor->add_statement_uses("afterWhile");
		extractor->add_expr_segment("afterWhile");
		extractor->end_expr();

		// }
		extractor->end_nesting();

		// 13  afterIf = afterIf;
		extractor->add_statement(TokenType::ASSIGN);
		extractor->add_variable("afterIf");
		extractor->add_statement_modifies("afterIf");
		extractor->start_expr();
		extractor->add_statement_uses("afterIf");
		extractor->add_expr_segment("afterIf");
		extractor->end_expr();


		// procedure p2 {
		extractor->add_procedure("p2");

		// 14  p2Var = p2Var - 11;
		extractor->add_statement(TokenType::ASSIGN);
		extractor->add_variable("p2Var");
		extractor->add_statement_modifies("p2Var");
		extractor->start_expr();
		extractor->add_statement_uses("p2Var");
		extractor->add_constant(11);
		extractor->add_expr_segment("p2Var-11");
		extractor->end_expr();


		std::vector<Procedure*> procedures = extractor->get_procedures();
		std::vector<Statement*> statements = extractor->get_statements();
		std::unordered_set<var_name> variables = extractor->get_variables();
		std::unordered_set<constant> constants = extractor->get_constants();


		ASSERT_EQ(procedures.size(), 2);
		ASSERT_EQ(statements.size(), 14);
		ASSERT_EQ(variables.size(), 13);
		ASSERT_EQ(constants.size(), 6);
		
		ASSERT_NO_THROW(extractor->validate());
		ASSERT_NO_THROW(extractor->populate_post_validation());


		// test constants
		std::vector<constant> expected_constants = {1,13,2,4,15,11};
		ASSERT_EQ(constants.size(), expected_constants.size());
		for (constant c : expected_constants) {
			ASSERT_TRUE(constants.find(c) != constants.end());
		}
		

		// test variables
		std::vector<var_name> expected_variables = {
			"mainX","readVar","printVar","beforeIf",
			"mainIfCond","beforeCall","afterCall","beforeWhile",
			"whileCond","inWhile","afterWhile","afterIf","p2Var"
		};
		ASSERT_EQ(variables.size(), expected_variables.size());
		for (var_name v : expected_variables) {
			ASSERT_TRUE(variables.find(v) != variables.end());
		}


		// test statements
		// stmt1 mainX = 1;
		Statement* s1 = statements[0];
		ASSERT_EQ(s1->get_index(), 1);
		ASSERT_EQ(s1->get_type(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s1->get_proc_name(), "main");
		ASSERT_EQ(s1->get_callee(), "");
		ASSERT_EQ(s1->get_expr_str(), "1");
		ASSERT_EQ(s1->get_direct_parent(), 0);
		ASSERT_EQ(s1->get_direct_child().size(), 0);
		ASSERT_EQ(s1->get_used_variable().size(), 0);
		ASSERT_EQ(s1->get_modified_variable().size(), 1);

		ASSERT_EQ(s1->get_modified_variable()[0], "mainX");

		ASSERT_EQ(s1->get_stmt_list(), 1);


		// stmt2 read readVar;
		Statement* s2 = statements[1];
		ASSERT_EQ(s2->get_index(), 2);
		ASSERT_EQ(s2->get_type(), StmtType::STMT_READ);
		ASSERT_EQ(s2->get_proc_name(), "main");
		ASSERT_EQ(s2->get_callee(), "");
		ASSERT_EQ(s2->get_expr_str(), "");
		ASSERT_EQ(s2->get_direct_parent(), 0);
		ASSERT_EQ(s2->get_direct_child().size(), 0);
		ASSERT_EQ(s2->get_used_variable().size(), 0);
		ASSERT_EQ(s2->get_modified_variable().size(), 1);

		ASSERT_EQ(s2->get_modified_variable()[0], "readVar");

		ASSERT_EQ(s2->get_stmt_list(), 1);


		// stmt3 print printVar;
		Statement* s3 = statements[2];
		ASSERT_EQ(s3->get_index(), 3);
		ASSERT_EQ(s3->get_type(), StmtType::STMT_PRINT);
		ASSERT_EQ(s3->get_proc_name(), "main");
		ASSERT_EQ(s3->get_callee(), "");
		ASSERT_EQ(s3->get_expr_str(), "");
		ASSERT_EQ(s3->get_direct_parent(), 0);
		ASSERT_EQ(s3->get_direct_child().size(), 0);
		ASSERT_EQ(s3->get_used_variable().size(), 1);
		ASSERT_EQ(s3->get_modified_variable().size(), 0);

		ASSERT_EQ(s3->get_used_variable()[0], "printVar");

		ASSERT_EQ(s3->get_stmt_list(), 1);


		// stmt4 beforeIf = beforeIf * mainX;
		Statement* s4 = statements[3];
		ASSERT_EQ(s4->get_index(), 4);
		ASSERT_EQ(s4->get_type(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s4->get_proc_name(), "main");
		ASSERT_EQ(s4->get_callee(), "");
		ASSERT_EQ(s4->get_expr_str(), "beforeIf*mainX");
		ASSERT_EQ(s4->get_direct_parent(), 0);
		ASSERT_EQ(s4->get_direct_child().size(), 0);
		ASSERT_EQ(s4->get_used_variable().size(), 2);
		ASSERT_EQ(s4->get_modified_variable().size(), 1);
		std::unordered_set<var_name> expected_stmt4_used = {"beforeIf","mainX"};
		for (var_name v : s4->get_used_variable()) {
			ASSERT_TRUE(expected_stmt4_used.find(v) != expected_stmt4_used.end());
		}
		ASSERT_EQ(s4->get_modified_variable()[0], "beforeIf");

		ASSERT_EQ(s4->get_stmt_list(), 1);


		// stmt5 if(mainIfCond==13) then {....
		Statement* s5 = statements[4];
		ASSERT_EQ(s5->get_index(), 5);
		ASSERT_EQ(s5->get_type(), StmtType::STMT_IF);
		ASSERT_EQ(s5->get_proc_name(), "main");
		ASSERT_EQ(s5->get_callee(), "");
		ASSERT_EQ(s5->get_expr_str(), "");
		ASSERT_EQ(s5->get_direct_parent(), 0);
		ASSERT_EQ(s5->get_direct_child().size(), 6);
		ASSERT_EQ(s5->get_used_variable().size(), 8);
		ASSERT_EQ(s5->get_modified_variable().size(), 6);

		std::unordered_set<stmt_index> expected_stmt5_child = { 6,7,8,9,10,12 };
		ASSERT_EQ(s5->get_direct_child().size(), expected_stmt5_child.size());
		for (stmt_index id : s5->get_direct_child()) {
			ASSERT_TRUE(expected_stmt5_child.find(id) != expected_stmt5_child.end());
		}

		std::unordered_set<var_name> expected_stmt5_used = {
			"mainIfCond","beforeCall","p2Var","afterCall",
			"beforeWhile","whileCond","inWhile","afterWhile"
		};
		for (var_name v : s5->get_used_variable()) {
			ASSERT_TRUE(expected_stmt5_used.find(v) != expected_stmt5_used.end());
		}

		std::unordered_set<var_name> expected_stmt5_modified = {
			"beforeCall","p2Var","afterCall","beforeWhile","inWhile","afterWhile"
		};
		for (var_name v : s5->get_modified_variable()) {
			ASSERT_TRUE(expected_stmt5_modified.find(v) != expected_stmt5_modified.end());
		}

		ASSERT_EQ(s5->get_stmt_list(), 1);


		// stmt6 beforeCall = beforeCall + 2;
		Statement* s6 = statements[5];
		ASSERT_EQ(s6->get_index(), 6);
		ASSERT_EQ(s6->get_type(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s6->get_proc_name(), "main");
		ASSERT_EQ(s6->get_callee(), "");
		ASSERT_EQ(s6->get_expr_str(), "beforeCall+2");
		ASSERT_EQ(s6->get_direct_parent(), 5);
		ASSERT_EQ(s6->get_direct_child().size(), 0);
		ASSERT_EQ(s6->get_used_variable().size(), 1);
		ASSERT_EQ(s6->get_modified_variable().size(), 1);
		ASSERT_EQ(s6->get_used_variable()[0], "beforeCall");
		ASSERT_EQ(s6->get_modified_variable()[0], "beforeCall");

		ASSERT_EQ(s6->get_stmt_list(), 6);


		// stmt7 call p2;
		Statement* s7 = statements[6];
		ASSERT_EQ(s7->get_index(), 7);
		ASSERT_EQ(s7->get_type(), StmtType::STMT_CALL);
		ASSERT_EQ(s7->get_proc_name(), "main");
		ASSERT_EQ(s7->get_callee(), "p2");
		ASSERT_EQ(s7->get_expr_str(), "");
		ASSERT_EQ(s7->get_direct_parent(), 5);
		ASSERT_EQ(s7->get_direct_child().size(), 0);
		ASSERT_EQ(s7->get_used_variable().size(), 1);
		ASSERT_EQ(s7->get_modified_variable().size(), 1);
		ASSERT_EQ(s7->get_used_variable()[0], "p2Var");
		ASSERT_EQ(s7->get_modified_variable()[0], "p2Var");

		ASSERT_EQ(s7->get_stmt_list(), 6);


		// stmt8 afterCall = afterCall + 4;
		Statement* s8 = statements[7];
		ASSERT_EQ(s8->get_index(), 8);
		ASSERT_EQ(s8->get_type(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s8->get_proc_name(), "main");
		ASSERT_EQ(s8->get_callee(), "");
		ASSERT_EQ(s8->get_expr_str(), "afterCall+4");
		ASSERT_EQ(s8->get_direct_parent(), 5);
		ASSERT_EQ(s8->get_direct_child().size(), 0);
		ASSERT_EQ(s8->get_used_variable().size(), 1);
		ASSERT_EQ(s8->get_modified_variable().size(), 1);
		ASSERT_EQ(s8->get_used_variable()[0], "afterCall");
		ASSERT_EQ(s8->get_modified_variable()[0], "afterCall");

		ASSERT_EQ(s8->get_stmt_list(), 6);


		// stmt9 beforeWhile = beforeWhile;
		Statement* s9 = statements[8];
		ASSERT_EQ(s9->get_index(), 9);
		ASSERT_EQ(s9->get_type(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s9->get_proc_name(), "main");
		ASSERT_EQ(s9->get_callee(), "");
		ASSERT_EQ(s9->get_expr_str(), "beforeWhile");
		ASSERT_EQ(s9->get_direct_parent(), 5);
		ASSERT_EQ(s9->get_direct_child().size(), 0);
		ASSERT_EQ(s9->get_used_variable().size(), 1);
		ASSERT_EQ(s9->get_modified_variable().size(), 1);
		ASSERT_EQ(s9->get_used_variable()[0], "beforeWhile");
		ASSERT_EQ(s9->get_modified_variable()[0], "beforeWhile");

		ASSERT_EQ(s9->get_stmt_list(), 9);


		// stmt10 while(whileCond < 15) {......
		Statement* s10 = statements[9];
		ASSERT_EQ(s10->get_index(), 10);
		ASSERT_EQ(s10->get_type(), StmtType::STMT_WHILE);
		ASSERT_EQ(s10->get_proc_name(), "main");
		ASSERT_EQ(s10->get_callee(), "");
		ASSERT_EQ(s10->get_expr_str(), "");
		ASSERT_EQ(s10->get_direct_parent(), 5);
		ASSERT_EQ(s10->get_direct_child().size(), 1);
		ASSERT_EQ(s10->get_used_variable().size(), 2);
		ASSERT_EQ(s10->get_modified_variable().size(), 1);
		ASSERT_EQ(s10->get_direct_child()[0], 11);
		std::unordered_set<var_name> expected_stmt10_used = {"whileCond","inWhile"};
		for (var_name v : s10->get_used_variable()) {
			ASSERT_TRUE(expected_stmt10_used.find(v) != expected_stmt10_used.end());
		}
		ASSERT_EQ(s10->get_modified_variable()[0], "inWhile");

		ASSERT_EQ(s10->get_stmt_list(), 9);


		// stmt11 inWhile = inWhile;
		Statement* s11 = statements[10];
		ASSERT_EQ(s11->get_index(), 11);
		ASSERT_EQ(s11->get_type(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s11->get_proc_name(), "main");
		ASSERT_EQ(s11->get_callee(), "");
		ASSERT_EQ(s11->get_expr_str(), "inWhile");
		ASSERT_EQ(s11->get_direct_parent(), 10);
		ASSERT_EQ(s11->get_direct_child().size(), 0);
		ASSERT_EQ(s11->get_used_variable().size(), 1);
		ASSERT_EQ(s11->get_modified_variable().size(), 1);
		ASSERT_EQ(s11->get_used_variable()[0], "inWhile");
		ASSERT_EQ(s11->get_modified_variable()[0], "inWhile");

		ASSERT_EQ(s11->get_stmt_list(), 11);


		// stmt12 afterWhile = afterWhile;}
		Statement* s12 = statements[11];
		ASSERT_EQ(s12->get_index(), 12);
		ASSERT_EQ(s12->get_type(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s12->get_proc_name(), "main");
		ASSERT_EQ(s12->get_callee(), "");
		ASSERT_EQ(s12->get_expr_str(), "afterWhile");
		ASSERT_EQ(s12->get_direct_parent(), 5);
		ASSERT_EQ(s12->get_direct_child().size(), 0);
		ASSERT_EQ(s12->get_used_variable().size(), 1);
		ASSERT_EQ(s12->get_modified_variable().size(), 1);
		ASSERT_EQ(s12->get_used_variable()[0], "afterWhile");
		ASSERT_EQ(s12->get_modified_variable()[0], "afterWhile");

		ASSERT_EQ(s12->get_stmt_list(), 9);


		// stmt13 beforeWhile = beforeWhile;
		Statement* s13 = statements[12];
		ASSERT_EQ(s13->get_index(), 13);
		ASSERT_EQ(s13->get_type(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s13->get_proc_name(), "main");
		ASSERT_EQ(s13->get_callee(), "");
		ASSERT_EQ(s13->get_expr_str(), "afterIf");
		ASSERT_EQ(s13->get_direct_parent(), 0);
		ASSERT_EQ(s13->get_direct_child().size(), 0);
		ASSERT_EQ(s13->get_used_variable().size(), 1);
		ASSERT_EQ(s13->get_modified_variable().size(), 1);
		ASSERT_EQ(s13->get_used_variable()[0], "afterIf");
		ASSERT_EQ(s13->get_modified_variable()[0], "afterIf");

		ASSERT_EQ(s13->get_stmt_list(), 1);


		// stmt14 p2Var = p2Var - 11;
		Statement* s14 = statements[13];
		ASSERT_EQ(s14->get_index(), 14);
		ASSERT_EQ(s14->get_type(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s14->get_proc_name(), "p2");
		ASSERT_EQ(s14->get_callee(), "");
		ASSERT_EQ(s14->get_expr_str(), "p2Var-11");
		ASSERT_EQ(s14->get_direct_parent(), 0);
		ASSERT_EQ(s14->get_direct_child().size(), 0);
		ASSERT_EQ(s14->get_used_variable().size(), 1);
		ASSERT_EQ(s14->get_modified_variable().size(), 1);
		ASSERT_EQ(s14->get_used_variable()[0], "p2Var");
		ASSERT_EQ(s14->get_modified_variable()[0], "p2Var");

		ASSERT_EQ(s14->get_stmt_list(), 14);




		// test procedures
		// procedure1 main
		Procedure* p1 = procedures[0];
		ASSERT_EQ(p1->get_name(), "main");
		ASSERT_EQ(p1->get_index(), 1);
		std::unordered_set<var_name> expected_proc1_used = {
			"mainX","printVar","beforeIf","mainIfCond","beforeCall","p2Var",
			"afterCall","beforeWhile","whileCond","inWhile","afterWhile","afterIf"
		};
		ASSERT_EQ(p1->get_used_variable().size(), expected_proc1_used.size());
		for (var_name v : p1->get_used_variable()) {
			ASSERT_TRUE(expected_proc1_used.find(v) != expected_proc1_used.end());
		}

		std::unordered_set<var_name> expected_proc1_modified = {
			"mainX","readVar","beforeIf","beforeCall","p2Var",
			"afterCall","beforeWhile","inWhile","afterWhile","afterIf"
		};
		ASSERT_EQ(p1->get_modified_variable().size(), expected_proc1_modified.size());
		for (var_name v : p1->get_modified_variable()) {
			ASSERT_TRUE(expected_proc1_modified.find(v) != expected_proc1_modified.end());
		}

		std::unordered_set<stmt_index> expected_proc1_child = { 1,2,3,4,5,6,7,8,9,10,11,12,13 };
		ASSERT_EQ(p1->get_child().size(), expected_proc1_child.size());
		for (stmt_index id : p1->get_child()) {
			ASSERT_TRUE(expected_proc1_child.find(id) != expected_proc1_child.end());
		}

		//ASSERT_EQ(p1->get_caller().size(), 0);

		// procedure2 p2
		Procedure* p2 = procedures[1];
		ASSERT_EQ(p2->get_name(), "p2");
		ASSERT_EQ(p2->get_index(), 2);
		ASSERT_EQ(p2->get_used_variable().size(), 1);
		ASSERT_EQ(p2->get_used_variable()[0], "p2Var");
		ASSERT_EQ(p2->get_modified_variable().size(), 1);
		ASSERT_EQ(p2->get_modified_variable()[0], "p2Var");
		ASSERT_EQ(p2->get_child().size(), 1);
		ASSERT_EQ(p2->get_child()[0], 14);
		//ASSERT_EQ(p1->get_caller().size(), 1);
		//ASSERT_EQ(p1->get_caller()[0], 1);

	}


	TEST(DesignExtractor, validation_duplicate_proc_name) {
		DesignExtractor* extractor = new DesignExtractor();

		/*
		* procedure p {
		1	x = x;
		  }
		*
		* procedure p {
		2	y = y;
		* }
		*/

		extractor->add_procedure("p");
		extractor->add_statement(TokenType::ASSIGN);
		extractor->add_variable("x");
		extractor->add_statement_modifies("x");
		extractor->start_expr();
		extractor->add_variable("x");
		extractor->add_statement_uses("x");
		extractor->add_expr_segment("x");
		extractor->end_expr();

		extractor->add_procedure("p");
		extractor->add_statement(TokenType::ASSIGN);
		extractor->add_variable("y");
		extractor->add_statement_modifies("y");
		extractor->start_expr();
		extractor->add_variable("y");
		extractor->add_statement_uses("y");
		extractor->add_expr_segment("y");
		extractor->end_expr();

		ASSERT_THROW(extractor->validate(), std::runtime_error);
	}


	TEST(DesignExtractor, validation_undefined_proc_name) {
		DesignExtractor* extractor = new DesignExtractor();

		/*
		* procedure p {
		1	call r;
		  }
		*
		* procedure q {
		2	y = y;
		* }
		*/

		extractor->add_procedure("p");
		extractor->add_statement(TokenType::CALL);
		extractor->add_callee("r");

		extractor->add_procedure("p");
		extractor->add_statement(TokenType::ASSIGN);
		extractor->add_variable("y");
		extractor->add_statement_modifies("y");
		extractor->start_expr();
		extractor->add_variable("y");
		extractor->add_statement_uses("y");
		extractor->add_expr_segment("y");
		extractor->end_expr();

		ASSERT_THROW(extractor->validate(), std::runtime_error);
	}


	TEST(DesignExtractor, validation_cyclic_call) {
		DesignExtractor* extractor = new DesignExtractor();

		/*
		* procedure p {
		1	call q;
		  }
		*
		* procedure q {
		2	call p;
		* }
		*/

		extractor->add_procedure("p");
		extractor->add_statement(TokenType::CALL);
		extractor->add_callee("q");

		extractor->add_procedure("q");
		extractor->add_statement(TokenType::CALL);
		extractor->add_callee("p");

		ASSERT_THROW(extractor->validate(), std::runtime_error);
	}


	TEST(DesignExtractor, validation_recursive) {
		DesignExtractor* extractor = new DesignExtractor();

		/*
		* procedure p {
		1	call p;
		  }
		*
		*/

		extractor->add_procedure("p");
		extractor->add_statement(TokenType::CALL);
		extractor->add_callee("p");

		ASSERT_THROW(extractor->validate(), std::runtime_error);
	}

}