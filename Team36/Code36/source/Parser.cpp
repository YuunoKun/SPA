#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include "Parser.h"
#include "FSM.h"
#include "DesignExtractor.h"

using namespace SourceProcessor;

Parser::Parser(void) {
	tokenizer = Tokenizer();
}

void Parser::load_file(std::string file) {
	/*
	FILE* input_file = fopen(file, "r");
	try {
		if (!input_file) {
			std::runtime_error("Error : Failed to open file.");
		}

		fseek(input_file, 0, SEEK_END);
		size_t file_size = ftell(input_file);
		fseek(input_file, 0, SEEK_SET);
		source_program = std::string(file_size, ' ');
		fread(source_program.data(), 1, file_size, input_file);

		fclose(input_file);
	}
	catch (...) {
		std::runtime_error("Error : Unknown error loading file.");
		source_program = "";
		fclose(input_file);
	}
	*/

	std::ifstream input_file;
	input_file.open(file);
	if (input_file.is_open()) {
		std::string temp;
		while (std::getline(input_file, temp)) {
			source_program.append(temp);
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
	// Actual parsing workflow

	/*
	TODO:
	If source_program = "";
	An error message should be pushed
	*/
	try {
		tokenizer.parseIntoTokens(source_program.c_str());
		FSM finite_state_machine(tokenizer);
		finite_state_machine.build();
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
		throw e;
	}
}