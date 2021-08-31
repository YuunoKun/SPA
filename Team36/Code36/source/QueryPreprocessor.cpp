#include "QueryPreprocessor.h"
#include <iostream>



//	return Query();
//}

void tokenize(std::string const& str, const char delim,
	std::vector<std::string>& out)
{
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
}

Query QueryPreprocessor::process(std::string str) {
	/*
	* std::vector <std::string> tokens;
	std::string delimiter = ";";
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		token = str.substr(0, pos);
		tokens.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	tokens.push_back(token);
	*/

	const char delim = ';';
	std::vector<std::string> tokens;
	tokenize(str, delim, tokens);


	// make new query
	Query query;


	//check through all tokens
	for (int i = 0; i < tokens.size(); i++) {

		// EntityType: STMT | READ | PRINT | CALL | WHILE | IF | ASSIGN | VARIABLE | CONSTANT | PROCEDURE | wild?
		// assign declarations to entites
		// add enetities 
		std::string CONSTANT = "constant";
		std::string PROCEDURE = "procedure";
		std::string SELECT = "Select";

		const char tempDelim = ' ';
		std::vector<std::string> tempTokens;
		tokenize(tokens[i], tempDelim, tempTokens);

		if (tokens[i].find(CONSTANT) != std::string::npos) {
			Entity entity(EntityType::CONSTANT, tempTokens[1]);
			query.addEntity(entity);
		}
		else if (tokens[i].find(PROCEDURE) != std::string::npos) {
			Entity entity(EntityType::PROCEDURE, tempTokens[1]);
			query.addEntity(entity);
		}
		else if (tokens[i].find(SELECT) != std::string::npos) {
			// selected type to get from unordered map entity
			Entity entity(EntityType::CONSTANT, tempTokens[1]);
			query.setSelected(entity);
		}
		else {

			std::cout << "invalid query" << std::endl;
		}

		// assign select to selected
		//setSelected
		//== "Select"
	}

	return query;
}

