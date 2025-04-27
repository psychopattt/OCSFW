#pragma once

class Shader
{
	public:
		Shader(const char* vertexShaderName, const char* fragmentShaderName);
		unsigned int GetId() const;
		void Activate() const;
		void SetBufferBinding(const char* name, unsigned int binding) const;
		void SetTextureBinding(const char* name, unsigned int binding) const;
		void SetUniform(const char* name, int value) const;
		void SetUniform(const char* name, int v0, int v1) const;
		void SetUniform(const char* name, int v0, int v1, int v2) const;
		void SetUniform(const char* name, int v0, int v1, int v2, int v3) const;
		void SetUniform(const char* name, unsigned int value) const;
		void SetUniform(const char* name, unsigned int v0, unsigned int v1) const;
		void SetUniform(const char* name, unsigned int v0, unsigned int v1, unsigned int v2) const;
		void SetUniform(const char* name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const;
		void SetUniform(const char* name, bool value) const;
		void SetUniform(const char* name, bool v0, bool v1) const;
		void SetUniform(const char* name, bool v0, bool v1, bool v2) const;
		void SetUniform(const char* name, bool v0, bool v1, bool v2, bool v3) const;
		void SetUniform(const char* name, float value) const;
		void SetUniform(const char* name, float v0, float v1) const;
		void SetUniform(const char* name, float v0, float v1, float v2) const;
		void SetUniform(const char* name, float v0, float v1, float v2, float v3) const;
		void SetUniform(const char* name, double value) const;
		void SetUniform(const char* name, double v0, double v1) const;
		void SetUniform(const char* name, double v0, double v1, double v2) const;
		void SetUniform(const char* name, double v0, double v1, double v2, double v3) const;
		~Shader();

	protected:
		Shader() = default;
		unsigned int id = 0;

		unsigned int Compile(const char* shaderName, int type, const char* code);
		void Link(unsigned int* shaderIds, size_t shaderCount);
		void LogParameterFailure(const char* name, unsigned int location) const;
};
