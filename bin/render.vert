#version 140
 
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;

out vec3 vertexPosition;
out vec2 vertexTexture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camera;

vec3 spherize(vec3 r) {
  const vec3 r2 = r * r;
  r.x *= sqrt(1.f - r2.y / 2.f - r2.z / 2.f + r2.y * r2.z / 3.f);
  r.y *= sqrt(1.f - r2.x / 2.f - r2.z / 2.f + r2.x * r2.z / 3.f);
  r.z *= sqrt(1.f - r2.x / 2.f - r2.y / 2.f + r2.x * r2.y / 3.f);
  return r;
}

void main(void) {
  gl_Position = projection * view * model * vec4(position, 1.f);
  vertexPosition = position;
  vertexTexture = texture;
}
