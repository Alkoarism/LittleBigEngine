#include "World/mesh.h"

Mesh::Mesh( const std::vector<float>& vData,
            const std::vector<unsigned int>& bufferLayout) 
{
    for (auto i = bufferLayout.begin(); i != bufferLayout.end(); i++){
        m_VBL.Push<float>(*i);
    }
    m_VBO.reset(new VertexBuffer(vData.data(), sizeof(decltype(vData[0])) * vData.size()));
    //m_VBO->Unbind();    //Usually unnecessary
    //m_IBO->Unbind();    //Usually unnecessary
}


Mesh::~Mesh(){

}

Mesh::Mesh(Mesh&& other) noexcept
    :   m_VBL(std::move(other.m_VBL)), m_VBO(std::move(other.m_VBO)) {
    }

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        m_VBL = std::move(other.m_VBL);
        m_VBO = std::move(other.m_VBO);
    }
    return *this;
}