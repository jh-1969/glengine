#pragma once

#include <stdlib.h>

#include <glad.hpp>



namespace vao {
  //vao is bound after creation
  GLuint create();
  void destroy(GLuint id);

  void bind(GLuint id);
  void unbind();

  void set_attrib(int layout, int count, size_t size, void* offset);
};
