#include "QueryPatternRelRefParser.h"

QueryPatternRelRefParser::QueryPatternRelRefParser() {}

void QueryPatternRelRefParser::parseParameterSuchThat(
    Query& query, QueryToken::QueryTokenType token_type,
    std::vector<QueryToken> token_chain) {
    switch (token_type) {
    case QueryToken::MODIFIES_S: {

        ModifiesParser usesParser;
        usesParser.parse(query, token_chain);
        break;
    }
    case QueryToken::USES_S: {
        UsesParser usesParser;
        usesParser.parse(query, token_chain);
        break;
    }
    case QueryToken::PARENT: {
        ParentParser parentParser;
        parentParser.parse(query, token_chain);
        break;
    }
    case QueryToken::PARENT_T: {
        ParentTParser parentTParser;
        parentTParser.parse(query, token_chain);
        break;
    }
    case QueryToken::FOLLOWS: {
        FollowsParser followsParser;
        followsParser.parse(query, token_chain);
        break;
    }
    case QueryToken::FOLLOWS_T: {
        FollowsTParser followsTParser;
        followsTParser.parse(query, token_chain);
        break;
    }
    case QueryToken::CALLS: {
        CallsParser callsParser;
        callsParser.parse(query, token_chain);
        break;
    }
    case QueryToken::CALLS_T: {
        CallsTParser callsTParser;
        callsTParser.parse(query, token_chain);
        break;
    }
    case QueryToken::AFFECTS: {
        // stmtRef , stmtRef

        break;
    }
    case QueryToken::AFFECTS_T: {
        // stmtRef , stmtRef

        break;
    }
    case QueryToken::NEXT: {
        NextParser nextParser;
        nextParser.parse(query, token_chain);
        break;
    }
    case QueryToken::NEXT_T: {
        NextTParser nextTParser;
        nextTParser.parse(query, token_chain);
        break;
    }
    default:
        throw SyntacticErrorException("Unknown RelRef query token type : \'" + token_type +
                            '\'');
    }
}

void QueryPatternRelRefParser::parseParameterPattern(
    Query& query, Entity& synonym_ent, std::vector<QueryToken> token_chain) {
    bool wild = false;
    switch (synonym_ent.getType()) {
    case ASSIGN: {
        AssignPatternParser parser;
        parser.parse(query, synonym_ent, token_chain);
        break;
        }
    case IF: {
        IfPatternParser parser;
        parser.parse(query, synonym_ent, token_chain);
        break;
    }
    case WHILE: {
        WhilePatternParser parser;
        parser.parse(query, synonym_ent, token_chain);
        break;
    }
    }

}