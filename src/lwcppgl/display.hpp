#ifndef LWCPPGL_DISPLAY_HPP
#define LWCPPGL_DISPLAY_HPP

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <SDL/SDL.h>

#include "keyboard.hpp"

class Display {
  public:
    static GLvoid create(GLuint, GLuint);
    static GLvoid update();
    static GLuint shaders(const char *, const char *);

  private:
    static GLvoid shaderlog(GLuint object);
    static std::string source(const GLchar *);
};

#endif // LWCPPGL_DISPLAY_HPP
