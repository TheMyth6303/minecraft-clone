#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

#include <array>

enum BlockName {
  AIR,
  DIRT,
  STONE,
  WATER,
  COUNT,
};

struct BlockData {
private:
  struct TextureLocation {
    // tile bottom left location in texture atlas
    float topFaceUV[2];
    float bottomFaceUV[2];
    float sideFaceUV[2];
  };

public:
  BlockData();
  ~BlockData();
  bool isOpaque;
  TextureLocation textureLocation;
};

extern std::array<BlockData, BlockName::COUNT> Block;

#endif