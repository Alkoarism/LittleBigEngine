#include <glad/glad.h>	//Never remove, responsible for OpenGL´s loading
#include <GLFW/glfw3.h>	//Never remove, responsible for window management

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "OpenGL/renderer.h"
#include "OpenGL/camera.h"

#include "World/things.h"
#include "World/mesh.h"

#include "Modules/font.h"
#include "Modules/bitmap.h"
#include "Modules/fontAtlas.h"

// function declarations ------------------------------------------------------
void processInput(GLFWwindow* window);

void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

void RenderText(Shader&, std::string, float, float, float, glm::vec3);

// global variables -----------------------------------------------------------
const int screenWidth = 600, screenHeight = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = screenWidth / 2, lastY = screenHeight / 2;
float fov = 45.0;
bool firstMouse = true;

std::unique_ptr<VertexArray> fontVAO;
std::unique_ptr<VertexBuffer> fontVBO;
std::unique_ptr<IndexBuffer> fontIBO;
std::unique_ptr<FontAtlas> fontAtlas;

bool textBlending = true;

int main() {
	// glfw: initialize and configure --------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw: window creation and setup -------------------------------------------
	GLFWwindow* window = glfwCreateWindow
		(screenWidth,  screenHeight, "OpenGL LittleBigEngine", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// glfw: setup
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// engine initialization -----------------------------------------------------
	// -> error texture fallback
	Texture& _error_texture = Things::LoadTexture(LBE_ERROR_TEXTURE_NAME, LBE_ERROR_TEXTURE_PATH, true);
	_error_texture.SetPar(GL_TEXTURE_WRAP_S, LBE_DEFAULT_TEXTURE_WRAP_S);
	_error_texture.SetPar(GL_TEXTURE_WRAP_T, LBE_DEFAULT_TEXTURE_WRAP_T);
	_error_texture.SetPar(GL_TEXTURE_MIN_FILTER, LBE_DEFAULT_TEXTURE_MIN_FILTER);
	_error_texture.SetPar(GL_TEXTURE_MAG_FILTER, LBE_DEFAULT_TEXTURE_MAG_FILTER);

	// -> font loading
	Font timesNewRoman("res/fonts/Times-New-Roman.ttf", 0, 64);
	fontAtlas = std::make_unique<FontAtlas>(timesNewRoman);

	glm::mat4 fontModel = glm::mat4(1.0f);
	glm::mat4 fontView = glm::mat4(1.0f);
	glm::mat4 fontProjection = glm::ortho(
		0.0f, static_cast<float>(screenHeight),
		0.0f, static_cast<float>(screenWidth));
	
	fontVAO = std::make_unique<VertexArray>();
	fontVBO = std::make_unique<VertexBuffer>(nullptr, sizeof(float) * 4 * 4, GL_DYNAMIC_DRAW);
	VertexBufferLayout fontVBL;
	fontVBL.Push<float>(4);
	fontVAO->AddBuffer(*fontVBO, fontVBL);

	unsigned int fontIndices[] = {
		0, 1, 2,
		0, 2, 3
	};
	fontIBO = std::make_unique<IndexBuffer>(fontIndices, 6);

	// texture handling ----------------------------------------------------------
	const Bitmap& atlasBMP = fontAtlas->GetBitmap();
	// The .bmp is generated internally, but can be exported and loaded separatelly.
	fontAtlas->ExportBitmapAtlas("res/bitmap/timesNewRoman.bmp");
	
	Texture atlasTexture(GL_TEXTURE_2D, GL_RED);
	atlasTexture.Bind();
	atlasTexture.SetBitUnpackingSize(1);
	atlasTexture.SetPar(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	atlasTexture.SetPar(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	atlasTexture.SetPar(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	atlasTexture.SetPar(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	atlasTexture.Load(atlasBMP.GetRawData(), atlasBMP.GetWidth(), atlasBMP.GetRows());

	// initialization before rendering -------------------------------------------
	Shader& fontShader = Things::LoadShader(
		"font2D",
		"res/shaders/main2D.vert",
		"res/shaders/font2D.frag");
	
	fontShader.SetUniform("projection", fontProjection);
	fontShader.SetUniform("model", fontModel);

	// render loop (happens every frame) -----------------------------------------
	while (!glfwWindowShouldClose(window)) {
		// -> frame time tracker
		Renderer::FrameTimeTracker();
		
		// -> input handling
		processInput(window);

		// --> space configurations and rendering
		Renderer::RenderConfig(0.4f, 0.4f, 0.4f);
		
		// ---> font rendering
		Renderer::SetRender3D(false);			
		bool shouldBlend = textBlending;		
		if (shouldBlend) glEnable(GL_BLEND);	//Blending control
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glActiveTexture(GL_TEXTURE0);
		atlasTexture.Bind();
		//Text Rendering
		if (shouldBlend) glDisable(GL_BLEND);	//Blending control

		// -> check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) textBlending = false;
	else textBlending = true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void RenderText(Shader& fs, std::string text, float x, float y, float scale, glm::vec3 color){
	fs.SetUniform("textColor", color);
	fontVAO->Bind();

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++){
		const CellData& charCellData = fontAtlas->GetCharacterData(*c);
		const GlyphMetrics& characterData = charCellData.glyphMetrics;

		float posX = x + (characterData.xBearing * scale);
		float pixelHeight = static_cast<float>(characterData.rows) - static_cast<float>(characterData.yBearing);
		float posY = y - (pixelHeight * scale);
		
		float w = characterData.width * scale;
		float z = characterData.rows * scale;
		
		// The 1 and 2 magic numbers trim the edges of the letters and correct bleeding
		float u = static_cast<float>(charCellData.xAtlasOffset + 1) / fontAtlas->GetBitmap().GetWidth();
		float v = static_cast<float>(charCellData.yAtlasOffset) / fontAtlas->GetBitmap().GetRows();

		float u1 = static_cast<float>(characterData.width - 2) / fontAtlas->GetBitmap().GetWidth();
		float v1 = static_cast<float>(characterData.rows) / fontAtlas->GetBitmap().GetRows();

		//update VBO for each character
		float vertices[4][4] = {
			{ posX,		posY + z,	u,		v },
			{ posX,		posY,		u,		v + v1},
			{ posX + w,	posY,		u + u1,	v + v1},
			{ posX + w,	posY + z,	u + u1,	v}
		};

		// render glyph texture over quad
		fontVBO->Update(vertices, sizeof(vertices), 0);
		Renderer::Render(*fontVAO, *fontIBO, fs);

		x += characterData.xAdvance * scale;
	}
}