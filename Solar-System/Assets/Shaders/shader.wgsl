struct UniformData 
{
	modelViewProjection: mat4x4<f32>,
}

@group(0) @binding(0) var<uniform> uniformData: UniformData;

struct VertexInput {
	@location(0) position: vec3<f32>,
	@location(1) normal: vec3<f32>,
};

struct VertexOutput {
	@builtin(position) position: vec4<f32>,
	@location(0) normal: vec3<f32>,
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

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
	var out: VertexOutput;

	out.position = uniformData.modelViewProjection * vec4<f32>(in.position, 1.0);
	out.normal = in.normal;
	return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
	let color: vec3<f32> = vec3<f32>(0.1, 0.2, 1.0);
	let lit_color: vec3<f32> = calculate_lighting(color, in.normal);
	let gamma_corrected_color: vec3<f32> = pow(lit_color, vec3<f32>(2.2));
	return vec4<f32>(gamma_corrected_color, 1.0);
}
