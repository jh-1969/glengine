#include <model.hpp>



Model::Model(const char *path) {
  Assimp::Importer import;
  const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);	

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  {
    std::cout << "assimp:" << import.GetErrorString() << std::endl;
    return;
  }
  
  this->mesh = new Mesh();
  this->process_node(scene->mRootNode, scene);
  mesh->init_obs();
}

Model::~Model() {
  delete this->mesh;
}

void Model::draw(Shader &shader) {
  this->mesh->draw(shader);
}

void Model::process_node(aiNode* node, const aiScene *scene) {
  for(unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
    this->process_mesh(mesh, scene);
  }

  for(unsigned int i = 0; i < node->mNumChildren; i++) {
    process_node(node->mChildren[i], scene);
  }
}

void Model::process_mesh(aiMesh* aimesh, const aiScene *scene) {
  GLuint indiceOffset = this->mesh->indices.size();

  for(unsigned int i = 0; i < aimesh->mNumVertices; i++) {
    Vertex vertex = {
      glm::vec3(
      aimesh->mVertices[i].x,
      aimesh->mVertices[i].y,
      aimesh->mVertices[i].z
    ),glm::vec3(
      aimesh->mNormals[i].x,
      aimesh->mNormals[i].y,
      aimesh->mNormals[i].z
    ),glm::vec3(
      aimesh->mTangents[i].x,
      aimesh->mTangents[i].y,
      aimesh->mTangents[i].z
    ),glm::vec3(
      aimesh->mBitangents[i].x,
      aimesh->mBitangents[i].y,
      aimesh->mBitangents[i].z
    ),glm::vec2(
      0.0,
      0.0
    )};

    if(aimesh->mTextureCoords[0]) {
      vertex.uv.x = aimesh->mTextureCoords[0][i].x; 
      vertex.uv.y = aimesh->mTextureCoords[0][i].y;
    }

    this->mesh->vertices.push_back(vertex);
  }

  for(unsigned int i = 0; i < aimesh->mNumFaces; i++) {
    aiFace face = aimesh->mFaces[i];
    for(unsigned int j = 0; j < face.mNumIndices; j++)
      this->mesh->indices.push_back(face.mIndices[j] + indiceOffset);
  } 
}
