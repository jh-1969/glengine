#pragma once

#include <vector>

#include <glad.hpp>

#include <vertex.hpp>


namespace ebo {
 //ebo remains bound after creation
  GLuint create(std::vector<GLuint> indices);
  void destroy(GLuint id);

  void bind(GLuint id);
  void unbind();
}
