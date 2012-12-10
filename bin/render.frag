#version 140
 
in vec3 vertexPosition;

out vec4 color;

void main(void) {
  color = vec4(vertexPosition + .5f, 1.f);
}
