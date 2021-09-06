#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include "Parser.h"
#include "FSM.h"
#include "DesignExtractor.h"

using namespace SourceProcessor;

Parser::Parser(void) {
	m_tokenizer = Tokenizer();
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
		m_source_program = std::string(file_size, ' ');
		fread(m_source_program.data(), 1, file_size, input_file);

		fclose(input_file);
	}
	catch (...) {
		std::runtime_error("Error : Unknown error loading file.");
		m_source_program = "";
		fclose(input_file);
	}
	*/

	std::ifstream input_file;
	input_file.open(file);
	if (input_file.is_open()) {
		std::string temp;
		while (std::getline(input_file, temp)) {
			m_source_program.append(temp);
		}
	}
	else {
		std::cout << "Cannot open file." << std::endl;
	}
	input_file.close();
};

std::string Parser::get_source_program() {
	return m_source_program;
}

void Parser::parse() {
	// Actual parsing workflow

	/* 
	TODO:
	If m_source_program = "";
	An error message should be pushed
	*/

	m_tokenizer.parse_into_tokens(m_source_program.c_str());
	FSM finite_state_machine(m_tokenizer);
	finite_state_machine.build();
}