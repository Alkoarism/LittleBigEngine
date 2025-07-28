#ifndef IMGLOADER_H
#define IMGLOADER_H

#include "headers.h"

class ImgLoader{
public:
    ImgLoader(const char* location, bool flipImage = true);
    ~ImgLoader();
    
    inline unsigned char* GetData() const { return m_data; }
    inline int GetWidth() const { return m_imgWidth; }
    inline int GetHeight() const { return m_imgHeight; }
    inline int GetChannels() const { return m_nrChannels; }
    inline const char* GetLog() const { return m_failLog; }

private:
    unsigned char* m_data;
    int m_imgWidth;
    int m_imgHeight;
    int m_nrChannels;
    const char* m_failLog;
};

#endif