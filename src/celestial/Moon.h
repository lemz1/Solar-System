#pragma once

#include "wgpu/Texture2D.h"
#include "wgpu/Mesh.h"
#include "util/FractalNoiseSettings.h"

class Moon
{
public:
	Moon();
	~Moon();

	void Generate();

	void SetNoiseSettings(const FractalNoiseSettings& noiseSettings) { _noiseSettings = noiseSettings; }
	const FractalNoiseSettings& GetNoiseSettings() const { return _noiseSettings; }

	const Mesh* GetMesh() const { return _mesh; }
private:
	FractalNoiseSettings _noiseSettings = {};
	Mesh* _mesh = nullptr;
};
