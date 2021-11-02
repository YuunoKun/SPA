#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include "Common.h"

class CFGNode {
public:
	CFGNode();

	~CFGNode();

	void setProgramLines(std::vector<prog_line>);
	void setNextMain(CFGNode*);
	void setNextBranch(CFGNode*);
	void setNextCall(CFGNode*);
	void setPrevMain(CFGNode*);
	void setPrevBranch(CFGNode*);
	void toggleVisited();
	void setTermination();
	void setInvalid();
	void setIsCall();
	void setIsReturn();

	void insertLine(prog_line);
	void addLabel(prog_line);
	void addNextReturn(prog_line, CFGNode*);

	std::vector<prog_line> getProgramLines();
	std::vector<prog_line> getLabels();
	std::unordered_map<prog_line, CFGNode*> getNextReturn();
	CFGNode* getNextMain();
	CFGNode* getNextBranch();
	CFGNode* getNextCall();
	CFGNode* getPrevMain();
	CFGNode* getPrevBranch();
	std::list<CFGNode*>& getPrev();
	bool isVisited();
	bool isTermination();
	bool isInvalid();
	bool isCall();
	bool isReturn();

	template <class T, class U>
	void traverse(bool(*action)(CFGNode*, T, U), T, U);
	void traverse(bool(*action)(CFGNode*));
	void traverse(bool(*action)(CFGNode*, std::vector<std::pair<prog_line, prog_line>>&),
		std::vector<std::pair<prog_line, prog_line>>&);

private:
	std::vector<prog_line> program_lines;
	std::vector<prog_line> labels;
	CFGNode* next_main{nullptr};
	CFGNode* next_branch{nullptr};
	CFGNode* next_call{nullptr};
	std::unordered_map<prog_line, CFGNode*> next_return;
	std::list<CFGNode*> prev;
	bool is_visited;
	bool is_termination;
	bool is_invalid;
	bool is_call;
	bool is_return;
};


template <class T, class U>
void CFGNode::traverse(bool(*action)(CFGNode*, T, U), T t, U u) {
	if (action(this, t, u)) {
		if (next_main) {
			next_main->traverse<T, U>(action, t, u);
		}
		if (next_branch) {
			next_branch->traverse<T, U>(action, t, u);
		}
	}
};