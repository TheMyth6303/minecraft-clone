#include "LoggingService/Logger.h"
#include "LoggingService/LoggingUtils.h"

#include <iostream>

Logger::Logger() {}

void Logger::log(const char *message, LogType logType, const char *filePath,
                 int line) {
  std::cout << setLogColor(logType);
  std::cout << "[" << getDateTime() << "] ";
  std::cout << "(" << getLogSource(filePath, line) << ") ";
  std::cout << message << std::endl;
  std::cout << resetLogColor();
};

Logger::~Logger() {}
