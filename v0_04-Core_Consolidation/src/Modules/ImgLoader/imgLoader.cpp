#include "Modules/imgLoader/imgLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <Modules/imgLoader/stb_image.h>
#define STBI_FAILURE_USERMSG

ImgLoader::ImgLoader(const char* location, bool flipImage){
    stbi_set_flip_vertically_on_load(flipImage);
	m_data = stbi_load(location, &m_imgWidth, &m_imgHeight, &m_nrChannels, 0);
    m_failLog = stbi_failure_reason();
    if (m_data == nullptr){
        std::cout << "ERROR::IMGLOAD::FAILED_TO_LOAD\n" << m_failLog << std::endl;
    }
}

ImgLoader::~ImgLoader(){
    if (m_data){
        stbi_image_free(m_data);
    }
}