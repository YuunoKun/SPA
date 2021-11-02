#include "pch.h"

#include <iostream>
#include "../../source/CFG.h"

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

	TEST(CFG, findNode) {
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

		ASSERT_EQ(cfg->findNode(cfg->getHead(), 3)->getProgramLines(), v1);
		ASSERT_EQ(cfg->findNode(cfg->getHead(), 9)->getProgramLines(), v3);
		ASSERT_EQ(cfg->findNode(cfg->getHead(), 12)->getProgramLines(), v4);

		delete cfg;
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
		ASSERT_EQ(cfg2->getTail()->isInvalid(), true);

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
		std::vector<prog_line> target_v5 = { 9, 10 };

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

		ASSERT_EQ(cfg1->getHead()->getProgramLines(), target_node1->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getProgramLines(), target_node2->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextMain()->getProgramLines(), target_node3->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextMain()->getNextMain()->getProgramLines(), target_node5->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextBranch()->getProgramLines(), target_node4->getProgramLines());
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getNextBranch()->getNextMain()->getProgramLines(), target_node5->getProgramLines());
		ASSERT_EQ(cfg2->getTail()->isInvalid(), true);
		ASSERT_EQ(cfg3->getTail()->isInvalid(), true);
		ASSERT_EQ(cfg2->getHead(), nullptr);
		ASSERT_EQ(cfg3->getHead(), nullptr);

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
		delete target_node1, target_node2, target_node3, target_node4, target_node5, target_node7, target_node8, target_node9, target_node10, target_node11, target_node12;

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
		sort(target1.begin(), target1.end());
		sort(target2.begin(), target2.end());
		sort(target3.begin(), target3.end());

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
		sort(test1.begin(), test1.end());
		ASSERT_EQ(test1, target1);

		cfg2->fork(cfg3, cfg5, 8);
		auto test2 = cfg2->getNexts();
		sort(test2.begin(), test2.end());
		ASSERT_EQ(test2, target2);

		cfg1->loop(cfg2, 6);
		auto test3 = cfg1->getNexts();
		sort(test3.begin(), test3.end());
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
		sort(target4.begin(), target4.end());
		sort(target5.begin(), target5.end());
		sort(target6.begin(), target6.end());



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
		sort(test4.begin(), test4.end());
		ASSERT_EQ(test4, target4);

		cfg7->fork(cfg8, cfg11, 20);
		auto test5 = cfg7->getNexts();
		sort(test5.begin(), test5.end());
		ASSERT_EQ(test5, target5);

		cfg6->fork(cfg7, cfg12, 19);
		auto test6 = cfg6->getNexts();
		sort(test6.begin(), test6.end());
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
		sort(target7.begin(), target7.end());
		sort(target8.begin(), target8.end());
		sort(target9.begin(), target9.end());


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
		sort(test7.begin(), test7.end());
		ASSERT_EQ(test7, target7);

		cfg14->loop(cfg15, 31);
		auto test8 = cfg14->getNexts();
		sort(test8.begin(), test8.end());
		ASSERT_EQ(test8, target8);

		cfg13->loop(cfg14, 30);
		auto test9 = cfg13->getNexts();
		sort(test9.begin(), test9.end());
		ASSERT_EQ(test9, target9);

		delete cfg1, cfg2, cfg3, cfg4, cfg5, cfg6, cfg7, cfg8, cfg9, cfg10, cfg11, cfg12, cfg13, cfg14, cfg15, cfg16;
	}

	TEST(CFG, makeStandalone) {

		CFG* cfg_null = new CFG();
		cfg_null->add(1);
		ASSERT_THROW(cfg_null->makeStandalone(2), std::runtime_error);

		ASSERT_EQ(cfg_null->getHead()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg_null->getHead()->getProgramLines()[0], 1);
		ASSERT_TRUE(cfg_null->getHead()->getNextMain()->isTermination());
		ASSERT_EQ(cfg_null->getHead()->getNextMain()->getPrev().front(), cfg_null->getHead());
		delete cfg_null;

		
		CFG* cfg_def = new CFG();
		cfg_def->add(1);
		CFGNode* ptr_def = cfg_def->makeStandalone(1); // 1

		ASSERT_EQ(cfg_def->getHead()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg_def->getHead()->getProgramLines()[0], 1);
		ASSERT_TRUE(cfg_def->getHead()->getNextMain()->isTermination());
		ASSERT_EQ(ptr_def, cfg_def->getHead());
		ASSERT_EQ(ptr_def->getNextMain()->getPrev().front(), ptr_def);
		delete cfg_def;

		
		CFG* cfg1 = new CFG();
		cfg1->add(1);
		cfg1->add(2);
		CFGNode* ptr1 = cfg1->makeStandalone(1); // 1 -> 2
		
		ASSERT_EQ(cfg1->getHead()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg1->getHead()->getProgramLines()[0], 1);
		ASSERT_EQ(cfg1->getHead()->getNextMain()->getProgramLines()[0], 2);
		ASSERT_TRUE(cfg1->getHead()->getNextMain()->getNextMain()->isTermination());
		ASSERT_EQ(ptr1, cfg1->getHead());
		ASSERT_EQ(ptr1->getNextMain()->getPrev().front(), ptr1);
		delete cfg1;
		
		
		CFG* cfg2 = new CFG();
		cfg2->add(1);
		cfg2->add(2);
		CFGNode* ptr2 = cfg2->makeStandalone(2); // 1 -> 2
		
		ASSERT_EQ(cfg2->getHead()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg2->getHead()->getNextMain()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg2->getHead()->getProgramLines()[0], 1);
		ASSERT_EQ(cfg2->getHead()->getNextMain()->getProgramLines()[0], 2);
		ASSERT_TRUE(cfg2->getHead()->getNextMain()->getNextMain()->isTermination());
		ASSERT_EQ(ptr2, cfg2->getHead()->getNextMain());
		ASSERT_EQ(ptr2->getPrev().front(), cfg2->getHead());
		ASSERT_EQ(ptr2->getNextMain()->getPrev().front(), ptr2);
		delete cfg2;

		
		CFG* cfg3 = new CFG();
		cfg3->add(1);
		cfg3->add(2);
		cfg3->add(3);
		CFGNode* ptr3 = cfg3->makeStandalone(1); // 1 -> 2 3

		ASSERT_EQ(cfg3->getHead()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg3->getHead()->getNextMain()->getProgramLines().size(), 2);
		ASSERT_EQ(cfg3->getHead()->getProgramLines()[0], 1);
		ASSERT_EQ(cfg3->getHead()->getNextMain()->getProgramLines()[0], 2);
		ASSERT_EQ(cfg3->getHead()->getNextMain()->getProgramLines()[1], 3);
		ASSERT_TRUE(cfg3->getHead()->getNextMain()->getNextMain()->isTermination());
		ASSERT_EQ(ptr3, cfg3->getHead());
		ASSERT_EQ(ptr3->getNextMain()->getPrev().front(), ptr3);
		ASSERT_EQ(ptr3->getNextMain()->getNextMain()->getPrev().front(), ptr3->getNextMain());
		delete cfg3;

		
		CFG* cfg4 = new CFG();
		cfg4->add(1);
		cfg4->add(2);
		cfg4->add(3);
		CFGNode* ptr4 = cfg4->makeStandalone(3); // 1 2 -> 3

		ASSERT_EQ(cfg4->getHead()->getProgramLines().size(), 2);
		ASSERT_EQ(cfg4->getHead()->getNextMain()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg4->getHead()->getProgramLines()[0], 1);
		ASSERT_EQ(cfg4->getHead()->getProgramLines()[1], 2);
		ASSERT_EQ(cfg4->getHead()->getNextMain()->getProgramLines()[0], 3);
		ASSERT_TRUE(cfg4->getHead()->getNextMain()->getNextMain()->isTermination());
		ASSERT_EQ(ptr4, cfg4->getHead()->getNextMain());
		ASSERT_EQ(ptr4->getPrev().front(), cfg4->getHead());
		ASSERT_EQ(ptr4->getNextMain()->getPrev().front(), ptr4);
		delete cfg4;

		
		CFG* cfg5 = new CFG();
		cfg5->add(1);
		cfg5->add(2);
		cfg5->add(3);
		CFGNode* ptr5 = cfg5->makeStandalone(2); // 1 -> 2 -> 3

		ASSERT_EQ(cfg5->getHead()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg5->getHead()->getNextMain()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg5->getHead()->getNextMain()->getNextMain()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg5->getHead()->getProgramLines()[0], 1);
		ASSERT_EQ(cfg5->getHead()->getNextMain()->getProgramLines()[0], 2);
		ASSERT_EQ(cfg5->getHead()->getNextMain()->getNextMain()->getProgramLines()[0], 3);
		ASSERT_TRUE(cfg5->getHead()->getNextMain()->getNextMain()->getNextMain()->isTermination());
		ASSERT_EQ(ptr5, cfg5->getHead()->getNextMain());
		ASSERT_EQ(ptr5->getPrev().front(), cfg5->getHead());
		ASSERT_EQ(ptr5->getNextMain()->getPrev().front(), ptr5);
		ASSERT_EQ(ptr5->getNextMain()->getNextMain()->getPrev().front(), ptr5->getNextMain());
		delete cfg5;

		
		CFG* cfg6 = new CFG();
		cfg6->add(1);
		cfg6->add(3);
		cfg6->add(4);
		cfg6->add(5);
		cfg6->add(7);
		CFGNode* cfg6_node2 = new CFGNode();
		cfg6_node2->setProgramLines({2});
		cfg6_node2->setNextMain(cfg6->getHead()->getNextMain());
		cfg6->getHead()->getNextMain()->setPrevBranch(cfg6_node2);
		CFGNode* cfg6_node6 = new CFGNode();
		cfg6_node6->setProgramLines({6});
		cfg6->getHead()->getNextMain()->setNextBranch(cfg6_node6);
		cfg6_node6->setPrevBranch(cfg6->getHead()->getNextMain());
		CFGNode* ptr6 = cfg6->makeStandalone(3); // (1, 2) -> 3 -> 4 5 -> (6, 7)
		
		ASSERT_EQ(cfg6->getHead()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg6_node2->getProgramLines().size(), 1);
		ASSERT_EQ(cfg6->getHead()->getNextMain()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg6->getHead()->getNextMain()->getNextMain()->getProgramLines().size(), 2);
		ASSERT_EQ(cfg6_node6->getProgramLines().size(), 1);
		ASSERT_EQ(cfg6->getHead()->getNextMain()->getNextMain()->getNextMain()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg6->getHead()->getProgramLines()[0], 1);
		ASSERT_EQ(cfg6_node2->getProgramLines()[0], 2);
		ASSERT_EQ(cfg6->getHead()->getNextMain()->getProgramLines()[0], 3);
		ASSERT_EQ(cfg6->getHead()->getNextMain()->getNextMain()->getProgramLines()[0], 4);
		ASSERT_EQ(cfg6->getHead()->getNextMain()->getNextMain()->getProgramLines()[1], 5);
		ASSERT_EQ(cfg6_node6->getProgramLines()[0], 6);
		ASSERT_EQ(cfg6->getHead()->getNextMain()->getNextMain()->getNextMain()->getProgramLines()[0], 7);
		ASSERT_TRUE(cfg6->getHead()->getNextMain()->getNextMain()->getNextMain()->getNextMain()->isTermination());
		ASSERT_EQ(ptr6, cfg6->getHead()->getNextMain());

		ASSERT_EQ(cfg6_node2->getNextMain(), cfg6->getHead()->getNextMain());
		ASSERT_NE(find(ptr6->getPrev().begin(), ptr6->getPrev().end(), cfg6_node2), ptr6->getPrev().end());
		ASSERT_NE(find(ptr6->getPrev().begin(), ptr6->getPrev().end(), cfg6->getHead()), ptr6->getPrev().end());
		ASSERT_EQ(ptr6->getNextMain()->getNextBranch(), cfg6_node6);
		CFGNode* cfg6_node45 = ptr6->getNextMain();
		ASSERT_EQ(cfg6_node45->getPrev().front(), ptr6);
		ASSERT_EQ(cfg6_node45->getNextMain()->getPrev().front(), cfg6_node45);
		ASSERT_EQ(cfg6_node45->getNextBranch()->getPrev().front(), cfg6_node45);
		cfg6_node2->setNextMain(nullptr);
		ptr6->getPrev().erase(find(ptr6->getPrev().begin(), ptr6->getPrev().end(), cfg6_node2));
		delete cfg6_node2;
		cfg6_node6->getPrev().clear();
		cfg6_node45->setNextBranch(nullptr);
		delete cfg6_node6;
		delete cfg6;
		
		
		CFG* cfg7 = new CFG();
		cfg7->add(1);
		cfg7->add(3);
		cfg7->add(4);
		cfg7->add(5);
		cfg7->add(7);
		CFGNode* cfg7_node2 = new CFGNode();
		cfg7_node2->setProgramLines({ 2 });
		cfg7_node2->setNextMain(cfg7->getHead()->getNextMain());
		cfg7->getHead()->getNextMain()->setPrevBranch(cfg7_node2);
		CFGNode* cfg7_node6 = new CFGNode();
		cfg7_node6->setProgramLines({ 6 });
		cfg7->getHead()->getNextMain()->setNextBranch(cfg7_node6);
		cfg7_node6->setPrevBranch(cfg7->getHead()->getNextMain());
		CFGNode* ptr7 = cfg7->makeStandalone(4); // (1, 2) -> 3 -> 4 -> 5 -> (6, 7)

		ASSERT_EQ(cfg7->getHead()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg7_node2->getProgramLines().size(), 1);
		ASSERT_EQ(cfg7->getHead()->getNextMain()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg7->getHead()->getNextMain()->getNextMain()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg7->getHead()->getNextMain()->getNextMain()->getNextMain()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg7_node6->getProgramLines().size(), 1);
		ASSERT_EQ(cfg7->getHead()->getNextMain()->getNextMain()->getNextMain()->getNextMain()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg7->getHead()->getProgramLines()[0], 1);
		ASSERT_EQ(cfg7_node2->getProgramLines()[0], 2);
		ASSERT_EQ(cfg7->getHead()->getNextMain()->getProgramLines()[0], 3);
		ASSERT_EQ(cfg7->getHead()->getNextMain()->getNextMain()->getProgramLines()[0], 4);
		ASSERT_EQ(cfg7->getHead()->getNextMain()->getNextMain()->getNextMain()->getProgramLines()[0], 5);
		ASSERT_EQ(cfg7_node6->getProgramLines()[0], 6);
		ASSERT_EQ(cfg7->getHead()->getNextMain()->getNextMain()->getNextMain()->getNextMain()->getProgramLines()[0], 7);
		ASSERT_TRUE(cfg7->getHead()->getNextMain()->getNextMain()->getNextMain()->getNextMain()->getNextMain()->isTermination());
		ASSERT_EQ(ptr7, cfg7->getHead()->getNextMain()->getNextMain());

		ASSERT_EQ(cfg7_node2->getNextMain(), cfg7->getHead()->getNextMain()); // 2 -> 3
		ASSERT_EQ(cfg7->getHead()->getNextMain()->getPrev().size(), 2);
		ASSERT_EQ(count(cfg7->getHead()->getNextMain()->getPrev().begin(), cfg7->getHead()->getNextMain()->getPrev().end(), cfg7_node2), 1); // 2 -> 3 p
		ASSERT_EQ(count(cfg7->getHead()->getNextMain()->getPrev().begin(), cfg7->getHead()->getNextMain()->getPrev().end(), cfg7->getHead()), 1); // 1 -> 3 p
		ASSERT_EQ(ptr7->getPrev().front(), cfg7->getHead()->getNextMain()); // 3 -> 4 p
		ASSERT_EQ(ptr7->getNextMain()->getPrev().size(), 1);
		CFGNode* cfg7_node5 = ptr7->getNextMain();
		ASSERT_EQ(cfg7_node5->getPrev().front(), ptr7); // 4 -> 5 p
		ASSERT_EQ(cfg7_node5->getNextMain()->getPrev().size(), 1);
		ASSERT_EQ(cfg7_node5->getNextBranch()->getPrev().size(), 1);
		ASSERT_EQ(cfg7_node5->getNextMain()->getPrev().front(), cfg7_node5); // 5 -> 7 p
		ASSERT_EQ(cfg7_node5->getNextBranch()->getPrev().front(), cfg7_node5); // 5 -> 6 p
		cfg7_node2->setNextMain(nullptr);
		cfg7->getHead()->getNextMain()->getPrev().erase(find(cfg7->getHead()->getNextMain()->getPrev().begin(), cfg7->getHead()->getNextMain()->getPrev().end(), cfg7_node2));
		delete cfg7_node2;
		cfg7_node6->getPrev().clear();
		cfg7_node5->setNextBranch(nullptr);
		delete cfg7_node6;
		delete cfg7;
		

		CFG* cfg8 = new CFG();
		cfg8->add(1);
		cfg8->add(3);
		cfg8->add(4);
		cfg8->add(5);
		cfg8->add(7);
		CFGNode* cfg8_node2 = new CFGNode();
		cfg8_node2->setProgramLines({ 2 });
		cfg8_node2->setNextMain(cfg8->getHead()->getNextMain());
		cfg8->getHead()->getNextMain()->setPrevBranch(cfg8_node2);
		CFGNode* cfg8_node6 = new CFGNode();
		cfg8_node6->setProgramLines({ 6 });
		cfg8->getHead()->getNextMain()->setNextBranch(cfg8_node6);
		cfg8_node6->setPrevBranch(cfg8->getHead()->getNextMain());
		CFGNode* ptr8 = cfg8->makeStandalone(5); // (1, 2) -> 3 4 -> 5 -> (6, 7)

		ASSERT_EQ(cfg8->getHead()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg8_node2->getProgramLines().size(), 1);
		ASSERT_EQ(cfg8->getHead()->getNextMain()->getProgramLines().size(), 2);
		ASSERT_EQ(cfg8->getHead()->getNextMain()->getNextMain()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg8_node6->getProgramLines().size(), 1);
		ASSERT_EQ(cfg8->getHead()->getNextMain()->getNextMain()->getNextMain()->getProgramLines().size(), 1);
		ASSERT_EQ(cfg8->getHead()->getProgramLines()[0], 1);
		ASSERT_EQ(cfg8_node2->getProgramLines()[0], 2);
		ASSERT_EQ(cfg8->getHead()->getNextMain()->getProgramLines()[0], 3);
		ASSERT_EQ(cfg8->getHead()->getNextMain()->getProgramLines()[1], 4);
		ASSERT_EQ(cfg8->getHead()->getNextMain()->getNextMain()->getProgramLines()[0], 5);
		ASSERT_EQ(cfg8_node6->getProgramLines()[0], 6);
		ASSERT_EQ(cfg8->getHead()->getNextMain()->getNextMain()->getNextMain()->getProgramLines()[0], 7);
		ASSERT_TRUE(cfg8->getHead()->getNextMain()->getNextMain()->getNextMain()->getNextMain()->isTermination());
		ASSERT_EQ(ptr8, cfg8->getHead()->getNextMain()->getNextMain());

		CFGNode* cfg8_node34 = cfg8->getHead()->getNextMain();
		ASSERT_EQ(cfg8_node2->getNextMain(), cfg8_node34); // 2 -> 3 4
		ASSERT_EQ(cfg8_node34->getPrev().size(), 2);
		ASSERT_EQ(count(cfg8_node34->getPrev().begin(), cfg8_node34->getPrev().end(), cfg8_node2), 1); // 2 -> 3 4 p
		ASSERT_EQ(count(cfg8_node34->getPrev().begin(), cfg8_node34->getPrev().end(), cfg8->getHead()), 1); // 1 -> 3 4 p
		ASSERT_EQ(cfg8_node34->getNextMain()->getPrev().size(), 1);
		ASSERT_EQ(cfg8_node34->getNextMain()->getPrev().front(), cfg8_node34); // 3 4 -> 5 p
		ASSERT_EQ(ptr8->getNextMain()->getPrev().size(), 1);
		ASSERT_EQ(ptr8->getNextBranch()->getPrev().size(), 1);
		ASSERT_EQ(ptr8->getNextMain()->getPrev().front(), ptr8); // 5 -> 7 p
		ASSERT_EQ(ptr8->getNextBranch()->getPrev().front(), ptr8); // 5 -> 6 p
		cfg8_node2->setNextMain(nullptr);
		cfg8_node34->getPrev().erase(find(cfg8_node34->getPrev().begin(), cfg8_node34->getPrev().end(), cfg8_node2));
		delete cfg8_node2;
		cfg8_node6->getPrev().clear();
		ptr8->setNextBranch(nullptr);
		delete cfg8_node6;
		delete cfg8;
	}
	
	TEST(CFG, call) {

		CFG* cfg1 = new CFG();
		cfg1->add(1);
		cfg1->add(2);
		cfg1->add(3);

		CFG* cfg1_calls = new CFG();
		cfg1_calls->add(4);
		cfg1_calls->add(6);

		cfg1->call(cfg1_calls, 1);
		cfg1->call(cfg1_calls, 2);
		cfg1->call(cfg1_calls, 3);

		CFGNode* cfg1_node1 = cfg1->getHead();
		ASSERT_NE(cfg1_node1, nullptr);
		CFGNode* cfg1_node2 = cfg1_node1->getNextMain();
		ASSERT_NE(cfg1_node2, nullptr);
		CFGNode* cfg1_node3 = cfg1_node2->getNextMain();
		ASSERT_NE(cfg1_node3, nullptr);
		CFGNode* cfg1_node4 = cfg1_calls->getHead();
		ASSERT_NE(cfg1_node4, nullptr);
		CFGNode* cfg1_node5 = cfg1_node4->getNextMain();
		ASSERT_NE(cfg1_node5, nullptr);

		ASSERT_TRUE(cfg1_node1->isCall());
		ASSERT_TRUE(cfg1_node2->isCall());
		ASSERT_TRUE(cfg1_node3->isCall());

		std::vector<prog_line> cfg1_labels_n4 = cfg1_node4->getLabels();
		std::vector<prog_line> cfg1_labels_n4_expected = { 1, 2, 3 };
		ASSERT_EQ(cfg1_labels_n4.size(), cfg1_labels_n4_expected.size());
		ASSERT_TRUE(equal(cfg1_labels_n4.begin(), cfg1_labels_n4.end(), cfg1_labels_n4_expected.begin(), cfg1_labels_n4_expected.end()));
		std::vector<prog_line> cfg1_labels_n5 = cfg1_node5->getLabels();
		std::vector<prog_line> cfg1_labels_n5_expected = { 1, 2, 3 };
		ASSERT_EQ(cfg1_labels_n5.size(), cfg1_labels_n5_expected.size());
		ASSERT_TRUE(equal(cfg1_labels_n5.begin(), cfg1_labels_n5.end(), cfg1_labels_n5_expected.begin(), cfg1_labels_n5_expected.end()));

		ASSERT_TRUE(cfg1_node5->getNextMain()->isReturn());
		ASSERT_TRUE(cfg1_node5->getNextMain()->isTermination());

		std::unordered_map<prog_line, CFGNode*> map = cfg1_node5->getNextMain()->getNextReturn();
		ASSERT_EQ(map.size(), 3);
		ASSERT_TRUE(map.find(1) != map.end());
		ASSERT_TRUE(map.find(2) != map.end());
		ASSERT_TRUE(map.find(3) != map.end());
		ASSERT_EQ(map.find(1)->second, cfg1_node2);
		ASSERT_EQ(map.find(2)->second, cfg1_node3);
		ASSERT_EQ(map.find(3)->second, cfg1_node3->getNextMain());

		CFG* first = new CFG();
		first->add(11);
		first->add(13);
		first->add(15);
		first->add(17);
		first->add(19);

		CFGNode* first_node1 = first->getHead();
		ASSERT_NE(first_node1, nullptr);
		CFGNode* first_node2 = first_node1->getNextMain();
		ASSERT_NE(first_node2, nullptr);
		CFGNode* first_node3 = first_node2->getNextMain();
		ASSERT_NE(first_node3, nullptr);
		CFGNode* first_node4 = first_node3->getNextMain();
		ASSERT_NE(first_node4, nullptr);
		CFGNode* first_node5 = first_node4->getNextMain();
		ASSERT_NE(first_node5, nullptr);
		CFGNode* first_node6 = first_node5->getNextMain();
		ASSERT_NE(first_node6, nullptr);

		first->call(cfg1, 11);
		first->call(cfg1, 19);
		first->call(cfg1_calls, 15);

		ASSERT_TRUE(first_node1->isCall());
		ASSERT_TRUE(first_node3->isCall());
		ASSERT_TRUE(first_node5->isCall());

		std::vector<prog_line> expected_labels = { 11, 19 };
		std::vector<prog_line> expected_labels_call = { 1, 2, 3, 15 };
		ASSERT_EQ(cfg1_node1->getLabels(), expected_labels);
		ASSERT_EQ(cfg1_node2->getLabels(), expected_labels);
		ASSERT_EQ(cfg1_node3->getLabels(), expected_labels);
		ASSERT_EQ(cfg1_node4->getLabels(), expected_labels_call);
		ASSERT_EQ(cfg1_node5->getLabels(), expected_labels_call);

		std::unordered_map<prog_line, CFGNode*> map_cfg1 = cfg1_node3->getNextMain()->getNextReturn();
		std::unordered_map<prog_line, CFGNode*> map_cfg1_call = cfg1_node5->getNextMain()->getNextReturn();
		ASSERT_EQ(map_cfg1.size(), 2);
		ASSERT_TRUE(map_cfg1.find(11) != map_cfg1.end());
		ASSERT_TRUE(map_cfg1.find(19) != map_cfg1.end());
		ASSERT_EQ(map_cfg1_call.size(), 4);
		ASSERT_TRUE(map_cfg1_call.find(1) != map_cfg1_call.end());
		ASSERT_TRUE(map_cfg1_call.find(2) != map_cfg1_call.end());
		ASSERT_TRUE(map_cfg1_call.find(3) != map_cfg1_call.end());
		ASSERT_TRUE(map_cfg1_call.find(15) != map_cfg1_call.end());
		
		ASSERT_EQ(map_cfg1.find(11)->second, first_node2);
		ASSERT_EQ(map_cfg1.find(19)->second, first_node5->getNextMain());
		ASSERT_TRUE(first_node5->getNextMain()->isTermination());
		ASSERT_EQ(map_cfg1_call.find(1)->second, cfg1_node2);
		ASSERT_EQ(map_cfg1_call.find(2)->second, cfg1_node3);
		ASSERT_TRUE(cfg1_node3->getNextMain()->isTermination());
		ASSERT_EQ(map_cfg1_call.find(15)->second, first_node4);
	}
	

	TEST(CFG, getNextBip) {
		CFG* cfg1 = new CFG();
		CFG* cfg2 = new CFG();
		CFG* cfg3 = new CFG();

		cfg1->add(1);
		cfg1->add(2);
		cfg1->add(3);
		cfg1->add(4);
		cfg1->add(5);

		cfg2->add(6);
		cfg2->add(7);
		cfg2->add(8);

		cfg3->add(9);

		CFG* cfg3_if = new CFG();
		cfg3_if->add(10);
		CFG* cfg3_else = new CFG();
		cfg3_else->add(11);

		CFG* cfg_3_else_loop = new CFG();
		cfg_3_else_loop->add(12);
		cfg3_else->loop(cfg_3_else_loop, 11);
		cfg3->fork(cfg3_if, cfg3_else, 9);

		cfg2->call(cfg3, 7);
		cfg1->call(cfg2, 2);
		cfg1->call(cfg3, 4);
		std::vector<std::pair<prog_line, prog_line>> nextbip_actual = cfg1->getNextBip();
		std::vector<std::pair<prog_line, prog_line>> nextbip_expected = {
			{1, 2}, {2, 6}, {6, 7}, {7, 9}, {9, 10}, {9, 11},
			{10, 8}, {11, 8}, {8, 3}, {3, 4},
			{4, 9}, {10, 5}, {11, 5}, {11, 12}, {12, 11}
		};

		sort(nextbip_actual.begin(), nextbip_actual.end());
		sort(nextbip_expected.begin(), nextbip_expected.end());
		ASSERT_EQ(nextbip_actual, nextbip_expected);

		delete cfg_3_else_loop, cfg3_if, cfg3_else, cfg3, cfg2, cfg1;


		CFG* cfg4 = new CFG();
		CFG* cfg5 = new CFG();
		CFG* cfg6 = new CFG();
		CFG* cfg7 = new CFG();
		CFG* cfg7_loop = new CFG();
		CFG* cfg6_if = new CFG();
		CFG* cfg6_else = new CFG();

		cfg4->add(50);
		cfg4->add(51);

		cfg5->add(1);
		cfg5->add(2);

		cfg6->add(3);
		cfg6->add(4);

		cfg7->add(5);

		cfg7_loop->add(6);

		cfg6_if->add(7);
		cfg6_else->add(8);

		cfg6->fork(cfg6_if, cfg6_else, 3);
		cfg7->loop(cfg7_loop, 5);

		cfg6->call(cfg7, 4);
		cfg5->call(cfg6, 2);
		cfg4->call(cfg5, 50);

		std::vector<std::pair<prog_line, prog_line>> nextbip_actual_2 = cfg4->getNextBip();
		std::vector<std::pair<prog_line, prog_line>> nextbip_expected_2 = {
			{50, 1}, {1, 2}, {2, 3}, {3, 7}, {3, 8},
			{7, 4}, {8, 4}, {4, 5}, {5, 6}, {6, 5},
			{5, 51}
		};

		sort(nextbip_actual_2.begin(), nextbip_actual_2.end());
		sort(nextbip_expected_2.begin(), nextbip_expected_2.end());
		ASSERT_EQ(nextbip_actual_2, nextbip_expected_2);

		delete cfg6_if, cfg6_else, cfg7_loop, cfg4, cfg5, cfg6, cfg7;
	}

	TEST(CFG, getNextBipWithLabels) {
		CFG* cfg1 = new CFG();
		CFG* cfg2 = new CFG();
		CFG* cfg3 = new CFG();

		cfg1->add(1);
		cfg1->add(2);
		cfg1->add(3);
		cfg1->add(4);
		cfg1->add(5);

		cfg2->add(6);
		cfg2->add(7);
		cfg2->add(8);

		cfg3->add(9);

		CFG* cfg3_if = new CFG();
		cfg3_if->add(10);
		CFG* cfg3_else = new CFG();
		cfg3_else->add(11);

		CFG* cfg_3_else_loop = new CFG();
		cfg_3_else_loop->add(12);
		cfg3_else->loop(cfg_3_else_loop, 11);
		cfg3->fork(cfg3_if, cfg3_else, 9);

		cfg2->call(cfg3, 7);
		cfg1->call(cfg2, 2);
		cfg1->call(cfg3, 4);
		std::vector<std::pair<LabelledProgLine, LabelledProgLine>> nextbip_actual = cfg1->getNextBipWithLabel();
		std::vector<std::pair<LabelledProgLine, LabelledProgLine>> nextbip_expected = {
			{{1, 0}, {2, 0}},
			{{2, 0}, {6, 2}},
			{{6, 2}, {7, 2}},
			{{7, 2}, {9, 7}},
			{{9, 7}, {10, 7}},
			{{9, 7}, {11, 7}},
			{{11, 7}, {12, 7}},
			{{12, 7}, {11, 7}},
			{{10, 7}, {8, 2}},
			{{11, 7}, {8, 2}},
			{{8, 2}, {3, 0}},
			{{3, 0}, {4, 0}},
			{{4, 0}, {9, 4}},
			{{9, 4}, {10, 4}},
			{{9, 4}, {11, 4}},
			{{11, 4}, {12, 4}},
			{{12, 4}, {11, 4}},
			{{10, 4}, {5, 0}},
			{{11, 4}, {5, 0}}
		};

		sort(nextbip_actual.begin(), nextbip_actual.end());
		sort(nextbip_expected.begin(), nextbip_expected.end());
		ASSERT_EQ(nextbip_actual, nextbip_expected);

		delete cfg_3_else_loop, cfg3_if, cfg3_else, cfg3, cfg2, cfg1;


		CFG* cfg4 = new CFG();
		CFG* cfg5 = new CFG();
		CFG* cfg6 = new CFG();
		CFG* cfg7 = new CFG();
		CFG* cfg7_loop = new CFG();
		CFG* cfg6_if = new CFG();
		CFG* cfg6_else = new CFG();

		cfg4->add(50);
		cfg4->add(51);

		cfg5->add(1);
		cfg5->add(2);

		cfg6->add(3);
		cfg6->add(4);

		cfg7->add(5);

		cfg7_loop->add(6);

		cfg6_if->add(7);
		cfg6_else->add(8);

		cfg6->fork(cfg6_if, cfg6_else, 3);
		cfg7->loop(cfg7_loop, 5);

		cfg6->call(cfg7, 4);
		cfg5->call(cfg6, 2);
		cfg4->call(cfg5, 50);

		std::vector<std::pair<LabelledProgLine, LabelledProgLine>> nextbip_actual_2 = cfg4->getNextBipWithLabel();
		std::vector<std::pair<LabelledProgLine, LabelledProgLine>> nextbip_expected_2 = {
			{{50, 0}, {1, 50}},
			{{1, 50}, {2, 50}},
			{{2, 50}, {3, 2}},
			{{3, 2}, {7, 2}},
			{{3, 2}, {8, 2}},
			{{7, 2}, {4, 2}},
			{{8, 2}, {4, 2}},
			{{4, 2}, {5, 4}},
			{{5, 4}, {6, 4}},
			{{6, 4}, {5, 4}},
			{{5, 4}, {51, 0}}
		};

		//for (auto p: nextbip_actual_2) {
		//	std::cout << "(" << p.first.program_line << "," << p.first.label << ")";
		//	std::cout << " --> ";
		//	std::cout << "(" << p.second.program_line << "," << p.second.label << ")";
		//	std::cout << std::endl;
		//}

		sort(nextbip_actual_2.begin(), nextbip_actual_2.end());
		sort(nextbip_expected_2.begin(), nextbip_expected_2.end());
		ASSERT_EQ(nextbip_actual_2, nextbip_expected_2);

		delete cfg6_if, cfg6_else, cfg7_loop, cfg4, cfg5, cfg6, cfg7;
	}
}