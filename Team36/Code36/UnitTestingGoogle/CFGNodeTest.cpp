#include "pch.h"

#include <iostream>
#include "../../source/CFGNode.h"
#include "../../source/CFGNode.cpp"

namespace UnitTesting {
	TEST(CFGNode, setters_and_getters) {
		std::vector<prog_line> v1 = { 1, 2, 3 };
		CFGNode* node1 = new CFGNode();
		node1->setProgramLines(v1);
		ASSERT_EQ(node1->getProgramLines(), v1);
		ASSERT_EQ(node1->getNextMain(), nullptr);
		ASSERT_EQ(node1->getNextBranch(), nullptr);

		std::vector<prog_line> v2 = { 4, 5 };
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

		std::vector<prog_line> v3 = { 6, 7, 8 };
		CFGNode* node3 = new CFGNode();
		node3->setProgramLines(v3);
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
		CFGNode* node2 = new CFGNode();
		node2->setProgramLines(expectedV);
		ASSERT_EQ(node1->getProgramLines(), node2->getProgramLines());

		delete node1;
	}

	TEST(CFGNode, traverse) {
		CFGNode* node1 = new CFGNode();
		CFGNode* node2 = new CFGNode();
		CFGNode* node3 = new CFGNode();
		node1->insertLine(1);
		node2->insertLine(2);
		node3->insertLine(3);

		node1->setNextMain(node3);
		node1->setNextBranch(node2);
		node2->setNextMain(node3);

		auto action = [](CFGNode* node) {
			if (node->isVisited()) {
				return false;
			} else {
				node->addLabel(node->getProgramLines()[0]);
				node->toggleVisited();
				return true;
			}
		};

		node1->traverse(action);
		ASSERT_EQ(node1->getLabels().size(), 1);
		ASSERT_EQ(node2->getLabels().size(), 1);
		ASSERT_EQ(node3->getLabels().size(), 1);
		ASSERT_EQ(node1->getLabels()[0], 1);
		ASSERT_EQ(node2->getLabels()[0], 2);
		ASSERT_EQ(node3->getLabels()[0], 3);

		node1->setNextMain(nullptr);
		node1->setNextBranch(nullptr);
		node2->setNextMain(nullptr);

		delete node1, node2, node3;
	}

	TEST(CFGNode, traverse_t) {
		CFGNode* node1 = new CFGNode();
		CFGNode* node2 = new CFGNode();
		CFGNode* node3 = new CFGNode();
		node1->insertLine(1);
		node2->insertLine(2);
		node3->insertLine(3);

		node1->setNextMain(node3);
		node1->setNextBranch(node2);
		node2->setNextMain(node3);

		auto action = [](CFGNode* node, prog_line line, prog_line nil) {
			if (node->isVisited()) {
				return false;
			} else {
				node->setProgramLines({ line });
				node->toggleVisited();
				return true;
			}
		};
		prog_line label = 5;
		prog_line ph = 0;
		node1->traverse<prog_line, prog_line>(action, label, ph);
		ASSERT_EQ(node1->getProgramLines().size(), 1);
		ASSERT_EQ(node2->getProgramLines().size(), 1);
		ASSERT_EQ(node3->getProgramLines().size(), 1);
		ASSERT_EQ(node1->getProgramLines()[0], 5);
		ASSERT_EQ(node2->getProgramLines()[0], 5);
		ASSERT_EQ(node3->getProgramLines()[0], 5);

		node1->setNextMain(nullptr);
		node1->setNextBranch(nullptr);
		node2->setNextMain(nullptr);

		delete node1, node2, node3;
	}

	TEST(CFGNode, traverse_t_p) {
		CFGNode* node1 = new CFGNode();
		CFGNode* node2 = new CFGNode();
		CFGNode* node3 = new CFGNode();
		node1->insertLine(1);
		node2->insertLine(3);
		node3->insertLine(5);

		node1->setNextMain(node2);
		node2->setNextMain(node3);

		CFGNode* br = new CFGNode();

		auto action = [](CFGNode* node, prog_line line, CFGNode* branch) {
			node->setNextBranch(branch);
			return node != branch;
		};
		prog_line label = 5;
		node1->traverse<prog_line, CFGNode*>(action, label, br);
		ASSERT_EQ(node1->getNextMain(), node2);
		ASSERT_EQ(node2->getNextMain(), node3);
		ASSERT_EQ(node1->getNextBranch(), br);
		ASSERT_EQ(node2->getNextBranch(), br);
		ASSERT_EQ(node3->getNextBranch(), br);

		node1->setNextMain(nullptr);
		node1->setNextBranch(nullptr);
		node2->setNextMain(nullptr);
		node2->setNextBranch(nullptr);
		node3->setNextBranch(nullptr);

		delete node1, node2, node3, br;
	}
}