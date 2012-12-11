#version 140
 
in vec3 vertexPosition;
in vec2 vertexTexture;

out vec4 color;

uniform vec3 inColor;

void main(void) {
  color = vec4(inColor, 1.f);
}
