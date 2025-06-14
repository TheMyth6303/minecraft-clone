#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H

#include "RenderService/IndexBuffer.h"
#include "RenderService/VertexBuffer.h"

#include <cstdint>
#include <unordered_map>

using MeshId = uint32_t;

struct Mesh {
  VertexBuffer *vbo;
  IndexBuffer *ibo;
};

class MeshManager {
private:
  MeshManager();
  MeshManager(MeshManager const &) = delete;
  MeshManager &operator=(const MeshManager &) = delete;
  ~MeshManager();

  static std::unordered_map<MeshId, Mesh> meshes;
  static MeshId nextMeshId;

public:
  static MeshId createMesh();
  static void destroyMesh(MeshId mesh);
  static void setMeshVertexData(MeshId meshId, const void *data,
                                unsigned int count);
  static void setMeshIndexData(MeshId meshId, const unsigned int *data,
                               unsigned int count);
  template <typename T>
  static void pushMeshVertexAttribute(MeshId mesh, unsigned int dimension,
                                      bool normalize);
  static unsigned int getIndexCount(MeshId mesh);

  static void prepareMesh(MeshId mesh);
  static void destroyAllMeshes();
};

#endif