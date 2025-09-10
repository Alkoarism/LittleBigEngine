#include "Modules/fontAtlas.h"

unsigned int FONTATLAS_DEFAULT_COLORDEPTH = 8;  //8 bits per pixel
unsigned int FONTATLAS_DEFAULT_WIDTH = 10;      //Horizontal cell count
unsigned int FONTATLAS_DEFAULT_HEIGHT = 10;     //Vertical call count

unsigned int FONTATLAS_DEFAULT_STARTING_ASCII_CHAR = 33;
unsigned int FONTATLAS_DEFAULT_ENDING_ASCII_CHAR = 127;

/*
FontAtlas::FontAtlas(const unsigned char* atlasPath){

}
*/

FontAtlas::FontAtlas(Font& sourceFont):
    m_cellColorDepth(FONTATLAS_DEFAULT_COLORDEPTH),
    m_atlasWidth(FONTATLAS_DEFAULT_WIDTH),
    m_atlasHeight(FONTATLAS_DEFAULT_HEIGHT)
{
    m_cellHeight = sourceFont.GetPixelSizeHeight();
    if (sourceFont.GetPixelSizeWidth() == 0) m_cellWidth = m_cellHeight;
    else m_cellWidth = sourceFont.GetPixelSizeWidth();

    m_cellDataSize = m_cellHeight * m_cellWidth * m_cellColorDepth;
    unsigned int atlasDataSize = m_cellDataSize * m_atlasHeight * m_atlasWidth; 
    std::vector<unsigned char> atlasData(atlasDataSize, 0);

    m_atlas = std::make_unique<Bitmap>(
        m_atlasWidth * m_cellWidth,
        m_atlasHeight * m_cellHeight,
        m_cellColorDepth,
        atlasData);

    unsigned int xAtlasPos = 0;
    unsigned int yAtlasPos = 0;
    unsigned int totalSlotCnt = m_atlasHeight * m_atlasWidth;
    unsigned int characterCode = FONTATLAS_DEFAULT_STARTING_ASCII_CHAR;

    while (
        characterCode < FONTATLAS_DEFAULT_ENDING_ASCII_CHAR &&
        (yAtlasPos * xAtlasPos + xAtlasPos) < totalSlotCnt)
    {
        if (xAtlasPos == m_atlasWidth){
            yAtlasPos += 1;
            xAtlasPos = 0;
        } 

        Glyph character = sourceFont.GetGlyphBitmapData(characterCode);
        for (int i = 0; i < character.metrics.rows; i++){
            unsigned int glyphRowStart = i * character.metrics.width;
            std::vector<unsigned char> glyphRow(
                character.imgData.data() + glyphRowStart, 
                character.imgData.data() + glyphRowStart + character.metrics.width);
    
            m_atlas->ModifyPixel(glyphRow,
                xAtlasPos * m_cellWidth,
                i + (yAtlasPos * m_cellHeight));
        }

        CellData cellData;
        cellData.glyphMetrics = character.metrics;
        cellData.atlasPosX = xAtlasPos;
        cellData.atlasPosY = yAtlasPos;
        m_cellDataMap.emplace(characterCode, cellData);

        xAtlasPos += 1;
        characterCode += 1;
    }
}

/*
FontAtlas(Font sourceFont, const std::vector<unsigned long> customUnicodeCharSet){

}
*/

const CellData& FontAtlas::GetCharacterData(const unsigned long glyph) const{
    return m_cellDataMap.at(glyph);
}

void FontAtlas::ExportFontAtlas(const std::string path) const{

}

void FontAtlas::ExportBitmapAtlas(const std::string path) const{
    m_atlas->Export(path);
}