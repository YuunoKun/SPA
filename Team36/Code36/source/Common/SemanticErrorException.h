#pragma once
#include <iostream>
#include <exception>

#include "PQL/QueryObject/Query.h"

class SemanticErrorException : public std::exception {
public:
	SemanticErrorException() : std::exception() {};

	SemanticErrorException(std::string str) : std::exception(str.c_str()) {};

	SemanticErrorException(std::string str, Query& query) : std::exception(str.c_str()) {
		this->query = query;
	};

	Query getQuery() const throw() {
		return this->query;
	};

protected:
	Query query;
};