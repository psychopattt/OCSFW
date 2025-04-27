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

void Shader::SetBufferBinding(const char* name, unsigned int binding) const
{
	unsigned int blockIndex = glGetProgramResourceIndex(id, GL_SHADER_STORAGE_BLOCK, name);
	glShaderStorageBlockBinding(id, blockIndex, binding);
	LogParameterFailure(name, blockIndex);
}

void Shader::SetTextureBinding(const char* name, unsigned int binding) const
{
	SetUniform(name, static_cast<int>(binding));
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

void Shader::SetUniform(const char* name, int value) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform1i(id, location, value);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, int v0, int v1) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform2i(id, location, v0, v1);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, int v0, int v1, int v2) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform3i(id, location, v0, v1, v2);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, int v0, int v1, int v2, int v3) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform4i(id, location, v0, v1, v2, v3);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, unsigned int value) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform1ui(id, location, value);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, unsigned int v0, unsigned int v1) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform2ui(id, location, v0, v1);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, unsigned int v0, unsigned int v1, unsigned int v2) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform3ui(id, location, v0, v1, v2);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform4ui(id, location, v0, v1, v2, v3);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, bool value) const
{
	SetUniform(name, static_cast<int>(value));
}

void Shader::SetUniform(const char* name, bool v0, bool v1) const
{
	SetUniform(name, static_cast<int>(v0), static_cast<int>(v1));
}

void Shader::SetUniform(const char* name, bool v0, bool v1, bool v2) const
{
	SetUniform(name, static_cast<int>(v0), static_cast<int>(v1), static_cast<int>(v2));
}

void Shader::SetUniform(const char* name, bool v0, bool v1, bool v2, bool v3) const
{
	SetUniform(name, static_cast<int>(v0), static_cast<int>(v1), static_cast<int>(v2), static_cast<int>(v3));
}

void Shader::SetUniform(const char* name, float value) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform1f(id, location, value);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, float v0, float v1) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform2f(id, location, v0, v1);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, float v0, float v1, float v2) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform3f(id, location, v0, v1, v2);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, float v0, float v1, float v2, float v3) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform4f(id, location, v0, v1, v2, v3);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, double value) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform1d(id, location, value);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, double v0, double v1) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform2d(id, location, v0, v1);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, double v0, double v1, double v2) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform3d(id, location, v0, v1, v2);
	LogParameterFailure(name, location);
}

void Shader::SetUniform(const char* name, double v0, double v1, double v2, double v3) const
{
	unsigned int location = glGetUniformLocation(id, name);
	glProgramUniform4d(id, location, v0, v1, v2, v3);
	LogParameterFailure(name, location);
}

Shader::~Shader()
{
	glDeleteProgram(id);
}
