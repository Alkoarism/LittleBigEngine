#include "OpenGL/renderer.h"
#include "World/things.h"
#include "OpenGL/camera.h"

#include "World/mesh.h"

// function declarations ------------------------------------------------------
void processInput(GLFWwindow* window);

void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

// global variables -----------------------------------------------------------
const int screenWidth = 600, screenHeight = 800;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = screenWidth / 2, lastY = screenHeight / 2;
float fov = 45.0;
bool firstMouse = true;

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

	// engine initialization -----------------------------------------------------
	Texture& _error_texture = Things::LoadTexture(LBE_ERROR_TEXTURE_NAME, LBE_ERROR_TEXTURE_PATH, true);
	_error_texture.SetPar(GL_TEXTURE_WRAP_S, LBE_DEFAULT_TEXTURE_WRAP_S);
	_error_texture.SetPar(GL_TEXTURE_WRAP_T, LBE_DEFAULT_TEXTURE_WRAP_T);
	_error_texture.SetPar(GL_TEXTURE_MIN_FILTER, LBE_DEFAULT_TEXTURE_MIN_FILTER);
	_error_texture.SetPar(GL_TEXTURE_MAG_FILTER, LBE_DEFAULT_TEXTURE_MAG_FILTER);


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

	// Model (vertex and buffers) configurations -----------------------------------------
	Model& lightCube = Things::LoadModel("lightCube");
	Model& containerCube = Things::LoadModel("containerCube");

	Mesh lightMesh(light_vData, std::vector<unsigned int>{3});
	lightCube.PushMesh(lightMesh.GetVertexData(), lightMesh.GetVertexLayout());
	lightCube.SetIndexBuffer(vertexIndices);
	
	Mesh cubeMesh(cube_vData, std::vector<unsigned int>{3,3,2});
	containerCube.PushMesh(cubeMesh.GetVertexData(), cubeMesh.GetVertexLayout());
	containerCube.SetIndexBuffer(vertexIndices);
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
		Renderer::Render(containerCube.GetVertexArray(), containerCube.GetIBO(), test_shader);

		glm::vec3 lightPos(1.5 * sin(glfwGetTime()), 1.0f, 1.5 * cos(glfwGetTime()));
		test_shader.SetUniform("light.position", lightPos);
		model3D = glm::translate(model3D, lightPos);
		model3D = glm::scale(model3D, glm::vec3(0.2f));
		Renderer::SetModel(model3D);
		Renderer::Render(lightCube.GetVertexArray(), lightCube.GetIBO(), light_shader);
		glDisable(GL_DEPTH_TEST);
		
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