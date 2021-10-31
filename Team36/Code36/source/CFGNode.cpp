#include <algorithm>
#include "CFGNode.h"

CFGNode::CFGNode() {
	visited = false;
	termination = false;
	invalid = false;
	is_call = false;
	is_return = false;
}

CFGNode::~CFGNode() {
	delete next_main;
	delete next_branch;
}

void CFGNode::setProgramLines(std::vector<prog_line> lines) {
	program_lines = lines;
}

void CFGNode::setNextMain(CFGNode* next_m) {
	next_main = next_m;
}

void CFGNode::setNextBranch(CFGNode* next_m) {
	next_branch = next_m;
}

void CFGNode::setNextCall(CFGNode* node) {
	next_call = node;
}

void CFGNode::setPrevMain(CFGNode* prev_m) {
	if (prev.size() == 0) {
		prev.push_front(prev_m);
	} else {
		prev.pop_front();
		prev.push_front(prev_m);
	}
}

void CFGNode::setPrevBranch(CFGNode* prev_b) {
	prev.push_back(prev_b);
}

void CFGNode::setVisited() {
	visited = !visited;
}

void CFGNode::setTermination() {
	termination = true;
}

void CFGNode::setInvalid() {
	invalid = true;
	prev.clear();
	next_main = nullptr;
	next_branch = nullptr;
}

void CFGNode::setIsCall() {
	is_call = true;
}

void CFGNode::setIsReturn() {
	is_return = true;
}

void CFGNode::insertLine(prog_line programLine) {
	if (std::find(program_lines.begin(), program_lines.end(), programLine) == program_lines.end()) {
		program_lines.push_back(programLine);
	}
}

void CFGNode::addLabel(prog_line new_label) {
	labels.push_back(new_label);
}

void CFGNode::addNextReturn(prog_line from, CFGNode* to) {
	next_return.insert({from, to});
}

std::vector<prog_line> CFGNode::getProgramLines() {
	return program_lines;
}

std::vector<prog_line> CFGNode::getLabels() {
	return labels;
}

std::unordered_map<prog_line, CFGNode*> CFGNode::getNextReturn() {
	return next_return;
}

CFGNode* CFGNode::getNextMain() {
	return next_main;
}

CFGNode* CFGNode::getNextBranch() {
	return next_branch;
}

CFGNode* CFGNode::getNextCall() {
	return next_call;
}

CFGNode* CFGNode::getPrevMain() {
	return prev.front();
}

CFGNode* CFGNode::getPrevBranch() {
	return prev.back();
}

std::list<CFGNode*>& CFGNode::getPrev() {
	return prev;
}

bool CFGNode::getVisited() {
	return visited;
}

bool CFGNode::getTermination() {
	return termination;
}

bool CFGNode::getInvalid() {
	return invalid;
}

bool CFGNode::IsCall() {
	return is_call;
}

bool CFGNode::IsReturn() {
	return is_return;
}

void CFGNode::traverse(bool(*action)(CFGNode*)) {
	if (action(this)) {
		if (next_main) {
			next_main->traverse(action);
		}
		if (next_branch) {
			next_branch->traverse(action);
		}
	}
}

void CFGNode::traverse(bool(*action)(CFGNode*, std::vector<std::pair<prog_line, prog_line>>&),
	std::vector<std::pair<prog_line, prog_line>>& v) {
	if (action(this, v)) {
		if (next_main) {
			next_main->traverse(action, v);
		}
		if (next_branch) {
			next_branch->traverse(action, v);
		}
	}
}

template<class T, class U>
void CFGNode::traverse(bool(*action)(CFGNode*, T, U), T t, U u) {
	if (action(this, t, u)) {
		if (next_main) {
			next_main->traverse<T, U>(action, t, u);
		}
		if (next_branch) {
			next_branch->traverse<T, U>(action, t, u);
		}
	}
}