#include "WindowService/Window.h"

Window::Window() : window(nullptr), logger() {};

bool Window::initialize(const char *title, int width, int height,
                        bool fullscreen) {
  if (!glfwInit()) {
    Log(logger, "could not initialize glfw.", LOG_ERRR);
    return false;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  Log(logger, "glfw initialized.", LOG_INFO);

  window = glfwCreateWindow(width, height, title,
                            fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
  if (!window) {
    Log(logger, "could not create window.", LOG_ERRR);
    glfwTerminate();
    return false;
  }
  Log(logger, "window created.", LOG_INFO);

  return true;
};

bool Window::shouldClose() { return glfwWindowShouldClose(window); }

void Window::setClearColor(float r, float g, float b, float a) {
  glfwMakeContextCurrent(window);
  glClearColor(r, g, b, a);
};

void Window::clear() {
  if (glfwGetCurrentContext() != window) {
    glfwMakeContextCurrent(window);
  }
  glClear(GL_COLOR_BUFFER_BIT);
};

void Window::swapBuffers() { glfwSwapBuffers(window); }

void Window::pollEvents() { glfwPollEvents(); }

void Window::destroy() {
  glfwTerminate();
  Log(logger, "window destroyed.", LOG_INFO);
};

void Window::makeCurrentRenderingContext() {
  if (glfwGetCurrentContext() != window) {
    glfwMakeContextCurrent(window);
  }
}

Window::~Window() {};
