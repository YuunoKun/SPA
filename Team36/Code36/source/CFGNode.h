#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <functional>
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
	void setVisited();
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
	bool getVisited();
	bool getTermination();
	bool getInvalid();
	bool IsCall();
	bool IsReturn();

	void traverse(bool(*action)(CFGNode*));
	void traverse(bool(*action)(CFGNode*, std::vector<std::pair<prog_line, prog_line>>&),
		std::vector<std::pair<prog_line, prog_line>>&);
	template<class T, class U>
	void traverse(bool(*action)(CFGNode*, T, U), T, U);



private:
	std::vector<prog_line> program_lines;
	std::vector<prog_line> labels;
	bool visited;
	bool termination;
	bool invalid;
	bool is_call;
	bool is_return;
	CFGNode* next_main{nullptr};
	CFGNode* next_branch{nullptr};
	CFGNode* next_call{nullptr};
	std::unordered_map<prog_line, CFGNode*> next_return;
	std::list<CFGNode*> prev;
};