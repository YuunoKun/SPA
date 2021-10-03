#pragma once
#include <iostream>
#include <exception>
#include <string>

class SyntacticErrorException : public std::exception {
public:
	SyntacticErrorException();
	SyntacticErrorException(std::string str);
};