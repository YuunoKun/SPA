#include <algorithm>
#include <iostream>
#include <list>
#include <stdexcept>
#include <unordered_map>
#include <stack>
#include <unordered_set>
#include "CFG.h"

CFG::CFG() {
	head = new CFGNode();
	tail = head;
	tail->setTermination();
}

CFG::~CFG() {
	if (isInvalidCFG()) {
		delete tail;
	}
	else {
		delete head;
	}
}

CFGNode* CFG::getHead() {
	return head;
}

void CFG::setHead(CFGNode* node) {
	head = node;
}

CFGNode* CFG::getTail() {
	return tail;
}

LabelledProgLine CFG::getHeadLabelledProgLine() {
	prog_line h = 0;
	prog_line label = 0;
	if (head->getProgramLines().size() > 0) {
		h = head->getProgramLines().front();
	}
	if (head->getLabels().size() > 0) {
		label = head->getLabels().front();
	}
	return {h, label};
}

bool CFG::isEmptyCFG() {
	return head->isTermination();
}

bool CFG::isInvalidCFG() {
	return getTail()->isInvalid();
}

void CFG::add(prog_line new_line) {
	checkValidity();

	if (isEmptyCFG()) {
		CFGNode* new_head = new CFGNode();
		new_head->insertLine(new_line);
		head = new_head;
		head->setNextMain(tail);
		tail->setPrevMain(head);
		head->setPrevMain(nullptr);
	} else if (new_line <= tail->getPrevMain()->getProgramLines().back()) {
		return;
	} else if (new_line == tail->getPrevMain()->getProgramLines().back() + 1) {
		tail->getPrevMain()->insertLine(new_line);
	} else {
		CFGNode* new_node = new CFGNode();
		new_node->insertLine(new_line);
		tail->getPrevMain()->setNextMain(new_node);
		new_node->setPrevMain(tail->getPrevMain());
		new_node->setNextMain(tail);
		tail->setPrevMain(new_node);
	}
}

CFGNode* CFG::findNode(CFGNode* curr, prog_line line) {
	checkValidity();

	auto action = [](CFGNode* node, prog_line to_find, CFGNode** ref) {
		if (node->isVisited() || *ref) {
			return false;
		}
		node->toggleVisited();

		if (node->getProgramLines().size() > 0) {
			std::vector<prog_line> lines = node->getProgramLines();
			if (std::find(lines.begin(), lines.end(), to_find) != lines.end()) {
				*ref = node;
				return false;
			}
		}
		return true;
	};
	CFGNode* result = nullptr;
	CFGNode** ref = &result;
	head->traverse<prog_line, CFGNode**>(action, line, ref);
	resetAllVisited();
	return result;
}

void CFG::loop(CFG* cfg, prog_line line_attached) {
	checkValidity();

	CFGNode* target = makeStandalone(line_attached);
	target->setNextBranch(cfg->getHead());
	cfg->getHead()->setPrevBranch(target);

	for (auto const& previous : cfg->getTail()->getPrev()) {
		previous->setNextMain(target);
		target->setPrevBranch(previous);
	}

	cfg->getTail()->setInvalid();
	cfg->setHead(nullptr);
}

void CFG::fork(CFG* cfg_if, CFG* cfg_else, prog_line line_attached) {
	checkValidity();

	CFGNode* fork_start = makeStandalone(line_attached);
	CFGNode* fork_end = fork_start->getNextMain();

	if (fork_start->getNextMain() && !fork_start->getNextBranch()) {
		fork_start->setNextMain(cfg_if->getHead());
		cfg_if->getHead()->setPrevMain(fork_start);

		fork_start->setNextBranch(cfg_else->getHead());
		cfg_else->getHead()->setPrevBranch(fork_start);

		fork_end->setPrevMain(cfg_if->getTail()->getPrevMain());
		cfg_if->getTail()->getPrevMain()->setNextMain(fork_end);
		cfg_if->getTail()->getPrev().pop_front();

		for (auto const& previous : cfg_if->getTail()->getPrev()) {
			previous->setNextMain(fork_end);
			fork_end->setPrevBranch(previous);
		}
		cfg_if->getTail()->setInvalid();
		cfg_if->setHead(nullptr);

		for (auto const& previous : cfg_else->getTail()->getPrev()) {
			previous->setNextMain(fork_end);
			fork_end->setPrevBranch(previous);
		}
		cfg_else->getTail()->setInvalid();
		cfg_else->setHead(nullptr);
	}
	else {
		throw std::runtime_error("Unable to fork CFG. Target fork start already has a branch.");
	}
}

void CFG::call(CFG* cfg_call, prog_line call_node) {
	checkValidity();

	if (!cfg_call) {
		return;
	}
	
	CFGNode* target = makeStandalone(call_node);
	target->setIsCall();
	target->setNextCall(cfg_call->getHead());

	auto label_nodes = [](CFGNode* self, prog_line from, CFGNode* to) {
		if (self->isVisited()) {
			return false;
		}
		else {
			self->addLabel(from);
			self->toggleVisited();
		}

		if (self->isTermination()) {
			self->setIsReturn();
			self->addNextReturn(from, to);
			return false;
		}
		return true;
	};
	prog_line ph = 0;
	cfg_call->getHead()->traverse<prog_line, CFGNode*>(label_nodes, call_node, target->getNextMain());
	cfg_call->resetAllVisited();
}

std::vector<std::pair<prog_line, prog_line>> CFG::getNexts() {
	checkValidity();

	std::vector<std::pair<prog_line, prog_line>> result;
	auto action = [](CFGNode* node, std::vector<std::pair<prog_line, prog_line>>& res) {
		if (node->isTermination() || node->isVisited()) {
			return false;
		}

		std::vector<prog_line> lines = node->getProgramLines();
		for (size_t i = 0; i < lines.size() - 1; i++) {
			res.push_back({lines[i], lines[i+1]});
		}

		if (node->getNextMain() && !node->getNextMain()->isTermination()) {
			res.push_back({ lines.back(), node->getNextMain()->getProgramLines().front() });
		}
		if (node->getNextBranch() && !node->getNextBranch()->isTermination()) {
			res.push_back({ lines.back(), node->getNextBranch()->getProgramLines().front() });
		}		
		node->toggleVisited();
		return true;
	};
	head->traverse(action, result);

	resetAllVisited();
	return result;
}


std::vector<std::pair<prog_line, prog_line>> CFG::getNextBip() {
	checkValidity();
	
	std::unordered_multimap<prog_line, prog_line> result;
	auto action = [&result](CFGNode* node, auto action, std::stack<prog_line> call_stack) {

		if (node->isVisited()) {
			return;
		}

		node->toggleVisited();

		std::vector<prog_line> lines = node->getProgramLines();
		for (size_t i = 0; i < lines.size() - 1; i++) {
			result.insert({ lines[i], lines[i + 1] });
		}

		if (node->isCall()) {
			auto reset = [](CFGNode* node) {
				if (node->isVisited()) {
					node->toggleVisited();
					return true;
				}
				return false;
			};
			node->getNextCall()->traverse(reset);
			call_stack.push(node->getProgramLines().back());
			result.insert({ node->getProgramLines().back(), node->getNextCall()->getProgramLines().front() });
			action(node->getNextCall(), action, call_stack);
			return;
		}

		if (node->getNextMain() && node->getNextMain()->isTermination()) {
			if (call_stack.empty()) {
				return;
			}

			if (node->getNextBranch()) {
				result.insert({ node->getProgramLines().back(), node->getNextBranch()->getProgramLines().front() });
				action(node->getNextBranch(), action, call_stack);
			}

			prog_line from = call_stack.top();
			call_stack.pop();
			CFGNode* to = node->getNextMain()->getNextReturn()[from];
			if (!to) {
				throw std::runtime_error("Error traversing CFG for NextBip. Invalid call return.");
			}
			while (to->isTermination() && to->isReturn()) {
				from = call_stack.top();
				call_stack.pop();
				to = to->getNextReturn()[from];
				if (!to) {
					throw std::runtime_error("Error traversing CFG for NextBip. Invalid call return.");
				}
			}
			
			result.insert({ node->getProgramLines().back(), to->getProgramLines().front() });

			if (node->getNextMain()->getPrevMain() == node) {
				action(to, action, call_stack);
			}
			return;
		}

		if (node->getNextBranch()) {
			result.insert({ node->getProgramLines().back(), node->getNextBranch()->getProgramLines().front() });
			action(node->getNextBranch(), action, call_stack);
		}
		if (node->getNextMain()) {
			result.insert({ node->getProgramLines().back(), node->getNextMain()->getProgramLines().front() });
			action(node->getNextMain(), action, call_stack);
		}
	};
	action(head, action, std::stack<prog_line>());
	resetAllVisited();

	std::vector<std::pair<prog_line, prog_line>> temp(result.begin(), result.end());
	sort(temp.begin(), temp.end());
	std::vector<std::pair<prog_line, prog_line>> ret;
	for (std::pair<prog_line, prog_line> p : temp) {
		if (ret.empty() || ret.back().first != p.first || ret.back().second != p.second) {
			ret.push_back(p);
		}
	}
	return ret;
}

std::vector<std::pair<LabelledProgLine, LabelledProgLine>> CFG::getNextBipWithLabel() {
	checkValidity();

	std::unordered_multimap<LabelledProgLine, LabelledProgLine> result;
	auto action = [&result](CFGNode* node, auto action, std::stack<prog_line> call_stack) {

		if (node->isVisited()) {
			return;
		}

		node->toggleVisited();

		std::vector<prog_line> lines = node->getProgramLines();
		for (size_t i = 0; i < lines.size() - 1; i++) {
			result.insert({ {lines[i], call_stack.top()}, {lines[i + 1], call_stack.top()} });
		}

		if (node->isCall()) {
			auto reset = [](CFGNode* node) {
				if (node->isVisited()) {
					node->toggleVisited();
					return true;
				}
				return false;
			};
			node->getNextCall()->traverse(reset);
			result.insert({ {node->getProgramLines().back(), call_stack.top()}, {node->getNextCall()->getProgramLines().front(), node->getProgramLines().back()} });
			call_stack.push(node->getProgramLines().back());
			action(node->getNextCall(), action, call_stack);
			return;
		}

		if (node->getNextMain() && node->getNextMain()->isTermination()) {
			if (call_stack.size() == 1) {
				return;
			}

			if (node->getNextBranch()) {
				result.insert({ {node->getProgramLines().back(), call_stack.top()}, {node->getNextBranch()->getProgramLines().front(), call_stack.top()} });
				action(node->getNextBranch(), action, call_stack);
			}
			prog_line from = call_stack.top();
			prog_line label = from;
			call_stack.pop();
			CFGNode* to = node->getNextMain()->getNextReturn()[from];
			if (!to) {
				throw std::runtime_error("Error traversing CFG for NextBip. Invalid call return.");
			}
			while (to->isTermination() && to->isReturn()) {
				from = call_stack.top();
				call_stack.pop();
				to = to->getNextReturn()[from];
				if (!to) {
					throw std::runtime_error("Error traversing CFG for NextBip. Invalid call return.");
				}
			}
			result.insert({ {node->getProgramLines().back(), label}, {to->getProgramLines().front(), call_stack.top()} });

			if (node->getNextMain()->getPrevMain() == node) {
				action(to, action, call_stack);
			}
			return;
		}

		if (node->getNextBranch()) {
			result.insert({ {node->getProgramLines().back(), call_stack.top()}, {node->getNextBranch()->getProgramLines().front(), call_stack.top()} });
			action(node->getNextBranch(), action, call_stack);
		}
		if (node->getNextMain()) {
			result.insert({ {node->getProgramLines().back(), call_stack.top()}, {node->getNextMain()->getProgramLines().front(), call_stack.top()} });
			action(node->getNextMain(), action, call_stack);
		}
	};

	std::stack<prog_line> init_stk;
	init_stk.push(0);
	action(head, action, init_stk);
	resetAllVisited();
	return std::vector<std::pair<LabelledProgLine, LabelledProgLine>>(result.begin(), result.end());
}

void CFG::resetAllVisited() {
	if (head) {
		auto action = [](CFGNode* node) {
			if (node->isVisited()) {
				node->toggleVisited();
				return true;
			}
			else {
				return false;
			}
			
		};
		head->traverse(action);
	}
}

CFGNode* CFG::makeStandalone(prog_line target_line) {
	CFGNode* target_node = findNode(head, target_line);
	if (!target_node) {
		throw std::runtime_error("Unable to perform action on CFG. Target node not found.");
	}

	if (target_node->getProgramLines().size() == 1) {
		return target_node;
	}
	else if (target_node->getProgramLines().size() > 1) {
		std::vector<prog_line> to_split = target_node->getProgramLines();
		std::vector<prog_line>::iterator target_it = find(to_split.begin(), to_split.end(), target_line);

		CFGNode* curr = target_node;
		CFGNode* start = curr;
		CFGNode* next_main = curr->getNextMain();
		CFGNode* next_branch = curr->getNextBranch();
		
		if (target_it > to_split.begin()) {
			curr->setProgramLines(std::vector<prog_line>(to_split.begin(), target_it));
			CFGNode* new_node = new CFGNode();
			curr->setNextMain(new_node);
			curr->setNextBranch(nullptr);
			new_node->setPrevMain(curr);
			curr = curr->getNextMain();
		}
		
		curr->setProgramLines({*target_it});
		CFGNode* res = curr;

		if (target_it + 1 < to_split.end()) {
			CFGNode* new_node = new CFGNode();
			curr->setNextMain(new_node);
			curr->setNextBranch(nullptr);
			new_node->setPrevMain(curr);
			curr = curr->getNextMain();
			curr->setProgramLines(std::vector<prog_line>(target_it + 1, to_split.end()));
		}
		
		// connect bottom
		if (next_main) {
			curr->setNextMain(next_main);
			*find(next_main->getPrev().begin(), next_main->getPrev().end(), start) = curr;
		}
		if (next_branch) {
			curr->setNextBranch(next_branch);
			*find(next_branch->getPrev().begin(), next_branch->getPrev().end(), start) = curr;
		}
		
		return res;
	}
	else {
		throw std::runtime_error("Unable to perform action on CFG. Cannot split empty node.");
	}
}

void CFG::checkValidity() {
	if (isInvalidCFG()) {
		throw std::runtime_error("Invalid CFG.");
	}
}