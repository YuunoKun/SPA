#include "pch.h"

#include "QueryPreprocessor.cpp"
#include "QueryPreprocessor.h"
#include "Common.h"
#include <iostream>



namespace UnitTesting {
	class QueryPatternSuchThatValidationTest : public ::testing::Test {
	protected:
		QueryPatternSuchThatValidationTest() {
		}

		// If the constructor and destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		virtual void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}

		// Class members declared here can be used by all tests in the test suite
		// for Foo.
	};

	// Test setStmtRef
	// 3 diff params 
	TEST(QueryPreprocessor, setStmtRefTest) {
		Query query;
	/*	Entity declared_stmt = Entity(EntityType::STMT, "s");*/

		//WILDCARD
		//Result
		QueryToken wildcard_token = QueryToken(QueryToken::WILDCARD, "_");
		Entity ent_wildacrd = setStmtRef(query, wildcard_token);

		//Expected
		Entity expected_ent_wildcard = Entity(EntityType::WILD);

		EXPECT_TRUE(ent_wildacrd == expected_ent_wildcard);

		//CONSTANT
		//Result
		//QueryToken constant_token = QueryToken(QueryToken::CONSTANT, "9");
		//Entity ent_constant = setStmtRef(query, constant_token);

		////Expected
		//Entity expected_ent_constant = Entity(QueryToken::CONSTANT, "9");

		//EXPECT_TRUE(ent_constant == expected_ent_constant);
	}



	// Test setEntRef

	// Test setExpr

	// Test parseParameterSuchThat

	// Test parseParameterPattern



}