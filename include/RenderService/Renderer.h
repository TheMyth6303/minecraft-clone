#ifndef RENDERER_H
#define RENDERER_H

#include "LoggingService/Logger.h"

#include <GLM/glm.hpp>
#include <cstdint>
#include <string>

using MeshId = uint32_t;   // vbo, ibo
using ShaderId = uint32_t; // shader

class Renderer {
private:
  Renderer();
  Renderer(Renderer const &) = delete;
  Renderer &operator=(const Renderer &) = delete;
  ~Renderer();

  static Renderer *instance;
  static Logger logger;

  ShaderId currentShader = 0;

public:
  static Renderer *getInstance();
  static bool initialize();
  void destroy();

  MeshId createMesh();
  template <typename T>
  void pushMeshVertexAttribute(MeshId mesh, unsigned int dimension,
                               bool normalize);
  void setMeshVertexData(MeshId mesh, void *data, unsigned int count);
  void setMeshIndexData(MeshId mesh, unsigned int *data, unsigned int count);
  void destroyMesh(MeshId mesh);

  ShaderId createShader(const char *vertexShaderPath,
                        const char *fragmentShaderPath);
  void destroyShader(ShaderId shader);

  void setTexture(ShaderId shader, const char *textureName,
                  const char *texturePath);

  void setUniform1f(ShaderId shader, const char *uniformName, float f);
  void setUniform4f(ShaderId shader, const char *uniformName, //
                    float f1, float f2, float f3, float f4);
  void setUniformMat4f(ShaderId shader, const char *uniformName,
                       glm::mat4 &matrix);

  void draw(MeshId mesh, ShaderId shader);
};

#endif