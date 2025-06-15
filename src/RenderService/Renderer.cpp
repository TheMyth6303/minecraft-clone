#include "RenderService/Renderer.h"
#include "RenderService/MeshManager.h"
#include "RenderService/ShaderManager.h"

#include <GLEW/glew.h>

Renderer *Renderer::instance = nullptr;
Logger Renderer::logger;

Renderer::Renderer() {};
Renderer::~Renderer() {};

Renderer *Renderer::getInstance() {
  if (!instance) {
    instance = new Renderer();
  }
  return instance;
};

bool Renderer::initialize() {
  if (glewInit() != GLEW_OK) {
    Log(logger, "could not initialize opengl.", LOG_ERRR);
    return false;
  }
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  Log(logger, "renderer initialized successfully.", LOG_INFO);
  return true;
};

void Renderer::destroy() {
  MeshManager::destroyAllMeshes();
  ShaderManager::destroyAllShaders();
};

MeshId Renderer::createMesh() { return MeshManager::createMesh(); };

template <>
void Renderer::pushMeshVertexAttribute<float>(MeshId meshId,
                                              unsigned int dimension,
                                              bool normalize) {
  MeshManager::pushMeshVertexAttribute<float>(meshId, dimension, normalize);
}
template <>
void Renderer::pushMeshVertexAttribute<unsigned int>(MeshId meshId,
                                                     unsigned int dimension,
                                                     bool normalize) {
  MeshManager::pushMeshVertexAttribute<unsigned int>(meshId, dimension,
                                                     normalize);
}

void Renderer::setMeshVertexData(MeshId mesh, void *data, unsigned int count) {
  MeshManager::setMeshVertexData(mesh, data, count);
};

void Renderer::setMeshIndexData(MeshId mesh, unsigned int *data,
                                unsigned int count) {
  MeshManager::setMeshIndexData(mesh, data, count);
};

void Renderer::destroyMesh(MeshId mesh) { MeshManager::destroyMesh(mesh); };

ShaderId Renderer::createShader(const char *vertexShaderPath,
                                const char *fragmentShaderPath) {
  return ShaderManager::createShader(vertexShaderPath, fragmentShaderPath);
}

void Renderer::destroyShader(ShaderId shader) {
  ShaderManager::destroyShader(shader);
};

void Renderer::setTexture(ShaderId shader, const char *textureName,
                          const char *texturePath) {
  ShaderManager::setTexture(shader, textureName, texturePath);
}

void Renderer::setUniform1f(ShaderId shader, const char *uniformName, float f) {
  ShaderManager::setUniform1f(shader, uniformName, f);
}

void Renderer::setUniform4f(ShaderId shader, const char *uniformName, //
                            float f1, float f2, float f3, float f4) {
  ShaderManager::setUniform4f(shader, uniformName, f1, f2, f3, f4);
}

void Renderer::draw(MeshId mesh, ShaderId shader) {
  MeshManager::prepareMesh(mesh);
  ShaderManager::prepareShader(shader);
  glDrawElements(GL_TRIANGLES, MeshManager::getIndexCount(mesh),
                 GL_UNSIGNED_INT, nullptr);
}