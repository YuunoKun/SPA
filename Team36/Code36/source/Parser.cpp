#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <fstream>
#include "Parser.h"
#include "PKB.h"

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
	If m_source_program = "";
	An error message should be pushed
	*/

	m_tokenizer.parse_into_tokens(m_source_program.c_str());

	const std::vector<Token> v = m_tokenizer.get_token_chain();
	std::vector<procedure_name> p_n;
	std::unordered_set<constant> con;
	Token tk;

	for (Token token : v) {
		std::cout << "TYPE : " << tokenTypeStrings[token.m_type] << "  Value : " << token.m_token_value << std::endl;
		if (tk.m_type == TokenType::PROCEDURE && token.m_type == TokenType::IDENTIFIER) {
			p_n.push_back(token.m_token_value);
		}

		if(token.m_type == TokenType::CONSTANT) {
			std::cout << "CONSTANT : " << token.m_token_value << std::endl;
			con.insert(atoi(token.m_token_value.c_str()));
		}
		tk = token;
	}

	PKB::getInstance().setProcedures(p_n);
	PKB::getInstance().setConstants(std::vector<constant>(con.begin(), con.end()));

}