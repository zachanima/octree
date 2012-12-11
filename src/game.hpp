#ifndef GAME_HPP
#define GAME_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "lwcppgl/display.hpp"

#include "octree.hpp"
#include "camera.hpp"

#define WIDTH (512)
#define HEIGHT (512)

class Game {
  public:
    static GLvoid initialize();
    static GLvoid deinitialize();
    static GLvoid update();
    static GLvoid render();

  private:
    static GLuint viewUniform;
    static GLuint program;
    static GLvoid offsetOrientation(glm::vec3, GLfloat);
    static Camera camera;
    static Octree *octree;
};

#endif // GAME_HPP
