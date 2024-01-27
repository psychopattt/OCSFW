#pragma once

class Texture
{
	public:
		Texture(unsigned int width, unsigned int height);
		Texture(unsigned int width, unsigned int height, int format);
		Texture(unsigned int width, unsigned int height, int format, int wrap, int filtering);
		Texture(unsigned int width, unsigned int height, int format, int wrapX, int wrapY, int filtering);
		unsigned int GetId() const;
		void Activate() const;
		~Texture();

	private:
		unsigned int id;
		unsigned int width;
		unsigned int height;

		void LogGenerationFailure(int width, int height, int format,
			int wrapX, int wrapY, int filtering) const;
};
