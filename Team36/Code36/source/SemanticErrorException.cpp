#include "SemanticErrorException.h"
#include <iostream>
#include <exception>

SemanticErrorException::SemanticErrorException() {
	std::cout << "Semantic Error" << std::endl;
};

SemanticErrorException::SemanticErrorException(std::string str) {
	std::cout << "Semantic Error: " << str << std::endl;
};