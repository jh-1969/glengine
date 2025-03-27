#include <mesh.hpp>



Mesh::Mesh() {}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) {
  this->vertices = vertices;
  this->indices = indices;
}

Mesh::~Mesh() {
  vao::destroy(this->vao);
  vbo::destroy(this->vbo);
  ebo::destroy(this->ebo);
}

void Mesh::init_obs() {
  this->vao = vao::create();
  this->vbo = vbo::create(this->vertices);
  this->ebo = ebo::create(this->indices);
  
  vao::set_attrib(0, 3, sizeof(Vertex), (void*)0);
  vao::set_attrib(1, 3, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  //vao::set_attrib(2, 3, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
  //vao::set_attrib(3, 3, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
  vao::set_attrib(4, 2, sizeof(Vertex), (void*)offsetof(Vertex, uv));
  
  ebo::unbind();
  vbo::unbind();
  vao::unbind();
}

void Mesh::draw(Shader &shader) {
  vao::bind(this->vao);
  glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
  vao::unbind();
}
