#pragma once
#include "CFGNode.h"

using namespace std;

class CFG {
public:
	CFG();

	~CFG();

	bool isEmptyCFG();
	bool isInvalidCFG();

	CFGNode* getHead();
	CFGNode* getTail();
	vector<pair<prog_line, prog_line>> getNexts();
	void resetAllVisited();
	CFGNode* contains(CFGNode*, prog_line);
	void add(prog_line);
	void loop(CFG*, prog_line);
	void fork(CFG*, CFG*, prog_line);

private:
	CFGNode* head;
	CFGNode* tail;
};