#include "pch.h"

#include <chrono>

#include "PQL/QueryEvaluator/QueryEvaluator.h"
#include "PKB/PKBAdapter.h"
#include "PKB/PKB.h"
#include "Common/Common.h"
#include "Common/ExprParser.h"
#include "PQL/Helper/Utility.h"

namespace UnitTesting {
	class QueryEvaluatorStressTest : public testing::Test {
	protected:
		QueryEvaluatorStressTest() {
			for (int i = 0; i < source_code_size.size(); i++) {
				tables.push_back(generateTable(source_code_size[i]));
				tables2.push_back(generateTable(source_code_size[i]));
				join_tables.push_back(generateJoinTable(source_code_size[i]));
			}
		}

		std::list<std::vector<value>> generateTable(int size) {
			std::list<std::vector<value>> table;
			for (value i = 0; i < size; i++) {
				for (value j = 0; j < size; j++) {
					table.push_back({ i, j});
				}
			}
			return table;
		}

		std::unordered_multimap<value, std::vector<value>> generateJoinTable(int size) {
			std::unordered_multimap<value, std::vector<value>> table;

			for (value i = 0; i < size; i++) {
				for (value j = 0; j < size; j++) {
					table.insert({ i, { i, j } });
				}
			}
			return table;
		}
		void startTimer() {
			startTime = std::chrono::high_resolution_clock::now();
		}
		void endTimer() {
			endTime = std::chrono::high_resolution_clock::now();
		}
		void printTime(std::string message) {
			// Call the function, here sort()
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

			std::cout << message << duration.count() << " milliseconds" << std::endl;
		}

		std::chrono::steady_clock::time_point startTime, endTime;

		bool disable_stress_test = true;
		// table size = x ^ 2
		std::vector<int> source_code_size = { 10, 20, 30, 40, 50, 100 , 250 };//, 250 };
		std::vector<std::list<std::vector<value>>> tables;
		std::vector<std::list<std::vector<value>>> tables2;
		std::vector<std::unordered_multimap<value, std::vector<value>>> join_tables;
	};

	// Join Table merge column algorithm Speed Test
	TEST_F(QueryEvaluatorStressTest, join_table_common_column) {
		if (disable_stress_test) {
			return;
		}
		for (int i = 0; i < source_code_size.size(); i++) {
			std::list<std::vector<value>> out;
			startTimer();
			Utility::joinTable(tables[i], 0, join_tables[i], 0, out);
			endTimer();
			printTime("Codes of size " + std::to_string(source_code_size[i]) + " takes : ");
		}
		EXPECT_TRUE(disable_stress_test);
	}

	// Join Table algorithm Speed Test 
	TEST_F(QueryEvaluatorStressTest, join_table) {
		if (disable_stress_test) {
			return;
		}
		for (int i = 0; i < source_code_size.size(); i++) {
			std::list<std::vector<value>> out;
			startTimer();
			Utility::joinTable(tables[i], tables2[i], out);
			endTimer();
			printTime("Codes of size " + std::to_string(source_code_size[i]) + " takes : ");
		}
		EXPECT_TRUE(disable_stress_test);
	}

}