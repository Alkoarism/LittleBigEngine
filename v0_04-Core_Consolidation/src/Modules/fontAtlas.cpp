#include "Modules/fontAtlas.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

unsigned int FONTATLAS_DEFAULT_COLORDEPTH = 8;  //8 bits per pixel

unsigned int FONTATLAS_DEFAULT_STARTING_ASCII_CHAR = 32;
unsigned int FONTATLAS_DEFAULT_ENDING_ASCII_CHAR = 127;

/*
FontAtlas::FontAtlas(const unsigned char* atlasPath){

}
*/

FontAtlas::FontAtlas(Font& sourceFont):
    m_cellColorDepth(FONTATLAS_DEFAULT_COLORDEPTH)
{
    // Initialization of FontAtlas using ShelfPacking algorithm
    std::vector<Glyph> fontCharacters;
    m_atlasHeight = 0;
    m_atlasWidth = 0;

    for (int characterCode = FONTATLAS_DEFAULT_STARTING_ASCII_CHAR;
            characterCode < FONTATLAS_DEFAULT_ENDING_ASCII_CHAR;
            characterCode++)
    {
        fontCharacters.push_back(sourceFont.GetGlyphBitmapData(characterCode));
        m_atlasWidth += fontCharacters.back().metrics.width;
    }

    std::sort(fontCharacters.begin(), fontCharacters.end(),
    [](Glyph& i, Glyph& j) -> bool { return i.metrics.rows > j.metrics.rows;});
    m_atlasHeight = fontCharacters.front().metrics.rows;
    
    m_atlas = std::make_unique<Bitmap>(
        m_atlasWidth,
        m_atlasHeight,
        FONTATLAS_DEFAULT_COLORDEPTH);
    
    unsigned int xCursor = 0;
    for (size_t i = 0; i < fontCharacters.size(); i++){

        for (int j = 0; j < fontCharacters[i].metrics.rows; j++){
            unsigned int glyphRowStart = j * fontCharacters[i].metrics.width;
            std::vector<unsigned char> glyphRow(
                fontCharacters[i].imgData.data() + glyphRowStart, 
                fontCharacters[i].imgData.data() + glyphRowStart + fontCharacters[i].metrics.width);
    
            m_atlas->ModifyPixel(
                glyphRow, 
                xCursor,
                j);
        }

        CellData cellData;
        cellData.glyphMetrics = fontCharacters[i].metrics;
        cellData.yAtlasOffset = 0;
        cellData.xAtlasOffset = xCursor;
        m_cellDataMap.emplace(fontCharacters[i].glyph, cellData);

        xCursor += fontCharacters[i].metrics.width;
    }
}

/*
FontAtlas(Font sourceFont, const std::vector<unsigned long> customUnicodeCharSet){

}
*/

const CellData& FontAtlas::GetCharacterData(const unsigned long glyph) const{
    try{
        return m_cellDataMap.at(glyph);
    } catch(const std::out_of_range& error){
        std::cout << "ERROR::FONTATLAS::FAILED_TO_GET_CHARACTER" << std::endl;
        std::string _error = error.what() ;
        _error += " - Failed to find requested character: " + std::to_string(glyph);
        throw std::out_of_range(_error);
    }
}

void FontAtlas::ExportFontAtlas(const std::string path) const{

}

void FontAtlas::ExportBitmapAtlas(const std::string path) const{
    m_atlas->Export(path);
}