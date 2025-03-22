#pragma once

#include <glad.hpp>

#include <vector>



class Ssbo {
  private:
    int binding;
    GLuint id;

  public:
    template <typename T> Ssbo(int layout, std::vector<T> vec);
    ~Ssbo();

    void bind();
    void unbind();
};
