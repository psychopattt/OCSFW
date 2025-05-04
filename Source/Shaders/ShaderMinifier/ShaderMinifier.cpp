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
		RenameStructs(shader);
		RenameDefines(shader);
		RenameFunctions(shader);
		RenameVariables(shader);
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
		const regex structRegex(R"(^.*?\bstruct\s+([a-zA-Z_]\w*)\s*\{)");
		const regex defineRegex(R"(^#define\s+([^\s]+))");
		const regex functionRegex(R"(^.*?\b([a-z_]\w*)\s+([a-z_]\w*)\s*\()", regex::icase);
		const regex variableRegex(R"(^.*?\b\s*(?:(?:in|out|inout)\s+)?([a-zA-Z_]\w*)\s+([a-zA-Z_]\w*)\s*[;,=)])");
		const regex returnRegex(R"(^return$)");
		const regex mainRegex(R"(^main$)");
		const regex groupRegex(R"([{}()])");
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

		void RenameStructs(string& shader)
		{
			std::smatch match;
			size_t offset = 0;

			while (regex_search(shader.cbegin() + offset, shader.cend(), match, structRegex))
			{
				size_t matchEnd = match.position() + match.length();

				if (regex_search(match[0].first, match[0].second, directiveRegex))
				{
					offset += matchEnd;
					continue;
				}

				string matchedStruct = match.str(1);
				string replacement = GenerateUniqueIdentifier(shader);

				const regex matchedStructRegex(R"(([^\.\w]))" + matchedStruct + "\\b");
				shader = regex_replace(shader, matchedStructRegex, "$1" + replacement);
				offset += matchEnd - matchedStruct.length() + replacement.length();
			}
		}

		void RenameDefines(string& shader)
		{
			std::smatch match;
			size_t offset = 0;

			while (regex_search(shader.cbegin() + offset, shader.cend(), match, defineRegex))
			{
				string matchedDefine = match.str(1);
				size_t matchEnd = match.position() + match.length();
				string replacement = GenerateUniqueIdentifier(shader);

				const regex matchedDefineRegex(R"(([^\.\w]))" + matchedDefine + "\\b");
				shader = regex_replace(shader, matchedDefineRegex, "$1" + replacement);
				offset += matchEnd - matchedDefine.length() + replacement.length();
			}
		}

		void RenameFunctions(string& shader)
		{
			std::smatch match;
			size_t offset = 0;

			while (regex_search(shader.cbegin() + offset, shader.cend(), match, functionRegex))
			{
				size_t matchEnd = match.position() + match.length();

				if (regex_search(match[0].first, match[0].second, directiveRegex) ||
					regex_search(match[1].first, match[1].second, returnRegex) ||
					regex_search(match[2].first, match[2].second, mainRegex))
				{
					offset += matchEnd;
					continue;
				}

				string matchedFunction = match.str(2);
				string replacement = GenerateUniqueIdentifier(shader);

				const regex matchedFunctionRegex(R"(([^\.\w]))" + matchedFunction + "\\b");
				shader = regex_replace(shader, matchedFunctionRegex, "$1" + replacement);
				offset += matchEnd - matchedFunction.length() + replacement.length();
			}
		}

		void RenameVariables(string& shader)
		{
			std::smatch match;
			size_t offset = 0;

			while (regex_search(shader.cbegin() + offset, shader.cend(), match, functionRegex))
			{
				offset += match.position() + match.length();

				if (!regex_search(match[0].first, match[0].second, directiveRegex) &&
					!regex_search(match[1].first, match[1].second, returnRegex))
				{
					size_t functionStart = offset;
					AdvanceToNextFunctionEnd(shader, offset);
					RenameFunctionVariables(shader, functionStart, offset);
				}
			}
		}

		void AdvanceToNextFunctionEnd(const string& shader, size_t& offset)
		{
			std::smatch match;
			int bracketDepth = 0;
			int parenthesisDepth = 1;
			bool bodyFound = false;
			bool endFound = false;

			while (!endFound && regex_search(shader.cbegin() + offset, shader.cend(), match, groupRegex))
			{
				string symbol = match.str();
				parenthesisDepth += (symbol == "(") - (symbol == ")");
				bracketDepth += (symbol == "{") - (symbol == "}");

				bodyFound = bodyFound || (parenthesisDepth == 0 && bracketDepth == 1);
				endFound = bodyFound && parenthesisDepth == 0 && bracketDepth == 0;

				offset += match.position() + match.length();
			}
		}

		void RenameFunctionVariables(string& shader, size_t functionStart, size_t& functionEnd)
		{
			std::smatch match;
			size_t offset = functionStart;

			while (regex_search(shader.cbegin() + offset, shader.cbegin() + functionEnd, match, variableRegex))
			{
				size_t matchEnd = match.position() + match.length();

				if (regex_search(match[0].first, match[0].second, directiveRegex) ||
					regex_search(match[1].first, match[1].second, returnRegex) ||
					match.length(2) == 1)
				{
					offset += matchEnd;
					continue;
				}

				string matchedVariable = match.str(2);
				string replacement = GenerateUniqueIdentifier(shader);
				ReplaceFunctionVariable(shader, offset, functionEnd, matchedVariable, replacement);
				offset += matchEnd - matchedVariable.length() + replacement.length();
			}
		}

		void ReplaceFunctionVariable(string& shader, size_t start, size_t& end,
			const string& variable, const string& replacement)
		{
			std::smatch match;
			const regex matchRegex(R"(([^\.\w]))" + variable + "\\b");

			while (regex_search(shader.cbegin() + start, shader.cbegin() + end, match, matchRegex))
			{
				end -= variable.length() - replacement.length();
				start += match.position() + match.length() - variable.length() + replacement.length();
				shader.replace(match[0].first, match[0].second, match.str(1) + replacement);
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
