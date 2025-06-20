#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include "WorldManager/VoxelWorld/Commons.h"

class WorldGenerator {
private:
  WorldGenerator();
  WorldGenerator(WorldGenerator const &) = delete;
  WorldGenerator &operator=(const WorldGenerator &) = delete;
  ~WorldGenerator();

public:
  static int getTerrainHeight(int worldX, int worldY);
  static Chunk *generateChunk(int chunkX, int chunkY);
};

#endif