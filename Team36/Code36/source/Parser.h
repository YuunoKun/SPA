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
		std::string getSourceProgram();
			
		// Load and parse source file
		void parse();


	private:
		std::string source_program;
		Tokenizer tokenizer;
	};

} // namespace SourceProcessor
