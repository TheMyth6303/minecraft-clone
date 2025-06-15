#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <unordered_map>

using TextureId = uint32_t;

class TextureManager {
private:
  TextureManager();
  TextureManager(TextureManager const &) = delete;
  TextureManager &operator=(const TextureManager &) = delete;
  ~TextureManager();

  static std::unordered_map<std::string, TextureId> textures;

public:
  static TextureId createTexture(const char *texturePath);
  static void prepareTexture(TextureId texture, int slot);
  static void destroyTexture(TextureId texture);
  static void destroyAllTextures();
};

#endif