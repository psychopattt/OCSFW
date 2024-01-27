#include "SimulationDrawer.h"

#include "glad/gl.h"

#include "Shaders/Shader/Shader.h"
#include "Shaders/Buffers/Texture/Texture.h"
#include "Shaders/ComputeShader/ComputeShader.h"
#include "SimulationTransforms/SimulationTransforms.h"

using std::make_unique;

SimulationDrawer::SimulationDrawer()
{
	GenerateVertexObjects();
	screenQuad = make_unique<Shader>("VertexDefault", "FragmentDefault");

	simTransforms = make_unique<SimulationTransforms>(
		quadVertices, std::size(quadVertices)
	);
}

void SimulationDrawer::GenerateVertexObjects()
{
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	glGenBuffers(1, &vertexBufferId);
	UpdateQuadVertexBuffer();

	// Set VAO's X and Y scale
	glEnableVertexArrayAttrib(vertexArrayId, 0);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(*quadVertices), (void*)0);

	// Set VAO's X and Y position
	glEnableVertexArrayAttrib(vertexArrayId, 1);
	glVertexAttribPointer(
		1, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(*quadVertices),
		(void*)(2 * sizeof(*quadVertices))
	);
}

void SimulationDrawer::UpdateQuadVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_DYNAMIC_DRAW);
}

void SimulationDrawer::Draw(Texture* texture)
{
	if (simTransforms->ApplyTransforms())
		UpdateQuadVertexBuffer();

	screenQuad->SetTextureBinding("dataTexture", texture->GetId());
	screenQuad->Activate();
	texture->Activate();

	glBindVertexArray(vertexArrayId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

SimulationDrawer::~SimulationDrawer()
{
	glDeleteBuffers(1, &vertexBufferId);
	glDeleteVertexArrays(1, &vertexArrayId);
}
