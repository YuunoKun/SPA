#include "pch.h"

#include "Pattern.h"
#include "Common.h"

namespace UnitTesting {
	Entity validLeftExpressionType1(VARIABLE, "variableType");
	Entity validLeftExpressionType2(WILD, "wildType");
	Entity validPatternType(ASSIGN, "assignType");
	Entity invalidType1(STMT, "assignType");
	Entity invalidType2(IF, "assignType");
	Entity invalidType3(CALL, "assignType");

	TEST(Pattern, pattern) {
		try {
			// Negative Test Cases
			Pattern patternWithInvalidPatternType(invalidType1, validLeftExpressionType2, {}, true);
			Pattern patternWithInvalidPatternType2(invalidType2, validLeftExpressionType1, {}, true);
			Pattern patternWithInvalidLeftExpressionType(validLeftExpressionType1, invalidType2, {}, true);
			Pattern patternWithInvalidTypes(invalidType1, invalidType3, {}, true);
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Pattern Type is invalid"));
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

	//TODO: test case to be update with expression parser
	TEST(Pattern, getExpression) {
		//entity testentity(stmt, "test");
		//tnode tnode;
		//pattern pattern(testentity, tnode, true);
		//expct_eq(pattern.getexpression(), tnode);
	}

	TEST(Pattern, isWild) {
		Pattern pattern1(validPatternType, validLeftExpressionType1, {}, true);
		EXPECT_TRUE(pattern1.isWild());

		Pattern pattern2(validPatternType, validLeftExpressionType1, {}, false);
		EXPECT_FALSE(pattern2.isWild());
	}

	TEST(Pattern, equal) {
		Pattern e1(validPatternType, validLeftExpressionType1, {}, true);
		Pattern e2(validPatternType, validLeftExpressionType1, {}, true);
		EXPECT_EQ(e1, e2);

		Pattern e3(validPatternType, validLeftExpressionType1, {}, false);
		EXPECT_FALSE(e1 == e3);

		Pattern e4(validPatternType, validLeftExpressionType2, {}, true);
		EXPECT_FALSE(e1 == e4);

		Pattern e5(validPatternType, validLeftExpressionType2, {}, false);
		EXPECT_FALSE(e1 == e5);
	}
}