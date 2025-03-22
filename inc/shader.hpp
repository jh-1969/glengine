#pragma once

#include <glad.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



class Shader {
  private:
    GLuint id;

  public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use();
    void set_mat4(const char* name, glm::mat4 &mat);
    void set_vec3(const char* name, glm::vec3 &vec);
};
