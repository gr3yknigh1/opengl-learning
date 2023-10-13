#include <cstring>

#include <fmt/format.h>

#include "BEngine/Render/Model/Mesh.hpp"

#include "BEngine/Render/GL/Texture.hpp"

Mesh::Mesh(const std::vector<Vertex> vertices, const std::vector<uint32_t> indices,
           const std::vector<Ref<Texture>> textures)
    : m_Vertices(vertices), m_Indices(indices), m_Textures(textures), m_VertexArray(), m_VertexBuffer(vertices),
      m_IndexBuffer(indices)
{
    Initialize();
}

void Mesh::Draw(const Shader &shader) const
{
    shader.Bind();

    uint32_t diffuseN = 1, specularN = 1, emissionN = 1;

    for (uint32_t i = 0; i < m_Textures.size(); ++i)
    {
        const Ref<Texture> &texture = m_Textures[i];
        texture->BindTo(i);
        const char *textureTypeStr = TextureTypeToString(texture->GetType());

        if (strlen(textureTypeStr) == 0 || texture->GetType() == TextureType::None)
        {
            continue;
        }

        uint32_t number;
        if (texture->GetType() == TextureType::Diffuse)
        {
            number = diffuseN;
            ++diffuseN;
        }
        else if (texture->GetType() == TextureType::Specular)
        {
            number = specularN;
            ++specularN;
        }
        else if (texture->GetType() == TextureType::Emission)
        {
            number = emissionN;
            ++emissionN;
        }

        const std::string uniformName = fmt::format("material.{}{}", textureTypeStr, number);

        shader.SetUniform(uniformName, texture->GetSlot().value());
    }

    m_VertexArray.Bind();
    m_IndexBuffer.Bind();
    GL_Call(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0));
    m_VertexArray.Unbind();
}

void Mesh::Initialize(void)
{
    VertexBufferLayout layout;
    layout.PushFloat(3.0f);
    layout.PushFloat(3.0f);
    layout.PushFloat(2.0f);

    m_VertexArray.AddBuffer(m_VertexBuffer, layout);
    m_VertexArray.Unbind();
}
