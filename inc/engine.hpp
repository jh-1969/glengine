#pragma once

#include <iostream>
#include <vector>

#include <glad.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <shader.hpp>
#include <mesh.hpp>
#include <model.hpp>
#include <skybox.hpp>



class Engine {
  private:
    GLFWwindow* window;
    Shader* shader;

  public:
    Engine(int windowWidth, int windowHeight);
    ~Engine();

    bool run();
};
