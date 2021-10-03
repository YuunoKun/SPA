#include "SemanticErrorException.h"
#include <iostream>
#include <exception>

SemanticErrorException::SemanticErrorException() {
};

SemanticErrorException::SemanticErrorException(std::string str) {
	std::cout << str << std::endl;
};