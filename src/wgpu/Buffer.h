#pragma once

#include <stdint.h>
#include <webgpu/webgpu.hpp>

class Buffer 
{
public:
    Buffer(
        wgpu::BufferUsageFlags usage,
        uint32_t size
    );
    ~Buffer();

    void Write(
        const void* data,
        uint32_t size
    );

    wgpu::Buffer GetBuffer() const { return _buffer; }
    uint32_t GetSize() const { return _size; }
    wgpu::BufferUsageFlags GetUsageFlags() const { return _usage; }
private:
    wgpu::Buffer _buffer = nullptr;
    uint32_t _size = 0;
    wgpu::BufferUsageFlags _usage = wgpu::BufferUsage::None;
};
