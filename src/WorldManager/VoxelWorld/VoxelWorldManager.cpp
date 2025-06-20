#include "WorldManager/VoxelWorld/VoxelWorldManager.h"

#include <GLEW/glew.h>

VoxelWorldManager *VoxelWorldManager::instance = nullptr;

void VoxelWorldManager::initBlockData() {
  Block[AIR] = BlockData{};
  Block[AIR].isOpaque = false;
  Block[AIR].textureLocation = {{}, {}, {}};

  Block[DIRT] = BlockData{};
  Block[DIRT].isOpaque = true;
  Block[DIRT].textureLocation = {
      {0.125f, 0.875f}, {0.000f, 0.875f}, {0.000f, 0.875f}};

  Block[STONE] = BlockData{};
  Block[STONE].isOpaque = true;
  Block[STONE].textureLocation = {
      {0.250f, 0.875f}, {0.250f, 0.875f}, {0.250f, 0.875f}};

  Block[STONE] = BlockData{};
  Block[STONE].isOpaque = false;
  Block[STONE].textureLocation = {
      {0.375f, 0.875f}, {0.375f, 0.875f}, {0.375f, 0.875f}};
}

VoxelWorldManager::VoxelWorldManager() {
  initBlockData();
  chunkManager = ChunkManager::getInstance();
  voxelRenderer = new VoxelWorldRenderer();
};

VoxelWorldManager::~VoxelWorldManager() { delete voxelRenderer; };

VoxelWorldManager *VoxelWorldManager::getInstance() {
  if (!instance) {
    instance = new VoxelWorldManager();
  }
  return instance;
}

void VoxelWorldManager::blockUpdated(int worldX, int worldY, int worldZ,
                                     BlockName blockName) {
  chunkManager->updateBlock(worldX, worldY, worldZ, blockName);
  voxelRenderer->updateChunkMesh(worldX / CHUNKSIZEX, worldY / CHUNKSIZEY);
};

void VoxelWorldManager::render(float playerX, float playerY, float playerZ, //
                               float theta, float phi) {
  voxelRenderer->render(playerX, playerY, playerZ, theta, phi);
}
