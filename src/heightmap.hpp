#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP

#include <GL/gl.h>
#include <glm/glm.hpp>

class Heightmap {
  public:
    static GLfloat simplex(glm::vec3, GLuint, GLfloat, GLfloat);

  private:
    static glm::vec3 mod289(glm::vec3);
    static glm::vec4 mod289(glm::vec4);
    static glm::vec4 permute(glm::vec4);
    static glm::vec4 taylorInvSqrt(glm::vec4);
    static GLfloat snoise(glm::vec3);
    static GLfloat ridge(GLfloat, GLfloat);
};

#endif // HEIGHTMAP_HPP
