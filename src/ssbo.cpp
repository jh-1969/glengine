#include <ssbo.hpp>
#include <engine.hpp>



template <typename T>
Ssbo::Ssbo(int layout, std::vector<T> vec) {
  glCreateBuffers(1, &this->id);

  glNamedBufferStorage(
    this->id,
    sizeof(T) * vec.size(),
    (const void *)vec.data(),
    GL_DYNAMIC_STORAGE_BIT
  );
  this->binding = layout;
}

Ssbo::~Ssbo() {
  glDeleteBuffers(1, &this->id);
}

void Ssbo::bind() {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, this->binding, this->id);
}

void Ssbo::unbind() {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, this->binding, 0);
}

template Ssbo::Ssbo(int, std::vector<GLuint>);
template Ssbo::Ssbo(int, std::vector<GLuint64>);
template Ssbo::Ssbo(int, std::vector<Vertex>);
