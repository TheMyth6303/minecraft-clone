#include "WorldManager/VoxelWorld/VoxelWorldRenderer.h"
#include "RenderService/Renderer.h"
#include "WorldManager/VoxelWorld/BlockData.h"

#include <GLM/glm.hpp>

Renderer *renderer = Renderer::getInstance();

/* ############################################################ */
/* CACHE FOR CHUNK MESHES */
/* ############################################################ */
VoxelWorldRenderer::MeshCache::Node::Node(Node *next, Node *prev,
                                          ChunkCoords chunkCoords,
                                          std::vector<float> vertices,
                                          std::vector<unsigned int> indices) {
  this->next = next;
  this->prev = prev;
  this->chunkCoords = chunkCoords;
  this->vertices = vertices;
  this->indices = indices;
};

void VoxelWorldRenderer::MeshCache::setRecent(Node *node) {
  if (node == head) {
    return;
  }
  if (nodeMap.count(node->chunkCoords)) {
    node->prev->next = node->next;
    if (node != tail) {
      node->next->prev = node->prev;
    } else {
      tail = node->prev;
    }
    head->prev = node;
    node->next = head;
    node->prev = nullptr;
    head = node;
  } else {
    if (!count) {
      node->prev = nullptr;
      node->next = nullptr;
      head = node;
      tail = node;
    } else {
      node->prev = nullptr;
      node->next = head;
      head->prev = node;
      head = node;
    }
    count++;
  }
}

void VoxelWorldRenderer::MeshCache::deleteOldest() {
  nodeMap.erase(tail->chunkCoords);
  tail = tail->prev;
  delete tail->next;
  tail->next = nullptr;
  count--;
}

VoxelWorldRenderer::MeshCache::MeshCache() {
  head = nullptr;
  tail = nullptr;
  count = 0;
}

VoxelWorldRenderer::MeshCache::~MeshCache() {
  for (auto it = nodeMap.begin(); it != nodeMap.end(); ++it) {
    delete it->second;
  }
  nodeMap.clear();
}

bool VoxelWorldRenderer::MeshCache::hasMeshData(int chunkX, int chunkY) {
  return nodeMap.count(std::make_pair(chunkX, chunkY)) == 1;
}

void VoxelWorldRenderer::MeshCache::updateMeshData(
    int chunkX, int chunkY, //
    std::vector<float> vertexData, std::vector<unsigned int> indexData) {
  ChunkCoords chunkCoords = std::make_pair(chunkX, chunkY);
  if (!nodeMap.count(chunkCoords)) {
    Node *node = new Node(nullptr, nullptr, chunkCoords, vertexData, indexData);
    setRecent(node);
    nodeMap.insert({chunkCoords, node});
    if (count > maxCount) {
      deleteOldest();
    }
  } else {
    Node *node = nodeMap[chunkCoords];
    setRecent(node);
  }
}

std::pair<std::vector<float> &, std::vector<unsigned int> &>
VoxelWorldRenderer::MeshCache::getMeshData(int chunkX, int chunkY) {
  ChunkCoords chunkCoords = std::make_pair(chunkX, chunkY);
  setRecent(nodeMap[chunkCoords]);
  auto &node = nodeMap[chunkCoords];
  return {node->vertices, node->indices};
}

/* ############################################################ */
/* ADDING FACES TO MESH VECTORS */
/* ############################################################ */

void addNegXFace(std::vector<float> &vertexData, unsigned int &vertexCount,
                 std::vector<unsigned int> &indexData, unsigned int &indexCount,
                 float blockX, float blockY, float blockZ, BlockName block) {
  vertexData.insert(vertexData.end(),
                    {blockX, blockY + 1.0f, blockZ, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0],
                     Block[block].textureLocation.sideFaceUV[1]});
  vertexData.insert(vertexData.end(),
                    {blockX, blockY, blockZ, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0],
                     Block[block].textureLocation.sideFaceUV[1] + 0.125f});
  vertexData.insert(vertexData.end(),
                    {blockX, blockY, blockZ + 1.0f, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0] + 0.125f,
                     Block[block].textureLocation.sideFaceUV[1]});
  vertexData.insert(vertexData.end(),
                    {blockX, blockY + 1.0f, blockZ + 1.0f, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0] + 0.125f,
                     Block[block].textureLocation.sideFaceUV[1] + 0.125f});
  indexData.insert(indexData.end(),
                   {vertexCount + 0, vertexCount + 1, vertexCount + 2,
                    vertexCount + 2, vertexCount + 3, vertexCount + 0});
  vertexCount += 4;
  indexCount += 6;
}

void addPosXFace(std::vector<float> &vertexData, unsigned int &vertexCount,
                 std::vector<unsigned int> &indexData, unsigned int &indexCount,
                 float blockX, float blockY, float blockZ, BlockName block) {
  vertexData.insert(vertexData.end(),
                    {blockX + 1.0f, blockY, blockZ, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0],
                     Block[block].textureLocation.sideFaceUV[1]});
  vertexData.insert(vertexData.end(),
                    {blockX + 1.0f, blockY + 1.0f, blockZ, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0],
                     Block[block].textureLocation.sideFaceUV[1] + 0.125f});
  vertexData.insert(vertexData.end(),
                    {blockX + 1.0f, blockY + 1.0f, blockZ + 1.0f, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0] + 0.125f,
                     Block[block].textureLocation.sideFaceUV[1]});
  vertexData.insert(vertexData.end(),
                    {blockX + 1.0f, blockY, blockZ + 1.0f, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0] + 0.125f,
                     Block[block].textureLocation.sideFaceUV[1] + 0.125f});
  indexData.insert(indexData.end(),
                   {vertexCount + 0, vertexCount + 1, vertexCount + 2,
                    vertexCount + 2, vertexCount + 3, vertexCount + 0});
  vertexCount += 4;
  indexCount += 6;
}

void addNegYFace(std::vector<float> &vertexData, unsigned int &vertexCount,
                 std::vector<unsigned int> &indexData, unsigned int &indexCount,
                 float blockX, float blockY, float blockZ, BlockName block) {
  vertexData.insert(vertexData.end(),
                    {blockX, blockY, blockZ, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0],
                     Block[block].textureLocation.sideFaceUV[1]});
  vertexData.insert(vertexData.end(),
                    {blockX + 1.0f, blockY, blockZ, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0],
                     Block[block].textureLocation.sideFaceUV[1] + 0.125f});
  vertexData.insert(vertexData.end(),
                    {blockX + 1.0f, blockY, blockZ + 1.0f, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0] + 0.125f,
                     Block[block].textureLocation.sideFaceUV[1]});
  vertexData.insert(vertexData.end(),
                    {blockX, blockY, blockZ + 1.0f, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0] + 0.125f,
                     Block[block].textureLocation.sideFaceUV[1] + 0.125f});
  indexData.insert(indexData.end(),
                   {vertexCount + 0, vertexCount + 1, vertexCount + 2,
                    vertexCount + 2, vertexCount + 3, vertexCount + 0});
  vertexCount += 4;
  indexCount += 6;
}

void addPosYFace(std::vector<float> &vertexData, unsigned int &vertexCount,
                 std::vector<unsigned int> &indexData, unsigned int &indexCount,
                 float blockX, float blockY, float blockZ, BlockName block) {
  vertexData.insert(vertexData.end(),
                    {blockX + 1.0f, blockY + 1.0f, blockZ, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0],
                     Block[block].textureLocation.sideFaceUV[1]});
  vertexData.insert(vertexData.end(),
                    {blockX, blockY + 1.0f, blockZ, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0],
                     Block[block].textureLocation.sideFaceUV[1] + 0.125f});
  vertexData.insert(vertexData.end(),
                    {blockX, blockY + 1.0f, blockZ + 1.0f, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0] + 0.125f,
                     Block[block].textureLocation.sideFaceUV[1]});
  vertexData.insert(vertexData.end(),
                    {blockX + 1.0f, blockY + 1.0f, blockZ + 1.0f, 1.0f,
                     Block[block].textureLocation.sideFaceUV[0] + 0.125f,
                     Block[block].textureLocation.sideFaceUV[1] + 0.125f});
  indexData.insert(indexData.end(),
                   {vertexCount + 0, vertexCount + 1, vertexCount + 2,
                    vertexCount + 2, vertexCount + 3, vertexCount + 0});
  vertexCount += 4;
  indexCount += 6;
}

void addNegZFace(std::vector<float> &vertexData, unsigned int &vertexCount,
                 std::vector<unsigned int> &indexData, unsigned int &indexCount,
                 float blockX, float blockY, float blockZ, BlockName block) {
  vertexData.insert(vertexData.end(),
                    {blockX + 1.0f, blockY + 1.0f, blockZ, 1.0f,
                     Block[block].textureLocation.bottomFaceUV[0],
                     Block[block].textureLocation.bottomFaceUV[1]});
  vertexData.insert(vertexData.end(),
                    {blockX + 1.0f, blockY, blockZ, 1.0f,
                     Block[block].textureLocation.bottomFaceUV[0],
                     Block[block].textureLocation.bottomFaceUV[1] + 0.125f});
  vertexData.insert(vertexData.end(),
                    {blockX, blockY, blockZ, 1.0f,
                     Block[block].textureLocation.bottomFaceUV[0] + 0.125f,
                     Block[block].textureLocation.bottomFaceUV[1]});
  vertexData.insert(vertexData.end(),
                    {blockX, blockY + 1.0f, blockZ, 1.0f,
                     Block[block].textureLocation.bottomFaceUV[0] + 0.125f,
                     Block[block].textureLocation.bottomFaceUV[1] + 0.125f});
  indexData.insert(indexData.end(),
                   {vertexCount + 0, vertexCount + 1, vertexCount + 2,
                    vertexCount + 2, vertexCount + 3, vertexCount + 0});
  vertexCount += 4;
  indexCount += 6;
}

void addPosZFace(std::vector<float> &vertexData, unsigned int &vertexCount,
                 std::vector<unsigned int> &indexData, unsigned int &indexCount,
                 float blockX, float blockY, float blockZ, BlockName block) {
  vertexData.insert(vertexData.end(),
                    {blockX, blockY + 1.0f, blockZ + 1.0f, 1.0f,
                     Block[block].textureLocation.topFaceUV[0],
                     Block[block].textureLocation.topFaceUV[1]});
  vertexData.insert(vertexData.end(),
                    {blockX, blockY, blockZ + 1.0f, 1.0f,
                     Block[block].textureLocation.topFaceUV[0],
                     Block[block].textureLocation.topFaceUV[1] + 0.125f});
  vertexData.insert(vertexData.end(),
                    {blockX + 1.0f, blockY, blockZ + 1.0f, 1.0f,
                     Block[block].textureLocation.topFaceUV[0] + 0.125f,
                     Block[block].textureLocation.topFaceUV[1]});
  vertexData.insert(vertexData.end(),
                    {blockX + 1.0f, blockY + 1.0f, blockZ + 1.0f, 1.0f,
                     Block[block].textureLocation.topFaceUV[0] + 0.125f,
                     Block[block].textureLocation.topFaceUV[1] + 0.125f});
  indexData.insert(indexData.end(),
                   {vertexCount + 0, vertexCount + 1, vertexCount + 2,
                    vertexCount + 2, vertexCount + 3, vertexCount + 0});
  vertexCount += 4;
  indexCount += 6;
}

/* ############################################################ */
/* ACTUAL VOXEL RENDERER CODE */
/* ############################################################ */

VoxelWorldRenderer::VoxelWorldRenderer() {
  chunkManager = ChunkManager::getInstance();
  mesh = renderer->createMesh();
  renderer->pushMeshVertexAttribute<float>(mesh, 4, false);
  renderer->pushMeshVertexAttribute<float>(mesh, 2, false);
  shader = renderer->createShader("resources/shaders/vertex.shader",
                                  "resources/shaders/fragment.shader");
  renderer->setTexture(shader, "textureAtlas", "resources/textures/blocks.png");
}

VoxelWorldRenderer::~VoxelWorldRenderer() {}

void VoxelWorldRenderer::setTransformMatrices(float playerX, float playerY,
                                              float playerZ, float theta,
                                              float phi) {
  projectionMatrix = glm::mat4(                         //
      0, 0, LAMBDA, 1,                                  //
      -1.0f / tan(FOV * M_PI / 360.0), 0, 0, 0,         //
      0, tan(FOV * M_PI / 360.0f) / ASPECT_RATIO, 0, 0, //
      0, 0, -LAMBDA * D_NEAR, 0);
  translationMatrix = glm::mat4( //
      1, 0, 0, 0,                //
      0, 1, 0, 0,                //
      0, 0, 1, 0,                //
      -playerX, -playerY, -playerZ, 1);
  rotationThetaMatrix = glm::mat4(                                   //
      cos(theta * M_PI / 180.0f), -sin(theta * M_PI / 180.0f), 0, 0, //
      sin(theta * M_PI / 180.0f), cos(theta * M_PI / 180.0f), 0, 0,  //
      0, 0, 1, 0,                                                    //
      0, 0, 0, 1);
  rotationPhiMatrix = glm::mat4(                                 //
      cos(phi * M_PI / 180.0f), 0, -sin(phi * M_PI / 180.0f), 0, //
      0, 1, 0, 0,                                                //
      sin(phi * M_PI / 180.0f), 0, cos(phi * M_PI / 180.0f), 0,  //
      0, 0, 0, 1);

  renderer->setUniformMat4f(shader, "projectionMatrix", projectionMatrix);
  renderer->setUniformMat4f(shader, "translationMatrix", translationMatrix);
  renderer->setUniformMat4f(shader, "rotationThetaMatrix", rotationThetaMatrix);
  renderer->setUniformMat4f(shader, "rotationPhiMatrix", rotationPhiMatrix);
}

void VoxelWorldRenderer::renderChunk(int chunkX, int chunkY) {
  if (!meshCache.hasMeshData(chunkX, chunkY)) {
    updateChunkMesh(chunkX, chunkY);
  }
  std::pair<std::vector<float> &, std::vector<unsigned int> &> meshData =
      meshCache.getMeshData(chunkX, chunkY);
  renderer->setMeshVertexData(mesh, meshData.first.data(),
                              meshData.first.size() / 6);
  renderer->setMeshIndexData(mesh, meshData.second.data(),
                             meshData.second.size());
  renderer->draw(mesh, shader);
}

void VoxelWorldRenderer::render(float playerX, float playerY,
                                float playerZ, //
                                float theta, float phi) {
  setTransformMatrices(playerX, playerY, playerZ, theta, phi);
  int centralChunkX = (int)playerX / CHUNKSIZEX;
  int centralChunkY = (int)playerY / CHUNKSIZEY;
  for (int i = -RENDER_DISTANCE; i <= RENDER_DISTANCE; i++) {
    for (int j = -RENDER_DISTANCE; j <= RENDER_DISTANCE; j++) {
      renderChunk(centralChunkX + i, centralChunkY + j);
    }
  }
}

void VoxelWorldRenderer::updateChunkMesh(int chunkX, int chunkY) {
  std::vector<float> vertexData;
  std::vector<unsigned int> indexData;

  unsigned int vertexCount = 0;
  unsigned int indexCount = 0;

  Chunk *chunk = chunkManager->getChunk(chunkX, chunkY);

  Chunk *negXChunk = chunkManager->getChunk(chunkX - 1, chunkY);
  Chunk *posXChunk = chunkManager->getChunk(chunkX + 1, chunkY);
  Chunk *negYChunk = chunkManager->getChunk(chunkX, chunkY - 1);
  Chunk *posYChunk = chunkManager->getChunk(chunkX, chunkY + 1);

  for (int i = 0; i < CHUNKSIZEX; i++) {
    for (int j = 0; j < CHUNKSIZEY; j++) {
      for (int k = 0; k < CHUNKSIZEZ; k++) {
        BlockName block = (BlockName)chunk->blocks[i][j][k];
        if (Block[block].isOpaque) {
          float blockX = (float)(chunkX * CHUNKSIZEX + i);
          float blockY = (float)(chunkY * CHUNKSIZEY + j);
          float blockZ = (float)(k);

          // negative x faces
          if (i != 0 && !Block[chunk->blocks[i - 1][j][k]].isOpaque) {
            addNegXFace(vertexData, vertexCount, indexData, indexCount, blockX,
                        blockY, blockZ, block);
          } else if (i == 0 &&
                     !Block[negXChunk->blocks[CHUNKSIZEX - 1][j][k]].isOpaque) {
            addNegXFace(vertexData, vertexCount, indexData, indexCount, blockX,
                        blockY, blockZ, block);
          }
          // positive x faces
          if (i != CHUNKSIZEX - 1 &&
              !Block[chunk->blocks[i + 1][j][k]].isOpaque) {
            addPosXFace(vertexData, vertexCount, indexData, indexCount, blockX,
                        blockY, blockZ, block);
          } else if (i == CHUNKSIZEX - 1 &&
                     !Block[posXChunk->blocks[0][j][k]].isOpaque) {
            addPosXFace(vertexData, vertexCount, indexData, indexCount, blockX,
                        blockY, blockZ, block);
          }

          // negative y faces
          if (j != 0 && !Block[chunk->blocks[i][j - 1][k]].isOpaque) {
            addNegYFace(vertexData, vertexCount, indexData, indexCount, blockX,
                        blockY, blockZ, block);
          } else if (j == 0 &&
                     !Block[negYChunk->blocks[i][CHUNKSIZEY - 1][k]].isOpaque) {
            addNegYFace(vertexData, vertexCount, indexData, indexCount, blockX,
                        blockY, blockZ, block);
          }
          // positive y faces
          if (j != CHUNKSIZEY - 1 &&
              !Block[chunk->blocks[i][j + 1][k]].isOpaque) {
            addPosYFace(vertexData, vertexCount, indexData, indexCount, blockX,
                        blockY, blockZ, block);
          } else if (j == CHUNKSIZEY - 1 &&
                     !Block[posYChunk->blocks[i][0][k]].isOpaque) {
            addPosYFace(vertexData, vertexCount, indexData, indexCount, blockX,
                        blockY, blockZ, block);
          }

          // negative z faces
          if (k != 0 && !Block[chunk->blocks[i][j][k - 1]].isOpaque) {
            addNegZFace(vertexData, vertexCount, indexData, indexCount, blockX,
                        blockY, blockZ, block);
          } else if (k == 0) {
            addNegZFace(vertexData, vertexCount, indexData, indexCount, blockX,
                        blockY, blockZ, block);
          }

          // positive z faces
          if (k != CHUNKSIZEZ && !Block[chunk->blocks[i][j][k + 1]].isOpaque) {
            addPosZFace(vertexData, vertexCount, indexData, indexCount, blockX,
                        blockY, blockZ, block);
          } else if (k == CHUNKSIZEZ) {
            addPosZFace(vertexData, vertexCount, indexData, indexCount, blockX,
                        blockY, blockZ, block);
          }
        }
      }
    }
  }

  meshCache.updateMeshData(chunkX, chunkY, vertexData, indexData);
};