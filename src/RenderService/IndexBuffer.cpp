#include "RenderService/IndexBuffer.h"

#include <GLEW/glew.h>

IndexBuffer::IndexBuffer() { glGenBuffers(1, &id); }

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &id); }

void IndexBuffer::loadData(const unsigned int *data, unsigned int count) {
  this->count = count;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data,
               GL_STATIC_DRAW);
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }

void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

unsigned int IndexBuffer::getCount() { return count; }
