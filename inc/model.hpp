#pragma once

#include <vector>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <shader.hpp>
#include <mesh.hpp>



class Model {
  private:
    Mesh* mesh;

    void process_node(aiNode* node, const aiScene *scene);
    void process_mesh(aiMesh* aimesh, const aiScene *scene);

  public:
    Model(const char *path);
    ~Model();

    void draw(Shader &shader);	
};
