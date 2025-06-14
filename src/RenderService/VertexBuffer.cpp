#include "RenderService/VertexBuffer.h"

#include <GLEW/glew.h>

VertexBuffer::VertexBuffer() : size(0) { glGenBuffers(1, &id); }

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &id); }

template <>
void VertexBuffer::pushAttribute<float>(unsigned int dimension,
                                        bool normalize) {
  VertexBufferAttributeSection vbas(
      {dimension, GL_FLOAT, normalize, dimension * sizeof(float)});
  attributes.push_back(vbas);
  size += vbas.size;
}
template <>
void VertexBuffer::pushAttribute<unsigned int>(unsigned int dimension,
                                               bool normalize) {
  VertexBufferAttributeSection vbas({dimension, GL_UNSIGNED_INT, normalize,
                                     dimension * sizeof(unsigned int)});
  attributes.push_back(vbas);
  size += vbas.size;
}

void VertexBuffer::loadData(const void *data, unsigned int count) {
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, count * size, data, GL_DYNAMIC_DRAW);
}

void VertexBuffer::bindAttributes() const {
  size_t pointer = 0;
  for (int i = 0; i < attributes.size(); i++) {
    VertexBufferAttributeSection vbas = attributes[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, vbas.dimension, vbas.type, vbas.normalize, size,
                          (void *)pointer);
    pointer += vbas.size;
  }
}

void VertexBuffer::unbindAttributes() const {
  for (int i = 0; i < attributes.size(); i++) {
    glDisableVertexAttribArray(i);
  }
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
