struct UniformData {
	viewProjection: mat4x4<f32>,
	stScale: vec2<f32>,
	stTiling: vec2<f32>,
	nmScale: vec2<f32>,
	nmTiling: vec2<f32>,
	stBlendSharpness: f32,
	nmBlendSharpness: f32
}

@group(0) @binding(0) var<uniform> uniformData: UniformData;
@group(0) @binding(1) var textureSampler: sampler;
@group(0) @binding(2) var surfaceTexture: texture_2d<f32>;
@group(0) @binding(3) var normalMap: texture_2d<f32>;

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

fn calculateLighting(
    color: vec3<f32>, 
    normal: vec3<f32>
) -> vec3<f32> {
    let ambientStrength: f32 = 0.2;
    let ambientColor: vec3<f32> = color * ambientStrength;

    let lightDir: vec3<f32> = vec3<f32>(1.0, 1.0, 0.0);
    let diffuseStrength: f32 = max(dot(normal, lightDir), 0.0);
    let diffuseColor: vec3<f32> = color * diffuseStrength;

    let litColor: vec3<f32> = ambientColor + diffuseColor;

    return clamp(litColor, vec3<f32>(0.0), vec3<f32>(1.0));
}

fn triplanarMap(
	position: vec3<f32>,
	surfaceNormal: vec3<f32>,
	texture: texture_2d<f32>,
	scale: vec2<f32>,
	tiling: vec2<f32>,
	blendSharpness: f32
) -> vec3<f32> {
	let textureResolution: vec2<f32> = vec2<f32>(textureDimensions(texture));
	let uvX: vec2<f32> = position.zy * 0.5 + 0.5;
	let uvY: vec2<f32> = position.xz * 0.5 + 0.5;
	let uvZ: vec2<f32> = position.xy * 0.5 + 0.5;

	var colorX: vec3<f32> = textureSample(texture, textureSampler, scaleTileUV(uvX, scale, tiling)).rgb;
	var colorY: vec3<f32> = textureSample(texture, textureSampler, scaleTileUV(uvY, scale, tiling)).rgb;
	var colorZ: vec3<f32> = textureSample(texture, textureSampler, scaleTileUV(uvZ, scale, tiling)).rgb;

	var weights: vec3<f32> = abs(surfaceNormal);
	weights = pow(weights, vec3<f32>(blendSharpness));
	weights = weights / (weights.x + weights.y + weights.z);

	colorX *= weights.x;
	colorY *= weights.y;
	colorZ *= weights.z;

	return colorX + colorY + colorZ;
}

fn triplanarNormal(
	position: vec3<f32>,
	surfaceNormal: vec3<f32>,
	texture: texture_2d<f32>,
	scaleTiling: vec4<f32>
) -> vec3<f32> {
	return vec3<f32>(0.0);
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
	let color: vec3<f32> = triplanarMap(
		in.objectPosition, 
		in.normal, 
		surfaceTexture, 
		uniformData.stScale,
		uniformData.stTiling,
		uniformData.stBlendSharpness
	);

	let litColor: vec3<f32> = calculateLighting(color, in.normal);
	let gammaCorrectedColor: vec3<f32> = pow(litColor, vec3<f32>(2.2));
	return vec4<f32>(gammaCorrectedColor, 1.0);
}
