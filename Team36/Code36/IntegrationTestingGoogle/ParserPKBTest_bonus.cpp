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
	};

	TEST_F(ParserPKBTest, NoProcedureCallsTest_UsesP) {
		SourceProcessor::Parser parser;
		parser.load_file("./Tests/no_procedure_call_source.txt");
		parser.parse();

		std::vector<std::pair<proc_name, var_name>> expected_usesP = {
			{"mySecondProcedure ", "x"}, {"mySecondProcedure ", "y"}
		};
		std::sort(expected_usesP.begin(), expected_usesP.end());

		RelationTable<proc_name, var_name> table = PKB::getInstance().getUsesP();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_usesP);
	}

	TEST_F(ParserPKBTest, NoProcedureCallsTest_ModifiesP) {
		SourceProcessor::Parser parser;
		parser.load_file("./Tests/no_procedure_call_source.txt");
		parser.parse();

		std::vector<std::pair<proc_name, var_name>> expected_modifiesP = {
			{"myFirstProcedure", "x"}, {"myFirstProcedure", "y"},
			{"mySecondProcedure ", "z"}, {"mySecondProcedure ", "x"}, {"mySecondProcedure ", "w"}
		};
		std::sort(expected_modifiesP.begin(), expected_modifiesP.end());

		RelationTable<proc_name, var_name> table = PKB::getInstance().getModifiesP();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_modifiesP);
	}

	TEST_F(ParserPKBTest, ProcedureCallsTest_UsesP) {
		SourceProcessor::Parser parser;
		parser.load_file("./Tests/procedure_call_source.txt");
		parser.parse();

		std::vector<std::pair<proc_name, var_name>> expected_usesP = {
			{"main", "x"}, {"main", "y"},{"main", "count"},{"main", "cenX"},{"main", "cenY"},
			{"printResults", "cenX"},{"printResults", "cenY"},
			{"computeCentroid ", "x"}, {"computeCentroid ", "y"},{"computeCentroid ", "count"},
				{"computeCentroid ", "cenX"},{"computeCentroid ", "cenY"}
		};
		std::sort(expected_usesP.begin(), expected_usesP.end());

		RelationTable<proc_name, var_name> table = PKB::getInstance().getUsesP();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_usesP);
	}

	TEST_F(ParserPKBTest, ProcedureCallsTest_ModifiesP) {
		SourceProcessor::Parser parser;
		parser.load_file("./Tests/procedure_call_source.txt");
		parser.parse();

		std::vector<std::pair<proc_name, var_name>> expected_modifiesP = {
			{"main", "count"}, {"main", "x"}, {"main", "flag"},{"main", "cenX"},{"main", "normSq"},
			{"printResults", "x"},
			{"computeCentroid ", "count"}, {"computeCentroid ", "x"},{"computeCentroid ", "flag"},
				{"computeCentroid ", "cenX"},{"computeCentroid ", "normSq"}
		};
		std::sort(expected_modifiesP.begin(), expected_modifiesP.end());

		RelationTable<proc_name, var_name> table = PKB::getInstance().getModifiesP();
		auto v = table.getPairs();
		std::sort(v.begin(), v.end());
		EXPECT_EQ(v, expected_modifiesP);
	}
}