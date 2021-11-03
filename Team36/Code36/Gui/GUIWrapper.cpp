#include "GUIWrapper.h"

#include <iostream>

#include "..\source\QuerySystem.h"
#include "..\source\Parser.h"

// a default constructor
GUIWrapper::GUIWrapper() {
	// create any objects here as instance variables of this class
	// as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void GUIWrapper::parse(std::string filename) {
	// call your parser to do the parsing
	SourceProcessor::Parser parser;
	parser.loadSourceFile(filename);
	parser.parse();
	std::cout << "parsed " << filename << std::endl;
	// ...rest of your code...
}

// method to evaluating a query
void GUIWrapper::evaluate(std::string query, std::list<std::string>& results) {
	// call your evaluator to evaluate the query here
	  // ...code to evaluate query...

	std::cout << "query=  " << query << std::endl;
	QuerySystem querySystem;
	std::list<std::string> queryResult = querySystem.processQuery(query);
	for each (std::string s in queryResult) {
		results.push_back(s);
		std::cout << s << ", " << std::endl;
	}
	// store the answers to the query in the results list (it is initially empty)
	// each result must be a string.
}