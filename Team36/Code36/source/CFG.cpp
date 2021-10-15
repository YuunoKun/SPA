#include <algorithm>
#include <iostream>
#include <list>
#include <stdexcept>
#include "CFG.h"

CFG::CFG() {
	head = new CFGNode();
	tail = head;
	tail->setTermination();
}

CFG::~CFG() {
	if (isInvalidCFG() == false) {
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
	else {
		delete tail;
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

bool CFG::isInvalidCFG() {
	return getTail()->getInvalid();
}

void CFG::add(prog_line new_line) {
	if (isInvalidCFG() == true) {
		throw std::runtime_error("Unable to add program line because CFG is invalid");
	}
	if (isEmptyCFG()) {
		CFGNode* new_head = new CFGNode();
		new_head->insertLine(new_line);
		head = new_head;
		head->setNextMain(tail);
		tail->setPrevMain(head);
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

CFGNode* CFG::contains(CFGNode* curr, prog_line line) {
	CFGNode* result = nullptr;
	if (isInvalidCFG() == true) {
		throw std::runtime_error("Unable to search CFG because CFG is invalid");
	}
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
	if (isInvalidCFG() == true) {
		throw std::runtime_error("Unable to loop CFG because CFG is invalid");
	}

	CFGNode* target = contains(head, line_attached);
	std::vector<prog_line> split1;
	std::vector<prog_line> split2;
	if (target != nullptr) {
		if (target->getProgramLines().size() > 1) {
			split2.push_back(target->getProgramLines().back());
			split1 = target->getProgramLines();
			split1.pop_back();
			CFGNode* new_node = new CFGNode();
			new_node->setProgramLines(split1);

			target->setProgramLines(split2);

			if (target->getPrev().size() == 0) {
				head = new_node;
				head->setNextMain(target);
				target->setPrevMain(head);
			} else {
				target->getPrevMain()->setNextMain(new_node);
				new_node->setPrevMain(target->getPrevMain());

				new_node->setNextMain(target);
				target->setPrevMain(new_node);
			}
			
		}
		target->setNextBranch(cfg->getHead());
		cfg->getHead()->setPrevBranch(target);

		for (auto const& previous : cfg->getTail()->getPrev()) {
			previous->setNextMain(target);
			target->setPrevBranch(previous);
		}

		cfg->getTail()->setInvalid();

	}
}

void CFG::fork(CFG* cfg_if, CFG* cfg_else, prog_line line_attached) {
	if (isInvalidCFG() == true) {
		throw std::runtime_error("Unable to fork CFG because CFG is invalid");
	}
	CFGNode* target1 = contains(head, line_attached);
	CFGNode* target2 = target1->getNextMain();
	std::vector<prog_line> split1;
	std::vector<prog_line> split2;
	std::vector<prog_line> split3;
	std::vector<prog_line> split4;
	if (target1 != nullptr) {
		if (target1->getProgramLines().size() > 1) {
			split2.push_back(target1->getProgramLines().back());
			split1 = target1->getProgramLines();
			split1.pop_back();
			
			CFGNode* new_node1 = new CFGNode();
			new_node1->setProgramLines(split1);

			target1->setProgramLines(split2);

			if (target1->getPrev().size() == 0) {
				head = new_node1;
				head->setNextMain(target1);
				target1->setPrevMain(head);
			}
			else {
				target1->getPrevMain()->setNextMain(new_node1);
				new_node1->setPrevMain(target1->getPrevMain());

				new_node1->setNextMain(target1);
				target1->setPrevMain(new_node1);
			}
		}
		if (target2->getProgramLines().size() > 1) {
			split3.push_back(target2->getProgramLines().front());
			split4 = target2->getProgramLines();
			split4.erase(split4.begin());

			CFGNode* new_node2 = new CFGNode();
			new_node2->setProgramLines(split4);

			target2->setProgramLines(split3);

			new_node2->setNextMain(target2->getNextMain());
			target2->getNextMain()->setPrevMain(new_node2);

			target2->setNextMain(new_node2);
			new_node2->setPrevMain(target2);
		}
		if (target1->getNextBranch() == nullptr) {
			target1->setNextMain(cfg_if->getHead());
			cfg_if->getHead()->setPrevMain(target1);

			target1->setNextBranch(cfg_else->getHead());
			cfg_else->getHead()->setPrevBranch(target1);

			target2->setPrevMain(cfg_if->getTail()->getPrevMain());
			cfg_if->getTail()->getPrevMain()->setNextMain(target2);
			cfg_if->getTail()->getPrev().pop_front();

			for (auto const& previous : cfg_if->getTail()->getPrev()) {
				previous->setNextMain(target2);
				target2->setPrevBranch(previous);
			}
			cfg_if->getTail()->setInvalid();

			for (auto const& previous : cfg_else->getTail()->getPrev()) {
				previous->setNextMain(target2);
				target2->setPrevBranch(previous);
			}
			cfg_else->getTail()->setInvalid();
		}
		else {
			throw std::runtime_error("Unable to fork CFG");
		}
	}
	
}

vector<pair<prog_line, prog_line>> CFG::getNexts() {
	if (isInvalidCFG() == true) {
		throw std::runtime_error("Unable to get next line because CFG is invalid");
	}

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