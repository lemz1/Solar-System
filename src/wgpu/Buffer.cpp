#include "Buffer.h"

#include "core/Application.h"

Buffer::Buffer(
    wgpu::BufferUsageFlags usage,
    uint32_t size
)
:   _size(size),
    _usage(usage)
{
    wgpu::Device device = Application::GetWGPUContext()->device;
    wgpu::Queue queue = Application::GetWGPUContext()->queue;

    wgpu::BufferDescriptor bufferDesc = wgpu::Default;
    bufferDesc.label = "Buffer";
    bufferDesc.mappedAtCreation = false;
    bufferDesc.size = _size;
    bufferDesc.usage = _usage;
    _buffer = device.createBuffer(bufferDesc);
}

Buffer::~Buffer()
{
    _buffer.destroy();
    _buffer.release();
}

void Buffer::Write(
    const void *data, 
    uint32_t size
)
{
    wgpu::Device device = Application::GetWGPUContext()->device;
    wgpu::Queue queue = Application::GetWGPUContext()->queue;

    if (_size != size) 
    {
        _size = size;
        
        wgpu::BufferDescriptor bufferDesc = wgpu::Default;
        bufferDesc.label = "Buffer";
        bufferDesc.mappedAtCreation = false;
        bufferDesc.size = _size;
        bufferDesc.usage = _usage;
        _buffer = device.createBuffer(bufferDesc);
    }

    queue.writeBuffer(_buffer, 0, data, _size);
}
