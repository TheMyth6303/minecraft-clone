#include <iostream>

#include "LoggingService/Logger.h"
#include "WindowService/Window.h"

main(int argc, char const *argv[]) {
  Logger logger;
  Log(logger, "game launched", LOG_INFO);

  Window window1;
  window1.initialize("window1", 640, 480, false);
  Window window2;
  window2.initialize("window2", 640, 640, false);

  window1.setClearColor(1.0, 0.5, 0.5, 1.0);
  window2.setClearColor(0.5, 1.0, 0.5, 1.0);

  while (!window1.shouldClose() && !window2.shouldClose()) {
    window1.clear();
    window1.swapBuffers();
    window1.pollEvents();
    window2.clear();
    window2.swapBuffers();
    window2.pollEvents();
  }

  window1.destroy();
  window2.destroy();

  return 0;
}
