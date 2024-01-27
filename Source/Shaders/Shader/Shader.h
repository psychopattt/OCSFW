#pragma once

class Shader
{
	public:
		Shader(const char* vertexShaderName, const char* fragmentShaderName);
		unsigned int GetId() const;
		void Activate() const;
		void SetInt(const char* name, int value) const;
		void SetInt(const char* name, unsigned int value) const;
		void SetBool(const char* name, bool value) const;
		void SetFloat(const char* name, float value) const;
		void SetBufferBinding(const char* name, unsigned int binding) const;
		void SetTextureBinding(const char* name, unsigned int binding) const;
		~Shader();

	protected:
		Shader() = default;
		unsigned int id = 0;

		unsigned int Compile(const char* shaderName, int type, const char* code);
		void Link(unsigned int* shaderIds, size_t shaderCount);
		void LogParameterFailure(const char* name, unsigned int location) const;
};
