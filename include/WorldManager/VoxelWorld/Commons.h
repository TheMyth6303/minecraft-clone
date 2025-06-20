#ifndef CHUNK_H
#define CHUNK_H

#include <memory>
#include <utility>

const int CHUNKSIZEX = 16;
const int CHUNKSIZEY = 16;
const int CHUNKSIZEZ = 40;

const int RENDER_DISTANCE = 3;

const float FOV = 90.0f;
const float ASPECT_RATIO = 9.0f / 16.0f;
const float D_NEAR = 1.0f;
const float D_FAR = 250.0f;
const float LAMBDA = (D_FAR) / (D_FAR - D_NEAR);

using ChunkCoords = std::pair<int, int>;
struct Chunk {
  uint8_t blocks[CHUNKSIZEX][CHUNKSIZEY][CHUNKSIZEZ];
};

struct PairHash {
  std::size_t operator()(const std::pair<int, int> &p) const noexcept {
    return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
  }
};

#endif