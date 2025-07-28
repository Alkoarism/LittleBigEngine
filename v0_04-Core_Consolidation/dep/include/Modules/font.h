#ifndef FONT_H
#define FONT_H

#include "OpenGL/vertex_array.h"
#include "OpenGL/vertex_buffer.h"
#include "OpenGL/vertex_buffer_layout.h"
#include "OpenGL/texture.h"

class Font{
public:
    Font(const char* fpath);
//    int GetWidth(const char* text) const;
//    const VertexArray& GetVertexArray() const;
//    const IndexBuffer& GetIBO() const;

//    void SetColor(glm::vec4 color);
//    void SetPosition(glm::vec3 pos);
//    void SetScale(const float& scale);    
    
private:
//    VertexArray m_VAO;
//    std::unique_ptr<IndexBuffer> m_IBO;
};

#endif