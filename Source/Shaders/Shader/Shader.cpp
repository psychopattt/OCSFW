#include "Shader.h"

#include <string>

#include "glad/gl.h"

#include "../ShaderProvider/ShaderProvider.h"
#include "Settings/LogString/LogString.h"
#include "Settings/MainSettings.h"

Shader::Shader(const char* vertexShaderName, const char* fragmentShaderName)
{
	const std::string vertexCode = ShaderProvider::GetCode(vertexShaderName);
	const std::string fragmentCode = ShaderProvider::GetCode(fragmentShaderName);

	if (!vertexCode.empty() && !fragmentCode.empty())
	{
		unsigned int shaderIds[] = {
			Compile(vertexShaderName, GL_VERTEX_SHADER, vertexCode.c_str()),
			Compile(fragmentShaderName, GL_FRAGMENT_SHADER, fragmentCode.c_str())
		};

		Link(shaderIds, std::size(shaderIds));
	}
}

unsigned int Shader::Compile(const char* shaderName, int type, const char* code)
{
	unsigned int shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &code, NULL);
	glCompileShader(shaderId);

	int success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char log[512];
		glGetShaderInfoLog(shaderId, static_cast<int>(std::size(log)), NULL, log);
		MainSettings::Log << "Shader Error - Compilation failed for \"" <<
			shaderName << "\"\n" << log << "\n";

		return NULL;
	}

	return shaderId;
}

void Shader::Link(unsigned int* shaderIds, size_t shaderCount)
{
	id = glCreateProgram();

	for (int i = 0 ; i < shaderCount; i++)
		glAttachShader(id, shaderIds[i]);

	glLinkProgram(id);

	int success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		char log[512];
		glGetProgramInfoLog(id, static_cast<int>(std::size(log)), NULL, log);
		MainSettings::Log << "Shader Error - Linking failed\n" << log << "\n";
	}

	for (int i = 0; i < shaderCount; i++)
		glDeleteShader(shaderIds[i]);
}

unsigned int Shader::GetId() const
{
	return id;
}

void Shader::Activate() const
{
	glUseProgram(id);
}

void Shader::SetInt(const char* name, int value) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform1i(id, location, value);
	LogParameterFailure(name, location);
}

void Shader::SetInt(const char* name, unsigned int value) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform1ui(id, location, value);
	LogParameterFailure(name, location);
}

void Shader::SetBool(const char* name, bool value) const
{
	SetInt(name, static_cast<int>(value));
}

void Shader::SetFloat(const char* name, float value) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform1f(id, location, value);
	LogParameterFailure(name, location);
}

void Shader::SetDouble(const char* name, double value) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform1d(id, location, value);
	LogParameterFailure(name, location);
}

void Shader::SetBufferBinding(const char* name, unsigned int binding) const
{
	unsigned int blockIndex = glGetProgramResourceIndex(id, GL_SHADER_STORAGE_BLOCK, name);
	glShaderStorageBlockBinding(id, blockIndex, binding);
	LogParameterFailure(name, blockIndex);
}

void Shader::SetTextureBinding(const char* name, unsigned int binding) const
{
	SetInt(name, static_cast<int>(binding));
}

void Shader::LogParameterFailure(const char* name, unsigned int location) const
{
	int errorCode = glGetError();
	bool invalidLocation = location == GL_INVALID_INDEX;
	const char* accessType = invalidLocation ? "get" : "set";

	if (errorCode != GL_NO_ERROR || invalidLocation)
	{
		MainSettings::Log << "Shader Error - Failed to " << accessType <<
			" parameter\nName: \"" << name << "\", location: " <<
			location << ", shader id: " << id << ", error code: " <<
			errorCode << "\n\n";
	}
}

Shader::~Shader()
{
	glDeleteProgram(id);
}
