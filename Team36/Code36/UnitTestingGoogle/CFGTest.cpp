#include "pch.h"

#include <iostream>
#include "../../source/CFG.h"
#include "../../source/CFG.cpp"

namespace UnitTesting {
	TEST(CFG, add) {
		CFG* cfg = new CFG();
		std::vector<prog_line> v1 = { 1, 2, 3};
		std::vector<prog_line> v2 = { 5, 6 };
		std::vector<prog_line> v3 = { 9, 10 };
		std::vector<prog_line> v4 = { 12 };

		cfg->add(v1[0]);
		cfg->add(v1[1]);
		cfg->add(v1[2]);

		cfg->add(v2[0]);
		cfg->add(v2[1]);

		cfg->add(v3[0]);
		cfg->add(v3[1]);

		cfg->add(v4[0]);

		ASSERT_EQ(cfg->getHead()->getProgramLines(), v1);
		ASSERT_EQ(cfg->getHead()->getNextMain()->getProgramLines(), v2);
		ASSERT_EQ(cfg->getHead()->getNextMain()->getNextMain()->getProgramLines(), v3);
		ASSERT_EQ(cfg->getTail()->getProgramLines(), v4);

		delete cfg;
	}

	TEST(CFG, contains) {
		CFG* cfg = new CFG();
		std::vector<prog_line> v1 = { 1, 2, 3 };
		std::vector<prog_line> v2 = { 5, 6 };
		std::vector<prog_line> v3 = { 9, 10 };
		std::vector<prog_line> v4 = { 12 };

		cfg->add(v1[0]);
		cfg->add(v1[1]);
		cfg->add(v1[2]);

		cfg->add(v2[0]);
		cfg->add(v2[1]);

		cfg->add(v3[0]);
		cfg->add(v3[1]);

		cfg->add(v4[0]);
		
		CFGNode* target1 = new CFGNode(v1);
		CFGNode* target2 = new CFGNode(v3);
		CFGNode* target3 = new CFGNode(v4);
		ASSERT_EQ(cfg->contains(cfg->getHead(), 3)->getProgramLines(), target1->getProgramLines());
		ASSERT_EQ(cfg->contains(cfg->getHead(), 9)->getProgramLines(), target2->getProgramLines());
		ASSERT_EQ(cfg->contains(cfg->getHead(), 12)->getProgramLines(), target3->getProgramLines());

		delete cfg;
		delete target1, target2, target3;
	}

	TEST(CFG, loop) {
		CFG* cfg1 = new CFG();
		CFG* cfg2 = new CFG();
		std::vector<prog_line> v1 = { 1, 2, 3 };
		std::vector<prog_line> v2 = { 4, 5, 6 };
		std::vector<prog_line> v3 = { 7, 8};

		std::vector<prog_line> target_v1 = { 1, 2 };
		std::vector<prog_line> target_v2 = { 3 };
		std::vector<prog_line> target_v3 = { 4, 5, 6 };
		std::vector<prog_line> target_v4 = { 7, 8 };

		cfg1->add(v1[0]);
		cfg1->add(v1[1]);
		cfg1->add(v1[2]);
		cfg1->add(v3[0]);
		cfg1->add(v3[1]);

		cfg2->add(v2[0]);
		cfg2->add(v2[1]);
		cfg2->add(v2[2]);

		cfg1->loop(cfg2, 3);

		CFGNode* targetNode1 = new CFGNode(target_v1);
		CFGNode* targetNode2 = new CFGNode(target_v2);
		CFGNode* targetNode3 = new CFGNode(target_v3);
		CFGNode* targetNode4 = new CFGNode(target_v4);

		ASSERT_EQ(cfg1->getHead()->getProgramLines(), targetNode1->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getProgramLines(), targetNode2->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextMain()->getProgramLines(), targetNode4->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextBranch()->getProgramLines(), targetNode3->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextBranch()->getNextBranch()->getProgramLines(), targetNode2->getProgramLines());

		delete targetNode1, targetNode2, targetNode3, targetNode4;
		delete cfg1, cfg2;
	}

	TEST(CFG, fork) {
		CFG* cfg1 = new CFG();
		CFG* cfg2 = new CFG();
		std::vector<prog_line> v1 = { 1, 2, 3 };
		std::vector<prog_line> v2 = { 4, 5, 6 };
		std::vector<prog_line> v3 = { 7, 8 };

		std::vector<prog_line> target_v1 = { 1 };
		std::vector<prog_line> target_v2 = { 2 };
		std::vector<prog_line> target_v3 = { 3 };
		std::vector<prog_line> target_v4 = { 4, 5, 6 };
		std::vector<prog_line> target_v5 = { 7, 8 };

		cfg1->add(v1[0]);
		cfg1->add(v1[1]);
		cfg1->add(v1[2]);
		cfg1->add(v3[0]);
		cfg1->add(v3[1]);

		cfg2->add(v2[0]);
		cfg2->add(v2[1]);
		cfg2->add(v2[2]);

		cfg1->fork(cfg2, 2, 7);

		CFGNode* targetNode1 = new CFGNode(target_v1);
		CFGNode* targetNode2 = new CFGNode(target_v2);
		CFGNode* targetNode3 = new CFGNode(target_v3);
		CFGNode* targetNode4 = new CFGNode(target_v4);
		CFGNode* targetNode5 = new CFGNode(target_v5);

		ASSERT_EQ(cfg1->getHead()->getProgramLines(), targetNode1->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getProgramLines(), targetNode2->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextMain()->getProgramLines(), targetNode3->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextMain()->getNextMain()->getProgramLines(), targetNode5->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextBranch()->getProgramLines(), targetNode4->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextBranch()->getNextBranch()->getProgramLines(), targetNode5->getProgramLines());

		delete cfg1, cfg2;
		delete targetNode1, targetNode2, targetNode3, targetNode4, targetNode5;
	}

	TEST(CFG, getNext) {
		CFG* cfg1 = new CFG();
		CFG* cfg2 = new CFG();
		std::vector<prog_line> v1 = { 1, 2, 3, 6, 7};
		std::vector<prog_line> v2 = { 8, 9 };
		std::vector<std::pair<prog_line, prog_line>> target = { {1, 2}, {2, 3}, {3, 6}, {3, 8} , {6, 7}, {8, 9}, {9, 3} };

		cfg1->add(v1[0]);
		cfg1->add(v1[1]);
		cfg1->add(v1[2]);
		cfg1->add(v1[3]);
		cfg1->add(v1[4]);

		cfg2->add(v2[0]);
		cfg2->add(v2[1]);

		cfg1->loop(cfg2, 3);
		auto test = cfg1->getNexts();

		ASSERT_EQ(test, target);

		delete cfg1, cfg2;
	}
}