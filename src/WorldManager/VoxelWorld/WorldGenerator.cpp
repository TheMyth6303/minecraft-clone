#include "WorldManager/VoxelWorld/WorldGenerator.h"
#include "WorldManager/VoxelWorld/BlockData.h"

WorldGenerator::WorldGenerator() {}
WorldGenerator::~WorldGenerator() {}

int WorldGenerator::getTerrainHeight(int worldX, int worldY) {
  return abs((worldX) * (worldY)) / 100 + 1;
}

Chunk *WorldGenerator::generateChunk(int chunkX, int chunkY) {
  Chunk *chunk = new Chunk{};
  for (int i = 0; i < CHUNKSIZEX; i++) {
    for (int j = 0; j < CHUNKSIZEY; j++) {
      for (int k = 0; k < CHUNKSIZEZ; k++) {
        int x = CHUNKSIZEX * chunkX + i;
        int y = CHUNKSIZEY * chunkY + j;
        int z = k;

        int height = getTerrainHeight(x, y);

        if (k <= height || k >= CHUNKSIZEZ - height) {
          chunk->blocks[i][j][k] = DIRT;
        } else {
          chunk->blocks[i][j][k] = AIR;
        }
      }
    }
  }
  return chunk;
}