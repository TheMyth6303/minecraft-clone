#include "WorldManager/VoxelWorld/ChunkManager.h"
#include "WorldManager/VoxelWorld/WorldGenerator.h"

ChunkManager *ChunkManager::instance = nullptr;

void ChunkManager::generateChunk(int chunkX, int chunkY) {
  chunkData.insert({std::make_pair(chunkX, chunkY),
                    WorldGenerator::generateChunk(chunkX, chunkY)});
}

ChunkManager::ChunkManager() {}

ChunkManager *ChunkManager::getInstance() {
  if (!instance) {
    instance = new ChunkManager();
  }
  return instance;
};

Chunk *ChunkManager::getChunk(int chunkX, int chunkY) {
  if (chunkData.count(std::make_pair(chunkX, chunkY)) == 0) {
    generateChunk(chunkX, chunkY);
  }
  return chunkData.at(std::make_pair(chunkX, chunkY));
}

void ChunkManager::update(float playerX, float playerY) {}

void ChunkManager::updateBlock(int worldX, int worldY, int worldZ,
                               BlockName block) {
  int chunkX = worldX / CHUNKSIZEX;
  int chunkY = worldY / CHUNKSIZEY;
  chunkData.at(std::make_pair(chunkX, chunkY))
      ->blocks[worldX % CHUNKSIZEX][worldY % CHUNKSIZEY][worldZ] =
      static_cast<int>(block);
}

BlockName ChunkManager::getBlock(int worldX, int worldY, int worldZ) {
  int chunkX = worldX / CHUNKSIZEX;
  int chunkY = worldY / CHUNKSIZEY;
  return static_cast<BlockName>(
      chunkData.at(std::make_pair(chunkX, chunkY))
          ->blocks[worldX % CHUNKSIZEX][worldY % CHUNKSIZEY][worldZ]);
}

ChunkManager::~ChunkManager() {
  for (auto it = chunkData.begin(); it != chunkData.end(); it++) {
    delete it->second;
  }
  chunkData.clear();
}
