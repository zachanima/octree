#include "octree.hpp"



Octree::Octree(glm::vec3 position, GLuint level) {
  this->level = level;

  // Nullify children.
  for (size_t i = 0; i < 8; i++) {
    children[i] = NULL;
  }

  // Generate cube.
  size_t i = 0;
  for (GLuint z = 0; z < VERTICES_PER_EDGE; z++) {
    for (GLuint y = 0; y < VERTICES_PER_EDGE; y++) {
      for (GLuint x = 0; x < VERTICES_PER_EDGE; x++) {
        const glm::vec3 r((GLfloat)x-.5f, (GLfloat)y-.5f, (GLfloat)z-.5f);
        vs[i++].r = r;
      }
    }
  }

  // Compute indices.
  GLuint is[INDICES];
  for (size_t i = 0; i < INDICES; i++) {
    is[i] = i;
  }

  // Initialize vertex buffer object.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, VERTICES * sizeof(Vertex), vs, GL_STATIC_DRAW);

  // Initialize index buffer object.
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES * sizeof(GLuint), is, GL_STATIC_DRAW);
}



Octree::~Octree() {
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);

  for (GLuint i = 0; i < 8; i++) {
    delete children[i];
    children[i] = NULL;
  }
}



GLvoid Octree::render() {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, r));

  glDrawElements(GL_POINTS, INDICES, GL_UNSIGNED_INT, (GLvoid *)0);

  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

