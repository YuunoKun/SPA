#include "AssignPatternParser.h"
#include "QueryPatternRelRefParser.h"


AssignPatternParser::AssignPatternParser() {}

bool AssignPatternParser::isWild(std::vector<QueryToken> token_chain) {
    return token_chain[0].type == QueryToken::WILDCARD;
}

void AssignPatternParser::parseAssign(Query& query, Entity& entity, std::vector<QueryToken> token_chain) {
    QueryPatternRelRefParser parser;
    std::vector<QueryToken> temp_token_chain_1;
    std::vector<QueryToken> temp_token_chain_2;
    bool comma_count = 0;
    bool is_wild = false;
    size_t token_chain_size = token_chain.size();
    for (size_t i = 0; i < token_chain_size; i++) {
        if (token_chain[0].type == QueryToken::COMMA) {
            token_chain.erase(token_chain.begin());
            comma_count += 1;
        } else if (comma_count == 0) {
            // 1st param
            temp_token_chain_1.push_back(token_chain[0]);
            token_chain.erase(token_chain.begin());
        } else if (comma_count == 1){
            // 2nd param
            temp_token_chain_2.push_back(token_chain[0]);
            token_chain.erase(token_chain.begin());
        } else {
            throw SyntacticErrorException("Invalid parameters for assign pattern");
        }
    }

    if (!parser.isEntRef(query, temp_token_chain_1) || !parser.isExpr(temp_token_chain_2)) {
        throw SemanticErrorException("Invalid parameters for assign pattern");
    }

    if (!parser.isCorrectSynEntRef(query, temp_token_chain_1, EntityType::VARIABLE)) {
        throw SemanticErrorException("Invalid Left expression for pattern");
    }

    is_wild = isWild(temp_token_chain_2);

    query.addPattern(Pattern(entity, parser.setEntRef(query, temp_token_chain_1),
        parser.setExpr(temp_token_chain_2), is_wild));
}
