#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "lwcppgl/display.hpp"
#include "vertex.hpp"

#define CHUNK_SIZE (1)
#define VERTICES_PER_EDGE (CHUNK_SIZE + 1)
#define VERTICES_PER_FACE (VERTICES_PER_EDGE * VERTICES_PER_EDGE)
#define VERTICES (VERTICES_PER_FACE * VERTICES_PER_EDGE)
#define INDICES (VERTICES)

class Octree {
  public:
    Octree(glm::vec3, GLuint);
    ~Octree();
    void render();

  private:
    Octree *children[8];
    Vertex vs[VERTICES];
    GLuint level;
    GLuint vbo;
    GLuint ibo;
    GLvoid divide();
};

#endif // OCTREE_HPP
