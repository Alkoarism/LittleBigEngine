#include "World/model.h"

Model::Model() {

}

Model::~Model(){

}

void Model::PushMesh(const VertexBuffer& VBO, const VertexBufferLayout& VBL){
    m_VAO.Bind();
	m_VAO.AddBuffer(VBO, VBL);
}

void Model::SetIndexBuffer(const std::vector<unsigned int>& indices){
    m_IBO.reset(new IndexBuffer(&indices[0], indices.size()));
}