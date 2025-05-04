#ifdef DEBUG

#pragma once

#include <string>
#include <vector>

using std::string;

namespace ShaderMinifier
{
	void MinifyShader(string& shader);

	namespace
	{
		void ResetIdentifiers();
		void TrimComments(string& shader);
		void TrimSpaces(string& shader);
		const std::vector<string> ExtractDirectives(string& shader);
		void InsertDirectives(string& shader, const std::vector<string>& directives);
		void TrimZeros(string& shader);
		void RenameStructs(string& shader);
		void RenameDefines(string& shader);
		void RenameFunctions(string& shader);
		void RenameVariables(string& shader);
		void AdvanceToNextFunctionEnd(const string& shader, size_t& offset);
		void RenameFunctionVariables(string& shader, size_t functionStart, size_t& functionEnd);
		void ReplaceFunctionVariable(string& shader, size_t start, size_t& end,
			const string& variable, const string& replacement);
		const string GenerateUniqueIdentifier(const string& shader);
		const string GenerateIdentifier(size_t id);
		void EscapeNewlines(string& shader);
	}
};

#endif
