#include "OpenGL/renderer.h"
#include "World/things.h"
#include "OpenGL/camera.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "World/mesh.h"

#include "Modules/glyphLoader/bitmap_font.h"

// function declarations ------------------------------------------------------
void processInput(GLFWwindow* window);

void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

///* Freetype and text related code - Temporarily disabled
void RenderText(Shader&, std::string, float, float, float, glm::vec3);
//*/

// global variables -----------------------------------------------------------
const int screenWidth = 600, screenHeight = 800;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = screenWidth / 2, lastY = screenHeight / 2;
float fov = 45.0;
bool firstMouse = true;

///* Freetype and text related code - Temporarily disabled
struct Character {
	Texture& texture;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

std::map<char, Character> characters;
std::unique_ptr<VertexArray> VAO;
std::unique_ptr<VertexBuffer> VBO;
std::unique_ptr<IndexBuffer> IBO;
//*/

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
		(screenWidth,  screenHeight, "OpenGL LittleBigEngine-v0.04.01", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// glfw: setup
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	///* Freetype and text related code - Temporarily disabled
	// freetype: font loading and setup ------------------------------------------
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE::FAILED_TO_INITIALIZE" << std::endl;
		return -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, "res\\fonts\\Lora-Regular.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE::FAILED_TO_LOAD_FACE" << std::endl;
		return -1;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
		std::cout << "ERROR::FREETYPE::FAILED_TO_LOAD_GLYPH" << std::endl;
		return -1;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //disable byte-alignment restriction

	for (unsigned char c = 32; c < 127; ++c) {
		//load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE::FAILED_TO_LOAD_GLYPH: " 
				<< c << std::endl;
			continue;
		}

		//generate texture
		Texture& texture = Things::LoadTexture("Freetype_" + std::to_string(c));
		texture.type = GL_TEXTURE_2D;
		texture.format = GL_RED;
		texture.Load(
			face->glyph->bitmap.buffer, 
			face->glyph->bitmap.width, 
			face->glyph->bitmap.rows);

		//set texture options
		texture.SetPar(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		texture.SetPar(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		texture.SetPar(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		texture.SetPar(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	// initialization before rendering -------------------------------------------
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::ortho(
		0.0f, static_cast<float>(screenHeight), 
		0.0f, static_cast<float>(screenWidth));

	Shader& freetype2D = Things::LoadShader(
		"freetype2D",
		"res\\shaders\\main2D.vert",
		"res\\shaders\\freetype2D.frag"
	);
	freetype2D.SetUniform("projection", projection);
	freetype2D.SetUniform("model", model);

	Shader& bitmap2D = Things::LoadShader(
		"bitmap2D",
		"res\\shaders\\main2D.vert",
		"res\\shaders\\bitmap2D.frag"
	);
	bitmap2D.SetUniform("projection", projection);
	bitmap2D.SetUniform("model", model);

	VAO.reset(new VertexArray());
	VBO.reset(new VertexBuffer(nullptr, sizeof(float) * 4 * 4, GL_DYNAMIC_DRAW));
	VertexBufferLayout VBL;
	VBL.Push<float>(4);
	VAO->AddBuffer(*VBO, VBL);

	unsigned int textIndices[] = {
		0, 1, 2,
		0, 2, 3
	};
	IBO.reset(new IndexBuffer(textIndices, 6));

	BitmapFont font(bitmap2D, Things::LoadTexture("BitmapTexture"));
	font.Load("res\\bitmap\\timesNewRoman.bff");
	//*/

	///* Vertex data testing - Disabled due to texture and shader rebuild
	// vertices definition -------------------------------------------------------
	std::vector<float> cube_vData = {
		//vertex			  //Vertex Normal		//texture	r	
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 0.0f,	// 0
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 0.0f,	// 1
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	1.0f, 1.0f,	// 2
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f, 1.0f,	// 3

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 0.0f,	// 4
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 0.0f,	// 5
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	1.0f, 1.0f,	// 6
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,	0.0f, 1.0f,	// 7

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 0.0f,	// 8
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	1.0f, 1.0f,	// 9
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 1.0f,	// 10
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,	0.0f, 0.0f,	// 11

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 0.0f,	// 12
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	1.0f, 1.0f,	// 13
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 1.0f,	// 14
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,	0.0f, 0.0f,	// 15

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 1.0f,	// 16
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 1.0f,	// 17
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	1.0f, 0.0f,	// 18
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,	0.0f, 0.0f,	// 19

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 1.0f,	// 20
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 1.0f,	// 21
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	1.0f, 0.0f,	// 22
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,	0.0f, 0.0f	// 23
	};
	
	std::vector<float> light_vData = {
		//vertex			  //texture		
		-0.5f, -0.5f, -0.5f,	// 0
		 0.5f, -0.5f, -0.5f,	// 1
		 0.5f,  0.5f, -0.5f,	// 2
		-0.5f,  0.5f, -0.5f,	// 3

		-0.5f, -0.5f,  0.5f,	// 4
		 0.5f, -0.5f,  0.5f,	// 5
		 0.5f,  0.5f,  0.5f,	// 6
		-0.5f,  0.5f,  0.5f,	// 7

		-0.5f,  0.5f,  0.5f,	// 8
		-0.5f,  0.5f, -0.5f,	// 9
		-0.5f, -0.5f, -0.5f,	// 10
		-0.5f, -0.5f,  0.5f,	// 11

		 0.5f,  0.5f,  0.5f,	// 12
		 0.5f,  0.5f, -0.5f,	// 13
		 0.5f, -0.5f, -0.5f,	// 14
		 0.5f, -0.5f,  0.5f,	// 15

		-0.5f, -0.5f, -0.5f,	// 16
		 0.5f, -0.5f, -0.5f,	// 17
		 0.5f, -0.5f,  0.5f,	// 18
		-0.5f, -0.5f,  0.5f,	// 19

		-0.5f,  0.5f, -0.5f,	// 20
		 0.5f,  0.5f, -0.5f,	// 21
		 0.5f,  0.5f,  0.5f,	// 22
		-0.5f,  0.5f,  0.5f		// 23
	};

	std::vector<unsigned int> vertexIndices = {
		//cube faces
		0, 1, 2,		0, 3, 2,
		4, 5, 6,		4, 7, 6,
		8, 9, 10,		8, 11, 10,
		12, 13, 14,		12, 15, 14,
		16, 17, 18,		16, 19, 18,
		20, 21, 22,		20, 23, 22
	};


	// vertex and buffers configurations -----------------------------------------
	VertexArray light_va, cube_va;

	Mesh lightMesh(light_vData, std::vector<unsigned int>{3});
	light_va.Bind();
	light_va.AddBuffer(lightMesh.GetVertexData(), lightMesh.GetVertexLayout());
	
	Mesh cubeMesh(cube_vData, std::vector<unsigned int>{3,3,2});
	cube_va.Bind();
	cube_va.AddBuffer(cubeMesh.GetVertexData(), cubeMesh.GetVertexLayout());

	IndexBuffer ib(&vertexIndices[0], vertexIndices.size());
	//*/

	// texture handling ----------------------------------------------------------
	Texture& container = Things::LoadTexture("container", "res\\textures\\container.jpg", true);

	container.SetPar(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	container.SetPar(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	container.SetPar(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	container.SetPar(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// initialization before rendering -------------------------------------------
	Shader& light_shader = Things::LoadShader(
		"light_shader", 
		"res\\shaders\\lightSource.vert", 
		"res\\shaders\\lightSource.frag");
	
	Shader& test_shader = Things::LoadShader(
		"test_shader", 
		"res\\shaders\\test.vert", 
		"res\\shaders\\test.frag");

	Shader use_check;
	use_check.SetUniform("random", glm::vec3(1.0f));

	test_shader.SetUniform("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	test_shader.SetUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	test_shader.SetUniform("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
	test_shader.SetUniform("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
	test_shader.SetUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	test_shader.SetUniform("material.shininess", 32.0f);

	test_shader.SetUniform("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	test_shader.SetUniform("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	test_shader.SetUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	
	// render loop (happens every frame) -----------------------------------------
	while (!glfwWindowShouldClose(window)) {
		// -> frame time tracker
		Renderer::FrameTimeTracker();
		
		// -> input handling
		processInput(window);

		// --> space configurations and rendering
		Renderer::SetRender3D(true);
		Renderer::RenderConfig();
		glEnable(GL_DEPTH_TEST);
		
		// ---> world config
		glm::mat4 projection3D = glm::perspective
		(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		Renderer::SetProjection(projection3D);

		// ---> camera config
		glm::mat4 view3D = camera.GetViewMatrix();
		Renderer::SetView(view3D);
		test_shader.SetUniform("viewPos", camera.Position);

		// ---> model positioning
		glm::mat4 model3D = glm::mat4(1.0f);
		Renderer::SetModel(model3D);
		glActiveTexture(GL_TEXTURE0);
		container.Bind();
		//test_shader.SetUniform("textColor", glm::vec3(sin(glfwGetTime()), 1.0f, 1.0f));
		Renderer::Render(cube_va, ib, test_shader);

		glm::vec3 lightPos(1.5 * sin(glfwGetTime()), 1.0f, 1.5 * cos(glfwGetTime()));
		test_shader.SetUniform("light.position", lightPos);
		model3D = glm::translate(model3D, lightPos);
		model3D = glm::scale(model3D, glm::vec3(0.2f));
		Renderer::SetModel(model3D);
		Renderer::Render(light_va, ib, light_shader);
		glDisable(GL_DEPTH_TEST);

		///* Freetype and text related code - Temporarily disabled
		Renderer::SetRender3D(false);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		font.SetColor(glm::vec4(
			sin(glfwGetTime() + (2 * 3.14 / 3)),
			sin(glfwGetTime()),
			sin(glfwGetTime() - (2 * 3.14 / 3)),
			1.0f));
		font.Print("BitmapFont sample text.", 25.0f, 75.0f, 2.0f);

		font.SetColor(glm::vec4(
			sin(glfwGetTime() - (2 * 3.14 / 3)),
			sin(glfwGetTime()),
			sin(glfwGetTime() + (2 * 3.14 / 3)),
			1.0f));
		font.Print("(C) LearnOpenGL.com", 540.0f, 495.0f, 1.5f);

		RenderText(freetype2D, "Freetype sample text.", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		RenderText(freetype2D, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
		glDisable(GL_BLEND);
		//*/
		
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

	const float cameraSpeed = 2.5f * Renderer::GetDeltaTime();
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, Renderer::GetDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, Renderer::GetDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, Renderer::GetDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, Renderer::GetDeltaTime());
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.ProcessMouseScroll(yOffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

///* Freetype and text related code - Temporarily disabled
void RenderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color) {
	s.SetUniform("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	VAO->Bind();

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); ++c) {
		Character ch = characters.at(*c);

		float posX = x + (ch.bearing.x * scale);
		float posY = y - ((ch.size.y - ch.bearing.y) * scale);

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;
		
		//update VBO for each character
		float vertices[4][4] = {
				   { posX,     posY + h,   0.0f, 0.0f },
				   { posX,     posY,       0.0f, 1.0f },
				   { posX + w, posY,       1.0f, 1.0f },
				   { posX + w, posY + h,   1.0f, 0.0f }
		};

		// render glyph texture over quad
		ch.texture.Bind();
		VBO->Update(vertices, sizeof(vertices), 0);
		Renderer::Render(*VAO, *IBO, s);

		//now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale; //bitshift by 6 to get value in pixels (2^6 = 64)
	}
}
//*/