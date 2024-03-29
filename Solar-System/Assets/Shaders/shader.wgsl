struct UniformData 
{
	uTime: f32,
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
	let ratio: f32 = 1280.0 / 720.0;

	out.position = vec4<f32>(in.position.x / ratio + sin(uniformData.uTime) * 0.25, in.position.y + cos(uniformData.uTime) * 0.25, 0.0, 1.0);
	out.uv = in.uv;
	out.normal = in.normal;
	return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
	let color: vec3<f32> = vec3<f32>(in.uv, 0.0);
	let gamma_corrected_color: vec3<f32> = pow(color, vec3<f32>(2.2));
	return vec4<f32>(gamma_corrected_color, 1.0);
}
