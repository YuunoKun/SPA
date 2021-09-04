#include <vector>
#include "Common.h"
#include "Procedure.h"
#include "Statement.h"



namespace SourceProcessor {

	class DesignExtractor {

	public:


		void populateRelations();


	private:
		std::vector<Procedure> m_procedures;
		std::vector<Statement> m_statements;
		std::vector<Variable> m_variables;
		std::vector<constant> m_constants;


		void populateFollows();
		void populateFollowsT();
		void populateParent();
		void populateParentT();
		void populateUses();
		void populateModifies();

	};

} // namespace SourceProcessor