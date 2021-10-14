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

void CFGNode::setNextMain(CFGNode* next_m) {
	next_main = next_m;
}

void CFGNode::setNextBranch(CFGNode* next_m) {
	next_branch = next_m;
}

void CFGNode::setPrevMain(CFGNode* prev_m) {
	prev_main = prev_m;
}

void CFGNode::setPrevBranch(CFGNode* prev_b) {
	prev_branch = prev_b;
}

void CFGNode::setVisited() {
	if (visited == false) {
		visited = true;
	}else {
		visited = false;
	}
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

CFGNode* CFGNode::getPrevMain() {
	return prev_main;
}

CFGNode* CFGNode::getPrevBranch() {
	return prev_branch;
}

bool CFGNode::getVisited() {
	return visited;
}