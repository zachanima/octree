#include "display.hpp"

GLvoid Display::create(GLuint width, GLuint height) {
  const GLuint BPP = 16;
  const GLuint FLAGS = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_HWACCEL;
  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_WM_SetCaption("Game", 0);
  SDL_SetVideoMode(width, height, BPP, FLAGS);
  glewInit();
}



GLvoid Display::update() {
  Keyboard::update();
  SDL_GL_SwapBuffers();
}



GLuint Display::shaders(const GLchar *vertexFilename, const GLchar *fragmentFilename) {
  const std::string vertexSource = source(vertexFilename);
  const std::string fragmentSource = source(fragmentFilename);
  const GLchar *vertex = vertexSource.c_str();
  const GLchar *fragment = fragmentSource.c_str();

  const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertex, 0);
  glCompileShader(vertexShader);
  shaderlog(vertexShader);

  const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragment, 0);
  glCompileShader(fragmentShader);
  shaderlog(fragmentShader);

  const GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  shaderlog(program);

  return program;
}



GLvoid Display::shaderlog(GLuint object) {
  int infologLength = 0;
  int maxLength;

  if (glIsShader(object)) {
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &maxLength);
  } else {
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &maxLength);
  }

  char *infolog = new char[maxLength];

  if (glIsShader(object)) {
    glGetShaderInfoLog(object, maxLength, &infologLength, infolog);
  } else {
    glGetProgramInfoLog(object, maxLength, &infologLength, infolog);
  }

  if (infologLength > 0) {
    printf("%s\n", infolog);
  }
}



std::string Display::source(const char *filename) {
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in) {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(-1);
}
