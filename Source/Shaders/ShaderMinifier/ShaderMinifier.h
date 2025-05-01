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
		void RenameDefines(string& shader);
		void RenameFunctions(string& shader);
		const string GenerateUniqueIdentifier(const string& shader);
		const string GenerateIdentifier(size_t id);
		void EscapeNewlines(string& shader);
	}
};

#endif
