#pragma once
#include <iostream>
#include <exception>
#include "Query.h"

class SemanticErrorException : public std::exception {
protected:
	Query query;
public:
	SemanticErrorException();
	SemanticErrorException(std::string str);
	SemanticErrorException(std::string str, Query& query);
	Query getQuery() const throw();
};