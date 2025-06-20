#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "RenderService/TextureManager.h"

#include <GLM/glm.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using ShaderId = uint32_t;
using UniformId = uint32_t;

class ShaderManager {
private:
  ShaderManager();
  ShaderManager(ShaderManager const &) = delete;
  ShaderManager &operator=(const ShaderManager &) = delete;
  ~ShaderManager();

  static std::string parseShaderFile(const char *filePath);
  static UniformId getUniformLocation(ShaderId shader, std::string name);
  static int getTextureSlot(ShaderId shader, std::string name);
  static void setTextureSlot(ShaderId shader, int slot, TextureId texture);

  static std::unordered_set<ShaderId> shaders;
  static std::unordered_map<ShaderId,
                            std::unordered_map<std::string, UniformId>>
      uniformCache;

  static std::unordered_map<ShaderId, std::vector<std::string>> textureNames;
  static std::unordered_map<ShaderId, std::vector<TextureId>> textureIds;

public:
  static ShaderId createShader(const char *vertexShaderPath,
                               const char *fragmentShaderPath);
  static void prepareShader(ShaderId shader);

  static void setTexture(ShaderId shader, const char *uniformName,
                         const char *texturePath);

  static void setUniform1f(ShaderId shader, const char *uniformName, float f);
  static void setUniform4f(ShaderId shader, const char *uniformName, //
                           float f1, float f2, float f3, float f4);
  static void setUniformMat4f(ShaderId shader, const char *uniformName,
                              glm::mat4 &mat4f);

  static void destroyShader(ShaderId shader);
  static void destroyAllShaders();
};

#endif