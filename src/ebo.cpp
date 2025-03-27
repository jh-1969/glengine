#include <ebo.hpp>



GLuint ebo::create(std::vector<GLuint> indices) {
  GLuint id;

  glGenBuffers(1, &id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

  return id;
}

void ebo::destroy(GLuint id) {
  glDeleteBuffers(1, &id);
}

void ebo::bind(GLuint id) {
  glBindBuffer(GL_ARRAY_BUFFER, id);
}

void ebo::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
