#version 140
 
layout (location = 0) in vec3 position;

out vec3 vertexPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camera;

void main(void) {
  vec4 finalPosition = vec4(position, 1.f) - vec4(camera, 0.f);

  gl_Position = (projection * view * model) * vec4(position, 1.f);
  vertexPosition = position;
}
