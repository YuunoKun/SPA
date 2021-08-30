#pragma once

#include <string>
#include "Token.h"
#include "Tokenizer.h"

namespace SourceProcessor {

	class Parser {
	public:

		Parser();

		// Load source file
		void load_file(const char* file);

		// Return input string
		std::string get_source_program();
			
		// Load and parse source file
		void parse();


	private:
		std::string m_source_program;
		Tokenizer m_tokenizer;
	};

} // namespace SourceProcessor
