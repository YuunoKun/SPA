#include "pch.h"

#include "PQL/QueryObject/Pattern.h"
#include "Common/Common.h"
#include "Common/ExprParser.h"

namespace UnitTesting {
	ExprParser expr_parser;
	Entity valid_left_expression_type1(VARIABLE, Synonym{ "variableType" });
	Entity valid_left_expression_type2(WILD, Synonym{ "wildType" });
	Entity valid_pattern_type(ASSIGN, Synonym{ "assignType" });
	Entity invalid_type1(STMT, Synonym{ "assignType" });
	Entity invalid_type2(IF, Synonym{ "assignType" });
	Entity invalid_type3(CALL, Synonym{ "assignType" });
	std::string valid_expression1 = "x+y / 3";
	std::string valid_expression2 = "x";
	std::string empty_expression = "";
	expr valid_expr_node1 = expr_parser.parse(valid_expression1);
	expr valid_expr_node2 = expr_parser.parse(valid_expression2);
	expr empty_expr_node = expr_parser.parse(empty_expression);

	TEST(Pattern, patternTypeInvalid) {
		try {
			// Negative Test Cases
			Pattern patternWithInvalidPatternType(invalid_type1, valid_left_expression_type2, {}, true);
			Pattern patternWithInvalidPatternType2(invalid_type2, valid_left_expression_type1, {}, true);
		} catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Pattern Type is invalid"));
		} catch (...) {
			// Test case should fail if not caught as std::invalid argument
			FAIL();
		}
	}

	TEST(Pattern, leftExpressionTypeInvalid) {
		try {
			// Negative Test Cases
			Pattern patternWithInvalidLeftExpressionType(valid_pattern_type, invalid_type2, {}, true);
		} catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Left Expression Type is invalid"));
		} catch (...) {
			// Test case should fail if not caught as std::invalid argument
			FAIL();
		}
	}

	TEST(Pattern, getLeftExpression) {
		Pattern pattern(valid_pattern_type, valid_left_expression_type2, {}, true);
		EXPECT_EQ(pattern.getLeftExpression(), valid_left_expression_type2);
	}
	TEST(Pattern, getPatternType) {
		Pattern pattern(valid_pattern_type, valid_left_expression_type2, {}, true);
		EXPECT_EQ(pattern.getPatternType(), valid_pattern_type);
	}

	TEST(Pattern, getExpression) {
		Pattern pattern1(valid_pattern_type, valid_left_expression_type2, valid_expression1, true);

		Pattern pattern2(valid_pattern_type, valid_left_expression_type2, valid_expression2, true);

		Pattern pattern3(valid_pattern_type, valid_left_expression_type2, empty_expression, true);
		EXPECT_TRUE(pattern3.getExpression().equals(&empty_expr_node));
		EXPECT_FALSE(pattern3.getExpression().equals(&valid_expr_node1));
		EXPECT_FALSE(pattern3.getExpression().equals(&valid_expr_node2));
	}

	TEST(Pattern, isWild) {
		Pattern pattern1(valid_pattern_type, valid_left_expression_type1, {}, true);
		EXPECT_TRUE(pattern1.isWild());

		Pattern pattern2(valid_pattern_type, valid_left_expression_type1, {}, false);
		EXPECT_FALSE(pattern2.isWild());
	}

	TEST(Pattern, equal) {
		Pattern e1(valid_pattern_type, valid_left_expression_type1, valid_expression1, true);
		Pattern e2(valid_pattern_type, valid_left_expression_type1, valid_expression1, true);
		EXPECT_EQ(e1, e2);

		Pattern e3(valid_pattern_type, valid_left_expression_type1, {}, false);
		EXPECT_FALSE(e1 == e3);

		Pattern e4(valid_pattern_type, valid_left_expression_type2, {}, true);
		EXPECT_FALSE(e1 == e4);

		Pattern e5(valid_pattern_type, valid_left_expression_type2, {}, false);
		EXPECT_FALSE(e1 == e5);

		Pattern e6(valid_pattern_type, valid_left_expression_type1, valid_expression2, true);
		EXPECT_FALSE(e1 == e6);
	}
}