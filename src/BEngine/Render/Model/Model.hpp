#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <filesystem>
#include <vector>

#include <assimp/scene.h>

#include "BEngine/Core/Memory.hpp"

#include "BEngine/Render/Model/Mesh.hpp"

#include "BEngine/Render/GL/Shader.hpp"

class Model
{
public:
    Model(const std::filesystem::path &path);

    void Draw(const Shader &shader) const;

private:
    const std::filesystem::path m_Path;
    std::vector<Mesh> m_Meshes;

    void LoadModel();
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Ref<Texture>> LoadMaterialTextures(aiMaterial *mat,
                                                   aiTextureType type,
                                                   TextureType textureType);
};

#endif // MODEL_HPP_
