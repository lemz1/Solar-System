struct UniformData 
{
	modelViewProjection: mat4x4<f32>,
}

@group(0) @binding(0) var<uniform> uniformData: UniformData;

struct VertexInput {
	@location(0) position: vec3<f32>,
	@location(1) uv: vec2<f32>,
	@location(2) normal: vec3<f32>,
};

struct VertexOutput {
	@builtin(position) position: vec4<f32>,
	@location(0) uv: vec2<f32>,
	@location(1) normal: vec3<f32>,
};

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
	var out: VertexOutput;

	out.position = uniformData.modelViewProjection * vec4<f32>(in.position, 1.0);
	out.uv = in.uv;
	out.normal = in.normal;
	return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
	let light: f32 = dot(in.normal, vec3<f32>(1.0, 1.0, 0.0));
	let color: vec3<f32> = vec3<f32>(1.0, 1.0, 1.0);
	let gamma_corrected_color: vec3<f32> = pow(max(color * light, vec3<f32>(0.0)), vec3<f32>(2.2));
	return vec4<f32>(gamma_corrected_color, 1.0);
}
