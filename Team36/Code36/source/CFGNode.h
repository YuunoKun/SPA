#pragma once

#include <iostream>
#include <vector>

typedef unsigned int prog_line;
using namespace std;

class CFGNode {
public:
	CFGNode();

	CFGNode(vector<prog_line>);

	~CFGNode();

	void setProgramLines(vector<prog_line>);
	void setNextMain(CFGNode*);
	void setNextBranch(CFGNode*);
	void setPrevMain(CFGNode*);
	void setPrevBranch(CFGNode*);
	void setVisited();

	void insertLine(prog_line);

	vector<prog_line> getProgramLines();
	CFGNode* getNextMain();
	CFGNode* getNextBranch();
	CFGNode* getPrevMain();
	CFGNode* getPrevBranch();
	bool getVisited();

private:
	std::vector<prog_line> program_lines;
	bool visited;
	CFGNode* next_main{nullptr};
	CFGNode* next_branch{nullptr};
	CFGNode* prev_main{nullptr};
	CFGNode* prev_branch{nullptr};
};