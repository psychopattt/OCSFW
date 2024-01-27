#pragma once

#include <memory>

class ComputeBuffer;

class DualComputeBuffer
{
	public:
		DualComputeBuffer(size_t sizeBytes);
		DualComputeBuffer(const void* data, size_t sizeBytes);
		ComputeBuffer* GetBuffer(int bufferIndex);
		unsigned int GetId(int bufferIndex);
		void Swap();
		~DualComputeBuffer();

	private:
		bool swapped = false;
		std::unique_ptr<ComputeBuffer> buffer1;
		std::unique_ptr<ComputeBuffer> buffer2;
};
