#include "Modules/font.h" 

Font::Font(const char* path){
    if (m_ftCntFace == 0){
        if (FT_Init_FreeType(&m_ftLibrary)){
            std::cout << "ERROR::FONT::FREETYPE::FAILED_TO_INITIALIZE" << std::endl;
            throw std::runtime_error("Failed to initialize freetype library");
        }
    }

    if (FT_New_Face(m_ftLibrary, path, 0, &m_ftFace)){
        std::cout << "ERROR::FONT::FREETYPE::FAILED_TO_LOAD_FACE" << std::endl;
        std::string _error_what = "Failed to initialize freetype face at path: ";
        _error_what += path;
		throw std::runtime_error(_error_what);
    }
    m_ftCntFace += 1;
}

Font::Font(Font&& other) noexcept : m_ftFace(other.m_ftFace){
    other.m_ftFace = NULL;
}

Font& Font::operator=(Font&& other) noexcept{
    if (this != &other) {
        std::swap(m_ftFace, other.m_ftFace);
    }
    return *this;
}

Font::~Font(){
    m_ftCntFace -= 1;
    FT_Done_Face(m_ftFace);
    if (m_ftCntFace == 0){
        FT_Done_FreeType(m_ftLibrary);
    }
}

FT_Library Font::m_ftLibrary;
unsigned int Font::m_ftCntFace = 0;