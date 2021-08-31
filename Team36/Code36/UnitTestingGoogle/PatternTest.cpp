#include "pch.h"

#include "Pattern.h"
#include "Common.h"

namespace UnitTesting {
	TEST(Pattern, pattern) {
		try {
			// Negative Test Cases
			Entity invalidPatternType1(IF, "invalidPatternType");
			Entity invalidPatternType2(STMT, "invalidPatternType");
			Entity invalidPatternType3(WHILE, "invalidPatternType");
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
		Entity patternType(ASSIGN, "patternType");
		Entity leftExpr(VARIABLE, "leftExpr");
		Pattern pattern(patternType, leftExpr, {}, true);
		EXPECT_EQ(pattern.getLeftExpression(), leftExpr);
	}
	TEST(Pattern, getPatternType) {
		Entity testEntity(ASSIGN, "test");
		Pattern pattern(testEntity, testEntity, {}, true);
		EXPECT_EQ(pattern.getPatternType(), testEntity);
	}

	//TODO: test case to be update with expression parser
	TEST(Pattern, getExpression) {
		//entity testentity(stmt, "test");
		//tnode tnode;
		//pattern pattern(testentity, tnode, true);
		//expct_eq(pattern.getexpression(), tnode);
	}

	TEST(Pattern, isWild) {
		Entity testEntity(ASSIGN, "test");

		Pattern pattern1(testEntity, testEntity, {}, true);
		EXPECT_TRUE(pattern1.isWild());

		Pattern pattern2(testEntity, testEntity, {}, false);
		EXPECT_FALSE(pattern2.isWild());
	}

	TEST(Pattern, equal) {
		Entity testEntity(ASSIGN, "test");

		Pattern e1(testEntity, testEntity, {}, true);
		Pattern e2(testEntity, testEntity, {}, true);
		EXPECT_EQ(e1, e2);

		Pattern e3(testEntity, testEntity, {}, false);
		EXPECT_FALSE(e1 == e3);

		Entity testEntity1(ASSIGN, "test1");
		Pattern e4(testEntity1, testEntity, {}, true);
		EXPECT_FALSE(e1 == e4);

		Entity testEntity2(ASSIGN, "test1");
		Pattern e5(testEntity2, testEntity, {}, true);
		EXPECT_FALSE(e1 == e5);
	}
}