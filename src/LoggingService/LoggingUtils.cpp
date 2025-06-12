#include "LoggingService/LoggingUtils.h"

#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string getDateTime() {
  time_t timestamp = time(&timestamp);
  struct tm datetime = *localtime(&timestamp);

  std::ostringstream oss;
  oss << std::setw(4) << std::setfill('0') << datetime.tm_year + 1900 << "/";
  oss << std::setw(2) << std::setfill('0') << datetime.tm_mon + 1 << "/";
  oss << std::setw(2) << std::setfill('0') << datetime.tm_mday << " ";
  oss << std::setw(2) << std::setfill('0') << datetime.tm_hour << ":";
  oss << std::setw(2) << std::setfill('0') << datetime.tm_min << ":";
  oss << std::setw(2) << std::setfill('0') << datetime.tm_sec;

  return oss.str();
}

std::string getLogSource(const char *filePath, int line) {
  const char *filename = std::strrchr(filePath, '/');
  if (!filename) {
    filename = std::strrchr(filePath, '\\');
  }
  std::ostringstream linenumber;
  linenumber << std::setw(3) << std::setfill('0') << line;
  return std::string(filename ? filename + 1 : filePath) + ":" +
         linenumber.str();
};

std::string setLogColor(LogType logType) {
  switch (logType) {
  case LOG_INFO:
    return "";
  case LOG_WARN:
    return "\033[33m";
  case LOG_ERRR:
    return "\033[31m";
  default:
    break;
  }
}

std::string resetLogColor() { return "\033[0m"; };