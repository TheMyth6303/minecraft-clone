#include "RenderService/ShaderManager.h"

#include <GLEW/glew.h>
#include <fstream>
#include <sstream>

std::unordered_set<ShaderId> ShaderManager::shaders;
std::unordered_map<ShaderId, std::unordered_map<std::string, UniformId>>
    ShaderManager::uniformCache;

ShaderManager::ShaderManager() {}
ShaderManager::~ShaderManager() {}

std::string ShaderManager::parseShaderFile(const char *filePath) {
  std::ifstream stream(filePath);
  if (!stream.is_open()) {
    return "could not read file";
  }

  std::string line;
  std::stringstream ss;

  while (getline(stream, line)) {
    ss << line << "\n";
  }
  return ss.str();
}

UniformId ShaderManager::getUniform(ShaderId shader, std::string name) {
  auto &innerMap = uniformCache[shader];
  auto it = innerMap.find(name);
  if (it != innerMap.end()) {
    return it->second;
  }
  uniformCache[shader][name] = glGetUniformLocation(shader, name.c_str());
  return uniformCache[shader][name];
}

ShaderId ShaderManager::createShader(const char *vertexShaderPath,
                                     const char *fragmentShaderPath) {
  ShaderId shaderId = glCreateProgram();
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  std::string vertexShaderSourceTemp = parseShaderFile(vertexShaderPath);
  const char *vertexShaderSource = vertexShaderSourceTemp.c_str();
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);
  int vsuccess;
  char vinfoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vsuccess);
  if (!vsuccess) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, vinfoLog);
  }
  glAttachShader(shaderId, vertexShader);

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  std::string fragmentShaderSourceTemp = parseShaderFile(fragmentShaderPath);
  const char *fragmentShaderSource = fragmentShaderSourceTemp.c_str();
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);
  int fsuccess;
  char finfoLog[512];
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fsuccess);
  if (!fsuccess) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, finfoLog);
  }
  glAttachShader(shaderId, fragmentShader);

  glLinkProgram(shaderId);
  glValidateProgram(shaderId);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  shaders.insert(shaderId);
  uniformCache.emplace(shaderId, std::unordered_map<std::string, UniformId>{});

  return shaderId;
}

void ShaderManager::prepareShader(ShaderId shader) { glUseProgram(shader); }

void ShaderManager::setUniform4f(ShaderId shader, const char *uniformName, //
                                 float f1, float f2, float f3, float f4) {
  glUseProgram(shader);
  glUniform4f(glGetUniformLocation(shader, uniformName), f1, f2, f3, f4);
}

void ShaderManager::destroyShader(ShaderId shader) {
  glDeleteBuffers(1, &shader);
  shaders.erase(shader);
  uniformCache.erase(shader);
}

void ShaderManager::destroyAllShaders() {
  for (auto shaderId : shaders) {
    glDeleteProgram(shaderId);
  }
  shaders.clear();
  uniformCache.clear();
}