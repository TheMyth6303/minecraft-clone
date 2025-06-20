#ifndef VOXEL_WORLD_RENDERER_H
#define VOXEL_WORLD_RENDERER_H

#include "WorldManager/VoxelWorld/ChunkManager.h"
#include "WorldManager/VoxelWorld/Commons.h"

#include <GLM/glm.hpp>
#include <array>
#include <unordered_map>
#include <utility>
#include <vector>

class VoxelWorldRenderer {
private:
  class MeshCache {
  private:
    class Node {
    public:
      Node *next;
      Node *prev;
      ChunkCoords chunkCoords;
      std::vector<float> vertices;
      std::vector<unsigned int> indices;

      Node(Node *next, Node *prev, ChunkCoords chunkCoords,
           std::vector<float> vertices, std::vector<unsigned int> indices);
    };

    Node *head;
    Node *tail;
    std::unordered_map<ChunkCoords, Node *, PairHash> nodeMap;

    int maxCount = (2 * RENDER_DISTANCE + 2) * (2 * RENDER_DISTANCE + 2);
    int count;

    void setRecent(Node *node);
    void deleteOldest();

  public:
    MeshCache();
    ~MeshCache();
    bool hasMeshData(int chunkX, int chunkY);
    void updateMeshData(int chunkX, int chunkY, //
                        std::vector<float> vertexData,
                        std::vector<unsigned int> indexData);
    std::pair<std::vector<float> &, std::vector<unsigned int> &>
    getMeshData(int chunkX, int chunkY);
  };

  ChunkManager *chunkManager;

  unsigned int shader;
  unsigned int mesh;

  glm::mat4 projectionMatrix;
  glm::mat4 translationMatrix;
  glm::mat4 rotationThetaMatrix;
  glm::mat4 rotationPhiMatrix;

  void setTransformMatrices(float playerX, float playerY, float playerZ,
                            float theta, float phi);
  void renderChunk(int chunkX, int chunkY);

public:
  MeshCache meshCache;
  VoxelWorldRenderer();
  ~VoxelWorldRenderer();
  void updateChunkMesh(int chunkX, int chunkY);
  void render(float playerX, float playerY, float playerZ, //
              float theta, float phi);
};

#endif