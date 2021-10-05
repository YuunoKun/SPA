#include "pch.h"

#include "Pattern.h"
#include "Common.h"

namespace UnitTesting {
	ExprParser expr_parser;
	Entity validLeftExpressionType1(VARIABLE, Synonym{ "variableType" });
	Entity validLeftExpressionType2(WILD, Synonym{ "wildType" });
	Entity validPatternType(ASSIGN, Synonym{ "assignType" });
	Entity invalidType1(STMT, Synonym{ "assignType" });
	Entity invalidType2(IF, Synonym{ "assignType" });
	Entity invalidType3(CALL, Synonym{ "assignType" });
	std::string validExpression1 = "x+y / 3";
	std::string validExpression2 = "x";
	std::string emptyExpression = "";
	expr* validExprNode1 = expr_parser.parse(validExpression1);
	expr* validExprNode2 = expr_parser.parse(validExpression2);
	expr* emptyExprNode = expr_parser.parse(emptyExpression);

	TEST(Pattern, patternTypeInvalid) {
		try {
			// Negative Test Cases
			Pattern patternWithInvalidPatternType(invalidType1, validLeftExpressionType2, {}, true);
			Pattern patternWithInvalidPatternType2(invalidType2, validLeftExpressionType1, {}, true);
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Pattern Type is invalid"));
		}
		catch (...) {
			// Test case should fail if not caught as std::invalid argument
			FAIL();
		}
	}

	TEST(Pattern, leftExpressionTypeInvalid) {
		try {
			// Negative Test Cases
			Pattern patternWithInvalidLeftExpressionType(validPatternType, invalidType2, {}, true);
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Left Expression Type is invalid"));
		}
		catch (...) {
			// Test case should fail if not caught as std::invalid argument
			FAIL();
		}
	}

	TEST(Pattern, getLeftExpression) {
		Pattern pattern(validPatternType, validLeftExpressionType2, {}, true);
		EXPECT_EQ(pattern.getLeftExpression(), validLeftExpressionType2);
	}
	TEST(Pattern, getPatternType) {
		Pattern pattern(validPatternType, validLeftExpressionType2, {}, true);
		EXPECT_EQ(pattern.getPatternType(), validPatternType);
	}

	TEST(Pattern, getExpression) {
		Pattern pattern1(validPatternType, validLeftExpressionType2, validExpression1, true);

		Pattern pattern2(validPatternType, validLeftExpressionType2, validExpression2, true);

		Pattern pattern3(validPatternType, validLeftExpressionType2, emptyExpression, true);
		EXPECT_TRUE(pattern3.getExpression()->equals(emptyExprNode));
		EXPECT_FALSE(pattern3.getExpression()->equals(validExprNode1));
		EXPECT_FALSE(pattern3.getExpression()->equals(validExprNode2));
	}

	TEST(Pattern, isWild) {
		Pattern pattern1(validPatternType, validLeftExpressionType1, {}, true);
		EXPECT_TRUE(pattern1.isWild());

		Pattern pattern2(validPatternType, validLeftExpressionType1, {}, false);
		EXPECT_FALSE(pattern2.isWild());
	}

	TEST(Pattern, equal) {
		Pattern e1(validPatternType, validLeftExpressionType1, validExpression1, true);
		Pattern e2(validPatternType, validLeftExpressionType1, validExpression1, true);
		EXPECT_EQ(e1, e2);

		Pattern e3(validPatternType, validLeftExpressionType1, {}, false);
		EXPECT_FALSE(e1 == e3);

		Pattern e4(validPatternType, validLeftExpressionType2, {}, true);
		EXPECT_FALSE(e1 == e4);

		Pattern e5(validPatternType, validLeftExpressionType2, {}, false);
		EXPECT_FALSE(e1 == e5);

		Pattern e6(validPatternType, validLeftExpressionType1, validExpression2, true);
		EXPECT_FALSE(e1 == e6);
	}
}