#include "pch.h"
#include <iostream>
#include "SP/Parser.h"
#include "PKB/PKB.h"

namespace UnitTesting {

	using namespace SourceProcessor;
	TEST(Parser, load_file_pass) {
		Parser parser;
		ASSERT_NO_THROW(parser.loadSourceFile("../UnitTestingGoogle/SPTest/Load_file_test.txt"));
	}

	TEST(Parser, load_file_fail) {
		Parser parser;
		ASSERT_THROW(parser.loadSourceFile("../UnitTestingGoogle/SPTest/Load_file_test_does_not_exist.txt"), std::runtime_error);
	}
} // namespace UnitTesting