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
		ASSERT_EQ(cfg->getTail()->getPrevMain()->getProgramLines(), v4);

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
		
		CFGNode* target1 = new CFGNode();
		target1->setProgramLines(v1);
		CFGNode* target2 = new CFGNode();
		target2->setProgramLines(v3);
		CFGNode* target3 = new CFGNode();
		target3->setProgramLines(v4);
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

		CFGNode* target_node1 = new CFGNode();
		target_node1->setProgramLines(target_v1);
		CFGNode* target_node2 = new CFGNode();
		target_node2->setProgramLines(target_v2);
		CFGNode* target_node3 = new CFGNode();
		target_node3->setProgramLines(target_v3);
		CFGNode* target_node4 = new CFGNode();
		target_node4->setProgramLines(target_v4);

		ASSERT_EQ(cfg1->getHead()->getProgramLines(), target_node1->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getProgramLines(), target_node2->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextMain()->getProgramLines(), target_node4->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextBranch()->getProgramLines(), target_node3->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextBranch()->getNextMain()->getProgramLines(), target_node2->getProgramLines());
		ASSERT_EQ(cfg2->getTail()->getInvalid(), true);

		delete target_node1, target_node2, target_node3, target_node4;
		delete cfg1, cfg2;
	}

	TEST(CFG, fork) {
		CFG* cfg1 = new CFG();
		CFG* cfg2 = new CFG();
		CFG* cfg3 = new CFG();
		std::vector<prog_line> v1 = { 1, 2, 3 };
		std::vector<prog_line> v2 = { 4, 5, 6 };
		std::vector<prog_line> v3 = { 7, 8 };
		std::vector<prog_line> v4 = { 9, 10 };

		std::vector<prog_line> target_v1 = { 1, 2 };
		std::vector<prog_line> target_v2 = { 3 };
		std::vector<prog_line> target_v3 = { 4, 5, 6 };
		std::vector<prog_line> target_v4 = { 7, 8 };
		std::vector<prog_line> target_v5 = { 9 };
		std::vector<prog_line> target_v6 = { 10 };

		cfg1->add(v1[0]);
		cfg1->add(v1[1]);
		cfg1->add(v1[2]);
		cfg1->add(v4[0]);
		cfg1->add(v4[1]);

		cfg2->add(v2[0]);
		cfg2->add(v2[1]);
		cfg2->add(v2[2]);

		cfg3->add(v3[0]);
		cfg3->add(v3[1]);

		cfg1->fork(cfg2, cfg3, 3);

		CFGNode* target_node1 = new CFGNode();
		target_node1->setProgramLines(target_v1);
		CFGNode* target_node2 = new CFGNode();
		target_node2->setProgramLines(target_v2);
		CFGNode* target_node3 = new CFGNode();
		target_node3->setProgramLines(target_v3);
		CFGNode* target_node4 = new CFGNode();
		target_node4->setProgramLines(target_v4);
		CFGNode* target_node5 = new CFGNode();
		target_node5->setProgramLines(target_v5);
		CFGNode* target_node6 = new CFGNode();
		target_node6->setProgramLines(target_v6);

		ASSERT_EQ(cfg1->getHead()->getProgramLines(), target_node1->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getProgramLines(), target_node2->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextMain()->getProgramLines(), target_node3->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextMain()->getNextMain()->getProgramLines(), target_node5->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextBranch()->getProgramLines(), target_node4->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextBranch()->getNextMain()->getProgramLines(), target_node5->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextMain()->getNextMain()->getNextMain()->getProgramLines(), target_node6->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextBranch()->getNextMain()->getNextMain()->getProgramLines(), target_node6->getProgramLines());
		ASSERT_EQ(cfg2->getTail()->getInvalid(), true);
		ASSERT_EQ(cfg3->getTail()->getInvalid(), true);

		std::vector<prog_line> v5 = { 7, 8 };
		std::vector<prog_line> v6 = { 9, 10, 11, 12 };
		std::vector<prog_line> v7 = { 13, 14, 15 };
		std::vector<prog_line> v8 = { 16, 17, 18 };

		std::vector<prog_line> target_v7 = { 7 };
		std::vector<prog_line> target_v8 = { 8 };
		std::vector<prog_line> target_v9 = { 9, 10, 11 };
		std::vector<prog_line> target_v10 = { 12 };
		std::vector<prog_line> target_v11 = { 13, 14, 15 };
		std::vector<prog_line> target_v12 = { 16, 17, 18 };

		CFG* cfg4 = new CFG();
		CFG* cfg5 = new CFG();
		CFG* cfg6 = new CFG();
		CFG* cfg7 = new CFG();

		CFGNode* target_node7 = new CFGNode();
		target_node7->setProgramLines(target_v7);
		CFGNode* target_node8 = new CFGNode();
		target_node8->setProgramLines(target_v8);
		CFGNode* target_node9 = new CFGNode();
		target_node9->setProgramLines(target_v9);
		CFGNode* target_node10 = new CFGNode();
		target_node10->setProgramLines(target_v10);
		CFGNode* target_node11 = new CFGNode();
		target_node11->setProgramLines(target_v11);
		CFGNode* target_node12 = new CFGNode();
		target_node12->setProgramLines(target_v12);

		cfg4->add(v5[0]);
		cfg4->add(v5[1]);

		cfg5->add(v6[0]);
		cfg5->add(v6[1]);
		cfg5->add(v6[2]);
		cfg5->add(v6[3]);

		cfg6->add(v7[0]);
		cfg6->add(v7[1]);
		cfg6->add(v7[2]);

		cfg7->add(v8[0]);
		cfg7->add(v8[1]);
		cfg7->add(v8[2]);

		cfg5->loop(cfg6, 12);
		cfg4->fork(cfg5, cfg7, 8);
		ASSERT_EQ(cfg4->getHead()->getProgramLines(), target_node7->getProgramLines());
		ASSERT_EQ(cfg4->getHead()->getNextMain()->getProgramLines(), target_node8->getProgramLines());
		ASSERT_EQ(cfg4->getHead()->getNextMain()->getNextMain()->getProgramLines(), target_node9->getProgramLines());
		ASSERT_EQ(cfg4->getHead()->getNextMain()->getNextMain()->getNextMain()->getProgramLines(), target_node10->getProgramLines());
		ASSERT_EQ(cfg4->getHead()->getNextMain()->getNextMain()->getNextMain()->getNextBranch()->getProgramLines(), target_node11->getProgramLines());
		ASSERT_EQ(cfg4->getHead()->getNextMain()->getNextMain()->getNextMain()->getNextBranch()->getNextMain()->getProgramLines(), target_node10->getProgramLines());
		ASSERT_EQ(cfg4->getHead()->getNextMain()->getNextBranch()->getProgramLines(), target_node12->getProgramLines());
		ASSERT_EQ(cfg4->getTail()->getPrevMain()->getProgramLines(), target_node10->getProgramLines());
		ASSERT_EQ(cfg4->getTail()->getPrevBranch()->getProgramLines(), target_node12->getProgramLines());

		delete cfg1, cfg2, cfg3, cfg4, cfg5, cfg6, cfg7;
		delete target_node1, target_node2, target_node3, target_node4, target_node5, target_node6, target_node7, target_node8, target_node9, target_node10, target_node11, target_node12;

	}

	TEST(CFG, getNext) {
		CFG* cfg1 = new CFG();
		CFG* cfg2 = new CFG();
		CFG* cfg3 = new CFG();
		CFG* cfg4 = new CFG();
		CFG* cfg5 = new CFG();
		std::vector<prog_line> v1 = { 1, 2, 3, 4, 5, 6};
		std::vector<prog_line> v2 = { 7, 8 };
		std::vector<prog_line> v3 = { 9, 10, 11, 12};
		std::vector<prog_line> v4 = { 13, 14, 15};
		std::vector<prog_line> v5 = { 16, 17, 18};
		std::vector<std::pair<prog_line, prog_line>> target1 = { {9, 10}, {10, 11}, {11, 12}, {12, 13}, {13, 14}, {14, 15}, {15, 12} };
		std::vector<std::pair<prog_line, prog_line>> target2 = { {7, 8}, {8, 9}, {8, 16}, {9, 10}, {10, 11}, {11, 12}, {16, 17}, {17, 18}, {12, 13}, {13, 14}, {14, 15}, {15, 12} };
		std::vector<std::pair<prog_line, prog_line>> target3 = { {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {8, 16}, {9, 10}, {10, 11}, {11, 12}, {16, 17}, {17, 18}, {18, 6}, {12, 6},  {12, 13}, {13, 14}, {14, 15}, {15, 12} };

		cfg1->add(v1[0]);
		cfg1->add(v1[1]);
		cfg1->add(v1[2]);
		cfg1->add(v1[3]);
		cfg1->add(v1[4]);
		cfg1->add(v1[5]);

		cfg2->add(v2[0]);
		cfg2->add(v2[1]);
		
		cfg3->add(v3[0]);
		cfg3->add(v3[1]);
		cfg3->add(v3[2]);
		cfg3->add(v3[3]);

		cfg4->add(v4[0]);
		cfg4->add(v4[1]);
		cfg4->add(v4[2]);

		cfg5->add(v5[0]);
		cfg5->add(v5[1]);
		cfg5->add(v5[2]);

		cfg3->loop(cfg4, 12);
		auto test1 = cfg3->getNexts();
		ASSERT_EQ(test1, target1);

		cfg2->fork(cfg3, cfg5, 8);
		auto test2 = cfg2->getNexts();
		ASSERT_EQ(test2, target2);

		cfg1->loop(cfg2, 6);
		auto test3 = cfg1->getNexts();
		ASSERT_EQ(test3, target3);

		CFG* cfg6 = new CFG();
		CFG* cfg7 = new CFG();
		CFG* cfg8 = new CFG();
		CFG* cfg9 = new CFG();
		CFG* cfg10 = new CFG();
		CFG* cfg11 = new CFG();
		CFG* cfg12 = new CFG();
		std::vector<prog_line> v6 = { 19 };
		std::vector<prog_line> v7 = { 20 };
		std::vector<prog_line> v8 = { 21, 22, 23, 26 };
		std::vector<prog_line> v9 = { 24 };
		std::vector<prog_line> v10 = { 25 };
		std::vector<prog_line> v11 = { 27 };
		std::vector<prog_line> v12 = { 28, 29 };

		std::vector<std::pair<prog_line, prog_line>> target4 = { {21, 22}, {22, 23}, {23, 24}, {23, 25}, {24, 26}, {25, 26} };
		std::vector<std::pair<prog_line, prog_line>> target5 = { {20, 21}, {20, 27}, {21, 22}, {22, 23}, {23, 24}, {23, 25}, {24, 26}, {25, 26} };
		std::vector<std::pair<prog_line, prog_line>> target6 = { {19, 20}, {19, 28}, {20, 21}, {20, 27}, {28, 29}, {21, 22}, {22, 23}, {23, 24}, {23, 25}, {24, 26}, {25, 26} };

		cfg6->add(v6[0]);

		cfg7->add(v7[0]);

		cfg8->add(v8[0]);
		cfg8->add(v8[1]);
		cfg8->add(v8[2]);
		cfg8->add(v8[3]);

		cfg9->add(v9[0]);

		cfg10->add(v10[0]);

		cfg11->add(v11[0]);

		cfg12->add(v12[0]);
		cfg12->add(v12[1]);

		cfg8->fork(cfg9, cfg10, 23);
		auto test4 = cfg8->getNexts();
		ASSERT_EQ(test4, target4);

		cfg7->fork(cfg8, cfg11, 20);
		auto test5 = cfg7->getNexts();
		ASSERT_EQ(test5, target5);

		cfg6->fork(cfg7, cfg12, 19);
		auto test6 = cfg6->getNexts();
		ASSERT_EQ(test6, target6);

		CFG* cfg13 = new CFG();
		CFG* cfg14 = new CFG();
		CFG* cfg15 = new CFG();
		CFG* cfg16 = new CFG();
		std::vector<prog_line> v13 = { 30 };
		std::vector<prog_line> v14 = { 31 };
		std::vector<prog_line> v15 = { 32, 33, 34, 35, 36, 37, 38 };
		std::vector<prog_line> v16 = { 39, 40 };

		std::vector<std::pair<prog_line, prog_line>> target7 = { {32, 33}, {33, 34}, {34, 35}, {35, 36}, {36, 37}, {37, 38}, {38, 39}, {39, 40}, {40, 38} };
		std::vector<std::pair<prog_line, prog_line>> target8 = { {31, 32}, {32, 33}, {33, 34}, {34, 35}, {35, 36}, {36, 37}, {37, 38}, {38, 31}, {38, 39}, {39, 40}, {40, 38} };
		std::vector<std::pair<prog_line, prog_line>> target9 = { {30, 31}, {31, 30}, {31, 32}, {32, 33}, {33, 34}, {34, 35}, {35, 36}, {36, 37}, {37, 38}, {38, 31}, {38, 39}, {39, 40}, {40, 38} };

		cfg13->add(v13[0]);

		cfg14->add(v14[0]);

		cfg15->add(v15[0]);
		cfg15->add(v15[1]);
		cfg15->add(v15[2]);
		cfg15->add(v15[3]);
		cfg15->add(v15[4]);
		cfg15->add(v15[5]);
		cfg15->add(v15[6]);

		cfg16->add(v16[0]);
		cfg16->add(v16[1]);

		cfg15->loop(cfg16, 38);
		auto test7 = cfg15->getNexts();
		ASSERT_EQ(test7, target7);

		cfg14->loop(cfg15, 31);
		auto test8 = cfg14->getNexts();
		ASSERT_EQ(test8, target8);

		cfg13->loop(cfg14, 30);
		auto test9 = cfg13->getNexts();
		ASSERT_EQ(test9, target9);

		delete cfg1, cfg2, cfg3, cfg4, cfg5, cfg6, cfg7, cfg8, cfg9, cfg10, cfg11, cfg12, cfg13, cfg14, cfg15, cfg16;
	}
}