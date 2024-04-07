#pragma once

#include "wgpu/Buffer.h"

class ComputeBuffer 
{
public:
    ComputeBuffer(uint32_t size);
    ~ComputeBuffer();

    const void* GetData() const;
public:
    Buffer* outputBuffer;
    Buffer* mapBuffer;
};
