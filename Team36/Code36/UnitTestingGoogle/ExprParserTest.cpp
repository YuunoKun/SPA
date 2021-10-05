#include "pch.h"
#include <iostream>
#include "../../source/ExprNode.h"
#include "../../source/ExprParser.h"


namespace UnitTesting {

	TEST(ExprParser, parse_identifier) {
		ExprParser parser;

		std::string expr1 = "procedure";
		ExprNode test1 = parser.parse(expr1);
		ExprNode target1 =  ExprNode("procedure");
		ASSERT_TRUE(target1.equals(&test1));

		std::string expr2 = " va1r2 ";
		ExprNode test2 = parser.parse(expr2);
		ExprNode target2 =  ExprNode("va1r2");
		ASSERT_TRUE(target2.equals(&test2));
	}

	TEST(ExprParser, parse_constant) {
		ExprParser parser;

		std::string expr1 = "123";
		ExprNode test1 = parser.parse(expr1);
		ExprNode target1 =  ExprNode("123");
		ASSERT_TRUE(target1.equals(&test1));
	}

	TEST(ExprParser, parse_term) {
		ExprParser parser;

		std::string expr1 = "x  z";
		ExprNode test1 = parser.parse(expr1);
		ExprNode l_target1 =  ExprNode("x");
		ExprNode r_target1 =  ExprNode("z");
		ExprNode target1 =  ExprNode(ExprSymbol::EXPR_MUL);
		target1.setLHS(l_target1);
		target1.setRHS(r_target1);

		ASSERT_TRUE(target1.equals(&test1));

		std::string expr2 = "x / y % z";
		ExprNode test2 = parser.parse(expr2);
		ExprNode target2 =  ExprNode(ExprSymbol::EXPR_MOD);
		ExprNode r_target2 =  ExprNode("z");
		target2.setRHS(r_target2);
		ExprNode l_target2 =  ExprNode(ExprSymbol::EXPR_DIV);
		ExprNode l_l_target2 =  ExprNode("x");
		l_target2.setLHS(l_l_target2);
		ExprNode l_r_target2 =  ExprNode("y");
		l_target2.setRHS(l_r_target2);
		target2.setLHS(l_target2);

		ASSERT_TRUE(target2.equals(&test2));
	}

	TEST(ExprParser, parse_expression) {
		ExprParser parser;

		std::string expr1 = "x + z";
		ExprNode test1 = parser.parse(expr1);
		ExprNode l_target1 =  ExprNode("x");
		ExprNode r_target1 =  ExprNode("z");
		ExprNode target1 =  ExprNode(ExprSymbol::EXPR_PLUS);
		target1.setLHS(l_target1);
		target1.setRHS(r_target1);

		ASSERT_TRUE(target1.equals(&test1));
		

		std::string expr2 = "x - y + z";
		ExprNode test2 = parser.parse(expr2);
		ExprNode target2 =  ExprNode(ExprSymbol::EXPR_PLUS);
		ExprNode l_target2 =  ExprNode(ExprSymbol::EXPR_MINUS);
		target2.setLHS(l_target2);
		ExprNode r_target2 =  ExprNode("z");
		target2.setRHS(r_target2);
		ExprNode l_l_target2 =  ExprNode("x");
		l_target2.setLHS(l_l_target2);
		ExprNode l_r_target2 =  ExprNode("y");
		l_target2.setRHS(l_r_target2);

		ASSERT_TRUE(target2.equals(&test2));


		std::string expr3 = "x - y  z";
		ExprNode test3 = parser.parse(expr3);
		ExprNode target3 =  ExprNode(ExprSymbol::EXPR_MINUS);
		ExprNode l_target3 =  ExprNode("x");
		target3.setLHS(l_target3);
		ExprNode r_target3 =  ExprNode(ExprSymbol::EXPR_MUL);
		target3.setRHS(r_target3);
		ExprNode r_l_target3 =  ExprNode("y");
		r_target3.setLHS(r_l_target3);
		ExprNode r_r_target3 =  ExprNode("z");
		r_target3.setRHS(r_r_target3);

		ASSERT_TRUE(target3.equals(&test3));


		std::string expr4 = "x  y + z";
		ExprNode test4 = parser.parse(expr4);
		ExprNode target4 =  ExprNode(ExprSymbol::EXPR_PLUS);
		ExprNode l_target4 =  ExprNode(ExprSymbol::EXPR_MUL);
		target4.setLHS(l_target4);
		ExprNode r_target4 =  ExprNode("z");
		target4.setRHS(r_target4);
		ExprNode l_l_target4 =  ExprNode("x");
		l_target4.setLHS(l_l_target4);
		ExprNode l_r_target4 =  ExprNode("y");
		l_target4.setRHS(l_r_target4);

		ASSERT_TRUE(target4.equals(&test4));
	}

	TEST(ExprParser, parse_expression_with_parentheses) {
		ExprParser parser;

		std::string expr1 = "(x)";
		ExprNode test1 = parser.parse(expr1);
		ExprNode target1 =  ExprNode("x");
		ASSERT_TRUE(target1.equals(&test1));


		std::string expr2 = "(223)";
		ExprNode test2 = parser.parse(expr2);
		ExprNode target2 =  ExprNode("223");
		ASSERT_TRUE(target2.equals(&test2));


		std::string expr3 = "(x + z)";
		ExprNode test3 = parser.parse(expr3);
		ExprNode l_target3 =  ExprNode("x");
		ExprNode r_target3 =  ExprNode("z");
		ExprNode target3 =  ExprNode(ExprSymbol::EXPR_PLUS);
		target3.setLHS(l_target3);
		target3.setRHS(r_target3);

		ASSERT_TRUE(target3.equals(&test3));


		std::string expr4 = "(x  z)";
		ExprNode test4 = parser.parse(expr4);
		ExprNode l_target4 =  ExprNode("x");
		ExprNode r_target4 =  ExprNode("z");
		ExprNode target4 =  ExprNode(ExprSymbol::EXPR_MUL);
		target4.setLHS(l_target4);
		target4.setRHS(r_target4);

		ASSERT_TRUE(target4.equals(&test4));


		std::string expr5 = "(x - y)  3";
		ExprNode test5 = parser.parse(expr5);
		ExprNode target5 =  ExprNode(ExprSymbol::EXPR_MUL);
		ExprNode l_target5 =  ExprNode(ExprSymbol::EXPR_MINUS);
		target5.setLHS(l_target5);
		ExprNode r_target5 =  ExprNode("3");
		target5.setRHS(r_target5);
		ExprNode l_l_target5 =  ExprNode("x");
		l_target5.setLHS(l_l_target5);
		ExprNode l_r_target5 =  ExprNode("y");
		l_target5.setRHS(l_r_target5);

		ASSERT_TRUE(target5.equals(&test5));


		std::string expr6 = "x  (5 + z)";
		ExprNode test6 = parser.parse(expr6);
		ExprNode target6 =  ExprNode(ExprSymbol::EXPR_MUL);
		ExprNode l_target6 =  ExprNode("x");
		target6.setLHS(l_target6);
		ExprNode r_target6 =  ExprNode(ExprSymbol::EXPR_PLUS);
		target6.setRHS(r_target6);
		ExprNode r_l_target6 =  ExprNode("5");
		r_target6.setLHS(r_l_target6);
		ExprNode r_r_target6 =  ExprNode("z");
		r_target6.setRHS(r_r_target6);

		ASSERT_TRUE(target6.equals(&test6));
	}

	TEST(ExprParser, parse_expression_with_parentheses_special) {
		ExprParser parser;

		std::string expr1 = "((((x))))";
		ExprNode test1 = parser.parse(expr1);
		ExprNode target1 =  ExprNode("x");
		ASSERT_TRUE(target1.equals(&test1));


		std::string expr2 = "((((223))))";
		ExprNode test2 = parser.parse(expr2);
		ExprNode target2 =  ExprNode("223");
		ASSERT_TRUE(target2.equals(&test2));


		std::string expr3 = "((((x)) + ((z))))";
		ExprNode test3 = parser.parse(expr3);
		ExprNode l_target3 =  ExprNode("x");
		ExprNode r_target3 =  ExprNode("z");
		ExprNode target3 =  ExprNode(ExprSymbol::EXPR_PLUS);
		target3.setLHS(l_target3);
		target3.setRHS(r_target3);

		ASSERT_TRUE(target3.equals(&test3));


		std::string expr4 = "((((x))  ((z))))";
		ExprNode test4 = parser.parse(expr4);
		ExprNode l_target4 =  ExprNode("x");
		ExprNode r_target4 =  ExprNode("z");
		ExprNode target4 =  ExprNode(ExprSymbol::EXPR_MUL);
		target4.setLHS(l_target4);
		target4.setRHS(r_target4);

		ASSERT_TRUE(target4.equals(&test4));


		std::string expr5 = "((((x) - (y)))  (3))";
		ExprNode test5 = parser.parse(expr5);
		ExprNode target5 =  ExprNode(ExprSymbol::EXPR_MUL);
		ExprNode l_target5 =  ExprNode(ExprSymbol::EXPR_MINUS);
		target5.setLHS(l_target5);
		ExprNode r_target5 =  ExprNode("3");
		target5.setRHS(r_target5);
		ExprNode l_l_target5 =  ExprNode("x");
		l_target5.setLHS(l_l_target5);
		ExprNode l_r_target5 =  ExprNode("y");
		l_target5.setRHS(l_r_target5);

		ASSERT_TRUE(target5.equals(&test5));


		std::string expr6 = "(((x)  (((5) + (z)))))";
		ExprNode test6 = parser.parse(expr6);
		ExprNode target6 =  ExprNode(ExprSymbol::EXPR_MUL);
		ExprNode l_target6 =  ExprNode("x");
		target6.setLHS(l_target6);
		ExprNode r_target6 =  ExprNode(ExprSymbol::EXPR_PLUS);
		target6.setRHS(r_target6);
		ExprNode r_l_target6 =  ExprNode("5");
		r_target6.setLHS(r_l_target6);
		ExprNode r_r_target6 =  ExprNode("z");
		r_target6.setRHS(r_r_target6);

		ASSERT_TRUE(target6.equals(&test6));
	}


	TEST(ExprParser, invalid_expression) {
		ExprParser parser;

		ASSERT_THROW(parser.parse("+"), std::runtime_error);
		ASSERT_THROW(parser.parse("1-"), std::runtime_error);
		ASSERT_THROW(parser.parse("var"), std::runtime_error);
		ASSERT_THROW(parser.parse("()"), std::runtime_error);
		ASSERT_THROW(parser.parse("(x"), std::runtime_error);
		ASSERT_THROW(parser.parse("x)"), std::runtime_error);
	}




} // namespace UnitTesting