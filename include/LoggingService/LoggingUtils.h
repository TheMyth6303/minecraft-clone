#ifndef LOGGER_UTILS_H
#define LOGGER_UTILS_H

#include <string>

enum LogType { LOG_INFO = 0, LOG_WARN = 1, LOG_ERRR = 2 };

std::string getDateTime();
std::string getLogSource(const char *filePath, int line);
std::string setLogColor(LogType logType);
std::string resetLogColor();

#endif