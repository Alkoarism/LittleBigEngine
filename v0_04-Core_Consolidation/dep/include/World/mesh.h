#ifndef MESH_H
#define MESH_H

#include "OpenGL/renderer.h"

//STORES THE DATA managed by a single mesh on a currently running program
class Mesh{
public:
    Mesh(const std::vector<float>& vData,
         const std::vector<unsigned int>& bufferLayout);

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
	
    ~Mesh();
    
    const VertexBuffer& GetVertexData() const { return *m_VBO; }
    const VertexBufferLayout& GetVertexLayout() const { return m_VBL; }
    
private:
    VertexBufferLayout m_VBL;
    std::unique_ptr<VertexBuffer> m_VBO;
};

#endif