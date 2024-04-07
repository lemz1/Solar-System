#include "ComputeBuffer.h"

ComputeBuffer::ComputeBuffer(uint32_t size)
{
    outputBuffer = new Buffer(wgpu::BufferUsage::Storage | wgpu::BufferUsage::CopySrc, size);
    mapBuffer = new Buffer(wgpu::BufferUsage::MapRead | wgpu::BufferUsage::CopyDst, size);
}

ComputeBuffer::~ComputeBuffer()
{
    delete mapBuffer;
    delete outputBuffer;
}
