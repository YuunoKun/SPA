#include "QueryPatternRelRefParser.h"

QueryPatternRelRefParser::QueryPatternRelRefParser() {}

void QueryPatternRelRefParser::parseParameterSuchThat(
    Query& query, QueryToken::QueryTokenType token_type,
    std::vector<QueryToken> token_chain) {
    switch (token_type) {
    case QueryToken::MODIFIES_S: {
        ModifiesParser modifies_Parser;
        modifies_Parser.parse(query, token_chain, token_type);
        break;
    }
    case QueryToken::USES_S: {
        UsesParser uses_parser;
        uses_parser.parse(query, token_chain, token_type);
        break;
    }
    case QueryToken::PARENT: 
    case QueryToken::PARENT_T: {
        ParentParser parent_parser;
        parent_parser.parse(query, token_chain, token_type);
        break;
    }
    case QueryToken::FOLLOWS: 
    case QueryToken::FOLLOWS_T: {
        FollowsParser follows_parser;
        follows_parser.parse(query, token_chain, token_type);
        break;
    }
    case QueryToken::CALLS: 
    case QueryToken::CALLS_T: {
        CallsParser calls_parser;
        calls_parser.parse(query, token_chain, token_type);
        break;
    }
    case QueryToken::NEXT: 
    case QueryToken::NEXT_T: 
    case QueryToken::NEXT_BIP: 
    case QueryToken::NEXT_BIP_T: {
        NextParser next_parser;
        next_parser.parse(query, token_chain, token_type);
        break;
    }
    case QueryToken::AFFECTS: 
    case QueryToken::AFFECTS_T: 
    case QueryToken::AFFECTS_BIP: 
    case QueryToken::AFFECTS_BIP_T: {
        AffectsParser affects_parser;
        affects_parser.parse(query, token_chain, token_type);
        break;
    }
    default:
        throw SyntacticErrorException("Unknown RelRef query token type : \'" + token_type +
                            '\'');
    }
}

void QueryPatternRelRefParser::parseParameterPattern(
    Query& query, Entity& synonym_ent, std::vector
    <QueryToken> token_chain) {
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

void QueryPatternRelRefParser::parseWith(Query& query, std::vector
    <QueryToken> token_chain) {
    WithParser parser;
    parser.parse(query, token_chain);
}