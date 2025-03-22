#version 460 core

#extension GL_ARB_bindless_texture : require

layout(binding = 2, std430) readonly buffer ssbo3 {
  sampler2D textures[];
};



uniform vec3 fsViewPosition;



in vec3 fsNormal;
in vec2 fsUv;
in vec3 fsPosition;



out vec4 fragColor;

vec3 c_ambient(float strength, vec3 lightColor) {
  return lightColor * strength;
}

vec3 c_diffuse(vec3 normal, vec3 lightDirection, vec3 lightColor) { 
  float lightDot = max(dot(normal, lightDirection), 0.0);
  return lightDot * lightColor;
}

vec3 c_specular(float strength, vec3 normal, vec3 viewPosition, vec3 position, vec3 lightDirection, vec3 lightColor) {
  vec3 viewDirection = normalize(viewPosition - position);
  vec3 reflectDirection = reflect(-lightDirection, normal);

  
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
  return strength * spec * lightColor;
}

void main() {
  sampler2D tex = textures[0];
  vec3 objectColor = texture(tex, fsUv).rgb;

  vec3 normal = normalize(fsNormal);

  vec3 lightColor = vec3(1.0, 1.0, 1.0);
  vec3 lightPosition = vec3(4.0, 2.0, 0.0);
  vec3 lightDirection = normalize(lightPosition - fsPosition);
  
  vec3 light =
    c_ambient(0.2, lightColor) +
    c_diffuse(normal, lightDirection, lightColor) +
    c_specular(0.5, normal, fsViewPosition, fsPosition, lightDirection, lightColor);

  fragColor = vec4(light * objectColor, 1.0);
}
