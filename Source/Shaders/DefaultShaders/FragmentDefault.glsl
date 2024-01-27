#version 460 core

in vec2 fragmentTextureCoords;
uniform sampler2D dataTexture;
out vec4 color;

void main()
{
	color = texture(dataTexture, fragmentTextureCoords);
}
