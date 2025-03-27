#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <glad.hpp>
#include <stb_image_impl.hpp>
#include <glm/glm.hpp>

#include <shader.hpp>
#include <vao.hpp>
#include <vbo.hpp>



class Skybox {
  private:
    Shader* shader;

    GLuint vao;
    GLuint vbo;
    GLuint cubemap;

    GLuint load_cubemap(std::vector<std::string> faces);

  public:
    Skybox(std::vector<std::string> faces, const char* vert_path, const char* frag_path);
    ~Skybox();

    void draw(glm::mat4 view, glm::mat4 projection);
};
