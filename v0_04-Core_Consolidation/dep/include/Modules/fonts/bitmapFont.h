#ifndef _BITMAP_FONTCLASS_H
#define _BITMAP_FONTCLASS_H

#include "OpenGL/renderer.h"

#define BFG_MAXSTRING 255     // Maximum string length

#define WIDTH_DATA_OFFSET  20 // Offset to width data with BFF file
#define MAP_DATA_OFFSET   276 // Offset to texture image data with BFF file

typedef struct
{
	unsigned char ID1, ID2;
	unsigned char BPP;
	int ImageWidth, ImageHeight, CellWidth, CellHeight;
	unsigned char StartPoint;
}FontFileHeader;

class BitmapFont
{
public:
	bool print_2D;

	BitmapFont(Shader&, Texture&);

	bool Load(const char* fname);
	int  GetWidth(const char* Text);

	void SetColor(glm::vec4 color);
	void SetColor
		(const float& r, const float& g, const float& b, const float& a = 1.0f);
	void ReverseYAxis(const bool&);
	void Print(const char* txt, float x, float y,const float scale = 1.0f);

private:
	int m_CellX, m_CellY, m_YOffset, m_RowPitch;
	char m_Base;
	char m_Width[256];
	float m_RowFactor, m_ColFactor;
	bool m_InvertYAxis;

	Shader& m_Shader;
	Texture& m_Texture;

	void Bind();
	void Unbind();
};

#endif