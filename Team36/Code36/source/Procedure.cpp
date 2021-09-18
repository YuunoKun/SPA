#include "Procedure.h"


using namespace SourceProcessor;


Procedure::Procedure(proc_name name, proc_index id) {
	procedure_name = name;
	procedure_index = id;
}


proc_name Procedure::getName() {
	return procedure_name;
}


proc_index Procedure::getIndex() {
	return procedure_index;
}


void Procedure::addCaller(stmt_index id) {
	procedure_caller.insert(id);
}


std::vector<stmt_index> Procedure::getCaller() {
	return std::vector<stmt_index>(procedure_caller.begin(), procedure_caller.end());
}


void Procedure::addChild(stmt_index id) {
	procedure_contained_stmt.push_back(id);
}


std::vector<stmt_index> Procedure::getChild() {
	return procedure_contained_stmt;
}


void Procedure::addUsesVariable(var_name name) {
	procedure_uses.insert(name);
}


std::vector<var_name> Procedure::getUsedVariable() {
	return std::vector<var_name>(procedure_uses.begin(), procedure_uses.end());
}


void Procedure::addModifiesVariable(var_name name) {
	procedure_modifies.insert(name);
}


std::vector<var_name> Procedure::getModifiedVariable() {
	return std::vector<var_name>(procedure_modifies.begin(), procedure_modifies.end());
}