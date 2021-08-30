#pragma once

#include <string>
#include "Token.h"
#include "Tokenizer.h"


namespace Parser {

	class Parser {
	public:

		Parser();

		// Load source file
		void load_file(std::string file);

		// Load and parse source file
		void parse();

	private:

		std::string m_source_program;
		Tokenizer::Tokenizer m_tokenizer;
	};
}