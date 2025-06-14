#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

class IndexBuffer {
private:
  unsigned int id;
  unsigned int count;

public:
  IndexBuffer();
  ~IndexBuffer();
  void loadData(const unsigned int *data, unsigned int count);
  void bind() const;
  void unbind() const;
  unsigned int getCount();
};

#endif