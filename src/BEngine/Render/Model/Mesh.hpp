#ifndef MESH_HPP_
#define MESH_HPP_

#include <cstdint>
#include <vector>

#include "BEngine/Core/Memory.hpp"

#include "BEngine/Render/GL/IndexBuffer.hpp"
#include "BEngine/Render/GL/Shader.hpp"
#include "BEngine/Render/GL/Texture.hpp"
#include "BEngine/Render/GL/Vertex.hpp"
#include "BEngine/Render/GL/VertexArray.hpp"

// NOTE: I don't like way that guide say to deal with textures.
// TODO: Find way to make it more convinient later.
class Mesh
{
public:
    Mesh(const std::vector<Vertex> vertices,
         const std::vector<uint32_t> indices,
         const std::vector<Ref<Texture>> textures);

    // TODO: Remove actual draw call from Mesh class
    void Draw(const Shader &shader) const;

    constexpr const std::vector<Vertex> &GetVertices(void) const
    {
        return m_Vertices;
    }

    constexpr const std::vector<uint32_t> &GetIndices(void) const
    {
        return m_Indices;
    }

private:
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
    std::vector<Ref<Texture>> m_Textures;

    VertexArray m_VertexArray;
    VertexBuffer m_VertexBuffer;
    IndexBuffer m_IndexBuffer;

    void Initialize(void);
};

#endif // MESH_HPP_
