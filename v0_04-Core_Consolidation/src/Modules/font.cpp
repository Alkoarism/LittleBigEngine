#include "Modules/font.h" 

#include <iostream>
#include <string>

Font::Font(const char* path, unsigned int width, unsigned int height):
    m_pixelSizeWidth(width), 
    m_pixelSizeHeight(height)
{
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

    if(SetPixelSize(m_pixelSizeWidth, m_pixelSizeHeight)){
        std::string _error_what = "Failed to define standard pixel size (0, 512) for face at path: ";
        _error_what += path;
        throw std::runtime_error(_error_what);
    }
}

Font::~Font(){
    m_ftCntFace -= 1;
    FT_Done_Face(m_ftFace);
    if (m_ftCntFace == 0){
        FT_Done_FreeType(m_ftLibrary);
    }
}

bool Font::SetPixelSize(const unsigned int width, const unsigned int height){
    // Setting the bitmap with pixels sizes (logical dimensions) instead of real-world measures.
    if (FT_Set_Pixel_Sizes(m_ftFace, width, height)){
        std::cout << "ERROR::FONT::FREETYPE::FAILED_TO_SET_PIXEL_SIZES" << std::endl;
        return 1; //Error
    }
    m_pixelSizeWidth = width;
    m_pixelSizeHeight = height;
    return 0; //No error
}

const Glyph Font::GetGlyphBitmapData(const unsigned long glyph){
    if (FT_Load_Char(m_ftFace, glyph, FT_LOAD_RENDER)){
        std::cout << "ERROR::FONT::FREETYPE::FAILED_TO_DIRECT_LOAD_GLYPH" << std::endl;
        std::cout << "glyph: " << glyph << std::endl;
        Glyph error;
        return error; //Error
    }

    auto& ftGlyph = m_ftFace->glyph;
    Glyph loadedGlyph;
    
    loadedGlyph.metrics.rows = ftGlyph->bitmap.rows;
    loadedGlyph.metrics.width = ftGlyph->bitmap.width;
    loadedGlyph.metrics.xAdvance = static_cast<float>(ftGlyph->advance.x) / 64;
    loadedGlyph.metrics.xBearing = ftGlyph->bitmap_left;
    loadedGlyph.metrics.yBearing = ftGlyph->bitmap_top;
    
    loadedGlyph.glyph = glyph;
    if (ftGlyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY) loadedGlyph.bitspPixel = 8;

    unsigned int dataSize = ftGlyph->bitmap.rows * ftGlyph->bitmap.pitch;
    loadedGlyph.imgData = std::vector<unsigned char>(ftGlyph->bitmap.buffer, ftGlyph->bitmap.buffer + dataSize);

    return loadedGlyph;
}

FT_Library Font::m_ftLibrary;
unsigned int Font::m_ftCntFace = 0;