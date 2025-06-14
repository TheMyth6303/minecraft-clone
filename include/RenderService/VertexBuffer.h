#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <stddef.h>
#include <vector>

struct VertexBufferAttributeSection {
  unsigned int dimension;
  int type;
  bool normalize;
  size_t size;
};

class VertexBuffer {
private:
  unsigned int id;
  std::vector<VertexBufferAttributeSection> attributes;
  size_t size;

public:
  VertexBuffer();
  ~VertexBuffer();
  void destroy();

  template <typename T>
  void pushAttribute(unsigned int dimension, bool normalize);
  void loadData(const void *data, unsigned int count);
  void bindAttributes() const;
  void unbindAttributes() const;
  void bind() const;
  void unbind() const;
};

#endif