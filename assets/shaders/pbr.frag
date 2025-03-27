#version 460 core

#extension GL_ARB_bindless_texture : require

out vec4 fragColor;

in vec3 fsNormal;
in vec2 fsUv;
in vec3 fsPosition;



uniform vec3 lightPosition = vec3(1.0, 0.0, 1.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

uniform vec3 viewPos;

uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D normalMap;

const float PI = 3.14159265359;



vec3 getNormalFromMap();
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main() {
  vec3 albedo = texture(albedoMap, fsUv).rgb;
  float metallic  = texture(metallicMap, fsUv).r;
  float roughness = texture(roughnessMap, fsUv).r;
  float ao        = texture(aoMap, fsUv).r;

  vec3 N = getNormalFromMap();
  vec3 V = normalize(viewPos - fsPosition);

  vec3 F0 = vec3(0.04); 
  F0 = mix(F0, albedo, metallic);
           
  // reflectance equation
  vec3 Lo = vec3(0.0);
  // calculate per-light radiance
  vec3 L = normalize(lightPosition - fsPosition);
  vec3 H = normalize(V + L);
  float distance    = length(lightPosition - fsPosition);
  float attenuation = 1.0 / (distance * distance);
  attenuation = 1.0;
  vec3 radiance     = lightColor * attenuation;        
  
  // cook-torrance brdf
  float NDF = DistributionGGX(N, H, roughness);        
  float G   = GeometrySmith(N, V, L, roughness);      
  vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
  
  vec3 kS = F;
  vec3 kD = vec3(1.0) - kS;
  kD *= 1.0 - metallic;	  
  
  vec3 numerator    = NDF * G * F;
  float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
  vec3 specular     = numerator / denominator;  
      
  // add to outgoing radiance Lo
  float NdotL = max(dot(N, L), 0.0);                
  Lo += (kD * albedo / PI + specular) * radiance * NdotL; 

  vec3 ambient = vec3(0.03) * albedo * ao;
  vec3 color = ambient + Lo;

  color = color / (color + vec3(1.0));
  color = pow(color, vec3(1.0/2.2));  
 
  fragColor = vec4(color, 1.0);
}



float DistributionGGX(vec3 N, vec3 H, float roughness) {
  float a      = roughness*roughness;
  float a2     = a*a;
  float NdotH  = max(dot(N, H), 0.0);
  float NdotH2 = NdotH*NdotH;

  float num   = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;

  return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
  float r = (roughness + 1.0);
  float k = (r*r) / 8.0;

  float num   = NdotV;
  float denom = NdotV * (1.0 - k) + k;

  return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx2  = GeometrySchlickGGX(NdotV, roughness);
  float ggx1  = GeometrySchlickGGX(NdotL, roughness);

  return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 getNormalFromMap() {
    vec3 tangentNormal = texture(normalMap, fsUv).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fsPosition);
    vec3 Q2  = dFdy(fsPosition);
    vec2 st1 = dFdx(fsUv);
    vec2 st2 = dFdy(fsUv);

    vec3 N   = normalize(fsNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
