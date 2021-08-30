#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
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


void Parser::load_file(std::string file) {

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

	const vector<Tokenizer::Token> v = m_tokenizer.get_token_chain();
	std::vector<procedure_name> p_n;
	std::unordered_set<constant> con;
	Tokenizer::Token tk;

	for (Tokenizer::Token token : v) {
		if (tk.m_type == Tokenizer::TokenType::PROCEDURE && token.m_type == Tokenizer::TokenType::IDENTIFIER) {
			p_n.push_back(token.m_token_value);
		}

		if(tk.m_type == Tokenizer::TokenType::CONSTANT) {
			con.insert(atoi(token.m_token_value.c_str));
		}
		tk = token;
	}

	PKB::getInstance().setProcedures(p_n);
	PKB::getInstance().setConstants(con);

}