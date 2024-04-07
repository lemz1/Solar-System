struct UniformData {
    numVertices: u32,
}

@group(0) @binding(0) var<storage, read> vertices: array<f32>; // vec3
@group(0) @binding(1) var<storage, read_write> heights: array<f32>;
@group(0) @binding(2) var<uniform> uniformData : UniformData;

fn func(val: vec3<f32>) -> f32 {
    return 2.0;
}

@compute @workgroup_size(32)
fn main(@builtin(global_invocation_id) id: vec3<u32>) {
    if (id.x >= uniformData.numVertices) {
        return;
    }

    let vertexIndex = id.x * 3u;
    let vertex: vec3<f32> = vec3<f32>(
        vertices[vertexIndex + 0u], 
        vertices[vertexIndex + 1u], 
        vertices[vertexIndex + 2u]
    );

    heights[id.x] = func(vertex);
}
