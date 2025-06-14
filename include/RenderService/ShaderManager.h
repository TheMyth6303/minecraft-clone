#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>

using ShaderId = uint32_t;
using UniformId = uint32_t;

class ShaderManager {
private:
  ShaderManager();
  ShaderManager(ShaderManager const &) = delete;
  ShaderManager &operator=(const ShaderManager &) = delete;
  ~ShaderManager();

  static std::string parseShaderFile(const char *filePath);
  static UniformId getUniform(ShaderId shader, std::string name);

  static std::unordered_set<ShaderId> shaders;
  static std::unordered_map<ShaderId,
                            std::unordered_map<std::string, UniformId>>
      uniformCache;

public:
  static ShaderId createShader(const char *vertexShaderPath,
                               const char *fragmentShaderPath);
  static void prepareShader(ShaderId shader);

  static void setUniform4f(ShaderId shader, const char *uniformName, //
                           float f1, float f2, float f3, float f4);

  static void destroyShader(ShaderId shader);
  static void destroyAllShaders();
};

#endif