#include "octree.hpp"



Octree::Octree(glm::vec3 position, GLuint level) {
  this->level = level;
  this->position = position;

  // Nullify children.
  for (size_t i = 0; i < 8; i++) {
    children[i] = NULL;
  }

  // Generate cube vertices.
  size_t i = 0;
  Vertex vs[VERTICES];
  const GLfloat factor = 2.f * glm::pow(2.f, (GLfloat)level) / 65536.f;
  for (GLuint z = 0; z < 2; z++) {
    for (GLuint y = 0; y < 2; y++) {
      for (GLuint x = 0; x < 2; x++) {
        const glm::vec3 r = glm::vec3((GLfloat)x, (GLfloat)y, (GLfloat)z) - .5f;
        vs[i++].r = position + r * factor;
      }
    }
  }

  // Assign indices.
  GLuint is[INDICES] = { 
    0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
  };

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

  for (size_t i = 0; i < 8; i++) {
    if (children[i] != NULL) {
      delete children[i];
      children[i] = NULL;
    }
  }
}



GLvoid Octree::update(glm::vec3 camera) {
  if (level > 0 && glm::distance(camera, position) < glm::pow(glm::pow(2.f, (GLfloat)level) / 65536.f, 2.f) * 16.f) {
    if (children[0] == NULL) {
      divide();
    }
  } else if (children[0] != NULL) {
    for (size_t i = 0; i < 8; i++) {
      delete children[i];
      children[i] = NULL;
    }
  }

  if (children[0] != NULL) {
    for (size_t i = 0; i < 8; i++) {
      children[i]->update(camera);
    }
  }
}



GLvoid Octree::render() {
  if (children[0] == NULL) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, r));

    glDrawElements(GL_TRIANGLE_STRIP, INDICES, GL_UNSIGNED_INT, (GLvoid *)0);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

  } else {
    for (size_t i = 0; i < 8; i++) {
      children[i]->render();
    }
  }
}



GLvoid Octree::divide() {
  const GLfloat factor = glm::pow(2.f, (GLfloat)level) / 65536.f;
  children[0] = new Octree(position + glm::vec3(-.5f, -.5f, -.5f) * factor, level-1);
  children[1] = new Octree(position + glm::vec3(-.5f, -.5f,  .5f) * factor, level-1);
  children[2] = new Octree(position + glm::vec3(-.5f,  .5f, -.5f) * factor, level-1);
  children[3] = new Octree(position + glm::vec3(-.5f,  .5f,  .5f) * factor, level-1);
  children[4] = new Octree(position + glm::vec3( .5f, -.5f, -.5f) * factor, level-1);
  children[5] = new Octree(position + glm::vec3( .5f, -.5f,  .5f) * factor, level-1);
  children[6] = new Octree(position + glm::vec3( .5f,  .5f, -.5f) * factor, level-1);
  children[7] = new Octree(position + glm::vec3( .5f,  .5f,  .5f) * factor, level-1);
}
