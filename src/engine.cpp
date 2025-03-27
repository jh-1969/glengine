#include <engine.hpp>
#include <unistd.h>



float alpha = 0.0;
float beta = 0.0;
float radius = 5.0;



Model* tmodel;
GLuint t1;
GLuint t2;
GLuint t3;
GLuint t4;
GLuint t5;



static GLuint
load_texture(const char* path, int type);

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
    "assets/shaders/pbr.vert",
    "assets/shaders/pbr.frag"
  );

  glEnable(GL_DEPTH_TEST);
  
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT); 
  glFrontFace(GL_CW); 

  tmodel = new Model("assets/plane.fbx");

  t1 = load_texture("assets/textures/metal/stained_patterned_metal_26_98_diffuse.jpg", GL_RGB);
  t2 = load_texture("assets/textures/metal/stained_patterned_metal_26_98_metalness.jpg", GL_RED);
  t3 = load_texture("assets/textures/metal/stained_patterned_metal_26_98_roughness.jpg", GL_RED);
  t4 = load_texture("assets/textures/metal/stained_patterned_metal_26_98_ao.jpg", GL_RED);
  t5 = load_texture("assets/textures/metal/stained_patterned_metal_26_98_normal.jpg", GL_RGB);
}

Engine::~Engine() {
  delete this->shader;
  glfwTerminate();
}

bool Engine::run() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glm::mat4 model = glm::mat4(1.0f);
  
  glm::vec3 viewPosition = glm::vec3(
      radius * cos(alpha) * cos(beta),
      radius * sin(beta),
      radius * sin(alpha) * cos(beta));

  glm::mat4 view = glm::lookAt(viewPosition, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
  
  this->shader->use();
  this->shader->set_mat4("model", model);
  this->shader->set_mat4("view", view);
  this->shader->set_mat4("projection", projection);
  this->shader->set_vec3("viewPos", viewPosition);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, t1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, t2);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, t3);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, t4);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, t5);

  shader->set_int("albedoMap", 0);
  shader->set_int("metallicMap", 1);
  shader->set_int("roughnessMap", 2);
  shader->set_int("aoMap", 3);
  shader->set_int("normalMap", 4);
  
  tmodel->draw(*shader);

  glBindTexture(GL_TEXTURE_2D, 0);

  glfwSwapBuffers(window);
  glfwPollEvents();
  
  return !glfwWindowShouldClose(this->window);
}



static GLuint
load_texture(const char* path, int type) {
  GLuint texture = 0;

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
  
  if(!data) {
    std::cout << "Failed to load texture" << std::endl;
    exit(1);
  }

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, (void*)data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);

  return texture;
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
