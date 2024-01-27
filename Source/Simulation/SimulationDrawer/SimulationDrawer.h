#pragma once

#include <memory>

using std::unique_ptr;

class SimulationDrawer
{
	public:
		SimulationDrawer();
		void Draw(class Texture* texture);
		~SimulationDrawer();

	private:
		unsigned int vertexArrayId;
		unsigned int vertexBufferId;
		unique_ptr<class Shader> screenQuad;
		unique_ptr<class SimulationTransforms> simTransforms;

		double quadVertices[16] = {
			-1.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f
		};

		void GenerateVertexObjects();
		void UpdateQuadVertexBuffer();
};
