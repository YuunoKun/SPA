#include "pch.h"
#include <iostream>
#include "../../source/ExprNode.h"


namespace UnitTesting {

	TEST(ExprNode, getter_and_setter) {

		ExprNode* node = new ExprNode(ExprSymbol::EXPR_MINUS);

		ASSERT_EQ(node->getSymbol(), ExprSymbol::EXPR_MINUS);
		ASSERT_EQ(node->getValue(), "");
		ASSERT_EQ(node->getLHS(), nullptr);
		ASSERT_EQ(node->getRHS(), nullptr);


		node = new ExprNode("x");

		ASSERT_EQ(node->getSymbol(), ExprSymbol::EXPR_IDENTIFIER);
		ASSERT_EQ(node->getValue(), "x");
		ASSERT_EQ(node->getLHS(), nullptr);
		ASSERT_EQ(node->getRHS(), nullptr);

		ExprNode* child1 = new ExprNode(ExprSymbol::EXPR_MOD);
		ExprNode* child2 = new ExprNode(ExprSymbol::EXPR_MOD);
		node->setSymbol(ExprSymbol::EXPR_PLUS);
		node->setValue("1234");
		node->setLHS(child1);
		node->setRHS(child2);

		ASSERT_EQ(node->getSymbol(), ExprSymbol::EXPR_PLUS);
		ASSERT_EQ(node->getValue(), "1234");
		ASSERT_EQ(node->getLHS(), child1);
		ASSERT_EQ(node->getRHS(), child2);

		delete node;
	}

	TEST(ExprNode, equals) {

		ExprNode* target = new ExprNode(ExprSymbol::EXPR_PLUS);
		ExprNode* test = new ExprNode(ExprSymbol::EXPR_PLUS);
		ASSERT_TRUE(target->equals(test));

		target->setSymbol(ExprSymbol::EXPR_MINUS);
		ASSERT_FALSE(target->equals(test));

		target->setSymbol(ExprSymbol::EXPR_PLUS);
		target->setValue("a");
		ASSERT_FALSE(target->equals(test));

		target->setValue("abc");
		test->setValue("abc");
		ASSERT_TRUE(target->equals(test));

		ExprNode* l_target = new ExprNode(ExprSymbol::EXPR_MOD);
		ExprNode* l_test = new ExprNode(ExprSymbol::EXPR_MOD);
		target->setSymbol(ExprSymbol::EXPR_DIV);
		test->setSymbol(ExprSymbol::EXPR_DIV);
		target->setLHS(l_target);
		ASSERT_FALSE(target->equals(test));
		test->setLHS(l_test);
		ASSERT_TRUE(target->equals(test));
		target->setLHS(nullptr);
		ASSERT_FALSE(target->equals(test));
		test->setLHS(nullptr);


		ExprNode* r_target = new ExprNode(ExprSymbol::EXPR_MUL);
		ExprNode* r_test = new ExprNode(ExprSymbol::EXPR_MUL);
		target->setRHS(r_target);
		ASSERT_FALSE(target->equals(test));
		test->setRHS(r_test);
		ASSERT_TRUE(target->equals(test));
		target->setRHS(nullptr);
		ASSERT_FALSE(target->equals(test));
		test->setRHS(nullptr);

		delete l_target, l_test, r_target, r_test;
		delete target, test;
	}


	TEST(ExprNode, contains) {
		ExprNode* target = new ExprNode(ExprSymbol::EXPR_PLUS);
		ExprNode* l_target = new ExprNode("left");
		ExprNode* r_target = new ExprNode("right");
		target->setLHS(l_target);
		target->setRHS(r_target);

		ASSERT_TRUE(target->contains(target));
		ASSERT_TRUE(target->contains(l_target));
		ASSERT_TRUE(target->contains(l_target));
		ASSERT_FALSE(target->contains(nullptr));
	}


} // namespace UnitTesting