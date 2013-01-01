#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "lwcppgl/display.hpp"
#include "heightmap.hpp"
#include "vertex.hpp"

#define VERTICES (8)
#define INDICES (14)

class Octree {
  public:
    GLboolean active;
    Octree(glm::vec3, Octree *, GLuint);
    ~Octree();
    static GLvoid initialize();
    GLvoid update(glm::vec3);
    GLvoid render(GLuint);

  private:
    Octree *children[8];
    Octree *parent;
    glm::vec3 position;
    glm::vec3 color;
    GLuint level;
    GLboolean leaf;
    static GLuint ibo;
    static GLuint vbo;
    GLvoid divide();
    const glm::vec3 spherize(glm::vec3);
};

#endif // OCTREE_HPP
