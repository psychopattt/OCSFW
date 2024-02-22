#pragma once

#include "../Shader/Shader.h"

class ComputeShader : public Shader
{
	public:
		ComputeShader(const char* shaderName, unsigned int width = 1,
			unsigned int height = 1, unsigned int depth = 1);
		void Execute() const;

	private:
		unsigned int groupCounts[3] = { };

		void ComputeGroupCounts(unsigned int width,
			unsigned int height, unsigned int depth);
};
