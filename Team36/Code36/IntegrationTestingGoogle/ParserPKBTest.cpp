#include <iostream>

#include "pch.h"
#include "Parser.h"
#include "PKB.h"
#include "RelationTable.h"
#include "RelationTable.cpp"

namespace IntegrationTesting {
	class ParserPKBTest : public testing::Test {
	protected:

		ParserPKBTest() {
			// You can do set-up work for each test here.
		}

		// If the constructor and destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		virtual void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
			PKB::getInstance().resetCache();
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}
		template<typename T>
		bool compare(std::vector<T> v1, std::vector<T> v2)
		{
			std::sort(v1.begin(), v1.end());
			std::sort(v2.begin(), v2.end());
			return v1 == v2;
		}

		std::vector<StmtInfo> sample5_stmts = { { 1, STMT_ASSIGN }, { 2, STMT_CALL} , { 3, STMT_CALL },
			{ 4, STMT_READ}, { 5, STMT_PRINT }, { 6, STMT_PRINT}, { 7, STMT_ASSIGN}, { 8, STMT_CALL},
			{ 9, STMT_WHILE} , { 10, STMT_ASSIGN} , { 11, STMT_CALL} , { 12, STMT_IF},
			{ 13, STMT_ASSIGN },  { 14, STMT_ASSIGN }, { 15, STMT_ASSIGN } };
	};

	TEST_F(ParserPKBTest, Sample3TokenizerTest) {
		SourceProcessor::Parser parser;
		parser.load_file("../UnitTestingGoogle/SPTest/TestSource/Sample3.txt");
		parser.parse();
		EXPECT_EQ(PKB::getInstance().getProcedures().size(), 1);
		EXPECT_EQ(PKB::getInstance().getConstants().size(), 1);
	}

	TEST_F(ParserPKBTest, Sample4TokenizerTest) {
		SourceProcessor::Parser parser;
		parser.load_file("../UnitTestingGoogle/SPTest/TestSource/Sample4.txt");
		parser.parse();
		EXPECT_EQ(PKB::getInstance().getProcedures().size(), 2);
		EXPECT_EQ(PKB::getInstance().getConstants().size(), 4);
	}

	TEST_F(ParserPKBTest, DuplicateProcedureNames) {
		SourceProcessor::Parser parser;
		parser.load_file("../IntegrationTestingGoogle/Tests/duplicate_procedure_name_source.txt");
		ASSERT_THROW(parser.parse(), std::runtime_error);
	}

	TEST_F(ParserPKBTest, CallUndefinedProcedureName) {
		SourceProcessor::Parser parser;
		parser.load_file("../IntegrationTestingGoogle/Tests/call_undefined_procedure_source.txt");
		ASSERT_THROW(parser.parse(), std::runtime_error);
	}

	TEST_F(ParserPKBTest, RecursiveCallSource) {
		SourceProcessor::Parser parser;
		parser.load_file("../IntegrationTestingGoogle/Tests/recursive_calls_source.txt");
		ASSERT_THROW(parser.parse(), std::runtime_error);
	}

	TEST_F(ParserPKBTest, CyclicCallSource) {
		SourceProcessor::Parser parser;
		parser.load_file("../IntegrationTestingGoogle/Tests/cyclic_calls_source.txt");
		ASSERT_THROW(parser.parse(), std::runtime_error);
	}

	TEST_F(ParserPKBTest, Sample5Test_Entities) {
		SourceProcessor::Parser parser;
		parser.load_file("../UnitTestingGoogle/SPTest/TestSource/Sample5.txt");
		parser.parse();

		std::vector<proc_name> proc_v = { "main", "readPoint", "printResults", "computeCentroid" };
		std::vector<constant> const_v = { 0, 1 };
		std::vector<var_name> var_v = { "flag", "x", "y", "count", "cenX", "cenY", "normSq" };

		EXPECT_TRUE(ParserPKBTest::compare(
			static_cast<std::vector<var_name>>(PKB::getInstance().getVariables()),
			static_cast<std::vector<var_name>>(var_v)));
		EXPECT_EQ(PKB::getInstance().getProcedures(), proc_v);
		EXPECT_EQ(PKB::getInstance().getConstants(), const_v);
		EXPECT_EQ(PKB::getInstance().getStmts(), ParserPKBTest::sample5_stmts);
	}

	TEST_F(ParserPKBTest, Sample5Test_Assign) {
		SourceProcessor::Parser parser;
		parser.load_file("../UnitTestingGoogle/SPTest/TestSource/Sample5.txt");
		parser.parse();

		std::vector<std::pair<stmt_index, var_name>> expected_assign = { {1, "flag"}, {7, "count"},
			{10, "count"}, {13, "flag"}, {14, "cenX"}, {15, "normSq"} };
		std::sort(expected_assign.begin(), expected_assign.end());

		UniqueRelationTable<stmt_index, var_name> table = PKB::getInstance().getAssigns();
		std::vector<std::pair<stmt_index, var_name>> v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_assign);
	}

	TEST_F(ParserPKBTest, Sample5Test_Expr) {
		SourceProcessor::Parser parser;
		parser.load_file("../UnitTestingGoogle/SPTest/TestSource/Sample5.txt");
		parser.parse();

		std::vector<std::pair<stmt_index, expr>> expected_expr = { {1, "0"}, {7, "0" }, {10, "count+1"},
			{13, "1"}, {14, "cenX/count"}, {15, "cenX*cenX+cenY*cenY"} };
		std::sort(expected_expr.begin(), expected_expr.end());

		UniqueRelationTable<stmt_index, expr> table = PKB::getInstance().getExpr();
		std::vector<std::pair<stmt_index, expr>> v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_expr);
	}

	TEST_F(ParserPKBTest, Sample5Test_Follows) {
		SourceProcessor::Parser parser;
		parser.load_file("../UnitTestingGoogle/SPTest/TestSource/Sample5.txt");
		parser.parse();

		std::vector<std::pair<stmt_index, stmt_index>> expected_follows = {
			{1,2}, {2,3}, {5,6}, {7,8}, {8,9}, {9, 12}, {10,11}, {12,15}
		};
		std::sort(expected_follows.begin(), expected_follows.end());

		std::vector<std::pair<StmtInfo, StmtInfo>> expected_follows_stmt;
		for (auto const& pair : expected_follows) {
			expected_follows_stmt.push_back({ ParserPKBTest::sample5_stmts[pair.first - 1], ParserPKBTest::sample5_stmts[pair.second - 1] });
		}

		UniqueRelationTable<StmtInfo, StmtInfo> table = PKB::getInstance().getFollows();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_follows_stmt);
	}

	TEST_F(ParserPKBTest, Sample5Test_FollowsT) {
		SourceProcessor::Parser parser;
		parser.load_file("../UnitTestingGoogle/SPTest/TestSource/Sample5.txt");
		parser.parse();

		std::vector<std::pair<stmt_index, stmt_index>> expected_followsT = {
			{1,2}, {1,3}, {2,3}, {5,6}, {7,8}, {7,9}, {7,12}, {7,15}, {8,9}, {8,12}, {8,15}, {9, 12}, {9, 15}, {10,11}, {12,15}
		};
		std::sort(expected_followsT.begin(), expected_followsT.end());

		std::vector<std::pair<StmtInfo, StmtInfo>> expected_followsT_stmt;
		for (auto const& pair : expected_followsT) {
			expected_followsT_stmt.push_back({ ParserPKBTest::sample5_stmts[pair.first - 1], ParserPKBTest::sample5_stmts[pair.second - 1] });
		}

		RelationTable<StmtInfo, StmtInfo> table = PKB::getInstance().getFollowsT();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_followsT_stmt);
	}

	TEST_F(ParserPKBTest, Sample5Test_Parent) {
		SourceProcessor::Parser parser;
		parser.load_file("../UnitTestingGoogle/SPTest/TestSource/Sample5.txt");
		parser.parse();

		std::vector<std::pair<stmt_index, stmt_index>> expected_parent = {
			{9,10}, {9,11}, {12,13}, {12,14}
		};
		std::sort(expected_parent.begin(), expected_parent.end());

		std::vector<std::pair<StmtInfo, StmtInfo>> expected_parent_stmt;
		for (auto const& pair : expected_parent) {
			expected_parent_stmt.push_back({ ParserPKBTest::sample5_stmts[pair.first - 1], ParserPKBTest::sample5_stmts[pair.second - 1] });
		}

		RelationTable<StmtInfo, StmtInfo> table = PKB::getInstance().getParent();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_parent_stmt);
	}

	TEST_F(ParserPKBTest, Sample5Test_ParentT) {
		SourceProcessor::Parser parser;
		parser.load_file("../UnitTestingGoogle/SPTest/TestSource/Sample5.txt");
		parser.parse();

		std::vector<std::pair<stmt_index, stmt_index>> expected_parentT = {
			{9,10}, {9,11}, {12,13}, {12,14}
		};
		std::sort(expected_parentT.begin(), expected_parentT.end());

		std::vector<std::pair<StmtInfo, StmtInfo>> expected_parentT_stmt;
		for (auto const& pair : expected_parentT) {
			expected_parentT_stmt.push_back({ ParserPKBTest::sample5_stmts[pair.first - 1], ParserPKBTest::sample5_stmts[pair.second - 1] });
		}

		RelationTable<StmtInfo, StmtInfo> table = PKB::getInstance().getParent();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_parentT_stmt);
	}

	TEST_F(ParserPKBTest, Sample5Test_UsesS) {
		SourceProcessor::Parser parser;
		parser.load_file("../UnitTestingGoogle/SPTest/TestSource/Sample5.txt");
		parser.parse();

		std::vector<std::pair<stmt_index, var_name>> expected_usesS = {
			{2, "count"}, {2, "x"}, {2, "y"}, {2, "cenX"}, {2, "cenY"}, {3, "cenX"}, {3, "cenY"},
			{5, "cenX"}, {6, "cenY"}, {9, "x"}, {9, "y"}, {9, "count"}, {10, "count"}, {12, "count"},
			{12, "cenX"}, {14, "cenX"}, {14, "count"}, {15, "cenX"}, {15, "cenY"}
		};
		std::sort(expected_usesS.begin(), expected_usesS.end());

		std::vector<std::pair<StmtInfo, var_name>> expected_usesS_stmt;
		for (auto const& pair : expected_usesS) {
			expected_usesS_stmt.push_back({ ParserPKBTest::sample5_stmts[pair.first - 1], pair.second });
		}

		RelationTable<StmtInfo, var_name> table = PKB::getInstance().getUsesS();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_usesS_stmt);
	}

	TEST_F(ParserPKBTest, Sample5Test_ModifiesS) {
		SourceProcessor::Parser parser;
		parser.load_file("../UnitTestingGoogle/SPTest/TestSource/Sample5.txt");
		parser.parse();

		std::vector<std::pair<stmt_index, var_name>> expected_modifiesS = {
			{1, "flag"}, {2, "count"}, {2, "x"}, {2, "flag"}, {2, "cenX"}, {2, "normSq"}, {4, "x"}, {7, "count"},
			{8, "x"}, {9, "count"}, {9, "x"}, {10, "count"}, {11, "x"}, {12, "flag"}, {12, "cenX"}, {13, "flag"}, {14, "cenX"},
			{15, "normSq"}
		};
		std::sort(expected_modifiesS.begin(), expected_modifiesS.end());

		std::vector<std::pair<StmtInfo, var_name>> expected_modifiesS_stmt;
		for (auto const& pair : expected_modifiesS) {
			expected_modifiesS_stmt.push_back({ ParserPKBTest::sample5_stmts[pair.first - 1], pair.second });
		}

		RelationTable<StmtInfo, var_name> table = PKB::getInstance().getModifiesS();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_modifiesS_stmt);
	}

	TEST_F(ParserPKBTest, NoProcedureCallsTest_UsesP) {
		SourceProcessor::Parser parser;
		parser.load_file("../IntegrationTestingGoogle/Tests/no_procedure_call_source.txt");
		parser.parse();

		std::vector<std::pair<proc_name, var_name>> expected_usesP = {
			{"mySecondProcedure", "x"}, {"mySecondProcedure", "y"}
		};
		std::sort(expected_usesP.begin(), expected_usesP.end());

		RelationTable<proc_name, var_name> table = PKB::getInstance().getUsesP();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_usesP);
	}

	TEST_F(ParserPKBTest, NoProcedureCallsTest_ModifiesP) {
		SourceProcessor::Parser parser;
		parser.load_file("../IntegrationTestingGoogle/Tests/no_procedure_call_source.txt");
		parser.parse();

		std::vector<std::pair<proc_name, var_name>> expected_modifiesP = {
			{"myFirstProcedure", "x"}, {"myFirstProcedure", "y"},
			{"mySecondProcedure", "z"}, {"mySecondProcedure", "x"}, {"mySecondProcedure", "w"}
		};
		std::sort(expected_modifiesP.begin(), expected_modifiesP.end());

		RelationTable<proc_name, var_name> table = PKB::getInstance().getModifiesP();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_modifiesP);
	}

	TEST_F(ParserPKBTest, ProcedureCallsTest_UsesP) {
		SourceProcessor::Parser parser;
		parser.load_file("../IntegrationTestingGoogle/Tests/procedure_call_source.txt");
		parser.parse();

		std::vector<std::pair<proc_name, var_name>> expected_usesP = {
			{"main", "x"}, {"main", "y"},{"main", "count"},{"main", "cenX"},{"main", "cenY"},
			{"printResults", "cenX"},{"printResults", "cenY"},
			{"computeCentroid", "x"}, {"computeCentroid", "y"},{"computeCentroid", "count"},
				{"computeCentroid", "cenX"},{"computeCentroid", "cenY"}
		};
		std::sort(expected_usesP.begin(), expected_usesP.end());

		RelationTable<proc_name, var_name> table = PKB::getInstance().getUsesP();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_usesP);
	}

	TEST_F(ParserPKBTest, ProcedureCallsTest_ModifiesP) {
		SourceProcessor::Parser parser;
		parser.load_file("../IntegrationTestingGoogle/Tests/procedure_call_source.txt");
		parser.parse();

		std::vector<std::pair<proc_name, var_name>> expected_modifiesP = {
			{"main", "count"}, {"main", "x"}, {"main", "flag"},{"main", "cenX"},{"main", "normSq"},
			{"readPoint", "x"},
			{"computeCentroid", "count"}, {"computeCentroid", "x"},{"computeCentroid", "flag"},
				{"computeCentroid", "cenX"},{"computeCentroid", "normSq"}
		};
		std::sort(expected_modifiesP.begin(), expected_modifiesP.end());

		RelationTable<proc_name, var_name> table = PKB::getInstance().getModifiesP();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_modifiesP);
	}
}