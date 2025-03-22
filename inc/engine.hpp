#pragma once

#include <iostream>
#include <vector>

#include <glad.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <shader.hpp>
#include <ssbo.hpp>



struct Vertex {
    float position[3];
    float normal[3];
    float uv[2];
};



class Engine {
  private:
    GLFWwindow* window;
    Shader* shader;
    GLuint emptyVao; /*needed to avoid unwanted render errors 
                     when using SSBOs for vertex data*/

  public:
    Engine(int windowWidth, int windowHeight);
    ~Engine();

    bool run();
};
