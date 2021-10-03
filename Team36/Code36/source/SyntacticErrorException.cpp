#include "SyntacticErrorException.h"
#include <iostream>
#include <exception>
#include <string>

SyntacticErrorException::SyntacticErrorException() {
};

SyntacticErrorException::SyntacticErrorException(std::string str) {
	std::cout << str << std::endl;
};