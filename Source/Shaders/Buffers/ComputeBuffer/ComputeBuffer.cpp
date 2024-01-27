#include "ComputeBuffer.h"

#include "glad/gl.h"

#include "Settings/LogString/LogString.h"
#include "Settings/MainSettings.h"

ComputeBuffer::ComputeBuffer(size_t sizeBytes) :
	ComputeBuffer(nullptr, sizeBytes) { }

ComputeBuffer::ComputeBuffer(const void* data, size_t sizeBytes)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeBytes, data, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, id, id);

	LogGenerationFailure(sizeBytes);
}

unsigned int ComputeBuffer::GetId() const
{
	return id;
}

void* ComputeBuffer::Map(int accessType) const
{
	void* data = glMapNamedBuffer(id, accessType);
	LogMapFailure();
	return data;
}

void* ComputeBuffer::Map(int accessType, size_t offset, size_t length) const
{
	void* data = glMapNamedBufferRange(id, offset, length, accessType);
	LogMapFailure();
	return data;
}

bool ComputeBuffer::Unmap() const
{
	bool result = glUnmapNamedBuffer(id);

	if (!result)
	{
		MainSettings::Log << "Buffer Error - Failed to unmap buffer\n" <<
			"Id: " << id << ", buffer data might be corrupted\n\n";
	}

	return result;
}

void ComputeBuffer::LogGenerationFailure(size_t sizeBytes) const
{
	if (int errorCode = glGetError())
	{
		size_t sizeGb = sizeBytes / 1000000000;

		MainSettings::Log << "Buffer Error - Failed to generate buffer\nId: " <<
			id << ", size: " << sizeBytes << " bytes (~" << sizeGb <<
			"GB), error code: " << errorCode << "\n\n";
	}
}

void ComputeBuffer::LogMapFailure() const
{
	if (int errorCode = glGetError())
	{
		MainSettings::Log << "Buffer Error - Failed to map buffer\n" <<
			"Id: " << id << ", error code: " << errorCode << "\n\n";
	}
}

ComputeBuffer::~ComputeBuffer()
{
	glDeleteBuffers(1, &id);
}
