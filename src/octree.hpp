#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "lwcppgl/display.hpp"
#include "heightmap.hpp"
#include "vertex.hpp"

#define VERTICES (8)
#define INDICES (14)

class Octree {
  public:
    Octree(glm::vec3, GLuint);
    ~Octree();
    GLvoid update(glm::vec3);
    GLvoid render();

  private:
    Octree *children[8];
    glm::vec3 position;
    GLuint level;
    GLuint vbo;
    GLuint ibo;
    GLvoid divide();
    const glm::vec3 spherize(glm::vec3);
};

#endif // OCTREE_HPP
