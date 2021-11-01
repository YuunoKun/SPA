#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include "Parser.h"

using namespace SourceProcessor;

Parser::Parser(void) {
	tokenizer = Tokenizer();
}

void Parser::load_file(std::string file) {
	std::ifstream input_file;
	input_file.open(file);
	if (input_file.is_open()) {
		std::string temp;
		while (std::getline(input_file, temp)) {
			source_program.append(temp);
			source_program.push_back(' ');
		}
	}
	else {
		throw std::runtime_error("Failed to open file");
	}
	input_file.close();
};

std::string Parser::getSourceProgram() {
	return source_program;
}

void Parser::parse() {
	try {
		tokenizer.parseIntoTokens(source_program.c_str());
		FSM finite_state_machine(tokenizer);
		finite_state_machine.build();
	}
	catch (std::runtime_error runtime_e) {
		std::cout << runtime_e.what() << std::endl;
		throw runtime_e;
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
		throw e;
	}
}