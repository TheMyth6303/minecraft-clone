#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include "WorldManager/VoxelWorld/BlockData.h"
#include "WorldManager/VoxelWorld/Commons.h"

#include <unordered_map>

class ChunkManager {
private:
  std::unordered_map<ChunkCoords, Chunk *, PairHash> chunkData;
  void generateChunk(int chunkX, int chunkY);
  ChunkManager();
  ~ChunkManager();
  ChunkManager(ChunkManager const &) = delete;
  ChunkManager &operator=(const ChunkManager &) = delete;

  static ChunkManager *instance;

public:
  static ChunkManager *getInstance();
  Chunk *getChunk(int chunkX, int chunkY);
  void update(float playerX, float playerY);
  void updateBlock(int worldX, int worldY, int worldZ, BlockName block);
  BlockName getBlock(int worldX, int worldY, int worldZ);
};

#endif