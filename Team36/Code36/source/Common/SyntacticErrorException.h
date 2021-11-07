#pragma once
#include <iostream>
#include <exception>
#include <string>

class SyntacticErrorException : public std::exception {
public:
	SyntacticErrorException() : std::exception() {};
	SyntacticErrorException(std::string str) : std::exception(str.c_str()) {};
};