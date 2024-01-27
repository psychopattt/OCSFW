#version 460 core

in vec4 position;
in vec2 vertexTextureCoords;
out vec2 fragmentTextureCoords;

void main()
{
	gl_Position = position;
	fragmentTextureCoords = vertexTextureCoords;
}
