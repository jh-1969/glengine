#pragma once

#include <vector>

#include <glad.hpp>

#include <shader.hpp>
#include <vao.hpp>
#include <vbo.hpp>
#include <ebo.hpp>



class Mesh {
  private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
  
  public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
    ~Mesh();
    
    void init_obs();
    void draw(Shader &shader);
};
