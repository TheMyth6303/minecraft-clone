#include <iostream>

#include "LoggingService/Logger.h"

int main(int argc, char const *argv[]) {
  Logger logger;
  Log(logger, "game launched", LOG_INFO);
  return 0;
}
