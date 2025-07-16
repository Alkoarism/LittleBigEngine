#ifndef RENDERER_H
#define RENDERER_H

#include "headers.h"
#include "OpenGL/vertex_array.h"
#include "OpenGL/index_buffer.h"
#include "OpenGL/shader.h"
#include "OpenGL/texture.h"

class Renderer {
public:
	static void Render
	(const VertexArray&, const IndexBuffer&, const Shader&);
	static void RenderConfig
	(const float& r = 0.0f, const float& g = 0.0f,
		const float& b = 0.0f, const float& a = 0.0f);
	static void FrameTimeTracker();

	static void SetRender3D(const bool m) { render3D = m; }
	static void SetProjection(const glm::mat4& p) { projection = p; }
	static void SetView(const glm::mat4& v) { view = v; }
	static void SetModel(const glm::mat4& m) { model = m; }

	static float GetLastFrame() { return lastFrame; }
	static float GetDeltaTime() { return deltaTime; }
private:
	static bool render3D;
	static float lastFrame;
	static float deltaTime;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::mat4 model;
};

#endif