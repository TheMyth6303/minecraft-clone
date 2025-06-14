#include "RenderService/MeshManager.h"

#include <GLEW/glew.h>

std::unordered_map<MeshId, Mesh> MeshManager::meshes;
MeshId MeshManager::nextMeshId = 1;

MeshManager::MeshManager() {};
MeshManager::~MeshManager() {};

MeshId MeshManager::createMesh() {
  MeshId meshId = nextMeshId;
  nextMeshId++;
  VertexBuffer *vbo = new VertexBuffer();
  IndexBuffer *ibo = new IndexBuffer();
  Mesh mesh{vbo, ibo};
  meshes.emplace(meshId, mesh);
  return meshId;
};

void MeshManager::destroyMesh(MeshId meshId) {
  delete meshes[meshId].vbo;
  delete meshes[meshId].ibo;
  meshes.erase(meshId);
}

void MeshManager::setMeshVertexData(MeshId meshId, const void *data,
                                    unsigned int count) {
  meshes[meshId].vbo->loadData(data, count);
}

void MeshManager::setMeshIndexData(MeshId meshId, const unsigned int *data,
                                   unsigned int count) {
  meshes[meshId].ibo->loadData(data, count);
}

template <>
void MeshManager::pushMeshVertexAttribute<float>(MeshId meshId,
                                                 unsigned int dimension,
                                                 bool normalize) {
  meshes[meshId].vbo->pushAttribute<float>(dimension, normalize);
}
template <>
void MeshManager::pushMeshVertexAttribute<unsigned int>(MeshId meshId,
                                                        unsigned int dimension,
                                                        bool normalize) {
  meshes[meshId].vbo->pushAttribute<unsigned int>(dimension, normalize);
}

unsigned int MeshManager::getIndexCount(MeshId meshId) {
  return meshes[meshId].ibo->getCount();
}

void MeshManager::prepareMesh(MeshId meshId) {
  meshes[meshId].vbo->bind();
  meshes[meshId].vbo->bindAttributes();
  meshes[meshId].ibo->bind();
}

void MeshManager::destroyAllMeshes() {
  for (auto it = meshes.begin(); it != meshes.end(); ++it) {
    delete meshes[it->first].vbo;
    delete meshes[it->first].ibo;
  }
  meshes.clear();
}
