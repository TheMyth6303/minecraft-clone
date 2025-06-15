#include "RenderService/ShaderManager.h"

#include <GLEW/glew.h>
#include <fstream>
#include <sstream>

std::unordered_set<ShaderId> ShaderManager::shaders;
std::unordered_map<ShaderId, std::unordered_map<std::string, UniformId>>
    ShaderManager::uniformCache;

std::unordered_map<ShaderId, std::vector<std::string>>
    ShaderManager::textureNames;
std::unordered_map<ShaderId, std::vector<TextureId>> ShaderManager::textureIds;

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

UniformId ShaderManager::getUniformLocation(ShaderId shader, std::string name) {
  auto &innerMap = uniformCache[shader];
  auto it = innerMap.find(name);
  if (it != innerMap.end()) {
    return it->second;
  }
  uniformCache[shader][name] = glGetUniformLocation(shader, name.c_str());
  return uniformCache[shader][name];
}

int ShaderManager::getTextureSlot(ShaderId shader, std::string name) {
  int i = 0;
  for (i; i < textureNames[shader].size(); i++) {
    if (textureNames[shader][i] == name) {
      return i;
    }
  }
  textureNames[shader].push_back(name);
  return i;
}

void ShaderManager::setTextureSlot(ShaderId shader, int slot,
                                   TextureId texture) {
  if (slot == textureIds[shader].size()) {
    textureIds[shader].push_back(texture);
  } else if (slot < textureIds[shader].size()) {
    textureIds[shader][slot] = texture;
  }
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
  textureNames.emplace(shaderId, std::vector<std::string>{});
  textureIds.emplace(shaderId, std::vector<TextureId>{});

  return shaderId;
}

void ShaderManager::prepareShader(ShaderId shader) {
  glUseProgram(shader);
  for (int slot = 0; slot < textureIds[shader].size(); slot++) {
    TextureManager::prepareTexture(textureIds[shader][slot], slot);
    glUniform1i(getUniformLocation(shader, textureNames[shader][slot]), slot);
  }
}

void ShaderManager::setTexture(ShaderId shader, const char *uniformName,
                               const char *texturePath) {
  TextureId texture = TextureManager::createTexture(texturePath);
  int slot = getTextureSlot(shader, uniformName);
  setTextureSlot(shader, slot, texture);
}

void ShaderManager::setUniform1f(ShaderId shader, const char *uniformName,
                                 float f) {
  glUseProgram(shader);
  glUniform1f(getUniformLocation(shader, uniformName), f);
}

void ShaderManager::setUniform4f(ShaderId shader, const char *uniformName, //
                                 float f1, float f2, float f3, float f4) {
  glUseProgram(shader);
  glUniform4f(getUniformLocation(shader, uniformName), f1, f2, f3, f4);
}

void ShaderManager::destroyShader(ShaderId shader) {
  glDeleteBuffers(1, &shader);
  shaders.erase(shader);
  uniformCache.erase(shader);
  textureIds.erase(shader);
  textureNames.erase(shader);
}

void ShaderManager::destroyAllShaders() {
  TextureManager::destroyAllTextures();
  for (auto shaderId : shaders) {
    glDeleteProgram(shaderId);
  }
  shaders.clear();
  uniformCache.clear();
  textureIds.clear();
  textureNames.clear();
}