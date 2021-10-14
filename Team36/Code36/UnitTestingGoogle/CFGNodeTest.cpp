#include "pch.h"

#include <iostream>
#include "../../source/CFGNode.h"
#include "../../source/CFGNode.cpp"

namespace UnitTesting {

	TEST(CFGNode, setters_and_getters) {
		std::vector<prog_line> v1 = { 1, 2, 3 };
		CFGNode* node1 = new CFGNode(v1);
		ASSERT_EQ(node1->getProgramLines(), v1);
		ASSERT_EQ(node1->getNextMain(), nullptr);
		ASSERT_EQ(node1->getNextBranch(), nullptr);

		std::vector<prog_line> v2 = {4, 5};
		CFGNode* node2 = new CFGNode();
		node2->setProgramLines(v2);
		ASSERT_EQ(node2->getProgramLines(), v2);
		ASSERT_EQ(node2->getNextMain(), nullptr);
		ASSERT_EQ(node2->getNextBranch(), nullptr);

		node1->setNextMain(node2);
		ASSERT_EQ(node1->getProgramLines(), v1);
		ASSERT_EQ(node1->getNextMain(), node2);
		ASSERT_EQ(node1->getNextBranch(), nullptr);

		ASSERT_EQ(node2->getProgramLines(), v2);
		ASSERT_EQ(node2->getNextMain(), nullptr);
		ASSERT_EQ(node2->getNextBranch(), nullptr);
		
		std::vector<prog_line> v3 = {6, 7, 8};
		CFGNode* node3 = new CFGNode(v3);
		node1->setNextBranch(node3);

		ASSERT_EQ(node1->getProgramLines(), v1);
		ASSERT_EQ(node1->getNextMain(), node2);
		ASSERT_EQ(node1->getNextBranch(), node3);

		ASSERT_EQ(node2->getProgramLines(), v2);
		ASSERT_EQ(node2->getNextMain(), nullptr);
		ASSERT_EQ(node2->getNextBranch(), nullptr);

		ASSERT_EQ(node3->getProgramLines(), v3);
		ASSERT_EQ(node3->getNextMain(), nullptr);
		ASSERT_EQ(node3->getNextBranch(), nullptr);
		
		delete node1;
	}

	TEST(CFGNode, insertLine) {
		CFGNode* node1 = new CFGNode();
		node1->insertLine(1);
		node1->insertLine(2);

		std::vector<prog_line> expectedV = { 1, 2 };
		CFGNode* node2 = new CFGNode(expectedV);
		ASSERT_EQ(node1->getProgramLines(), node2->getProgramLines());

		delete node1;
	}

}