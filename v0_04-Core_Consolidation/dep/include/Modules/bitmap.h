#ifndef BITMAP_H
#define BITMAP_H

#include <vector>
#include <string>

class Bitmap{
public:
    Bitmap(const int width, const int rows, const int colorDepth);
    Bitmap(const int width, const int rows, const int colorDepth, const unsigned char* rawData);
    Bitmap(const int width, const int rows, const int colorDepth, std::vector<unsigned char>& rawData);
    ~Bitmap() = default;

    const unsigned int GetDataSize() const {return m_rows * m_width * m_pixelSize;}
    const unsigned int GetPaddedDataSize() const {return m_rows * m_rowByteSize;}
    const unsigned int GetRows() const {return m_rows;}
    const unsigned int GetWidth() const {return m_width;}
    const unsigned int GetRowSize() const {return m_rowByteSize;}

    void ModifyPixel(const std::vector<unsigned char>& pixelData, const unsigned int xPos, const unsigned int yPos);
    void Export(const std::string path, const std::vector<uint8_t> eightBitColorPalete = {0xFF, 0xFF, 0xFF});

private:
    int32_t m_width;
    int32_t m_rows;
    int32_t m_rowByteSize;
    int16_t m_colorDepth;
    int32_t m_pixelSize;
    std::vector<unsigned char> m_buffer;

    void ComputeRowByteSize();
};

#endif