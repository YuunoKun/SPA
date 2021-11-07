#include "pch.h"
#include <iostream>
#include "Common/ExprNode.h"


namespace UnitTesting {

	TEST(ExprNode, getter_and_setter) {

		ExprNode node = ExprNode(ExprSymbol::EXPR_MINUS);
		ASSERT_EQ(node.getSymbol(), ExprSymbol::EXPR_MINUS);
		ASSERT_EQ(node.getValue(), "");
		ASSERT_EQ(node.getLHS(), nullptr);
		ASSERT_EQ(node.getRHS(), nullptr);

		node = ExprNode("x");

		ASSERT_EQ(node.getSymbol(), ExprSymbol::EXPR_IDENTIFIER);
		ASSERT_EQ(node.getValue(), "x");
		ASSERT_EQ(node.getLHS(), nullptr);
		ASSERT_EQ(node.getRHS(), nullptr);

		ExprNode child = ExprNode(ExprSymbol::EXPR_MOD);
		ExprNode child_l = ExprNode(ExprSymbol::EXPR_MOD);
		ExprNode child_r = ExprNode(ExprSymbol::EXPR_MOD);
		node.setSymbol(ExprSymbol::EXPR_PLUS);
		node.setValue("1234");
		node.setLHS(child_l);
		node.setRHS(child_r);

		ASSERT_EQ(node.getSymbol(), ExprSymbol::EXPR_PLUS);
		ASSERT_EQ(node.getValue(), "1234");
		ASSERT_EQ(*node.getLHS(), child_l);
		ASSERT_EQ(*node.getRHS(), child_r);

	}

	TEST(ExprNode, equals) {

		ExprNode target = ExprNode(ExprSymbol::EXPR_PLUS);
		ExprNode test = ExprNode(ExprSymbol::EXPR_PLUS);
		ASSERT_TRUE(target.equals(&test));
		ASSERT_TRUE(target.equals(&test));
		ASSERT_TRUE(test.equals(&target));

		target.setSymbol(ExprSymbol::EXPR_MINUS);
		ASSERT_FALSE(target.equals(&test));
		ASSERT_FALSE(test.equals(&target));

		target.setSymbol(ExprSymbol::EXPR_PLUS);
		target.setValue("a");
		ASSERT_FALSE(target.equals(&test));
		ASSERT_FALSE(test.equals(&target));

		target.setValue("abc");
		test.setValue("abc");
		ASSERT_TRUE(target.equals(&test));
		ASSERT_TRUE(test.equals(&target));

		ExprNode l_target = ExprNode(ExprSymbol::EXPR_MOD);
		ExprNode l_test = ExprNode(ExprSymbol::EXPR_MOD);
		target.setSymbol(ExprSymbol::EXPR_DIV);
		test.setSymbol(ExprSymbol::EXPR_DIV);
		target.setLHS(l_target);
		ASSERT_FALSE(target.equals(&test));
		ASSERT_FALSE(test.equals(&target));
		test.setLHS(l_test);
		ASSERT_TRUE(target.equals(&test));
		ASSERT_TRUE(test.equals(&target));
		target.setLHS(ExprNode());
		ASSERT_FALSE(target.equals(&test));
		ASSERT_FALSE(test.equals(&target));
		test.setLHS(ExprNode());


		ExprNode r_target = ExprNode(ExprSymbol::EXPR_MUL);
		ExprNode r_test = ExprNode(ExprSymbol::EXPR_MUL);
		target.setRHS(r_target);
		ASSERT_FALSE(target.equals(&test));
		ASSERT_FALSE(test.equals(&target));
		test.setRHS(r_test);
		ASSERT_TRUE(target.equals(&test));
		ASSERT_TRUE(test.equals(&target));
		target.setRHS(ExprNode());
		ASSERT_FALSE(target.equals(&test));
		ASSERT_FALSE(test.equals(&target));
		test.setRHS(ExprNode());

	}


	TEST(ExprNode, contains) {
		ExprNode target = ExprNode(ExprSymbol::EXPR_PLUS);
		ExprNode l_target = ExprNode("left");
		ExprNode r_target = ExprNode("right");
		target.setLHS(l_target);
		target.setRHS(r_target);

		ASSERT_TRUE(target.contains(&target));
		ASSERT_TRUE(target.contains(&l_target));
		ASSERT_TRUE(target.contains(&r_target));
		ASSERT_FALSE(target.contains(nullptr));
		ASSERT_FALSE(l_target.contains(&target));
		ASSERT_FALSE(r_target.contains(&target));
		ASSERT_FALSE(l_target.contains(&r_target));
		ASSERT_FALSE(r_target.contains(&l_target));

	}


	TEST(ExprNode, test_copy_constructor_and_operator) {
		ExprNode target = ExprNode(ExprSymbol::EXPR_PLUS);
		ExprNode l_target = ExprNode("left");
		ExprNode r_target = ExprNode("right");
		target.setLHS(l_target);
		target.setRHS(r_target);

		ExprNode a_copy(target);
		ASSERT_FALSE(target.getLHS() == a_copy.getLHS());
		ASSERT_FALSE(target.getRHS() == a_copy.getRHS());
		ASSERT_TRUE(target.equals(&a_copy));

		ExprNode b_copy = target;
		ASSERT_FALSE(target.getLHS() == b_copy.getLHS());
		ASSERT_FALSE(target.getRHS() == b_copy.getRHS());
		ASSERT_TRUE(target.equals(&b_copy));

	}


} // namespace UnitTesting