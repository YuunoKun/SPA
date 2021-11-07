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
	
	void addLine(prog_line);
	void addLoop(CFG*, prog_line);
	void addFork(CFG*, CFG*, prog_line);
	void addCall(CFG*, prog_line);

	CFGNode* findNode(CFGNode*, prog_line);
	CFGNode* makeStandalone(prog_line);

private:
	CFGNode* head;
	CFGNode* tail;

	void resetAllVisited();
	void checkValidity();
};