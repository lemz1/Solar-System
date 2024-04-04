struct UniformData 
{
	modelViewProjection: mat4x4<f32>,
}

@group(0) @binding(0) var<uniform> uniformData: UniformData;
@group(0) @binding(1) var surfaceTexture: texture_2d<f32>;

struct VertexInput {
	@location(0) position: vec3<f32>,
	@location(1) normal: vec3<f32>,
};

struct VertexOutput {
	@builtin(position) clip_position: vec4<f32>,
	@location(0) object_position: vec3<f32>,
	@location(1) normal: vec3<f32>,
};

fn calculate_lighting(
    color: vec3<f32>, 
    normal: vec3<f32>
) -> vec3<f32> {
    let ambient_strength: f32 = 0.2;
    let ambient_color: vec3<f32> = color * ambient_strength;

    let light_dir: vec3<f32> = vec3<f32>(1.0, 1.0, 0.0);
    let diffuse_strength: f32 = max(dot(normal, light_dir), 0.0);
    let diffuse_color: vec3<f32> = color * diffuse_strength;

    let lit_color: vec3<f32> = ambient_color + diffuse_color;

    return clamp(lit_color, vec3<f32>(0.0), vec3<f32>(1.0));
}

fn triplanar_map(
	position: vec3<f32>,
	normal: vec3<f32>,
	texture: texture_2d<f32>
) -> vec3<f32> {
	let textureResolution: vec2<f32> = vec2<f32>(textureDimensions(texture));
	let uv_front: vec2<f32> = position.zy * 0.5 + 0.5;
	let uv_side: vec2<f32> = position.xz * 0.5 + 0.5;
	let uv_top: vec2<f32> = position.xy * 0.5 + 0.5;

	var color_front: vec3<f32> = textureLoad(texture, vec2<i32>(uv_front * textureResolution), 0).rgb;
	var color_side: vec3<f32> = textureLoad(texture, vec2<i32>(uv_side * textureResolution), 0).rgb;
	var color_top: vec3<f32> = textureLoad(texture, vec2<i32>(uv_top * textureResolution), 0).rgb;

	var weights: vec3<f32> = abs(normal);
	weights = weights / (weights.x + weights.y + weights.z);

	color_front *= weights.x;
	color_side *= weights.y;
	color_top *= weights.z;

	return color_front + color_side + color_top;
}

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
	var out: VertexOutput;

	out.clip_position = uniformData.modelViewProjection * vec4<f32>(in.position, 1.0);
	out.object_position = in.position;
	out.normal = in.normal;
	return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
	let color: vec3<f32> = triplanar_map(in.object_position, in.normal, surfaceTexture);

	let lit_color: vec3<f32> = calculate_lighting(color, in.normal);
	let gamma_corrected_color: vec3<f32> = pow(lit_color, vec3<f32>(2.2));
	return vec4<f32>(gamma_corrected_color, 1.0);
}
