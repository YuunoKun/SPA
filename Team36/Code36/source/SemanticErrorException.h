#pragma once
#include <iostream>
#include <exception>

class SemanticErrorException : public std::exception {
public:
	SemanticErrorException();
	SemanticErrorException(std::string str);
};