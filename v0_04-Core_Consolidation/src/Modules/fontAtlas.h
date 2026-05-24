#ifndef FONTATLAS_H
#define FONTATLAS_H

#include <map>
#include <memory>
#include <string>

#include "Modules/bitmap.h"
#include "Modules/font.h"

struct CellData{
    GlyphMetrics glyphMetrics;
    unsigned int xAtlasOffset, yAtlasOffset;
};

class FontAtlas{
public:
    //FontAtlas(const unsigned char* atlasPath);
    FontAtlas(Font& sourceFont);
    //FontAtlas(Font sourceFont, const std::vector<unsigned long> customUnicodeCharSet);
    
    FontAtlas(const FontAtlas&) = delete;
    FontAtlas& operator=(const FontAtlas&) = delete;
    FontAtlas(FontAtlas&& other) = delete;
    FontAtlas& operator=(FontAtlas&& other) = delete; 

    const CellData& GetCharacterData(const unsigned long glyph) const;
    const Bitmap& GetBitmap() const {return *m_atlas;}

    void ExportFontAtlas(const std::string path) const;
    void ExportBitmapAtlas(const std::string path) const;

private:
    unsigned int m_cellColorDepth;
    unsigned int m_atlasWidth, m_atlasHeight;

    std::unique_ptr<Bitmap> m_atlas;
    std::map<unsigned long, CellData> m_cellDataMap;
};

#endif