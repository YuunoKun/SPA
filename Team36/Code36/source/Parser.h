#pragma once

#include <string>
#include "Tokenizer.h"

namespace SourceProcessor {

	class Parser {
	public:

		Parser();

		// Load source file
		void load_file(std::string file);

		// Return input string
		std::string get_source_program();
			
		// Load and parse source file
		void parse();


	private:
		std::string m_source_program;
		Tokenizer m_tokenizer;
	};

} // namespace SourceProcessor
