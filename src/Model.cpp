#include <stdexcept>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <fmt/core.h>

#include "glsandbox/Model.hpp"

Model::Model(const std::filesystem::path &path) : m_Path(path), m_Meshes()
{
    LoadModel();
}

void Model::Draw(const Shader &shader) const
{
    for (const Mesh &mesh : m_Meshes)
    {
        mesh.Draw(shader);
    }
}

void Model::LoadModel()
{
    if (!std::filesystem::exists(m_Path))
    {
        const std::string errorMessage = fmt::format(
            "Failed loading model: path doens't exists '{}'", m_Path.c_str());
        throw std::invalid_argument(errorMessage);
    }

    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(m_Path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode)
    {
        const std::string errorMessage = fmt::format(
            "Error loading Asimp scene: {}", importer.GetErrorString());
        throw std::invalid_argument(errorMessage);
    }

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

// TODO: Add reserves
Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Ref<Texture>> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        glm::vec3 readingVector3;
        glm::vec3 readingVector2;

        readingVector3.x = mesh->mVertices[i].x;
        readingVector3.y = mesh->mVertices[i].y;
        readingVector3.z = mesh->mVertices[i].z;
        vertex.Position = readingVector3;

        if (mesh->HasNormals())
        {
            readingVector3.x = mesh->mNormals[i].x;
            readingVector3.y = mesh->mNormals[i].y;
            readingVector3.z = mesh->mNormals[i].z;
            vertex.NormalVector = readingVector3;
        }

        if (mesh->mTextureCoords[0])
        {
            readingVector2.x = mesh->mTextureCoords[0][i].x;
            readingVector2.y = mesh->mTextureCoords[0][i].y;
            vertex.TextureCoordinates = readingVector2;
        }
        else
        {
            vertex.TextureCoordinates = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    // if (mesh->mMaterialIndex >= 0)
    // {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Ref<Texture>> diffuseMaps = LoadMaterialTextures(
        material, aiTextureType_DIFFUSE, TextureType::Diffuse);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Ref<Texture>> specularMaps = LoadMaterialTextures(
        material, aiTextureType_SPECULAR, TextureType::Specular);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // }

    return Mesh(vertices, indices, textures);
}

std::vector<Ref<Texture>> Model::LoadMaterialTextures(aiMaterial *mat,
                                                      aiTextureType type,
                                                      TextureType textureType)
{
    std::vector<Ref<Texture>> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString path;
        mat->GetTexture(type, i, &path);
        Ref<Texture> texture = MakeRef<Texture>(
            m_Path.parent_path().append(path.C_Str()), textureType);
        textures.push_back(texture);
    }
    std::cout << textures.size() << '\n';
    return textures;
}
