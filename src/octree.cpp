#include "octree.hpp"

GLuint Octree::ibo;
GLuint Octree::vbo;



Octree::Octree(glm::vec3 position, Octree *parent, GLuint level) {
  this->level = level;
  this->position = position;
  this->parent = parent;

  // Nullify children.
  for (size_t i = 0; i < 8; i++) {
    children[i] = NULL;
  }

  // Determine if this cube is active.
  active = Heightmap::simplex(position, 1, .5f, .5f) < .33f;

  color = glm::vec3(
    (GLfloat)rand() / (GLfloat)RAND_MAX,
    (GLfloat)rand() / (GLfloat)RAND_MAX,
    (GLfloat)rand() / (GLfloat)RAND_MAX
  );
}



Octree::~Octree() {
  for (size_t i = 0; i < 8; i++) {
    if (children[i] != NULL) {
      delete children[i];
      children[i] = NULL;
    }
  }
}



GLvoid Octree::initialize() {
  // Generate cube vertices.
  size_t i = 0;
  Vertex vs[VERTICES];
  for (GLuint z = 0; z < 2; z++) {
    for (GLuint y = 0; y < 2; y++) {
      for (GLuint x = 0; x < 2; x++) {
        const glm::vec3 r = glm::vec3((GLfloat)x, (GLfloat)y, (GLfloat)z) - .5f;
        vs[i++].r = r;
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



GLvoid Octree::update(glm::vec3 camera) {
  const GLboolean split = level > 8 && glm::distance(position, camera) < glm::pow(2.f * glm::pow(2.f, (GLfloat)level) / 65536.f, 2.f) * 512.f;

  if (children[0] == NULL && split) {
    divide();

  } else if (children[0] != NULL && !split) {
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



GLvoid Octree::render(GLuint program) {
  const static GLuint modelUniform = glGetUniformLocation(program, "model");
  const static GLuint inColorUniform = glGetUniformLocation(program, "inColor");

  if (children[0] == NULL) {
    GLboolean surface = false;
    for (size_t i = 0; i < 8; i++) {
      if (parent != NULL && !parent->children[i]->active) {
        surface = true;
        break;
      }
    }
    if (active && surface) {
      const glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.f), position), glm::vec3(2.f * glm::pow(2.f, (GLfloat)level) / 65536.f));
      glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
      glUniform3fv(inColorUniform, 1, glm::value_ptr(color));
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, r));
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, t));

      glDrawElements(GL_TRIANGLE_STRIP, INDICES, GL_UNSIGNED_INT, (GLvoid *)0);

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

  } else if (level > 10) {
    for (size_t i = 0; i < 8; i++) {
      children[i]->render(program);
    }
  }
}



GLvoid Octree::divide() {
  const GLfloat factor = glm::pow(2.f, (GLfloat)level) / 65536.f;
  children[0] = new Octree(position + glm::vec3(-.5f, -.5f, -.5f) * factor, this, level-1);
  children[1] = new Octree(position + glm::vec3(-.5f, -.5f,  .5f) * factor, this, level-1);
  children[2] = new Octree(position + glm::vec3(-.5f,  .5f, -.5f) * factor, this, level-1);
  children[3] = new Octree(position + glm::vec3(-.5f,  .5f,  .5f) * factor, this, level-1);
  children[4] = new Octree(position + glm::vec3( .5f, -.5f, -.5f) * factor, this, level-1);
  children[5] = new Octree(position + glm::vec3( .5f, -.5f,  .5f) * factor, this, level-1);
  children[6] = new Octree(position + glm::vec3( .5f,  .5f, -.5f) * factor, this, level-1);
  children[7] = new Octree(position + glm::vec3( .5f,  .5f,  .5f) * factor, this, level-1);
}
