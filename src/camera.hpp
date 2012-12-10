#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Camera {
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 direction;
  glm::vec3 target;
  glm::quat orientation;
  GLuint positionUniform;
};

#endif // CAMERA_HPP
