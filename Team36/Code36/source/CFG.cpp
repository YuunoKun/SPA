#include <algorithm>
#include <iostream>
#include <list>
#include "CFG.h"

CFG::CFG() {
	head = new CFGNode();
	tail = new CFGNode();
	head->setNextMain(tail);
	tail->setPrevMain(head);
}

CFGNode* CFG::getHead() {
	return head;
}

CFGNode* CFG::getTail() {
	return tail;
}

bool CFG::isEmptyCFG() {
	return head->getProgramLines().size() == 0;
}

void CFG::add(prog_line newLine) {
	if (isEmptyCFG()) {
		head->insertLine(newLine);
	} else if (newLine == (head->getProgramLines().back() + 1)) {
		head->insertLine(newLine);
	} else if (newLine <= head->getProgramLines().back() || (tail->getProgramLines().size() != 0 && newLine <= tail->getProgramLines().back())) {
		return;
	} else if (newLine > (head->getProgramLines().back() + 1) && tail->getProgramLines().size() == 0) {
		tail->insertLine(newLine);
	} else if (newLine == (tail->getProgramLines().back() + 1)) {
		tail->insertLine(newLine);
	} else {
		CFGNode* newNode = new CFGNode();
		newNode->insertLine(newLine);
		tail->setNextMain(newNode);
		newNode->setPrevMain(tail);
		tail = newNode;
	}
}

CFGNode* CFG::contains(CFGNode* curr, prog_line line) {
	CFGNode* result = nullptr;

	if (curr != nullptr) {
		std::vector<prog_line> curr_lines = curr->getProgramLines();
		for (auto const& l : curr_lines) {
			if (l == line) {
				result = curr;
				break;
			}
		}
		if (result != nullptr) {
			return result;
		}
		else {
			result = contains(curr->getNextMain(), line);
			if (result == nullptr) {
				result = contains(curr->getNextBranch(), line);
			}
			return result;
		}
	} else {
		return result;
	}
}

void CFG::loop(CFG* cfg, prog_line lineAttached) {
	CFGNode* target = contains(head, lineAttached);
	std::vector<prog_line> split1;
	std::vector<prog_line> split2;
	if (target != nullptr) {
		if (target->getProgramLines().size() == 1) {
			target->setNextBranch(cfg->getHead());
			cfg->getHead()->setPrevBranch(target);

			if (cfg->getTail()->getProgramLines().size() != 0 ) {
				cfg->getTail()->setNextBranch(target);
				target->setPrevBranch(cfg->getTail());
			} else {
				cfg->getHead()->setNextBranch(target);
				target->setPrevBranch(cfg->getHead());
			}
		} else {
			for (auto const& line : target->getProgramLines()) {
				if (line != lineAttached) {
					split1.push_back(line);
				}
				else {
					split2.push_back(line);
				}
			}
			CFGNode* newNode = new CFGNode();
			newNode->setProgramLines(split2);

			target->getProgramLines().clear();
			target->setProgramLines(split1);

			newNode->setNextMain(target->getNextMain());
			target->getNextMain()->setPrevMain(newNode);

			newNode->setPrevMain(target);
			target->setNextMain(newNode);

			newNode->setNextBranch(cfg->getHead());
			cfg->getHead()->setPrevBranch(newNode);

			if (cfg->getTail()->getProgramLines().size() != 0) {
				cfg->getTail()->setNextBranch(newNode);
				newNode->setPrevBranch(cfg->getTail());
			} else {
				cfg->getHead()->setNextBranch(newNode);
				newNode->setPrevBranch(cfg->getHead());
			}
		}

	}
}

void CFG::fork(CFG* cfg, prog_line lineStart, prog_line lineEnd) {
	CFGNode* target1 = contains(head, lineStart);
	CFGNode* target2 = contains(head, lineEnd);
	std::vector<prog_line> split1;
	std::vector<prog_line> split2; 
	std::vector<prog_line> split3;
	if (target1 != nullptr) {
		if (target1->getProgramLines().size() == 1) {
			target1->setNextBranch(cfg->getHead());
			cfg->getHead()->setPrevBranch(target1);

			if (cfg->getTail()->getProgramLines().size() != 0) {
				cfg->getTail()->setNextBranch(target2);
				target2->setPrevBranch(cfg->getTail());
			}
			else {
				cfg->getHead()->setNextBranch(target2);
				target2->setPrevBranch(cfg->getHead());
			}
		}
		else {
			for (auto const& line : target1->getProgramLines()) {
				if (line < lineStart) {
					split1.push_back(line);
				}
				else if (line == lineStart) {
					split2.push_back(line);
				}
				else {
					split3.push_back(line);
				}
			}
			target1->getProgramLines().clear();
			if (split1.size() != 0) {
				target1->setProgramLines(split1);

				CFGNode* newNode1 = new CFGNode();
				newNode1->setProgramLines(split2);

				CFGNode* newNode2 = new CFGNode();
				newNode2->setProgramLines(split3);


				newNode1->setNextMain(newNode2);
				newNode2->setPrevMain(newNode1);

				newNode2->setNextMain(target1->getNextMain());
				target1->getNextMain()->setPrevMain(newNode2);

				target1->setNextMain(newNode1);
				newNode1->setPrevMain(target1);

				newNode1->setNextBranch(cfg->getHead());
				cfg->getHead()->setPrevBranch(newNode1);
			}
			else {
				target1->setProgramLines(split2);

				CFGNode* newNode = new CFGNode();
				newNode->setProgramLines(split3);

				newNode->setNextMain(target1->getNextMain());
				target1->getNextMain()->setPrevMain(newNode);

				target1->setNextMain(newNode);
				newNode->setPrevMain(target1);

				target1->setNextBranch(cfg->getHead());
				cfg->getHead()->setPrevBranch(target1);
			}

			if (cfg->getTail()->getProgramLines().size() != 0) {
				cfg->getTail()->setNextBranch(target2);
				target2->setPrevBranch(cfg->getTail());
			}
			else {
				cfg->getHead()->setNextBranch(target2);
				target2->setPrevBranch(cfg->getHead());
			}
		}
	}
}

vector<pair<prog_line, prog_line>> CFG::getNexts() {
	std::vector<std::pair<prog_line, prog_line>> result;
	CFGNode* node;
	std::list<CFGNode*> s;
	s.push_back(getHead());
	getHead()->setVisited();
	while (!s.empty()) {
		node = s.front();
		if (node->getProgramLines().size() > 1) {
			for (int i = 0; i < (node->getProgramLines().size() - 1); i++) {
				result.push_back({ node->getProgramLines()[i], node->getProgramLines()[i + 1] });
			}
		}
		if (node->getNextMain() != nullptr && node->getNextMain()->getProgramLines().size() > 0) {
			result.push_back({ node->getProgramLines().back(), node->getNextMain()->getProgramLines().front() });
			if (node->getNextMain()->getVisited() == false) {
				s.push_back(node->getNextMain());
				node->getNextMain()->setVisited();
			}
		}
		if (node->getNextBranch() != nullptr && node->getNextBranch()->getProgramLines().size() > 0) {
			result.push_back({ node->getProgramLines().back(), node->getNextBranch()->getProgramLines().front() });
			if (node->getNextBranch()->getVisited() == false) {
				s.push_back(node->getNextBranch());
				node->getNextBranch()->setVisited();
			}
		}
		s.pop_front();
	}
	return result;
}
