#include <vbo.hpp>



GLuint vbo::create(std::vector<Vertex> vertices) {
  GLuint id;

  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  return id;
}

GLuint vbo::create(float* vertices, size_t size) {
  GLuint id;

  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, size, &vertices, GL_STATIC_DRAW);

  return id;
}

void vbo::destroy(GLuint id) {
  glDeleteBuffers(1, &id);
}

void vbo::bind(GLuint id) {
  glBindBuffer(GL_ARRAY_BUFFER, id);
}

void vbo::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
