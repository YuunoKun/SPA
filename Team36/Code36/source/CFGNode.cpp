#include <algorithm>
#include "CFGNode.h"

CFGNode::CFGNode() {
	visited = false;
}

CFGNode::CFGNode(vector<prog_line> lines) {
	program_lines = lines;
	visited = false;
}

CFGNode::~CFGNode() {
	delete next_main;
	delete next_branch;
}

void CFGNode::setProgramLines(vector<prog_line> lines) {
	program_lines = lines;
}

void CFGNode::setNextMain(CFGNode* nextMain) {
	next_main = nextMain;
}

void CFGNode::setNextBranch(CFGNode* nextBranch) {
	next_branch = nextBranch;
}

void CFGNode::setVisited() {
	visited = true;
}

void CFGNode::insertLine(prog_line programLine) {
	if (std::find(program_lines.begin(), program_lines.end(), programLine) == program_lines.end()) {
		program_lines.push_back(programLine);
	}
}

vector<prog_line> CFGNode::getProgramLines() {
	return program_lines;
}

CFGNode* CFGNode::getNextMain() {
	return next_main;
}

CFGNode* CFGNode::getNextBranch() {
	return next_branch;
}

bool CFGNode::getVisited() {
	return visited;
}