#include "Texture.h"

#include "glad/gl.h"

#include "Settings/LogString/LogString.h"
#include "Settings/MainSettings.h"

Texture::Texture(unsigned int width, unsigned int height) :
	Texture(width, height, GL_RGBA32F, GL_CLAMP_TO_BORDER, GL_NEAREST) { }

Texture::Texture(unsigned int width, unsigned int height, int format) :
	Texture(width, height, format, GL_CLAMP_TO_BORDER, GL_NEAREST) { }

Texture::Texture(unsigned int width, unsigned int height, int format, int wrap, int filtering) :
	Texture(width, height, format, wrap, wrap, filtering) { }

Texture::Texture(unsigned int width, unsigned int height, int format, int wrapX, int wrapY, int filtering)
{
	this->width = width;
	this->height = height;

	glGenTextures(1, &id);
	Activate();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glBindImageTexture(id, id, 0, GL_FALSE, 0, GL_READ_WRITE, format);

	LogGenerationFailure(width, height, format, wrapX, wrapY, filtering);
}

unsigned int Texture::GetId() const
{
	return id;
}

unsigned int Texture::GetWidth() const
{
	return width;
}

unsigned int Texture::GetHeight() const
{
	return height;
}

void Texture::Activate() const
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::SetWrapX(int wrapX) const
{
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, wrapX);
	LogParameterFailure("wrap X", wrapX);
}

void Texture::SetWrapY(int wrapY) const
{
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, wrapY);
	LogParameterFailure("wrap Y", wrapY);
}

void Texture::SetWrap(int wrap) const
{
	SetWrapX(wrap);
	SetWrapY(wrap);
}

void Texture::SetFilter(int filter) const
{
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, filter);
	LogParameterFailure("filter min", filter);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, filter);
	LogParameterFailure("filter mag", filter);
}

void Texture::LogParameterFailure(const char* label, int value) const
{
	if (int errorCode = glGetError())
	{
		MainSettings::Log << "Texture Error - Failed to set " << label <<
			"\nId: " << id << ", value: " << value << ", error code: " <<
			errorCode << "\n\n";
	}
}

void Texture::LogGenerationFailure(int width, int height, int format,
	int wrapX, int wrapY, int filtering) const
{
	if (int errorCode = glGetError())
	{
		MainSettings::Log << "Texture Error - Failed to generate texture\nId: " <<
			id << ", size: [" << width << ", " << height << "], format: " <<
			format << ", wrap: [" << wrapX << ", " << wrapY << "], filtering: " <<
			filtering << ", error code: " << errorCode << "\n";

		int maxTextureSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

		if (width > maxTextureSize || height > maxTextureSize)
		{
			MainSettings::Log << "Max texture size supported by GPU: [" <<
				maxTextureSize << ", " << maxTextureSize << "]\n";
		}

		MainSettings::Log << "\n";
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}
