#ifndef GAME_HPP
#define GAME_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "lwcppgl/display.hpp"

#include "octree.hpp"
#include "camera.hpp"

#define WIDTH (1280)
#define HEIGHT (720)

class Game {
  public:
    static GLvoid initialize();
    static GLvoid update();
    static GLvoid render();

  private:
    static Camera camera;
    static GLuint program;
    static GLuint viewUniform;
    static Octree *octree;
    static GLvoid offsetOrientation(glm::vec3, GLfloat);
};

#endif // GAME_HPP
