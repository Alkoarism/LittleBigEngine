#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "headers.h"

class Font{
public:
    Font(const char* fpath);
    ~Font();

    Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;

	Font(Font&& other) noexcept;
	Font& operator=(Font&& other) noexcept;
//    int GetWidth(const char* text) const;
//    const VertexArray& GetVertexArray() const;
//    const IndexBuffer& GetIBO() const;

//    void SetColor(glm::vec4 color);
//    void SetPosition(glm::vec3 pos);
//    void SetScale(const float& scale);    

private:
    FT_Face m_ftFace;
    //    VertexArray m_VAO;
    //    std::unique_ptr<IndexBuffer> m_IBO;

    //Handling of freetype´s global state
    static FT_Library m_ftLibrary;
    static unsigned int m_ftCntFace;

};

#endif