#ifndef FONT_H
#define FONT_H

#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

//--- Info on a individual glyph ---
struct GlyphMetrics{
    float xAdvance;
    int xBearing, yBearing;
    unsigned int width, rows;
};

struct Glyph{
    GlyphMetrics metrics;
    unsigned long glyph;
    unsigned int bitspPixel;
    std::vector<unsigned char> imgData;
};

class Font{
public:
    Font(const char* fpath, unsigned int width = 0, unsigned int height = 512);
    ~Font();

    Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;
	Font(Font&& other) = delete;
	Font& operator=(Font&& other) = delete;  
    
    bool SetPixelSize(const unsigned int width, const unsigned int height);

    const unsigned int GetPixelSizeWidth() const {return m_pixelSizeWidth;}
    const unsigned int GetPixelSizeHeight() const {return m_pixelSizeHeight;}
    const Glyph GetGlyphBitmapData(const unsigned long glyph);

private:
    unsigned int m_pixelSizeWidth, m_pixelSizeHeight;
    FT_Face m_ftFace;

    //Handling of freetype´s global state
    static FT_Library m_ftLibrary;
    static unsigned int m_ftCntFace;
};

#endif