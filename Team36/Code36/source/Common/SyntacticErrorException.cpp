#include "SyntacticErrorException.h"
#include <iostream>
#include <exception>
#include <string>

SyntacticErrorException::SyntacticErrorException() {
	std::cout << "Syntactic Error" << std::endl;
};

SyntacticErrorException::SyntacticErrorException(std::string str) {
	std::cout << "Syntactic Error: " << str << std::endl;
};