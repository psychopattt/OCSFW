#ifdef DEBUG

#include "ShaderMerger.h"

#include <regex>

#include "../ShaderProvider/ShaderProvider.h"

using std::regex;
using std::string;

namespace ShaderMerger
{
	const regex includeRegex(R"(^\s*(#\s*include\s+\"([^\"]*)\"))");

	void ProcessIncludes(string& shader)
	{
		std::smatch match;
		size_t offset = 0;

		while (regex_search(shader.cbegin() + offset, shader.cend(), match, includeRegex))
		{
			size_t matchLength = match.length(1);
			size_t matchPosition = match.position(1);
			string includedShader = ShaderProvider::GetCode(match.str(2));

			shader.replace(offset + matchPosition, matchLength, includedShader);
			offset += matchPosition + includedShader.length();
		}
	}
}

#endif
