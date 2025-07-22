#include "Modules/fonts/bitmapFont.h"

using namespace std;

BitmapFont::BitmapFont(Shader& s, Texture& t) 
    : m_Shader(s), m_Texture(t), m_InvertYAxis(false) {
    this->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

bool BitmapFont::Load(const char* fname) {
    fstream in;
    char bpp;
    int ImgX, ImgY;

    in.open(fname, ios_base::binary | ios_base::in);

    if (in.fail())
        return false;

    // Get Filesize
    in.seekg(0, ios_base::end);
    auto fileSize = in.tellg();
    in.seekg(0, ios_base::beg);

    // allocate space for file data
    unique_ptr<char> dat(new char[fileSize]);

    // Read filedata
    if (!dat)
        return false;

    in.read(dat.get(), fileSize);

    if (in.fail())
    {
        in.close();
        return false;
    }

    in.close();

    // Check ID is 'BFF2'
    if ((unsigned char)dat.get()[0] != 0xBF || (unsigned char)dat.get()[1] != 0xF2)
        return false;

    // Grab the rest of the header
    memcpy(&ImgX, &dat.get()[2], sizeof(int));
    memcpy(&ImgY, &dat.get()[6], sizeof(int));
    memcpy(&m_CellX, &dat.get()[10], sizeof(int));
    memcpy(&m_CellY, &dat.get()[14], sizeof(int));
    bpp = dat.get()[18];
    m_Base = dat.get()[19];

    // Check filesize
    if (fileSize != ((MAP_DATA_OFFSET)+((ImgX * ImgY) * (bpp / 8))))
        return false;

    // Calculate font params
    m_RowPitch = ImgX / m_CellX;
    m_ColFactor = (float)m_CellX / (float)ImgX;
    m_RowFactor = (float)m_CellY / (float)ImgY;
    m_YOffset = m_CellY;

    // Determine blending options based on BPP
    switch (bpp)
    {
    case 8:
        //OpenGL supports single channel images through the RED channel
        m_Texture.format = GL_RED;
        break;

    case 24:
        m_Texture.format = GL_RGB;
        break;

    case 32:
        m_Texture.format = GL_RGBA;
        break;

    default: // Unsupported BPP
        return false;
        break;
    }

    // Allocate space for image
    unique_ptr<char> img(new char[(ImgX * ImgY) * (bpp / 8)]);

    if (!img)
        return false;

    // Grab char widths
    memcpy(m_Width, &dat.get()[WIDTH_DATA_OFFSET], 256);

    // Grab image data
    memcpy(img.get(), &dat.get()[MAP_DATA_OFFSET], (ImgX * ImgY) * (bpp / 8));

    m_Texture.type = GL_TEXTURE_2D;

    // Fonts should be rendered at native resolution so no need for texture filtering
    m_Texture.SetPar(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_Texture.SetPar(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Stop chararcters from bleeding over edges
    m_Texture.SetPar(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    m_Texture.SetPar(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    m_Texture.Load(img.get(), ImgX, ImgY);

    Unbind();

    return true;
}

// Returns the width in pixels of the specified text
int BitmapFont::GetWidth(const char* Text) {
    int size = 0;
    size_t sLen = strnlen(Text, BFG_MAXSTRING);

    for (size_t loop = 0; loop != sLen; loop++)
    {
        size += m_Width[Text[loop]];
    }

    return size;
}

void BitmapFont::SetColor(glm::vec4 color) {
    m_Shader.SetUniform("fontColor", color);
}

void BitmapFont::SetColor 
    (const float& r, const float& g, const float& b, const float& a) {
    
    this->SetColor(glm::vec4(r, g, b, a));
}

void BitmapFont::ReverseYAxis(const bool& State) {
    if (State || m_InvertYAxis)
        m_YOffset = -m_CellY;
    else
        m_YOffset = m_CellY;

    m_InvertYAxis = State;
}

void BitmapFont::Print
    (const char* text, float posX, float posY, const float scale) {

    //texture mapping, top and bottom
    float u, v, u1, v1;
    int row, col;
    size_t sLen = strnlen(text, BFG_MAXSTRING);

    glActiveTexture(GL_TEXTURE0);
    Bind();

    VertexArray vao;

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    IndexBuffer ib(indices, 6);

    VertexBuffer vbo_2D(nullptr, sizeof(float) * 4 * 4);
    VertexBufferLayout vbl_2D;
    vbl_2D.Push<float>(4);
    vao.AddBuffer(vbo_2D, vbl_2D);

    for (size_t i = 0; i != sLen; i++) {
        row = (text[i] - m_Base) / m_RowPitch;
        col = (text[i] - m_Base) - (row * m_RowPitch);

        u = col * m_ColFactor;
        v = row * m_RowFactor;
        u1 = u + m_ColFactor;
        v1 = v + m_RowFactor;

        float w = m_CellX * scale;
        float h = m_CellY * scale;

        float vertices_2D[] = {
            //vertex data                   //texture	
            (posX + w),     (posY + h),     u1, v,      //top right
            (posX + w),      posY,          u1, v1,     //bottom right
             posX,           posY,          u,  v1,     //bottom left
             posX,          (posY + h),     u,  v,      //top left
        };

        posX += m_Width[text[i]] * scale;

        vbo_2D.Update(vertices_2D, sizeof(vertices_2D), 0);
        Renderer::Render(vao, ib, m_Shader);
    }

    Unbind();
}

void BitmapFont::Bind() {
    m_Texture.Bind();
}

void BitmapFont::Unbind() {
    m_Texture.Unbind();
}
