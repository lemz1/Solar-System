#pragma once

#include "WGPU/Texture2D.h"
#include "WGPU/Mesh.h"
#include "Util/FractalNoiseSettings.h"

class Planet
{
public:
	Planet();
	~Planet();

	void Generate();

	void SetNoiseSettings(const FractalNoiseSettings& noiseSettings) { _noiseSettings = noiseSettings; }
	const FractalNoiseSettings& GetNoiseSettings() const { return _noiseSettings; }

	const Mesh* GetMesh() const { return _mesh; }
private:
	FractalNoiseSettings _noiseSettings = {};
	Mesh* _mesh = nullptr;
	Texture2D* _surfaceTexture = nullptr;
	Texture2D* _normalMap = nullptr;
};
