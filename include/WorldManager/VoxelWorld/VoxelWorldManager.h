#ifndef VOXEL_WORLD_MANAGER_H
#define VOXEL_WORLD_MANAGER_H

#include "WorldManager/VoxelWorld/ChunkManager.h"
#include "WorldManager/VoxelWorld/VoxelWorldRenderer.h"

class VoxelWorldManager {
private:
  VoxelWorldManager();
  VoxelWorldManager(VoxelWorldManager const &) = delete;
  VoxelWorldManager &operator=(const VoxelWorldManager &) = delete;
  ~VoxelWorldManager();
  void initBlockData();

  static VoxelWorldManager *instance;

  ChunkManager *chunkManager;
  VoxelWorldRenderer *voxelRenderer;

public:
  static VoxelWorldManager *getInstance();
  void blockUpdated(int worldX, int worldY, int worldZ, BlockName blockName);
  void render(float playerX, float playerY, float playerZ, //
              float theta, float phi);
};

#endif