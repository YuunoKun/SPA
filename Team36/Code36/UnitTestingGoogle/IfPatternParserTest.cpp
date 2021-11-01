#include "pch.h"

#include "QueryPatternRelRefParser.h"
#include "Common.h"
#include <iostream>

namespace UnitTesting {
	class IfPatternParserTest : public ::testing::Test {
	protected:
		IfPatternParserTest() {
		}

		virtual void SetUp() override {
		}

		void TearDown() override {
		}

	};

	TEST(IfPatternParserTest, IFIdentWildCardTest) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

		//Synonym
		//Expecteds
		Synonym synonym;
		synonym.name = "ifs";
		Entity expected_declared_syn = Entity(EntityType::IF, synonym);
		query.addEntity(expected_declared_syn);

		//Entity expected_ent_wildcard = Entity(EntityType::WILD);

		Pattern expected_pat = Pattern(expected_declared_syn, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE));

		//Result
		validator.parseParameterPattern(query, expected_declared_syn, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(IfPatternParserTest, ifSynWildCardTest) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

		//Expected
		Synonym synonym;
		synonym.name = "ifs";
		Entity expected_declared_syn = Entity(EntityType::IF, synonym);
		query.addEntity(expected_declared_syn);

		Synonym synonym2;
		synonym2.name = "var";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_2);

		Pattern expected_pat = Pattern(expected_declared_syn, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE));

		//Result
		validator.parseParameterPattern(query, expected_declared_syn, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(IfPatternParserTest, IfWildCardWildCardTest) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

		//Expected
		Synonym synonym;
		synonym.name = "ifs";
		Entity expected_declared_syn = Entity(EntityType::IF, synonym);
		query.addEntity(expected_declared_syn);

		Pattern expected_pat = Pattern(expected_declared_syn, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE));

		//Result
		validator.parseParameterPattern(query, expected_declared_syn, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}


}