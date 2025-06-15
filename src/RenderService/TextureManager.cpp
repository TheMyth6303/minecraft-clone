#include "RenderService/TextureManager.h"

#include <GLEW/glew.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureManager::TextureManager() {}
TextureManager::~TextureManager() {}

std::unordered_map<std::string, TextureId> TextureManager::textures;

TextureId TextureManager::createTexture(const char *texturePath) {
  if (textures.count(texturePath) == 0) {
    TextureId texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *localBuffer;
    int width, height, BPP;
    localBuffer = stbi_load(texturePath, &width, &height, &BPP, 4);

    if (!localBuffer) {
      // log error : could not load texture.
    } else {
      // log texture loading success.
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, localBuffer);
    stbi_image_free(localBuffer);

    textures.insert({texturePath, texture});

    return texture;
  }
  return textures[texturePath];
}

void TextureManager::prepareTexture(TextureId texture, int slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, texture);
}

void TextureManager::destroyTexture(TextureId texture) {
  for (auto it = textures.begin(); it != textures.end();) {
    if (it->second == texture) {
      it = textures.erase(it);
      break;
    }
  }
  glDeleteTextures(1, &texture);
}

void TextureManager::destroyAllTextures() {
  for (auto it = textures.begin(); it != textures.end(); ++it) {
    glDeleteTextures(1, &it->second);
  }
  textures.clear();
}