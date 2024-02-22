#pragma once

class Texture
{
	public:
		Texture(unsigned int width, unsigned int height);
		Texture(unsigned int width, unsigned int height, int format);
		Texture(unsigned int width, unsigned int height, int format, int wrap, int filtering);
		Texture(unsigned int width, unsigned int height, int format, int wrapX, int wrapY, int filtering);
		unsigned int GetId() const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
		void Activate() const;
		void SetWrapX(int wrapX) const;
		void SetWrapY(int wrapY) const;
		void SetWrap(int wrap) const;
		void SetFilter(int filter) const;
		void Clear() const;
		~Texture();

	private:
		unsigned int id;
		unsigned int width;
		unsigned int height;

		void LogParameterFailure(const char* label, int value) const;
		void LogGenerationFailure(int width, int height, int format,
			int wrapX, int wrapY, int filtering) const;
};
