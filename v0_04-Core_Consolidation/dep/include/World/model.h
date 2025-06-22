#ifndef MODEL_H
#define MODEL

#include "OpenGL/renderer.h"

class Model{
public:
    Model(std::string& shader);
    ~Model();

    void PushMesh(const VertexBuffer&, const VertexBufferLayout&);
    void DrawModel();

    void SetIndexBuffer(const std::vector<unsigned int>& indices);

    const VertexArray& GetVertexArray() const { return m_VAO; }
    const IndexBuffer& GetIBO() const { return *m_IBO; }

private:
    VertexArray m_VAO;
    std::unique_ptr<IndexBuffer> m_IBO;
};

#endif