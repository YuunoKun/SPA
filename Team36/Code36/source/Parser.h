#pragma once

#include <string>
#include "SPCommon.h"
#include "Tokenizer.h"
#include "FSM.h"

namespace SourceProcessor {

	class Parser {
	public:
		Parser();

		void loadSourceFile(std::string file);
		std::string getSourceProgram();
		void parse();

	private:
		std::string source_program;
		Tokenizer tokenizer;
	};

} // namespace SourceProcessor
