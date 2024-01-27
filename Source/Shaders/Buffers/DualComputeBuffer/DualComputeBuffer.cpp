#include "DualComputeBuffer.h"

#include "../ComputeBuffer/ComputeBuffer.h"

DualComputeBuffer::DualComputeBuffer(size_t sizeBytes) :
	DualComputeBuffer(nullptr, sizeBytes) { }

DualComputeBuffer::DualComputeBuffer(const void* data, size_t sizeBytes)
{
	buffer1 = std::make_unique<ComputeBuffer>(data, sizeBytes);
	buffer2 = std::make_unique<ComputeBuffer>(data, sizeBytes);
}

ComputeBuffer* DualComputeBuffer::GetBuffer(int bufferIndex)
{
	if (!swapped)
	{
		if (bufferIndex == 0)
			return buffer1.get();
		else
			return buffer2.get();
	}
	else
	{
		if (bufferIndex == 0)
			return buffer2.get();
		else
			return buffer1.get();
	}
}

unsigned int DualComputeBuffer::GetId(int bufferIndex)
{
	return GetBuffer(bufferIndex)->GetId();
}

void DualComputeBuffer::Swap()
{
	swapped = !swapped;
}

DualComputeBuffer::~DualComputeBuffer() { }
