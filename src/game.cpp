#include "game.hpp"

Camera Game::camera;
GLuint Game::program;
GLuint Game::viewUniform;
Octree *Game::octree = NULL;



GLvoid Game::initialize() {
  const GLfloat FOV = 45.f;
  const GLfloat ASPECT = (GLfloat)WIDTH / (GLfloat)HEIGHT;
  const GLfloat ZNEAR = 1.f / 256.f;
  const GLfloat ZFAR = 256.f;
  const glm::mat4 model(
    glm::vec4(1.f, 0.f, 0.f, 0.f),
    glm::vec4(0.f, 1.f, 0.f, 0.f),
    glm::vec4(0.f, 0.f, 1.f, 0.f),
    glm::vec4(0.f, 0.f, 0.f, 1.f)
  );
  const glm::mat4 projection = glm::perspective(FOV, ASPECT, ZNEAR, ZFAR);
  GLuint modelUniform;
  GLuint projectionUniform;

  // Initialize shaders.
  program = Display::shaders("render.vert", "render.frag");

  // Initialize uniforms.
  camera.positionUniform = glGetUniformLocation(program, "camera");
  modelUniform =           glGetUniformLocation(program, "model");
  viewUniform =            glGetUniformLocation(program, "view");
  projectionUniform =      glGetUniformLocation(program, "projection");
  
  // Initialize camera.
  camera.position = glm::vec3(0.f, 0.f, 8.f);
  offsetOrientation(glm::vec3(0.f, 1.f, 0.f), 0.f);

  // Initialize octree.
  octree = new Octree(glm::vec3(0.f, 0.f, 0.f), 16);

  // Apply uniforms.
  glUseProgram(program);
  glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
  glUseProgram(0);
}



GLvoid Game::deinitialize() {
  delete octree;
}



GLvoid Game::update() {
  static GLuint ticks = SDL_GetTicks();
  const GLuint delta = SDL_GetTicks() - ticks;
  ticks = SDL_GetTicks();

  octree->update(camera.position);
  if (Keyboard::isKeyDown(KEY_UP)) { offsetOrientation(glm::vec3( 1.f, 0.f,  0.f), 0.00025f * delta); }
  if (Keyboard::isKeyDown(KEY_DOWN)) { offsetOrientation(glm::vec3(-1.f, 0.f,  0.f), 0.00025f * delta); }
  if (Keyboard::isKeyDown(KEY_LEFT)) { offsetOrientation(glm::vec3( 0.f, 0.f, -1.f), 0.0005f * delta); }
  if (Keyboard::isKeyDown(KEY_RIGHT)) { offsetOrientation(glm::vec3( 0.f, 0.f,  1.f), 0.0005f * delta); }

  if (Keyboard::isKeyDown(KEY_W)) {
    glm::vec3 direction = camera.direction;
    direction *= 0.0000025f * delta;
    camera.velocity += direction;
    camera.target += direction;
  }
  if (Keyboard::isKeyDown(KEY_S)) {
    glm::vec3 direction = camera.direction;
    direction *= 0.0000025f * delta;
    camera.velocity -= direction;
    camera.target -= direction;
  }
  if (Keyboard::isKeyDown(KEY_A)) {
    offsetOrientation(glm::vec3( 0.f, 1.f, 0.f), 3.1415926535f / 2.f);
    glm::vec3 direction = camera.direction;
    direction *= 0.0000025f * delta;
    camera.velocity -= direction;
    camera.target -= direction;
    offsetOrientation(glm::vec3( 0.f, -1.f, 0.f), 3.1415926535f / 2.f);
  }
  if (Keyboard::isKeyDown(KEY_D)) {
    offsetOrientation(glm::vec3( 0.f, -1.f, 0.f), 3.1415926535f / 2.f);
    glm::vec3 direction = camera.direction;
    direction *= 0.0000025f * delta;
    camera.velocity -= direction;
    camera.target -= direction;
    offsetOrientation(glm::vec3( 0.f, 1.f, 0.f), 3.1415926535f / 2.f);
  }

  // Move camera.
  camera.position += camera.velocity;
}



GLvoid Game::render() {
  glm::mat4 view = glm::mat4_cast(camera.orientation);
  view = glm::translate(view, -camera.position);

  glClearColor(1.f, 1.f, 1.0, 1.f);
  glClearDepth(1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  // glEnable(GL_CULL_FACE);
  // glEnable(GL_TEXTURE_2D);

  glUseProgram(program);

  glUniform3fv(camera.positionUniform, 1, glm::value_ptr(camera.position));
  glUniformMatrix4fv(viewUniform,      1, GL_FALSE, glm::value_ptr(view));

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  octree->render();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glUseProgram(0);
}



GLvoid Game::offsetOrientation(const glm::vec3 axis, GLfloat angle) {
  const GLfloat scalar = cosf(angle / 2.f);

  glm::vec3 naxis = glm::normalize(axis);
  naxis = naxis * sinf(angle / 2.f);

  glm::quat offset(scalar, naxis.x, naxis.y, naxis.z);

  glm::mat4 cameraMatrix = glm::lookAt(camera.position, camera.target, glm::vec3(0.f, 1.f, 0.f));
  glm::quat view = glm::quat_cast(cameraMatrix);
  glm::quat invView = glm::conjugate(view);

  glm::quat world = invView * offset * view;
  camera.orientation = world * camera.orientation;
  camera.orientation = glm::normalize(camera.orientation);
  camera.direction = glm::vec3(0.f, 0.f, -1.f) * camera.orientation;
  camera.direction = glm::normalize(camera.direction);
}
