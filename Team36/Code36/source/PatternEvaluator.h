#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "Common.h"


class PatternEvaluatorInterface {
public:
    virtual std::vector<std::pair<stmt_index, var_name>> getAllPair() = 0;
    virtual std::vector<stmt_index> getPatternType() = 0;
    virtual std::vector<stmt_index> getPatternType(var_name) = 0;
protected:
    PKBAdapter pkb;
};
class IfsEvaluator : public PatternEvaluatorInterface {
public:
    std::vector<std::pair<stmt_index, var_name>> getAllPair() override {
        return pkb.getAllIfUses();
    }

    std::vector<stmt_index> getPatternType() override {
        return pkb.getIfUses();
    }

    std::vector<stmt_index> getPatternType(var_name var) override {
        return pkb.getIfUses(var);
    }
};
class WhileEvaluator : public PatternEvaluatorInterface {
protected:
    std::vector<std::pair<stmt_index, var_name>> getAllPair() override {
        return pkb.getAllWhileUses();
    }

    std::vector<stmt_index> getPatternType() override {
        return pkb.getWhileUses();
    }

    std::vector<stmt_index> getPatternType(var_name var) override {
        return pkb.getWhileUses(var);
    }
};


class PatternEvaluator {
public:
	void evaluatePattern(QueryResult&, Pattern&);

private:

	void evaluateAssignPattern(QueryResult&, Pattern&);
	void evaluatePattern(QueryResult&, Pattern&, PatternEvaluatorInterface&);

	bool isExpressionWild(Pattern&);

    PKBAdapter pkb;
};