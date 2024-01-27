#pragma once

class ComputeBuffer
{
	public:
		ComputeBuffer(size_t sizeBytes);
		ComputeBuffer(const void* data, size_t sizeBytes);
		unsigned int GetId() const;
		void* Map(int accessType) const;
		void* Map(int accessType, size_t offset, size_t length) const;
		bool Unmap() const;
		~ComputeBuffer();

	private:
		unsigned int id;

		void LogGenerationFailure(size_t sizeBytes) const;
		void LogMapFailure() const;
};
