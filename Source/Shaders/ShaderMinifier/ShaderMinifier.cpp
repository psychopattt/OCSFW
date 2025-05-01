#ifdef DEBUG

#include "ShaderMinifier.h"

#include <regex>

using std::regex;

namespace ShaderMinifier
{
	void MinifyShader(string& shader)
	{
		ResetIdentifiers();
		TrimComments(shader);
		TrimSpaces(shader);
		TrimZeros(shader);
		RenameFunctions(shader);
		EscapeNewlines(shader);
	}

	namespace
	{
		size_t identifierOffset;
		const string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

		const regex commentRegex(R"((?:\/\/.*\n|\/\*(?:.|\n)+?\*\/))");
		const regex directiveRegex(R"((?:^|\n)\s*(#.+?)\s*(?:$|\n))");
		const regex tokenSpaceRegex(R"(\s*([.,;{}()\[\]?:=+-/*%<>|&!^])\s*)");
		const regex directiveKeyRegex(R"(@(\d+)@)");
		const regex leadingTrailingSpaceRegex(R"((^|\n) *(.*?) *($|\n))");
		const regex extraSpaceRegex(R"(  +)");
		const regex extraNewlineRegex(R"(\n\n+)");
		const regex edgeNewlineRegex(R"((^\n|\n$))");
		const regex leadingZeroRegex(R"(\b0+(\.\d+))");
		const regex trailingZeroRegex(R"((\d+\.)0+\b)");
		const regex functionRegex(R"(^.*?\b([a-z_]\w*)\s+([a-z_]\w*)\s*\()", regex::icase);
		const regex returnRegex(R"(^return$)");
		const regex mainRegex(R"(^main$)");
		const regex newlineRegex(R"(\n)");

		void ResetIdentifiers()
		{
			identifierOffset = 0;
		}

		void TrimComments(string& shader)
		{
			shader = regex_replace(shader, commentRegex, "\n");
		}

		void TrimSpaces(string& shader)
		{
			const std::vector<string> directives = ExtractDirectives(shader);
			shader = regex_replace(shader, tokenSpaceRegex, "$1");
			InsertDirectives(shader, directives);

			shader = regex_replace(shader, leadingTrailingSpaceRegex, "$1$2$3");
			shader = regex_replace(shader, extraSpaceRegex, " ");
			shader = regex_replace(shader, extraNewlineRegex, "\n");
			shader = regex_replace(shader, edgeNewlineRegex, "");
		}

		const std::vector<string> ExtractDirectives(string& shader)
		{
			std::smatch match;
			size_t offset = 0;
			std::vector<string> directives;

			while (regex_search(shader.cbegin() + offset, shader.cend(), match, directiveRegex))
			{
				string directive = match.str(1);
				size_t directivePosition = match.position(1);
				string directiveKey = "@" + std::to_string(directives.size()) + "@";

				directives.push_back(directive);
				shader.replace(offset + directivePosition, directive.length(), directiveKey);
				offset += directivePosition + directiveKey.length();
			}

			return directives;
		}

		void InsertDirectives(string& shader, const std::vector<string>& directives)
		{
			std::smatch match;
			size_t offset = 0;

			while (regex_search(shader.cbegin() + offset, shader.cend(), match, directiveKeyRegex))
			{
				size_t matchPosition = match.position();
				int directiveIndex = stoi(match.str(1));
				string directive = "\n" + directives[directiveIndex] + "\n";

				shader.replace(offset + matchPosition, match.length(), directive);
				offset += matchPosition + directive.length();
			}
		}

		void TrimZeros(string& shader)
		{
			shader = regex_replace(shader, leadingZeroRegex, "$1");
			shader = regex_replace(shader, trailingZeroRegex, "$1");
		}

		void RenameFunctions(string& shader)
		{
			std::smatch match;
			size_t offset = 0;

			while (regex_search(shader.cbegin() + offset, shader.cend(), match, functionRegex))
			{
				if (regex_search(match[0].first, match[0].second, directiveRegex) ||
					regex_search(match[1].first, match[1].second, returnRegex) ||
					regex_search(match[2].first, match[2].second, mainRegex))
				{
					offset += match.position() + match.length();
					continue;
				}

				string matchedFunction = match.str(2);
				size_t matchEnd = match.position() + match.length();
				string replacement = GenerateUniqueIdentifier(shader);

				const regex matchedFunctionRegex(R"(([^\.\w]))" + matchedFunction + "\\b");
				shader = regex_replace(shader, matchedFunctionRegex, "$1" + replacement);
				offset += matchEnd - matchedFunction.length() + replacement.length();
			}
		}

		const string GenerateUniqueIdentifier(const string& shader)
		{
			string identifier;

			do {
				identifier = GenerateIdentifier(++identifierOffset);
			} while (regex_search(shader, regex("\\b" + identifier + "\\b")));

			return identifier;
		}

		const string GenerateIdentifier(size_t id)
		{
			return id == 0 ? "" :
				GenerateIdentifier((id - 1) / characters.size()) +
				characters[(id - 1) % characters.size()];
		}

		void EscapeNewlines(string& shader)
		{
			shader = regex_replace(shader, newlineRegex, "\\n");
		}
	}
}

#endif
