#pragma once

#include <iostream>
#include <vector>
#include <list>

typedef unsigned int prog_line;
using namespace std;

class CFGNode {
public:
	CFGNode();

	~CFGNode();

	void setProgramLines(vector<prog_line>);
	void setNextMain(CFGNode*);
	void setNextBranch(CFGNode*);
	void setPrevMain(CFGNode*);
	void setPrevBranch(CFGNode*);
	void setVisited();
	void setTermination();
	void setInvalid();

	void insertLine(prog_line);

	vector<prog_line> getProgramLines();
	CFGNode* getNextMain();
	CFGNode* getNextBranch();
	CFGNode* getPrevMain();
	CFGNode* getPrevBranch();
	list<CFGNode*> getPrev();
	bool getVisited();
	bool getTermination();
	bool getInvalid();

private:
	std::vector<prog_line> program_lines;
	bool visited;
	bool termination;
	bool invalid;
	CFGNode* next_main{nullptr};
	CFGNode* next_branch{nullptr};
	std::list<CFGNode*> prev;
};