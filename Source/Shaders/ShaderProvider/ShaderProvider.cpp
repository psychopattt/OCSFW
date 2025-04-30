#include "ShaderProvider.h"

#include "Settings/LogString/LogString.h"
#include "Settings/MainSettings.h"

#if DEBUG
#include <fstream>
#include <sstream>
#include <filesystem>
#include "../ShaderMinifier/ShaderMinifier.h"
#endif

using namespace std;
using MainSettings::Log;

static constexpr char shaderExtension[] = ".glsl";

const string ShaderProvider::GetCode(string shaderName)
{
	if (!shaderName.ends_with(shaderExtension))
		shaderName += shaderExtension;

#if DEBUG
	return ReadFile(shaderName);
#else
	return UnpackShader(shaderName);
#endif
}

constexpr uint32_t ShaderProvider::GetHash(const string_view text) noexcept
{
	uint32_t hash = 5381;

	for (const char& c : text)
		hash = ((hash << 5) + hash) + c;

	return hash;
}

#if DEBUG

const string ShaderProvider::ReadFile(const string& shaderName)
{
	ifstream file;
	file.exceptions(ifstream::failbit | ifstream::badbit);
	const string fullPath = FindFile(shaderName);

	try
	{
		file.open(fullPath, ios_base::in);
		stringstream stream;
		stream << file.rdbuf();
		file.close();

		return stream.str();
	}
	catch (ifstream::failure exception)
	{
		Log << "Shader Error - Failed to load file \"" << fullPath << "\"\n";
		return "";
	}
}

const string ShaderProvider::FindFile(const string& searchedFile)
{
	using namespace filesystem;

	if (exists(searchedFile))
		return searchedFile;

	for (const directory_entry& file : recursive_directory_iterator("./"))
	{
		const string filePath = file.path().string();

		if (filePath.ends_with(searchedFile))
			return filePath;
	}

	Log << "Shader Error - Failed to find file \"" << searchedFile << "\"\n";
	return searchedFile;
}

void ShaderProvider::PackShaders()
{
	using namespace filesystem;

	ofstream outputFile;
	outputFile.exceptions(ifstream::failbit | ifstream::badbit);
	
	try
	{
		outputFile.open(
			"./Source/Shaders/PackedShaders.cpp",
			ios_base::out | ios_base::trunc
		);
	}
	catch (ifstream::failure exception)
	{
		Log << "Shader Error - Failed to find file \"PackedShaders.cpp\"\n\n";
		return;
	}

	outputFile << "// Generated code\n\n#if !DEBUG\n#include \"Shaders/"
		"ShaderProvider/ShaderProvider.h\"\nconst string ShaderProvider::"
		"UnpackCode(const uint32_t& h) {\nswitch (h) {\n";

	for (const directory_entry& file : recursive_directory_iterator("./"))
	{
		const path& filePath = file.path();

		if (file.is_regular_file() && filePath.extension() == shaderExtension)
			PackShader(filePath.string(), outputFile);
	}

	outputFile << "default: return \"\"; }}\n#endif\n";
	outputFile.close();

	Log << "Shader Packing - Finished packing shaders\n\n";
}

void ShaderProvider::PackShader(const string& shaderPath, ofstream& outputFile)
{
	string code = ReadFile(shaderPath);
	const string shaderName = filesystem::path(shaderPath).filename().string();

	if (code.empty())
	{
		Log << "Shader Packing - Failed to pack shader \"" << shaderName << "\"\n";
		return;
	}

	ShaderMinifier::MinifyShader(code);
	outputFile << "case " << GetHash(shaderName) << ": return \"" << code << "\";\n";
	Log << "Shader Packing - Successfully packed \"" << shaderName << "\"\n";
}

#else

const string ShaderProvider::UnpackShader(const string& shaderName)
{
	const string code = UnpackCode(GetHash(shaderName));

	if (code.empty())
		Log << "Shader Error - Failed to load shader \"" << shaderName << "\"\n";

	return code;
}

#endif
