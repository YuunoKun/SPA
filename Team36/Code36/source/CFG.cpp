#include <algorithm>
#include <iostream>
#include <list>
#include <stdexcept>
#include "CFG.h"

CFG::CFG() {
	head = new CFGNode();
	tail = new CFGNode();
	head->setNextMain(tail);
	tail->setPrevMain(head);
}

CFG::~CFG() {
	if (head->getPrevMain() == nullptr && head->getPrevBranch() == nullptr) {
		std::list<CFGNode*> s;
		CFGNode* node;
		s.push_back(head);
		head->setVisited();
		while (!s.empty()) {
			node = s.front();
			if (node->getNextMain() != nullptr) {
				if (node->getNextMain()->getVisited() == false) {
					node->getNextMain()->setVisited();
					s.push_back(node->getNextMain());
				}
				else {
					node->setNextMain(nullptr);
				}
			}
			if (node->getNextBranch() != nullptr) {
				if (node->getNextBranch()->getVisited() == false) {
					node->getNextBranch()->setVisited();
					s.push_back(node->getNextBranch());
				}
				else {
					node->setNextBranch(nullptr);
				}
			}
			s.pop_front();
		}
		delete head;
	}
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

void CFG::add(prog_line new_line) {
	if (isEmptyCFG()) {
		head->insertLine(new_line);
	} else if (new_line == (head->getProgramLines().back() + 1)) {
		head->insertLine(new_line);
	} else if (new_line <= head->getProgramLines().back() || (tail->getProgramLines().size() != 0 && new_line <= tail->getProgramLines().back())) {
		return;
	} else if (new_line > (head->getProgramLines().back() + 1) && tail->getProgramLines().size() == 0) {
		tail->insertLine(new_line);
	} else if (new_line == (tail->getProgramLines().back() + 1)) {
		tail->insertLine(new_line);
	} else {
		CFGNode* new_node = new CFGNode();
		new_node->insertLine(new_line);
		tail->setNextMain(new_node);
		new_node->setPrevMain(tail);
		tail = new_node;
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

void CFG::loop(CFG* cfg, prog_line line_attached) {
	CFGNode* target = contains(head, line_attached);
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
				target->setPrevBranch(cfg->getTail());
			}
		} else {
			for (auto const& line : target->getProgramLines()) {
				if (line != line_attached) {
					split1.push_back(line);
				}
				else {
					split2.push_back(line);
				}
			}
			CFGNode* new_node = new CFGNode();
			new_node->setProgramLines(split2);

			target->setProgramLines(split1);

			new_node->setNextMain(target->getNextMain());
			target->getNextMain()->setPrevMain(new_node);

			target->setNextMain(new_node);
			new_node->setPrevMain(target);

			new_node->setNextBranch(cfg->getHead());
			cfg->getHead()->setPrevBranch(new_node);

			if (cfg->getTail()->getProgramLines().size() != 0) {
				cfg->getTail()->setNextBranch(new_node);
				new_node->setPrevBranch(cfg->getTail());
			} else {
				cfg->getHead()->setNextBranch(new_node);
				new_node->setPrevBranch(cfg->getHead());
			}
		}

	}
}

void CFG::fork(CFG* cfg, prog_line line_start, prog_line line_end) {
	CFGNode* target1 = contains(head, line_start);
	CFGNode* target2 = contains(head, line_end);
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
				target2->setPrevBranch(cfg->getTail());
			}
		}
		else {
			for (auto const& line : target1->getProgramLines()) {
				if (line < line_start) {
					split1.push_back(line);
				}
				else if (line == line_start) {
					split2.push_back(line);
				}
				else {
					split3.push_back(line);
				}
			}
			if (split1.size() != 0) {
				target1->setProgramLines(split1);

				CFGNode* new_node1 = new CFGNode();
				new_node1->setProgramLines(split2);

				CFGNode* new_node2 = new CFGNode();
				new_node2->setProgramLines(split3);


				new_node1->setNextMain(new_node2);
				new_node2->setPrevMain(new_node1);

				new_node2->setNextMain(target1->getNextMain());
				target1->getNextMain()->setPrevMain(new_node2);

				target1->setNextMain(new_node1);
				new_node1->setPrevMain(target1);

				new_node1->setNextBranch(cfg->getHead());
				cfg->getHead()->setPrevBranch(new_node1);

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
				target1->setProgramLines(split2);

				CFGNode* new_node = new CFGNode();
				new_node->setProgramLines(split3);

				new_node->setNextMain(target1->getNextMain());
				target1->getNextMain()->setPrevMain(new_node);

				target1->setNextMain(new_node);
				new_node->setPrevMain(target1);

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
	resetAllVisited();
	return result;
}

void CFG::resetAllVisited() {
	std::list<CFGNode*> s;
	CFGNode* node;
	s.push_back(head);
	head->setVisited();
	while (!s.empty()) {
		node = s.front();
		if (node->getNextMain() != nullptr) {
			if (node->getNextMain()->getVisited() == true) {
				node->getNextMain()->setVisited();
				s.push_back(node->getNextMain());
			}
		}
		if (node->getNextBranch() != nullptr) {
			if (node->getNextBranch()->getVisited() == true) {
				node->getNextBranch()->setVisited();
				s.push_back(node->getNextBranch());
			}
		}
		s.pop_front();
	}
}