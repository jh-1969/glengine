#pragma once

#include <vector>

#include <glad.hpp>

#include <vertex.hpp>



namespace vbo {
 //vbo remains bound after creation
  GLuint create(std::vector<Vertex> vertices);
  GLuint create(float* vertices, size_t size);
  void destroy(GLuint id);

  void bind(GLuint id);
  void unbind();
}
