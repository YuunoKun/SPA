#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "PKB.h"
#include "TNode.h"
#include "Parser.hpp"
#include "Token.h"
#include "Tokenizer.hpp"


// Constructor
Parser::Parser() {
	m_tokenizer = Tokenizer::Tokenizer();
}


void Parser::load_file(const char* file) {

	FILE* input_file = fopen(file, "r");
	try {
		if (!input_file) {
			std::runtime_error("Error : Failed to open file.");
		}

		fseek(input_file, 0, SEEK_END);
		size_t file_size = ftell(input_file);
		fseek(input_file, 0, SEEK_SET);
		m_source_program = std::string(file_size, ' ');
		fread(m_source_program.data(), 1, file_size, input_file);

		fclose(input_file);
	}
	catch (...) {
		std::runtime_error("Error : Unknown error loading file.");
		m_source_program = "";
		fclose(input_file);
	}
}

void Parser::parse() {
	// Actual parsing workflow

	/*
	If m_source_program = "";
	An error message should be pushed
	*/

	m_tokenizer.parse(m_source_program);
}