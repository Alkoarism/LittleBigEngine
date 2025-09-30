#include "Modules/bitmap.h"

#include <fstream>
#include <memory>
#include <iostream>


bool    BITMAP_TOP_TO_BOTTOM = true;
int     BITMAP_WINDOWS_STANDARD_DPI = 96;
float   BITMAP_INCH_PER_METRE = 39.3701;

Bitmap::Bitmap(const int width, const int rows, const int colorDepth) 
    : m_width(width), m_rows(rows), m_colorDepth(colorDepth), m_pixelSize(colorDepth/8)
{    
    ComputeRowByteSize();    
    m_buffer = std::vector<unsigned char>(this->GetDataSize(), 0);
}

Bitmap::Bitmap(const int width, const int rows, const int colorDepth, const unsigned char* rawData)
    : m_width(width), m_rows(rows), m_colorDepth(colorDepth), m_pixelSize(colorDepth/8)
{
    ComputeRowByteSize();
    unsigned int dataSize = m_rows * m_rowByteSize;
    m_buffer = std::vector<unsigned char>(rawData, rawData + dataSize);
}

Bitmap::Bitmap(const int width, const int rows, const int colorDepth, std::vector<unsigned char>& rawData)
    : m_width(width), m_rows(rows), m_colorDepth(colorDepth), m_pixelSize(colorDepth/8)
{
    ComputeRowByteSize();
    m_buffer = std::vector<unsigned char>(rawData);
}


void Bitmap::ModifyPixel(
    const std::vector<unsigned char>& pixelData, 
    const unsigned int xPos,
    const unsigned int yPos)
{
    //realPos = (n of rows + pos in row) * pixelSize
    int realPos = (m_width * yPos + xPos) * m_pixelSize;
    for (int i = 0; i < pixelData.size(); i++) m_buffer[realPos + i] = pixelData[i];
}

void Bitmap::Export(const std::string path, const std::vector<uint8_t> eightBitColorPalete){
    std::vector<uint8_t> BMPFileHeader(14, 0);
    std::vector<uint8_t> DIBHeader(40, 0);

    uint32_t rowSize = m_pixelSize * m_width;
    uint32_t rowPadding = m_rowByteSize - rowSize;
    uint32_t imgDataSize = m_rows * m_rowByteSize;

    // ---------- RGBQUAD Array Setup -----------------------------------------
    int32_t colorsUsed = 0;
    int32_t importantColors = 0;
    std::unique_ptr<std::vector<uint8_t>> RGBQUADArray = std::make_unique<std::vector<uint8_t>>();
    
    if (m_colorDepth == 8){
        colorsUsed = 256;

        RGBQUADArray.reset();
        RGBQUADArray = std::make_unique<std::vector<uint8_t>>(4 * colorsUsed, 0);

        for (int i = 0; i < colorsUsed; i++){
            int index = 4 * i;
            float normalizedColor = static_cast<float>(i) / static_cast<float>(colorsUsed - 1);
            uint8_t blue    = static_cast<uint8_t>(normalizedColor * eightBitColorPalete[0]);
            uint8_t green   = static_cast<uint8_t>(normalizedColor * eightBitColorPalete[1]);
            uint8_t red     = static_cast<uint8_t>(normalizedColor * eightBitColorPalete[2]);
            (*RGBQUADArray)[index] = blue;
            (*RGBQUADArray)[index + 1] = green;
            (*RGBQUADArray)[index + 2] = red;
            //RGBQUADArrayTemp[index + 3] = 0; Redundant as all values are zeroed at creation
        }
    }

    // ---------- Bitmap Header data ------------------------------------------
    //File type idetifier -> BM for Bitmap files
    BMPFileHeader[0] = 'B';
    BMPFileHeader[1] = 'M';
    
    //Size of the BMP file (4 bytes)
    uint32_t sizeOfBMP = 
        BMPFileHeader.size() + 
        DIBHeader.size() +
        imgDataSize +
        RGBQUADArray->size();  

    memcpy(&BMPFileHeader[2], &sizeOfBMP, sizeof(sizeOfBMP));

    // Starting address of the byte where the bitmap image data can be found
    uint32_t pixelDataOffset = BMPFileHeader.size() + DIBHeader.size() + RGBQUADArray->size();
    memcpy(&BMPFileHeader[10], &pixelDataOffset, sizeof(pixelDataOffset));
    
    // ---------- DIB (BITMAPINFOHEADER) data ---------------------------------
    // Size of this header
    uint32_t DIBHeaderOffset = DIBHeader.size();
    memcpy(&DIBHeader[0], &DIBHeaderOffset, sizeof(DIBHeaderOffset));   

    //Bitmap width (int - 4 bytes) and height (int - 4 bytes)
    memcpy(&DIBHeader[4], &m_width, sizeof(m_width));
    if (BITMAP_TOP_TO_BOTTOM){
        int32_t negHeight = -m_rows; // Configures the BMP format to be top-to-bottom
        memcpy(&DIBHeader[8], &negHeight, sizeof(negHeight));
    } else{
        memcpy(&DIBHeader[8], &m_rows, sizeof(m_rows));
    } 

    // Number of color planes (must be 1) (2 bytes)
    int16_t colorPlanes = 1;
    memcpy(&DIBHeader[12], &colorPlanes, sizeof(colorPlanes));

    // Number of bits/pixel
    memcpy(&DIBHeader[14], &m_colorDepth, sizeof(m_colorDepth));
    
    // Raw bitmap data size
    memcpy(&DIBHeader[20], &imgDataSize, sizeof(imgDataSize));

    // Print resolution of the image horizontal and vertical
    int32_t pixelPerMetre = static_cast<int32_t>(BITMAP_WINDOWS_STANDARD_DPI * BITMAP_INCH_PER_METRE);
    memcpy(&DIBHeader[24], &pixelPerMetre, sizeof(pixelPerMetre));
    memcpy(&DIBHeader[28], &pixelPerMetre, sizeof(pixelPerMetre));

    // DIB byte padding due to zeroed data - optional, redundant
    memcpy(&DIBHeader[32], &colorsUsed, sizeof(colorsUsed)); // Number of colors in the color pallete (0 defaults to 2^n)

    // ---------- Data to file output -----------------------------------------
    std::ofstream fileBMP(path, std::ios::binary);

    fileBMP.write(reinterpret_cast<char*>(BMPFileHeader.data()), BMPFileHeader.size());
    fileBMP.write(reinterpret_cast<char*>(DIBHeader.data()), DIBHeader.size());
    fileBMP.write(reinterpret_cast<char*>(RGBQUADArray->data()), RGBQUADArray->size());

    std::vector<uint8_t> padding(rowPadding, 0);
    for (int i = 0; i < this->m_rows; i++){
        const unsigned char *rowStart = m_buffer.data() + (i * rowSize);
        fileBMP.write(reinterpret_cast<const char*>(rowStart), rowSize); 
        fileBMP.write(reinterpret_cast<char*>(padding.data()), padding.size());
    }

    fileBMP.close();
}

void Bitmap::ComputeRowByteSize(){
    uint32_t rowSize = m_pixelSize * m_width;
    int32_t rowPadding = (4 - (rowSize % 4)) % 4; // Padding is calculated per row of pixels
    m_rowByteSize = (m_width * m_pixelSize) + rowPadding; // (width + padding) = totalWidthLength
}