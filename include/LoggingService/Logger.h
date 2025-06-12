#ifndef LOGGER_H
#define LOGGER_H

#include "LoggingService/LoggingUtils.h"

class Logger {
private:
public:
  Logger();
  void log(const char *message, LogType logType, const char *filePath,
           int line);
  ~Logger();
};

#define Log(logger, message, logType)                                          \
  logger.log(message, logType, __FILE__, __LINE__)

#endif