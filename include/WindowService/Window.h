#ifndef WINDOW_H
#define WINDOW_H

#include "LoggingService/Logger.h"
#include <GLFW/glfw3.h>

class Window {
private:
  GLFWwindow *window;
  Logger logger;

public:
  Window();
  bool initialize(const char *title, int width, int height, bool fullscreen);
  bool shouldClose();
  void setClearColor(float r, float g, float b, float a);
  void clear();
  void swapBuffers();
  void pollEvents();
  void destroy();
  ~Window();
};

#endif