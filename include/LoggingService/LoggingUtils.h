#ifndef LOGGER_UTILS_H
#define LOGGER_UTILS_H

#include "LoggingService/LogType.h"

#include <string>

std::string getDateTime();
std::string getLogSource(const char *filePath, int line);
std::string setLogColor(LogType logType);
std::string resetLogColor();

#endif