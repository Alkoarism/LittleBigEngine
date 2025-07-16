#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H

#include <glad/glad.h>

//===================== Core engine Defaults ==================================
constexpr char* LBE_ERROR_TEXTURE_NAME = "errorTexture";
constexpr char* LBE_ERROR_TEXTURE_PATH = "res\\textures\\error.jpg";

//===================== OpenGL Defaults =======================================
constexpr GLenum LBE_DEFAULT_TEXTURE_WRAP_S = GL_REPEAT;
constexpr GLenum LBE_DEFAULT_TEXTURE_WRAP_T = GL_REPEAT;
constexpr GLenum LBE_DEFAULT_TEXTURE_MIN_FILTER = GL_LINEAR_MIPMAP_LINEAR;
constexpr GLenum LBE_DEFAULT_TEXTURE_MAG_FILTER = GL_LINEAR;

#endif