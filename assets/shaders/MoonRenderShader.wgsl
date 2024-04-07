struct UniformData {
	viewProjection: mat4x4<f32>,
	moonColor: vec3<f32>,
	normalMapScale: vec2<f32>,
	normalMapTiling: vec2<f32>,
	normalMapBlendSharpness: f32
}

@group(0) @binding(0) var<uniform> uniformData: UniformData;
@group(0) @binding(1) var textureSampler: sampler;
@group(0) @binding(2) var normalMap: texture_2d<f32>;

struct VertexInput {
	@location(0) position: vec3<f32>,
	@location(1) normal: vec3<f32>,
};

struct VertexOutput {
	@builtin(position) clipPosition: vec4<f32>,
	@location(0) objectPosition: vec3<f32>,
	@location(1) normal: vec3<f32>,
};

fn scaleTileUV(
	uv: vec2<f32>,
	scale: vec2<f32>,
	tiling: vec2<f32>
) -> vec2<f32> {
	return uv * scale + tiling;
}

fn unpackNormal(normal: vec3<f32>) -> vec3<f32> {
	return normal * 2.0 - 1.0;
}

fn calculateLighting(
    color: vec3<f32>, 
    normal: vec3<f32>
) -> vec3<f32> {
    let ambientStrength: f32 = 0.2;
    let ambientColor: vec3<f32> = color * ambientStrength;

	// light from top-right to bottom-left
    let lightDir: vec3<f32> = normalize(vec3<f32>(-1.0, -1.0, 0.0)); 
    let diffuseStrength: f32 = max(-dot(normal, lightDir), 0.0);
    let diffuseColor: vec3<f32> = color * diffuseStrength;

    let litColor: vec3<f32> = ambientColor + diffuseColor;

    return clamp(litColor, vec3<f32>(0.0), vec3<f32>(1.0));
}

// https://www.ronja-tutorials.com/post/010-triplanar-mapping
fn triplanarMap(
	position: vec3<f32>,
	surfaceNormal: vec3<f32>,
	texture: texture_2d<f32>,
	scale: vec2<f32>,
	tiling: vec2<f32>,
	blendSharpness: f32
) -> vec3<f32> {
	let uvX: vec2<f32> = position.zy * 0.5 + 0.5;
	let uvY: vec2<f32> = position.xz * 0.5 + 0.5;
	let uvZ: vec2<f32> = position.xy * 0.5 + 0.5;

	var colorX: vec3<f32> = textureSample(texture, textureSampler, scaleTileUV(uvX, scale, tiling)).rgb;
	var colorY: vec3<f32> = textureSample(texture, textureSampler, scaleTileUV(uvY, scale, tiling)).rgb;
	var colorZ: vec3<f32> = textureSample(texture, textureSampler, scaleTileUV(uvZ, scale, tiling)).rgb;

	var blend: vec3<f32> = abs(surfaceNormal);
	blend = pow(blend, vec3<f32>(blendSharpness));
	blend = blend / (blend.x + blend.y + blend.z);

	colorX *= blend.x;
	colorY *= blend.y;
	colorZ *= blend.z;

	return colorX + colorY + colorZ;
}

// https://bgolus.medium.com/normal-mapping-for-a-triplanar-shader-10bf39dca05a
fn triplanarNormal(
	position: vec3<f32>,
	surfaceNormal: vec3<f32>,
	texture: texture_2d<f32>,
	scale: vec2<f32>,
	tiling: vec2<f32>,
	blendSharpness: f32
) -> vec3<f32> {
	let uvX: vec2<f32> = position.zy * 0.5 + 0.5;
	let uvY: vec2<f32> = position.xz * 0.5 + 0.5;
	let uvZ: vec2<f32> = position.xy * 0.5 + 0.5;

	var tNormalX: vec3<f32> = unpackNormal(textureSample(texture, textureSampler, scaleTileUV(uvX, scale, tiling)).rgb);
	var tNormalY: vec3<f32> = unpackNormal(textureSample(texture, textureSampler, scaleTileUV(uvY, scale, tiling)).rgb);
	var tNormalZ: vec3<f32> = unpackNormal(textureSample(texture, textureSampler, scaleTileUV(uvZ, scale, tiling)).rgb);

	var blend: vec3<f32> = abs(surfaceNormal);
	blend = pow(blend, vec3<f32>(blendSharpness));
	blend = blend / (blend.x + blend.y + blend.z);

	tNormalX = vec3<f32>(
		tNormalX.xy + surfaceNormal.zy,
		abs(tNormalX.z) * surfaceNormal.x
	);
	tNormalY = vec3<f32>(
		tNormalY.xy + surfaceNormal.xz,
		abs(tNormalY.z) * surfaceNormal.y
	);
	tNormalZ = vec3<f32>(
		tNormalZ.xy + surfaceNormal.xy,
		abs(tNormalZ.z) * surfaceNormal.z
	);

	return normalize(
		tNormalX.zyx * blend.x +
		tNormalY.xzy * blend.y +
		tNormalZ.xyz * blend.z
	);
}

@vertex
fn vsMain(in: VertexInput) -> VertexOutput {
	var out: VertexOutput;

	out.clipPosition = uniformData.viewProjection * vec4<f32>(in.position, 1.0);
	out.objectPosition = in.position;
	out.normal = in.normal;
	return out;
}

@fragment
fn fsMain(in: VertexOutput) -> @location(0) vec4<f32> {
	let normal: vec3<f32> = triplanarNormal(
		in.objectPosition, 
		in.normal, 
		normalMap, 
		uniformData.normalMapScale,
		uniformData.normalMapTiling,
		uniformData.normalMapBlendSharpness
	);

	let litColor: vec3<f32> = calculateLighting(uniformData.moonColor, normal);
	let gammaCorrectedColor: vec3<f32> = pow(litColor, vec3<f32>(2.2));
	return vec4<f32>(gammaCorrectedColor, 1.0);
}
