#version 460 core

#extension GL_ARB_bindless_texture : require

struct Vertex {
  float position[3];
  float normal[3];
  float uv[2];
};



layout(binding = 0, std430) readonly buffer ssbo1 {
  Vertex vertices[];
};

layout(binding = 1, std430) readonly buffer ssbo2 {
  uint indices[];
};



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



out vec3 fsNormal;
out vec2 fsUv;
out vec3 fsPosition;

void main() {
  uint index = indices[gl_VertexID];

  vec3 position = vec3(
    vertices[index].position[0],
    vertices[index].position[1],
    vertices[index].position[2]
  );

  vec3 normal = vec3(
    vertices[index].normal[0],
    vertices[index].normal[1],
    vertices[index].normal[2]
  );

  vec2 uv = vec2(
    vertices[index].uv[0],
    vertices[index].uv[1]
  );

  gl_Position = projection * view * model * vec4(position, 1.0);
  
  fsNormal = mat3(transpose(inverse(model))) * normal;
  fsUv = uv;
  fsPosition = vec3(model * vec4(position, 1.0));
}
