#include <engine.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.hpp>



GLuint texture;
GLuint64 texHandle;

float alpha = 0.0;
float beta = 0.0;
float radius = 5.0;



static void
populate_test_cube(std::vector<Vertex> &vertices, std::vector<GLuint> &indices);

static void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

static void
scroll_callback(GLFWwindow* window, double xoffset, double yoffset);



Engine::Engine(int windowWidth, int windowHeight) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  this->window = glfwCreateWindow(
    windowWidth, windowHeight, "konina", NULL, NULL
  );

  if(this->window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(1);
  }

  glfwMakeContextCurrent(window);
  
  glfwSetKeyCallback(window, key_callback);
  glfwSetScrollCallback(window, scroll_callback);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(1);
  }

  glViewport(0, 0, windowWidth, windowHeight);
  
  this->shader = new Shader(
    "assets/shaders/def.vert",
    "assets/shaders/def.frag"
  );

  glEnable(GL_DEPTH_TEST);
  glGenVertexArrays(1, &this->emptyVao);
  glBindVertexArray(this->emptyVao);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0);
  
  if(!data) {
    std::cout << "Failed to load texture" << std::endl;
  }

  GLuint texture;
  glCreateTextures(GL_TEXTURE_2D, 1, &texture);
  glTextureStorage2D(texture, 1, GL_RGB8, width, height);
  glTextureSubImage2D(
      texture, 
      // level, xoffset, yoffset, width, height
      0, 0, 0, width, height, 
      GL_RGB, GL_UNSIGNED_BYTE, 
      (const void *)data);
  glGenerateTextureMipmap(texture);
  
  stbi_image_free(data);

  texHandle = glGetTextureHandleARB(texture);
  if (texHandle == 0) {
    std::cerr << "Error! Handle returned null" << std::endl;
    exit(-1);
  }

  glDeleteTextures(1, &texture);
}

Engine::~Engine() {
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &emptyVao);

  delete this->shader;
  glfwTerminate();
}

bool Engine::run() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  populate_test_cube(vertices, indices);
  std::vector<GLuint64> handles;
  handles.push_back(texHandle);

  glm::mat4 model = glm::mat4(1.0f);
  
  glm::vec3 viewPosition = glm::vec3(
      radius * cos(alpha) * cos(beta),
      radius * sin(beta),
      radius * sin(alpha) * cos(beta));

  glm::mat4 view;
  view = glm::lookAt(viewPosition, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  Ssbo vertexSsbo(0, vertices);
  Ssbo indiceSsbo(1, indices);
  Ssbo handlesSsbo(2, handles);

  this->shader->use();
  this->shader->set_mat4("model", model);
  this->shader->set_mat4("view", view);
  this->shader->set_mat4("projection", projection);
  this->shader->set_vec3("fsViewPosition", viewPosition);

  vertexSsbo.bind();
  indiceSsbo.bind();
  handlesSsbo.bind();
  glDrawArrays(GL_TRIANGLES, 0, indices.size());

  glfwSwapBuffers(window);
  glfwPollEvents();

  return !glfwWindowShouldClose(this->window);
}



static void
scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  alpha -= xoffset / 10.0;

  beta += yoffset / 10.0;
  if(beta > 1.4)
    beta = 1.4;
  if(beta < -1.4)
    beta = -1.4;
}

static void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if(action == GLFW_PRESS) {
    switch(key) {
      case GLFW_KEY_W: radius -= 0.5; break;
      case GLFW_KEY_S: radius += 0.5; break;
    }
  }
}

static void
populate_test_cube(std::vector<Vertex> &vertices, std::vector<GLuint> &indices) {
  for(int i = -1; i < 2; i += 2) {
    Vertex v1 = {{-1, -1, (float)i},
      {0, 0, (float)i},
      {0, 0}};
    Vertex v2 = {{1, -1, (float)i},
      {0, 0, (float)i},
      {1, 0}};
    Vertex v3 = {{1, 1, (float)i},
      {0, 0, (float)i},
      {1, 1}};
    Vertex v4 = {{-1, -1, (float)i},
      {0, 0, (float)i},
      {0, 0}};
    Vertex v5 = {{1, 1, (float)i},
      {0, 0, (float)i},
      {1, 1}};
    Vertex v6 = {{-1, 1, (float)i},
      {0, 0, (float)i},
      {0, 1}};

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);
  }

  for(int i = -1; i < 2; i += 2) {
    Vertex v1 = {{(float)i, -1, -1},
      {(float)i, 0, 0},
      {0, 0}};
    Vertex v2 = {{(float)i, -1, 1},
      {(float)i, 0, 0},
      {1, 0}};
    Vertex v3 = {{(float)i, 1, 1},
      {(float)i, 0, 0},
      {1, 1}};
    Vertex v4 = {{(float)i, -1, -1},
      {(float)i, 0, 0},
      {0, 0}};
    Vertex v5 = {{(float)i,  1, 1},
      {(float)i, 0 ,0},
      {1, 1}};
    Vertex v6 = {{(float)i, 1, -1},
      {(float)i, 0, 0},
      {0, 1}};

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);
  }
  
  for(int i = -1; i < 2; i += 2) {
    Vertex v1 = {{-1, (float)i, -1},
      {0, (float)i, 0},
      {0, 0}};
    Vertex v2 = {{-1, (float)i, 1},
      {0, (float)i, 0},
      {1, 0}};
    Vertex v3 = {{1, (float)i, 1},
      {0, (float)i, 0},
      {1, 1}};
    Vertex v4 = {{-1, (float)i, -1},
      {0, (float)i, 0},
      {0, 0}};
    Vertex v5 = {{1, (float)i, 1},
      {0, (float)i, 0},
      {1, 1}};
    Vertex v6 = {{1, (float)i, -1},
      {0, (float)i, 0},
      {0, 1}};

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);
  }

  for(int i = 0; i < 36; i++)
    indices.push_back(i);
}
