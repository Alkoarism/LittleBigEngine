#include "OpenGL/renderer.h"

#include <GLFW/glfw3.h>

void Renderer::Render(
	const VertexArray& va, const IndexBuffer& ib, const Shader& s, const GLenum& mode)
{
	va.Bind();
	ib.Bind();
	s.Use();

	if (render3D){
		s.SetUniform("projection", projection);
		s.SetUniform("view", view);
		s.SetUniform("model", model);
	}

	glDrawElements(mode, ib.GetCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::Render(
	const VertexArray& va, const Shader& s, const int& firstPoint, const int& pointCnt,const GLenum& mode)
{
	va.Bind();
	s.Use();

	glDrawArrays(mode, firstPoint, pointCnt);
}

void Renderer::RenderConfig(
	const float& r, const float& g,
	const float& b, const float& a)
{

	glClearColor(r, g, b, a);
    if (render3D)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    else
        glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::FrameTimeTracker() {
	const float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

bool Renderer::render3D = false;
float Renderer::lastFrame = 0.0f;
float Renderer::deltaTime = 0.0f;
glm::mat4 Renderer::projection = glm::mat4(1.0f);
glm::mat4 Renderer::view = glm::mat4(1.0f);
glm::mat4 Renderer::model = glm::mat4(1.0f);