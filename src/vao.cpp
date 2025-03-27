#include <vao.hpp>



GLuint vao::create() {
  GLuint id;

  glGenVertexArrays(1, &id);
  glBindVertexArray(id);

  return id;
}

void vao::destroy(GLuint id) {
  glDeleteVertexArrays(1, &id);
}

void vao::bind(GLuint id) {
  glBindVertexArray(id);
}

void vao::unbind() {
  glBindVertexArray(0);
}


void vao::set_attrib(int layout, int count, size_t size, void* offset) {
  glEnableVertexAttribArray(layout);	
  glVertexAttribPointer(layout, count, GL_FLOAT, GL_FALSE, size, offset);
}

