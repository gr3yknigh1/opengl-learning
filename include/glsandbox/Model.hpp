#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <filesystem>
#include <vector>

#include <assimp/scene.h>

#include "glsandbox/Mesh.hpp"
#include "glsandbox/Shader.hpp"

class Model
{
public:
    Model(const std::filesystem::path &path);

    void Draw(const Shader &shader) const;

private:
    const std::filesystem::path m_Path;
    std::vector<Mesh> m_Meshes;

    void LoadModel(const std::filesystem::path &path);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial *mat,
                                              aiTextureType type,
                                              TextureType textureType);
};

#endif // MODEL_HPP_
