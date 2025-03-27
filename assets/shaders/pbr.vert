#version 460 core

#extension GL_ARB_bindless_texture : require



layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aUv;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



out vec3 fsNormal;
out vec2 fsUv;
out vec3 fsPosition;

void main() {
  gl_Position = projection * view * model * vec4(aPosition, 1.0);
  
  fsNormal = mat3(transpose(inverse(model))) * aNormal;
  fsUv = aUv;
  fsPosition = vec3(model * vec4(aPosition, 1.0));
}
