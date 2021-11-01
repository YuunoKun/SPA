#pragma once

#include "Common.h"
#include "CFGNode.h"

class CFG {
public:
	CFG();

	~CFG();

	bool isEmptyCFG();
	bool isInvalidCFG();

	CFGNode* getHead();
	void setHead(CFGNode*);
	CFGNode* getTail();
	LabelledProgLine getHeadLabelledProgLine();
	std::vector<std::pair<prog_line, prog_line>> getNexts();
	std::vector<std::pair<prog_line, prog_line>> getNextBip();
	std::vector<std::pair<LabelledProgLine, LabelledProgLine>> getNextBipWithLabel();
	void resetAllVisited();
	CFGNode* contains(CFGNode*, prog_line);
	void add(prog_line);
	void loop(CFG*, prog_line);
	void fork(CFG*, CFG*, prog_line);
	void call(CFG*, prog_line);

	CFGNode* makeStandalone(prog_line);
private:
	CFGNode* head;
	CFGNode* tail;

	void checkValidity();
};